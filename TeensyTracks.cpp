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
#include "TeensyTracks.h"

float         MasterTrack::volume;
float         MasterTrack::beatTime;
keys          MasterTrack::key;

IntervalTimer MasterTrack::MasterTimer;

volatile uint8_t  MasterTrack::count;
volatile uint8_t  MasterTrack::timeSignatureHigh;
volatile uint8_t  MasterTrack::timeSignatureLow;
volatile uint16_t MasterTrack::currentBar;
volatile uint16_t MasterTrack::currentBeat;
volatile uint16_t MasterTrack::measures;

MasterTrack * MasterTrack::first_track = NULL;
Thread        MasterTrack::masterTrackThreadClass( 2000 );
unsigned long MasterTrack::wholeBarMulitplier;
unsigned long MasterTrack::halfBarMulitplier;
unsigned long MasterTrack::quarterBarMulitplier;
unsigned long MasterTrack::eighthBarMulitplier;
unsigned long MasterTrack::sixteenthBarMulitplier;
unsigned long MasterTrack::thritySecondBarMulitplier;
unsigned long MasterTrack::quarterTripletBarMulitplier;
unsigned long MasterTrack::eighthTripletBarMulitplier;
volatile bool MasterTrack::isBeat;
volatile bool MasterTrack::isBar;
//elapsedMicros MasterTrack::beatMicros;
//elapsedMicros MasterTrack::barMicros;

/**
 *  Track timing control provided by the IntervalTimer.
 */
void MasterTrack::BeatTrack( void ) {
    //beatMicros = 0;
    bool bar = false;
    bool beat = false;
    
    if ( ++currentBeat >= ( timeSignatureHigh + 1 ) ) {
        if ( ++currentBar >= ( measures + 1 ) ) currentBar = 1;
        bar = true;
        currentBeat = 1;
        masterTrackThreadClass.restart_all( );
        //barMicros = beatMicros;
    }
    
    if ( currentBar == 0 ) return;
    
    isBeat = true;
    isBar = bar;
    if ( !bar ) {
        MasterTrack *p;
        for ( p = MasterTrack::first_track; p; p = p->next_track ) {
            uint8_t thRunType = p->threadRunType;
            if ( thRunType == ON_BEAT ) p->restart( );
        }
    }
}

/**
 *  Start off everything
 */
void MasterTrack::begin( void ) {
    innerBeatTime = 0;
    MasterTrack *p;
    for ( p = MasterTrack::first_track; p; p = p->next_track ) p->begin( );
    delayMicroseconds( 100 );
    MasterTimer.priority( 192 );
    MasterTimer.begin( BeatTrack, beatTime );
}