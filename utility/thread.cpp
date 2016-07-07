/*
 ||
 || @file 		thread.cpp
 || @version 	0.5
 || @author 	Colin Duffy
 || @contact 	cmduffy@engr.psu.edu
 || @author 	Warren Gay
 || @contact 	ve3wwg@gmail.com
 ||
 || @description
 || Light weight threaded library, based off the awesome fibers
 || library by Warren Gay. https://github.com/ve3wwg/teensy3_fibers
 ||
 || @license
 || | Copyright (c) 2014 Colin Duffy, (C) Warren Gay VE3WWG
 || | This library is free software; you can redistribute it and/or
 || | modify it under the terms of the GNU Lesser General Public
 || | License as published by the Free Software Foundation; version
 || | 2.1 of the License.
 || |
 || | This library is distributed in the hope that it will be useful,
 || | but WITHOUT ANY WARRANTY; without even the implied warranty of
 || | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 || | Lesser General Public License for more details.
 || |
 || | You should have received a copy of the GNU Lesser General Public
 || | License along with this library; if not, write to the Free Software
 || | Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 || #
 ||
 */

#include "thread.h"
#include "threadHelper.h"
/***************************************************
 *----------------Editible Options------------------*
 ****************************************************/
#if defined(KINETISK)
#define MAX_TASKS 32
#elif defined(KINETISL)
#define MAX_TASKS 8
#endif
// minimum main stack size.
#define MIN_MAIN_STACK_SIZE 200
// Memory fill pattern used to estimate stack usage.
#define MEMORY_FILL_PATTERN 0XFFFFFFFFUL
// Size of the inter thread message buffer
#define MSG_BUFFER_SIZE 20
/****************************************************
 *----------------End Editable Options---------------*
 *****************************************************/
//TODO: use the PSP stack pointer for threads and MSP for ISR.
#define MAIN_RETURN         0xFFFFFFF9  //Tells the handler to return using the MSP
#define THREAD_RETURN       0xFFFFFFFD //Tells the handler to return using the PSP

#ifdef __cplusplus
extern "C" {
#endif
    void init_stack( uint32_t main_stack, uint32_t pattern_override );
#if defined(KINETISK)
    ThreadState thread_create( thread_func_t func, size_t stack_size, void *arg );
#elif defined(KINETISL)
    ThreadState thread_create( volatile stack_frame_t *frame, size_t stack_size, thread_func_t func, void *arg );
#endif
    ThreadState  thread_state   ( thread_func_t func );
    ThreadState  main_state     ( loop_func_t func );
    ThreadState  thread_sync    ( thread_func_t func );
    ThreadState  thread_restart ( thread_func_t func );
    ThreadState  thread_return  ( thread_func_t func );
    ThreadState  thread_pause   ( thread_func_t func );
    ThreadState  thread_resume  ( thread_func_t func );
    uint32_t     thread_memory  ( thread_func_t func );
    uint32_t     main_memory    ( loop_func_t func );
    //inline uint32_t sys_acquire_lock( volatile unsigned int *lock_var );
    //inline uint32_t sys_release_lock( volatile unsigned int *lock_var );
    //void ThreadUseInterrupt(enum IRQ_NUMBER_t interruptName);
#ifdef __cplusplus
}
#endif

// end of bss section
extern unsigned long _ebss;
// end of heap/stack area
extern unsigned long _estack;
// Address of the stack root for last thread (else 0)
static volatile uint32_t stackroot = 0;
// Alternate fill pattern, NOT USED!!!
static uint32_t psp_fill_pattern = 0;
// Mask for thread swap
static volatile uint32_t thread_mask    = 0;
// Restore thread_mask when fifo is empty
static volatile uint32_t init_mask     = 0;
// Used for low level ISR that gets called for handler functions.
static volatile boolean update_in_progress  = false;
// Hold thread struct for context switch
static volatile stack_frame_t process_tasks[MAX_TASKS];
volatile uint8_t num_task = 0;
// Enum for clarity on what function was called and then passed to LOW Level ISR
// for completion.
typedef enum  {
    PAUSE,
    RESUME,
    RESTART,
    RETURN,
    TASK_STATE,
    MAIN_STATE,
    MEMORY
} calling_func_t;
calling_func_t CALLING_FUNCTION;
thread_func_t FUNCTION;
loop_func_t LOOP_FUNCTION;
ThreadState _STATE;
uint32_t STACK_MEMORY;
thread_msg_t msg[MSG_BUFFER_SIZE];
uint32_t msgHead;
uint32_t msgTail;

Thread::Thread( uint16_t main_stack_size, const uint32_t pattern ) {
    NVIC_SET_PRIORITY( IRQ_RTC_SECOND, 0xFF ); // 0xFF = lowest priority
    NVIC_ENABLE_IRQ( IRQ_RTC_SECOND );
    init_stack( main_stack_size, pattern );
    for (int i = 0; i <= MSG_BUFFER_SIZE; i++) {
        msg[i].func_to_ptr = nullptr;
        msg[i].func_from_ptr = nullptr;
    }
}

ThreadState Thread::create( thread_func_t thread, size_t stack_size, void *arg ) {
    // Round stack size to a word multiple
    int s_size = ( stack_size + sizeof ( uint32_t ) ) / sizeof ( uint32_t ) * sizeof ( uint32_t );
    if ( ++num_task >= MAX_TASKS ) return ThreadInvalid;
#if defined(KINETISK)
    ThreadState p = thread_create( thread, s_size, arg );
#elif defined(KINETISL)
    volatile stack_frame_t *frame = &process_tasks[num_task];
    ThreadState p = thread_create( frame, s_size, thread, arg );
#endif
    return p;
}

ThreadState Thread::state( thread_func_t thread ) {
    ThreadState p = thread_state( thread );
    return p;
}

ThreadState Thread::resume( thread_func_t thread ) {
    ThreadState p = thread_resume( thread );
    return p;
}

ThreadState Thread::pause( thread_func_t thread ) {
    ThreadState p = thread_pause( thread );
    return p;
}

ThreadState Thread::state( loop_func_t thread ) {
    ThreadState p = main_state( thread );
    return p;
}

ThreadState Thread::sync( thread_func_t thread) {
    ThreadState p = thread_sync( thread );
    return p;
}

ThreadState Thread::restart( thread_func_t thread ) {
    ThreadState p = thread_restart( thread );
    return p;
}

ThreadState Thread::stop( thread_func_t thread ) {
    ThreadState p = thread_return( thread );
    return p;
}

uint32_t Thread::memory( thread_func_t thread ) {
    return thread_memory( thread );
}

uint32_t Thread::memory( loop_func_t thread ) {
    uint32_t tmp = 0;// = main_size(thread);
    return tmp;
}

bool Thread::transmit( void *p, thread_func_t thread_to, thread_func_t thread_from, int count ) {
    for (int i = 0; i < MSG_BUFFER_SIZE; i++) {
        if (msg[i].func_to_ptr == nullptr && msg[i].func_from_ptr == nullptr) {
            msg[i].func_to_ptr = thread_to;
            msg[i].func_from_ptr = thread_from;
            byte *tmp = ( byte * )p;
            memcpy( msg[i].type_b, tmp, count );
            return false;
        }
    }
    return true;
}

bool Thread::receive( thread_func_t thread_to, thread_func_t thread_from, void *p ) {
    for (int i = 0; i < MSG_BUFFER_SIZE; i++) {
        if (msg[i].func_to_ptr == thread_to && msg[i].func_from_ptr == thread_from) {
            byte *tmp = ( byte * )p;
            memcpy( tmp, msg[i].type_b, 4 );
            msg[i].func_to_ptr = nullptr;
            msg[i].func_from_ptr = nullptr;
            return false;
        }
    }
    return true;
}
/******************************************************************************************
 ________          ________          ________          ________          ________
 _|        |________|        |________|        |________|        |________|        |________
 *******************************************************************************************/
//////////////////////////////////////////////////////////////////////
// Initialize main stack
//////////////////////////////////////////////////////////////////////
void init_stack( uint32_t main_stack, uint32_t pattern_override ) {
    num_task = 0;
    psp_fill_pattern = pattern_override;
    
    int i = 0;
    for (i = 0; i < MAX_TASKS; i++) {
        volatile stack_frame_t *u = &process_tasks[i];
        u->address = 0;
        u->state = ThreadInvalid;
    }
    int stack_size = main_stack;
    stack_size = ( stack_size + sizeof (uint32_t) ) / sizeof (uint32_t) * sizeof (uint32_t);
    asm volatile( "MRS %[result], MSP\n" : [result] "=r" (stackroot) );
    volatile stack_frame_t *p = &process_tasks[num_task];
    
    if ( stack_size >= MIN_MAIN_STACK_SIZE ) {
        p->initial_sp = stackroot;
        p->stack_size = stack_size;
        stackroot -= stack_size;
    }
    else {
        p->stack_size = MIN_MAIN_STACK_SIZE;
        p->initial_sp = stackroot;
        stackroot -= MIN_MAIN_STACK_SIZE;
    }
    p->address = 1;
    p->state = ThreadCreated;
    thread_mask = ( 1 << 0 );
}
//////////////////////////////////////////////////////////////////////
// This is the thread's launch pad routine
//////////////////////////////////////////////////////////////////////
static void thread_start( void ) {
    stack_frame_t* p;
#if defined(KINETISK)
    asm volatile( "mov %[result], r12\n" : [result] "=r" ( p ) );		 // r12 points to thread initially
#elif defined(KINETISL)
    asm volatile("mov %[result], r7\n" : [result] "=r" ( p ) );          // r7 points to thread initially
#endif
    asm volatile( "mov r0, %[value]\n" : : [value] "r" ( p->arg ) );	 // Supply void *arg to thread call
    asm volatile( "mov r1, %[value]\n" : : [value] "r" ( p->func_ptr ) );// r1 now holds the function ptr to call
#if defined(KINETISK)
    asm volatile( "push {r2-r12}" ); // push to stack
    asm volatile( "blx  r1\n" );     // func(arg) call
    asm volatile( "pop  {r2-r12}" ); // pop from stack if thread is returned
#elif defined(KINETISL)
    asm volatile("push {r4-r7}\n");  // push r4-r7 to stack
    asm volatile("blx  r1\n");       // func(arg) call
    asm volatile("pop  {r4-r7}\n");  // pop r4-r7 from stack
#endif
    p->state = ThreadReturned;         // update state when thread has returned
    for (;;) yield( );               // returned thread, now loops here till restarted
}

static void threadReturnedState( void ) {
    yield( );
}
//////////////////////////////////////////////////////////////////////
// Set up a thread to execute (but don't launch it)
//////////////////////////////////////////////////////////////////////
#if defined(KINETISL)
ThreadState thread_create( volatile stack_frame_t *frame, size_t stack_size, thread_func_t func, void *arg ) {
    asm volatile("push {r0,r1,r2,r3}\n");
    asm volatile("stmia r0!,{r4,r5,r6,r7}\n");       // Save lower regs
    asm volatile("mov r1,r8\n");
    asm volatile("mov r2,r9\n");
    asm volatile("mov r3,sl\n");
    asm volatile("stmia r0!,{r1,r2,r3}\n");		// Save r8,r9 & sl
    asm volatile("mov r1,fp\n");
    asm volatile("mov r3,lr\n");
    asm volatile("stmia r0!,{r1,r2,r3}\n");		// Save fp,(placeholder for sp) & lr
    asm volatile("pop {r0,r1,r2,r3}\n");		// Restore regs
    frame->stack_size = stack_size;		// In r1
    frame->func_ptr 	= func;			// In r2
    frame->arg 	= arg;			// In r3
    frame->r7	= (uint32_t)frame;	// Overwrite r12 with fiber ptr
    frame->sp = stackroot;			// Save as Fiber's sp
    frame->lr = (void *) thread_start;	// Fiber startup code
    frame->state = ThreadCreated;		// Set state of this fiber
    frame->initial_sp = stackroot;		// Save sp for restart()
    stackroot    -= stack_size;             // This is the new root of the stack
    int address   = 1 << num_task;          // get thread address
    frame->address   |= address;                // set thread address
    thread_mask     = thread_mask | ( 1 << num_task ); // thread swap mask
    init_mask     = thread_mask;              // num of threads
    return ThreadCreated;
}
#elif defined(KINETISK)
ThreadState thread_create( thread_func_t func, size_t stack_size, void *arg ) {
    volatile stack_frame_t *p = &process_tasks[num_task]; // Thread struct
    asm volatile( "STMEA %0,{r1-r11}\n" : "+r" ( p ) :: "memory" );// Save r1-r11 to thread struct
    p->stack_size = stack_size;     // Save thread size
    p->func_ptr   = func;           // Save thread function
    p->arg        = arg;            // Save thread arg
    p->r12        = ( uint32_t )p;  // r12 points to struct
    p->sp         = stackroot;              // Save as threads's sp
    p->state      = ThreadCreated;		    // Set state of this thread
    p->initial_sp = stackroot;		        // Save sp for restart()
    p->lr         = ( void * ) thread_start;	// Thread startup code
    stackroot    -= stack_size;             // This is the new root of the stack
    int address   = 1 << num_task;          // get thread address
    p->address   |= address;                // set thread address
    thread_mask     = thread_mask | ( 1 << num_task ); // thread swap mask
    init_mask     = thread_mask;              // num of threads
    return ThreadCreated;
}
#endif
//////////////////////////////////////////////////////////////////////
// Swap one thread for another. This should be optimized as much as possible
//////////////////////////////////////////////////////////////////////
void thread_swap( volatile stack_frame_t *prevframe, volatile stack_frame_t *nextframe ) {
#if defined(KINETISK)
    asm volatile (
                  "MRS %[result], MSP\n"
                  : [result] "=r" ( prevframe->sp )
                  );
    asm volatile (
                  "ADD r0, #4"             "\n\t"   // &prevframe->r2
                  "STMEA r0!,{r2-r12,lr}"  "\n\t"   // Save r2-r12 + lr
                  "LDMFD r1,{r1-r12,lr}"   "\n\t"   // Restore r1(sp) and r2-r12, lr
                  "MSR MSP, r1"            "\n\t"   // Set new sp
                  "BX lr"                  "\n"
                  );
#elif defined(KINETISL)
    asm volatile("stmia r0!,{r4-r7}\n");	// Save r4,r5,r6,r7
    asm volatile("mov r2,r8\n");            // mov r8 to r2
    asm volatile("mov r3,r9\n");            // mov r9 to r3
    asm volatile("mov r4,sl\n");            // mov sl to r4
    asm volatile("stmia r0!,{r2-r4}\n");	// Save r8,r9,sl
    asm volatile("mov r2,fp\n");            // mov fp to r2
    asm volatile("mov r3,sp\n");            // mov sp to r3
    asm volatile("mov r4,lr\n");            // mov lr to r4
    asm volatile("stmia r0!,{r2-r4}\n");	// Save fp,sp,lr
    
    asm volatile("add r1,#16");             // r0 = &nextfibe->r8
    asm volatile("ldmia r1!,{r2-r4}\n");	// Load values for r8,r9,sl
    asm volatile("mov r8,r2\n");            // mov r2 to r8
    asm volatile("mov r9,r3\n");            // mov r3 to r9
    asm volatile("mov sl,r4\n");            // mov r4 to sl
    asm volatile("ldmia r1!,{r2-r4}\n");	// Load values for fp,sp,lr
    asm volatile("mov fp,r2\n");            // mov fp to r2
    asm volatile("mov sp,r3\n");            // mov sp to r3
    asm volatile("mov lr,r4\n");            // mov lr to r4
    asm volatile("sub r1,#40\n");           // r0 = nextfibe
    asm volatile("ldmia r1!,{r4-r7}\n");	// Restore r4-r7
#endif
}
//////////////////////////////////////////////////////////////////////
// all invocations of yield in teensyduino api go through this now.
//////////////////////////////////////////////////////////////////////
void yield( void ) {
    //digitalWriteFast(16, HIGH);
    // There is only the main context running
    if ( num_task == 0 ) return;
#ifdef USE_INTERRUPTS
    __disable_irq( );
#endif
    uint32_t mask = thread_mask;
    uint32_t tail = __builtin_ctz( mask );
    mask &= ( mask - 1 );
    uint32_t head = __builtin_ctz( mask );
    if( head > num_task ) head = 0;
    // previous thread to be stored
    volatile stack_frame_t *last = &process_tasks[tail];
    // next thread to be loaded
    volatile stack_frame_t *next = &process_tasks[head];
    enum ThreadState  lst = last->state;
    enum ThreadState  nxt = next->state;
    // set previous state to created or returned
    enum ThreadState l = ( lst != ThreadReturned ) ? ThreadCreated : ThreadReturned;
    last->state = ( lst == ThreadPause ) ? ThreadPause : l;
    // set current state to executing or returned
    enum ThreadState n = ( nxt != ThreadReturned ) ? ThreadExecuting : ThreadReturned;
    next->state = ( nxt == ThreadPause ) ? ThreadPause : n;
    // update next thread
    const uint32_t init = init_mask;
    thread_mask = mask == 0 ? init : mask;
    // make the swap
    thread_swap( last, next );
#ifdef USE_INTERRUPTS
    __enable_irq( );
#endif
    //digitalWriteFast(16, LOW);
}
//////////////////////////////////////////////////////////////////////
// pass thread state, pass loop state
// ** calls low priority software isr to update **
//////////////////////////////////////////////////////////////////////
ThreadState thread_state( thread_func_t func ) {
    FUNCTION = func;
    CALLING_FUNCTION = TASK_STATE;
    NVIC_SET_PENDING( IRQ_RTC_SECOND );
    while( update_in_progress ) ;
    return _STATE;
}
ThreadState main_state( loop_func_t func ) {
    LOOP_FUNCTION = func;
    CALLING_FUNCTION = MAIN_STATE;
    NVIC_SET_PENDING( IRQ_RTC_SECOND );
    while( update_in_progress ) ;
    return _STATE;
}
//////////////////////////////////////////////////////////////////////
// routine to block until selected thread return's.
// ** calls low priority software isr to update **
//////////////////////////////////////////////////////////////////////
ThreadState thread_sync( thread_func_t func ) {
    enum ThreadState return_state;
    do {
        yield( );
        return_state = thread_state( func );
    }
    while ( ( return_state == ThreadReturned ) || ( return_state == ThreadPause )  ) ; // Keep waiting
    return return_state;
}
//////////////////////////////////////////////////////////////////////
// start up returned thread
// ** calls low priority software isr to update **
//////////////////////////////////////////////////////////////////////
ThreadState thread_restart( thread_func_t func ) {
    FUNCTION = func;
    CALLING_FUNCTION = RESTART;
    update_in_progress = true;
    NVIC_SET_PENDING( IRQ_RTC_SECOND );
    while( update_in_progress ) ;
    return _STATE;
}
//////////////////////////////////////////////////////////////////////
// return a thread - puts in a returned state
// ** calls low priority software isr to update **
//////////////////////////////////////////////////////////////////////
ThreadState thread_return( thread_func_t func ) {
    FUNCTION = func;
    CALLING_FUNCTION = RETURN;
    update_in_progress = true;
    NVIC_SET_PENDING( IRQ_RTC_SECOND );
    while( update_in_progress ) ;
    return _STATE;
}
//////////////////////////////////////////////////////////////////////
// pause running thread
// ** calls low priority software isr to update **
//////////////////////////////////////////////////////////////////////
ThreadState thread_pause( thread_func_t func ) {
    FUNCTION = func;
    CALLING_FUNCTION = PAUSE;
    update_in_progress = true;
    NVIC_SET_PENDING( IRQ_RTC_SECOND );
    while( update_in_progress ) ;
    return _STATE;
}
//////////////////////////////////////////////////////////////////////
// start paused thread
// ** calls low priority software isr to update **
//////////////////////////////////////////////////////////////////////
ThreadState thread_resume( thread_func_t func ) {
    FUNCTION = func;
    CALLING_FUNCTION = RESUME;
    update_in_progress = true;
    NVIC_SET_PENDING( IRQ_RTC_SECOND );
    while( update_in_progress ) ;
    return _STATE;
}
//////////////////////////////////////////////////////////////////////
// TODO: return thread unused size, not working yet
//////////////////////////////////////////////////////////////////////
uint32_t thread_memory( thread_func_t func ) {
    FUNCTION = func;
    CALLING_FUNCTION = MEMORY;
    update_in_progress = true;
    NVIC_SET_PENDING( IRQ_RTC_SECOND );
    while( update_in_progress ) ;
    return STACK_MEMORY;
}

uint32_t main_memory( loop_func_t func ) {
    volatile stack_frame_t *p = &process_tasks[0];
    return p->initial_sp;
}
//////////////////////////////////////////////////////////////////////
// low priority isr to update or change the state of each thread
//////////////////////////////////////////////////////////////////////
void rtc_seconds_isr(void) {
    thread_func_t f_ptr;
    ThreadState state;
    uint32_t num;
    int i = 0;
    volatile stack_frame_t *p;
    do {
        p = &process_tasks[i];
        f_ptr = p->func_ptr;
        if ( i > num_task ) {
            _STATE = ThreadInvalid;
            update_in_progress = false;
            return;
        }
        i++;
    } while ( FUNCTION != f_ptr  );
    
    /* call function handlers */
    switch ( CALLING_FUNCTION ) {
        case PAUSE:
            p = &process_tasks[i-1];
            state = p->state;
            if ( state == ThreadPause ) {
                _STATE = ThreadPause;
                update_in_progress = false;
                break;
            }
            p->state = ThreadPause;
            num = init_mask;
            num &= ~( p->address );
            init_mask = num;
            _STATE = ThreadPause;
            update_in_progress = false;
            break;
        case RESUME:
            p = &process_tasks[i-1];
            state = p->state;
            _STATE = state;
            if ( state != ThreadPause ) {
                update_in_progress = false;
                break;
            }
            p->state = ThreadCreated;
            num = init_mask;
            num |= ( p->address );
            init_mask = num;
            _STATE = ThreadCreated;
            update_in_progress = false;
            break;
        case RESTART:
            p = &process_tasks[i-1];
            state = p->state;
            if ( state == ThreadInvalid ) {
                update_in_progress = false;
                break;
            }
            uint32_t num;
            num = init_mask;
            num |= ( p->address );
            init_mask = num;
            p->state = ThreadCreated;
#if defined(KINETISK)
            p->r12 = ( uint32_t )p;
#elif defined(KINETISL)
            p->r7 = ( uint32_t )p;
#endif
            p->sp = p->initial_sp;
            p->lr = ( void * )thread_start;
            _STATE = ThreadCreated;
            update_in_progress = false;
            break;
        case RETURN:
            p = &process_tasks[i-1];
            state = p->state;
            if ( state == ThreadReturned ) {
                update_in_progress = false;
                break;
            }
            p->lr = ( void * )threadReturnedState;
            num = init_mask;
            num &= ~( p->address );
            init_mask = num;
            _STATE = ThreadReturned;
            update_in_progress = false;
            break;
        case TASK_STATE:
            p = &process_tasks[i-1];
            _STATE = p->state;
            update_in_progress = false;
            break;
        case MAIN_STATE:
            p = &process_tasks[0];
            _STATE = p->state;
            update_in_progress = false;
            break;
        case MEMORY:
        {
            p = &process_tasks[i-1];
            volatile uint32_t *start = (volatile uint32_t *)p->initial_sp;
            volatile const uint32_t *stop = start - (p->stack_size/4);
            int count = 0;
            while ( start >= stop ) {
                //__disable_irq();
                if(*start == 0xFFFFFFFF) count++;
                start--;
                //__enable_irq();
            }
            STACK_MEMORY = (count-1)*4;
            update_in_progress = false;
        }
            break;
            
        default:
            update_in_progress = false;
            break;
    }
}