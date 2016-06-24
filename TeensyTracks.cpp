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

MasterTrack     * MasterTrack::first_track = NULL;
Zilch             MasterTrack::masterTrackThread( 2000 );

unsigned long MasterTrack::wholeBarMulitplier;
unsigned long MasterTrack::halfBarMulitplier;
unsigned long MasterTrack::quarterBarMulitplier;
unsigned long MasterTrack::eigthBarMulitplier;
unsigned long MasterTrack::sixteenthBarMulitplier;
unsigned long MasterTrack::thritySecondBarMulitplier;

/**
 *  Track timing control provided by the IntervalTimer.
 */
void MasterTrack::BeatTrack( void ) {
    
    if ( ++count > 32 ) {
        digitalWriteFast(LED_BUILTIN, !digitalReadFast(LED_BUILTIN));
        bool bar = false;
        if ( ++currentBeat >= ( timeSignatureHigh + 1 ) ) {
            if ( ++currentBar >= ( measures + 1 ) ) {
                currentBar = 1;
            };
            bar = true;
            currentBeat = 1;
        }
        
        count = 1;
        
        if ( currentBar == 0 ) return;
        
        MasterTrack *p;
        for ( p = MasterTrack::first_track; p; p = p->next_track ) {
            p->isBeat = true;
            p->isBar = bar;
        }
        
    }
}
/**
 *  Lets users know when the beat happens
 *
 *  @return true when a new beat
 */
bool MasterTrack::onBeat( void ) {
    const bool ob = isBeat;
    return ob;
}
/**
 *  Current beat based off the Time Signature
 *
 *  @return current beat
 */
uint8_t MasterTrack::getBeat( void ) {
    const uint8_t beat = currentBeat;
    return beat;
}
/**
 *  User must clear the isBeat var for the upcoming beat
 */
void MasterTrack::clearBeat( void ) {
    const bool beat = false;
    isBeat = beat;
}
/**
 *  Lets users know when the bar happens
 *
 *  @return true when a new bar, user must clear with clearBar
 */
bool MasterTrack::onBar( void ) {
    const bool ob = isBar;
    return ob;
}
/**
 *  Current bar based off the Time Signature and beats
 *
 *  @return current bar
 */
uint8_t MasterTrack::getBar( void ) {
    return currentBar;
}
/**
 *  User must clear the isBar var for the upcoming beat
 */
void MasterTrack::clearBar( void ) {
    const bool bar = false;
    isBar = bar;
}
