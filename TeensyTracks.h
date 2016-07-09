/* TeensyTracks
 * Copyright (c) 2016, Colin Duffy, https://github.com/duff2013
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice, development funding notice, and this permission
 * notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef TeensyTracks_h_
#define TeensyTracks_h_

#include "Arduino.h"
#include "utility/thread.h"
//#include "zilch.h"

enum keys {C, Cs, D, Ef, E, F, Fs, G, Gs, A, Bf, B};

class Track;
class MasterTrack;

#define WHOLE_BAR               0
#define HALF_BAR                1
#define QUARTER_BAR             2
#define EIGHTH_BAR              3
#define SIXTEENTH_BAR           4
#define THIRTY_SECOND_BAR       5
#define QUATER_TRIPLET_BAR      6
#define EIGHTH_TRIPLET_BAR      7

#define MASTER_TRACK MasterTrack &p
/**
 *  This is the delays that keep playing instruments or whatever in time with each other.
 *  User must supply the defined measure delays above WHOLE_BAR... etc.
 *
 *  @param MASTER_TRACK MasterTrack class, this is so we can use the trackDelayStart and
 *                      trackDelayEnd functions in the MasterTrack class.
 *  @param x            WHOLE_BAR,HALF_BAR,QUARTER_BAR,EIGHTH_BAR,SIXTEENTH_BAR,THIRTY_SECOND_BAR
 *
 *  @return nothing
 */
#define TRACK_DELAY( MASTER_TRACK, x  )    \
for ( uint8_t __ToDo = MASTER_TRACK.trackDelayStart( ); __ToDo;  __ToDo = MASTER_TRACK.trackDelayEnd( x ) )
/**
 *  Only one Master Track can be used, all Track Class inherit this class.
 *  TODO: make it a singleton!
 */
class MasterTrack {
public:
    MasterTrack( uint16_t tempo, keys trackKey, uint8_t bars, uint8_t sigHigh, uint8_t sigLow ) {
        beatTime          = 60000000.0 / tempo;
        timeSignatureHigh = sigHigh;
        timeSignatureLow  = sigLow;
        key               = trackKey;
        measures          = bars;
        volume            = 0;
        currentBar        = 0;
        currentBeat       = timeSignatureHigh - 1;
        
        wholeBarMulitplier          = beatTime * timeSignatureHigh;
        halfBarMulitplier           = wholeBarMulitplier/2;
        quarterBarMulitplier        = wholeBarMulitplier/4;
        eighthBarMulitplier         = wholeBarMulitplier/8;
        sixteenthBarMulitplier      = wholeBarMulitplier/16;
        thritySecondBarMulitplier   = wholeBarMulitplier/32;
        quarterTripletBarMulitplier = quarterBarMulitplier * 0.66666666666667f;
        eighthTripletBarMulitplier  = eighthBarMulitplier * 0.66666666666667f;
        count = 0;
    }
    /**
     *  Start IntervalTimer
     */
    virtual void begin( void ) {
        innerBeatTime = 0;
        MasterTrack *p;
        for ( p = MasterTrack::first_track; p; p = p->next_track ) {
            p->begin( );
        }
        MasterTimer.begin( BeatTrack, beatTime/32 );
    }
    /**
     *  Pause Master Track and Tracks Threads
     */
    virtual void pause( void ) {
        __disable_irq( );
        MasterTimer.end( );
        __enable_irq( );
        MasterTrack *p;
        for ( p = MasterTrack::first_track; p; p = p->next_track ) {
            p->pause( );
        }
    }
    /**
     *  Resume Master Track and Track Threads
     */
    virtual void resume( void ) {
        MasterTrack *p;
        for ( p = MasterTrack::first_track; p; p = p->next_track ) {
            p->resume( );
        }
        MasterTimer.begin( BeatTrack, beatTime/32 );
    }
    /**
     *  Restart IntervalTimer and Track Threads
     */
    virtual void restart( void ) {
        __disable_irq( );
        MasterTimer.end( );
        volume        = 0;
        currentBar    = 0;
        currentBeat   = timeSignatureHigh - 1;
        __enable_irq( );
        MasterTrack *p;
        for ( p = MasterTrack::first_track; p; p = p->next_track ) {
            p->restart( );
        }
        MasterTimer.begin( BeatTrack, beatTime/32 );
    }
    /**
     *  Stop all threads - puts them in a returned state
     */
    virtual void stop( void ) {
        __disable_irq( );
        MasterTimer.end( );
        __enable_irq( );
        MasterTrack *p;
        for ( p = MasterTrack::first_track; p; p = p->next_track ) {
            p->stop( );
        }
    }
    /**
     *  change tempo of the Master Track, all tracks will follow
     *
     *  @param bpm beat per minute
     */
    void tempo( uint16_t bpm ) {
        __disable_irq( );
        MasterTimer.end( );
        beatTime                    = 60000000 / ( float )bpm;
        wholeBarMulitplier          = beatTime * timeSignatureHigh;
        halfBarMulitplier           = wholeBarMulitplier/2;
        quarterBarMulitplier        = wholeBarMulitplier/4;
        eighthBarMulitplier          = wholeBarMulitplier/8;
        sixteenthBarMulitplier      = wholeBarMulitplier/16;
        thritySecondBarMulitplier   = wholeBarMulitplier/32;
        quarterTripletBarMulitplier = quarterBarMulitplier * 0.66666666666667f;
        eighthTripletBarMulitplier  = eighthBarMulitplier * 0.66666666666667f;
        __enable_irq( );
        MasterTimer.begin( BeatTrack, beatTime/32 );
    }
    /**
     *  <#Description#>
     *
     *  @param bars <#beats description#>
     *
     *  @return <#return value description#>
     */
    int16_t rewind( uint16_t bars = 0 ) {
        
        uint16_t cbeat;
        int16_t cbar;
        __disable_irq( );
        MasterTimer.end( );
        __enable_irq( );
        
        MasterTrack *p;
        for ( p = MasterTrack::first_track; p; p = p->next_track ) {
            p->isBeat = false;
            p->isBar = false;
        }
        
        cbar = currentBar;
        
        if ( bars == 0 ) {
            cbeat = timeSignatureHigh - 1;
            cbar -= 1;
            if ( cbar <= 0 ) cbar = measures;
            currentBeat = cbeat;
            currentBar = cbar;
            count = 16;
        }
        else {
            cbeat = timeSignatureHigh - 1;
            cbar -= bars;
            if ( cbar <= 0 ) cbar = measures;
            currentBeat = cbeat;
            currentBar = cbar;
            count = 16;
        }
        MasterTimer.begin( BeatTrack, beatTime/32 );
        
        cbar += 1;
        if ( cbar > measures ) {
            cbar = 1;
        }
        return cbar;
    }
    /**
     *  <#Description#>
     *
     *  @param bars <#beats description#>
     *
     *  @return <#return value description#>
     */
    int16_t fastForward( uint16_t bars = 0 ) {
        uint16_t cbeat;
        int16_t cbar;
        __disable_irq( );
        MasterTimer.end( );
        __enable_irq( );
        
        MasterTrack *p;
        for ( p = MasterTrack::first_track; p; p = p->next_track ) {
            p->isBeat = false;
            p->isBar = false;
        }
        
        cbar = currentBar;
        
        if ( bars == 0 ) {
            cbeat = timeSignatureHigh - 1;
            cbar += 1;
            if ( cbar > measures ) cbar = 0;
            currentBeat = cbeat;
            currentBar = cbar;
            count = 16;
        }
        else {
            cbeat = timeSignatureHigh - 1;
            cbar += bars;
            if ( cbar > measures ) cbar = measures;
            currentBeat = cbeat;
            currentBar = cbar;
            count = 16;
        }
        MasterTimer.begin( BeatTrack, beatTime/32 );
        
        cbar += 1;
        if ( cbar > measures ) {
            cbar = 1;
        }
        return cbar;
    }
    
    //
    bool    onBeat( void );
    uint8_t getBeat( void );
    void    clearBeat( void );
    
    bool    onBar( void );
    uint8_t getBar( void );
    void    clearBar( void );
    /**
     *  Used by the TRACK_DELAY Macro, this starts the timer before any instrument is played.
     *
     *  @return true for the Macro
     */
    uint8_t trackDelayStart( void ) {
        innerBeatTime = 0;
        return 1;
    }
    /**
     *  Used by the TRACK_DELAY Macro, called after instrument is played, this delays the next
     *  instrument so they play in time.
     *
     *  @param duration how long to delay, usually use the 1/32, 1/16, 1/8, 1/4, 1/2, 1 beats
     *         of the measure.
     *
     *  @return false for the Macro
     */
    uint8_t trackDelayEnd( unsigned long duration ) {
        elapsedMicros time = innerBeatTime;
        unsigned long delayLength;
        switch ( duration ) {
            case WHOLE_BAR:
                delayLength = wholeBarMulitplier;
                break;
            case HALF_BAR:
                delayLength = halfBarMulitplier;
                break;
            case QUARTER_BAR:
                delayLength = quarterBarMulitplier;
                break;
            case EIGHTH_BAR:
                delayLength = eighthBarMulitplier;
                break;
            case SIXTEENTH_BAR:
                delayLength = sixteenthBarMulitplier;
                break;
            case THIRTY_SECOND_BAR:
                delayLength = thritySecondBarMulitplier;
                break;
            case QUATER_TRIPLET_BAR:
                delayLength = quarterTripletBarMulitplier;
                break;
            case EIGHTH_TRIPLET_BAR:
                delayLength = eighthTripletBarMulitplier;
                break;
            default:
                return 0;
                break;
        }
        while ( time < delayLength ) yield( );
        return 0;
    }
protected:
    MasterTrack( void ) {
        if (first_track == NULL) {
            first_track = this;
        } else {
            MasterTrack *p;
            for ( p = first_track; p->next_track; p = p->next_track ) ;
            p->next_track= this;
        }
        next_track = NULL;
    }
    /**
     *  beat and bar signals
     */
    volatile bool isBeat;
    volatile bool isBar;
    /**
     *  This sets up the Track Threads, the main thread is the loop function.
     *  Set at 2000 bytes but it might need to increase depending how many local
     *  variables are defined in the loop function.
     */
    static Thread masterTrackThread;// main task
private:
    MasterTrack             *next_track;
    static MasterTrack      *first_track;
    
    static volatile uint16_t currentBar;
    static volatile uint16_t currentBeat;
    elapsedMicros            innerBeatTime;
    
    static IntervalTimer MasterTimer;
    static void          BeatTrack( void );
    static float         volume;
    static float         beatTime;
    static keys          key;
    
    static volatile uint8_t  count;
    static volatile uint8_t  timeSignatureHigh;
    static volatile uint8_t  timeSignatureLow;
    static volatile uint16_t measures;
    
    static unsigned long wholeBarMulitplier;
    static unsigned long halfBarMulitplier;
    static unsigned long quarterBarMulitplier;
    static unsigned long eighthBarMulitplier;
    static unsigned long sixteenthBarMulitplier;
    static unsigned long thritySecondBarMulitplier;
    static unsigned long quarterTripletBarMulitplier;
    static unsigned long eighthTripletBarMulitplier;
};
// ****************************************************************************************** //
/**
 *  Inherits from the Master Track that does the time keeping.
 */
class Track : public MasterTrack {
private:
    using MasterTrack::trackDelayStart;
    using MasterTrack::trackDelayEnd;
    using MasterTrack::tempo;
    using MasterTrack::rewind;
    using MasterTrack::fastForward;
    
    typedef void ( * task_func_t )( void *arg );
    task_func_t trackThread;
    uint16_t threadSize;
    /**
     *  Restarts the Track thread from the beginning.
     */
    void restart( void ) {
        masterTrackThread.restart( trackThread );
    }
    /**
     *  Returns the Track thread.
     */
    void stop( void ) {
        masterTrackThread.stop( trackThread );
    }
    /**
     *  Creates and starts a Track Thread, these run independently of each other
     */
    void begin( void ) {
        masterTrackThread.create(trackThread, threadSize, 0);
    }
    
public:
    /**
     *  Constructer to setup Track threads and their sizes.
     */
    Track( task_func_t track, uint16_t size = 1000 ) {
        trackThread = track;
        threadSize  = size;
        isBeat      = false;
        isBar       = false;
    }
    /**
     *  Pauses the Track thread where it is.
     */
    void pause( void ) {
        masterTrackThread.pause( trackThread );
    }
    /**
     *  Resumes the Track thread where it was paused.
     */
    void resume( void ) {
        masterTrackThread.resume( trackThread );
    }
    
};
#endif /* defined(TeensyTracks_h_) */
