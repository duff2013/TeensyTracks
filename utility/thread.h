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
 || Light weight thread scheduler library, based off the awesome fibers
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

#ifndef THREAD_h
#define THREAD_h
#ifdef __cplusplus

#if( !defined( TEENSYDUINO ) )
#error "Teensy 3.x & Teensy lc Only!!!"
#endif

#include "threadHelper.h"

/**************************************************
 * This allows yield calls in a ISR not to lockup,
 * the kernel. Uncomment if any ISR calls yield in
 * its handler code.
 **************************************************/
//#define USE_INTERRUPTS

class Thread {
private:
public:
    Thread              ( uint16_t main_stack_size, const uint32_t pattern = 0xA5A5A5A5 ) ;
    ThreadState create  ( thread_func_t thread, size_t stack_size, void *arg, bool isTrack ) ;
    ThreadState pause   ( thread_func_t thread ) ;
    ThreadState resume  ( thread_func_t thread ) ;
    ThreadState restart ( thread_func_t thread ) ;
    ThreadState stop    ( thread_func_t thread ) ;
    ThreadState sync    ( thread_func_t thread ) ;
    ThreadState state   ( thread_func_t thread ) ;
    ThreadState state   ( loop_func_t thread ) ;
    ThreadState restart_all( void );
    uint32_t    memory   ( thread_func_t thread ) ;
    uint32_t    memory   ( loop_func_t thread ) ;
    bool        transmit ( void *p, thread_func_t thread_to, thread_func_t thread_from, int count ) ;
    bool        receive  ( thread_func_t thread_to, thread_func_t thread_from, void *p ) ;
};
#endif
#endif

