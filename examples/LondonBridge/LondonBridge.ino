/*
 This simple example plays the song "London Bridge" (My Fair Lady)
 by an unknown artist using the Audio library's AudioSynthKarplusStrong
 to synthesize a guitar. The sheet music this is found here:
 http://www.guitarnick.com/london-bridge-easy-guitar-song-for-beginners-tab.html
 
 TeensyTracks keeps everything in time so you can play songs from a
 collection of instruments you choose. This example only has one track
 that is used to play the melody of the song.
 
 Since this library uses cooperative multi threading for the track threads,
 do not use any dynamic memory operations. Things like String, prinf and such
 should not be used. All tracks must call the "yield()" function at least once
 per spin of any loops so delayMicroseconds does not yield and should be avoided
 if possible.
 */
#include <TeensyTracks.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "chords.h"

// Must declare this static function here to be visible to the
// track class. This function will serve as your track thread
// parameter to the track class. All static thread functions
// have a "void *arg" parameter.
static void guitarThread(void *arg);

// Number of bars this song has
#define NUM_MEASURES 8

// The tempo of the song in Beats Per Minute (BPM)
uint16_t myTempo = 200;

// Master Track class needs the tempo, key (for reference only now), number of measures or (bars),
// and Time Signatures, this one uses 4/4 time. It sets up all the timing for Track_Delays and
// configures the Master Timer (IntervalTimer) to fire every beat of the song. This Master Timer
// provides the basis for starting each Track Thread.
MasterTrack LondonBridges(myTempo, G, NUM_MEASURES, 4, 4);

// Track Threads first argument is one of the static void function defined above, then the Track is
// either called every beat or every bar and finally is the amount of stack space it needs. If you
// run into problems with the Tracks not playing weird things happening it could be the stack space
// needs to increase. Track Threads can either be called every beat or every bar.
Track guitarTrack(guitarThread, ON_BAR, 2000);

// Prop Shield enable
#define PROP_AMP_ENABLE    5

// Guitar
AudioSynthKarplusStrong   audioGuitar;
// mix for volume control
AudioMixer4               mixer;
// play through the DAC
AudioOutputAnalog         dac;
#ifdef AUDIO_INTERFACE
AudioOutputUSB            usb;
#endif
// guitar to mixer
AudioConnection in(audioGuitar, mixer);
// mixer to dac
AudioConnection out(mixer, dac);
#ifdef AUDIO_INTERFACE
AudioConnection USBoutL(mixer, 0, usb, 0);
AudioConnection USBoutR(mixer, 0, usb, 1);
#endif

void setup() {
    while (!Serial);
    delay(100);
    Serial.println("Starting...");
    AudioMemory(26);
    mixer.gain(0, .25);
    // if using prop shield?
    pinMode(PROP_AMP_ENABLE, OUTPUT);
    digitalWrite(PROP_AMP_ENABLE, HIGH);// Enable Amplifier
    // start master track and all tracks
    LondonBridges.begin();
}

/*
 * Loop is considered a Thread too but use it for your non Thread music code.
 * Make sure you call "yield();" in any for or while loops so to not starve the
 * the Threads of cpu time.
 */
void loop() {
    int c = 0;
    if (Serial.available()) {
        c = Serial.read();
    }
    
    if (c > 0) {
        // type '+' in serial monitor to increase tempo
        if (c == '+') {
            myTempo += 20;
            LondonBridges.tempo(myTempo);
            Serial.print("Increasing Tempo: ");
            Serial.println(myTempo);
        }
        // decrease tempo
        else if (c == '-') {
            myTempo -= 20;
            LondonBridges.tempo(myTempo);
            Serial.print("Decreasing Tempo: ");
            Serial.println(myTempo);
        }
    }
    // must yield for threads to work, no free spinning loops.
    yield();
}
/*******************************************************************************
 Guitar Track code goes below
 *******************************************************************************/
static void guitarThread(void *arg) {
    // "getBar" return the current bar we are on.
    uint8_t bar = guitarTrack.getBar();
    // since this song has 8 bars, we have 8 case statements. Bars start
    // with 1 not 0.
    switch (bar) {
        case 1:
            // Track_Delay macro accounts for how long in this case the "notOn"
            // takes to execute. The remainder is delayed for whatever part of
            // bar is chosen to delay. There are 7 delays 1, 1/2, 1/4, 1/8,
            // 1/16 and 1/32 of a bar. These correspond to the duration of a
            // note you see in sheet music. Track_Delays "yield" also.
            TRACK_DELAY(guitarTrack, QUARTER_BAR) {
                audioGuitar.noteOn(NOTE_D4, .6);
                Serial.print("Bar 1 -> D4  ");
            }
            TRACK_DELAY(guitarTrack, QUARTER_BAR) {
                audioGuitar.noteOn(NOTE_E4, .6);
                Serial.print("E4  ");
            }
            TRACK_DELAY(guitarTrack, QUARTER_BAR) {
                audioGuitar.noteOn(NOTE_D4, .6);
                Serial.print("D4  ");
            }
            audioGuitar.noteOn(NOTE_C4, .6);
            Serial.println("C4");
            
            break;
        case 2:
            TRACK_DELAY(guitarTrack, QUARTER_BAR) {
                audioGuitar.noteOn(NOTE_B3, .6);
                Serial.print("Bar 2 -> B3  ");
                
            }
            TRACK_DELAY(guitarTrack, QUARTER_BAR) {
                audioGuitar.noteOn(NOTE_C4, .6);
                Serial.print("C4  ");
            }
            audioGuitar.noteOn(NOTE_D4, .6);
            Serial.println("D4");
            
            break;
        case 3:
            TRACK_DELAY(guitarTrack, QUARTER_BAR) {
                audioGuitar.noteOn(NOTE_A3, .6);
                Serial.print("Bar 3 -> A2  ");
            }
            TRACK_DELAY(guitarTrack, QUARTER_BAR) {
                audioGuitar.noteOn(NOTE_B3, .6);
                Serial.print("B3  ");
            }
            audioGuitar.noteOn(NOTE_C4, .6);
            Serial.println("C4");
            break;
        case 4:
            TRACK_DELAY(guitarTrack, QUARTER_BAR) {
                audioGuitar.noteOn(NOTE_B3, .6);
                Serial.print("Bar 4 -> B3  ");
            }
            TRACK_DELAY(guitarTrack, QUARTER_BAR) {
                audioGuitar.noteOn(NOTE_C4, .6);
                Serial.print("C4  ");
            }
            audioGuitar.noteOn(NOTE_D4, .6);
            Serial.println("D4");
            break;
        case 5:
            TRACK_DELAY(guitarTrack, QUARTER_BAR) {
                audioGuitar.noteOn(NOTE_D4, .6);
                Serial.print("Bar 5 -> D4  ");
            }
            TRACK_DELAY(guitarTrack, QUARTER_BAR) {
                audioGuitar.noteOn(NOTE_E4, .6);
                Serial.print("E4  ");
            }
            TRACK_DELAY(guitarTrack, QUARTER_BAR) {
                audioGuitar.noteOn(NOTE_D4, .6);
                Serial.print("D4  ");
            }
            audioGuitar.noteOn(NOTE_C4, .6);
            Serial.println("C4");
            break;
        case 6:
            TRACK_DELAY(guitarTrack, QUARTER_BAR) {
                audioGuitar.noteOn(NOTE_B3, .6);
                Serial.print("Bar 6 -> B3  ");
            }
            TRACK_DELAY(guitarTrack, QUARTER_BAR) {
                audioGuitar.noteOn(NOTE_C4, .6);
                Serial.print("C4  ");
            }
            audioGuitar.noteOn(NOTE_D4, .6);
            Serial.println("D4");
            break;
        case 7:
            TRACK_DELAY(guitarTrack, HALF_BAR) {
                audioGuitar.noteOn(NOTE_A3, .6);
                Serial.print("Bar 7 -> A3      ");
            }
            audioGuitar.noteOn(NOTE_D4, .6);
            Serial.println("G3");
            break;
        case 8:
            TRACK_DELAY(guitarTrack, HALF_BAR) {
                audioGuitar.noteOn(NOTE_G3, .6);
                Serial.println("Bar 8 -> G3");
            }
            break;
    }
    // must yield here
    yield();
}