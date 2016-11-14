/*
 This example plays the Song Imagine by John Lennon using
 the Audio library's AudioSynthKarplusStrong for the guitar
 and WAV drum samples using the prop shield using the DAC
 and the USB AUDIO if it is complied using USB->Audio. This
 means you can listen to it through your computer, nice!
 
 It expands on the "Guitar" and "SamplePlayer" examples
 from the Audio library examples. TeensyTracks keeps
 everything in time so you can play songs from a collection
 of instruments you choose. Since each of the Track threads
 inherit from the MasterTrack class you can do things like
 pause, restart or speed up or slow down the song. You can
 also pause and resume individual tracks. This example shows
 you how to do that. Look at the loop function for this.
 
 Since this library uses a scheduling library for the Track
 Threads do not use any dynamic memory operations. Things like
 String, prinf and such should not be used sorry.
 */
#include <TeensyTracks.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include "drums.h"
#include "chords.h"
// Must declare static Track Threads here to be visible to the Track class.
// See other tabs for the Track Thread code. Each one of these Tracks runs
// independent of each other by using a special version of the scheduling
// library, Zilch.
static void hihat(void *arg);
static void kick(void *arg);
static void snare(void *arg);
static void guitar(void *arg);
static void soloGuitar(void *arg);

// Number of measure this song has
#define NUM_MEASURES 20

// The tempo of the song in Beats Per Minute (BPM)
uint8_t myTempo = 85;

// Master Track class needs the tempo, key (for reference only now), number of measures or (bars),
// and Time Signatures, this one uses 4/4 time. It sets up all the timing for Track_Delays and
// configures the Master Timer (IntervalTimer) to fire every beat of the song. This Master Timer
// provides the basis for starting each Track Thread.
MasterTrack Imagine(myTempo, C, NUM_MEASURES, 4, 4);

// Track Threads first argument is one of the static void function defined above, then the Track is
// either called every beat or every bar and finally is the amount of stack space it needs. If you
// run into problems with the Tracks not playing weird things happening it could be the stack space
// needs to increase. Track Threads can either be called every beat or every bar.
Track DrumHiHat(hihat,       ON_BEAT, 1000);
Track DrumKick(kick,         ON_BEAT, 1000);
Track DrumSnare(snare,       ON_BEAT, 1000);
Track Guitar(guitar,         ON_BAR,  1000);
Track SoloGuitar(soloGuitar, ON_BAR,  1000);

// Prop Shield and Mic enables
#define PROP_AMP_ENABLE    5

#define CHORUS_DELAY_LENGTH (16*AUDIO_BLOCK_SAMPLES)
// Chorus for guitar Solo
AudioEffectChorus         chorus;
// For Bass
AudioFilterBiquad         bassBiquad;
// Rhythm guitar
AudioSynthKarplusStrong   E2string;
AudioSynthKarplusStrong   A2string;
AudioSynthKarplusStrong   D3string;
AudioSynthKarplusStrong   G3string;
AudioSynthKarplusStrong   B3string;
AudioSynthKarplusStrong   E4string;
AudioSynthKarplusStrong   soloStrings;
AudioSynthKarplusStrong   bassStrings;
// wav drum samples
AudioPlayMemory           audioHihat;
AudioPlayMemory           audioKick;
AudioPlayMemory           audioSnare;

AudioMixer4               mix1;
AudioMixer4               mix2;
AudioMixer4               mix3;
AudioMixer4               mix4;
AudioOutputAnalog         dac;  // play to DAC
#ifdef AUDIO_INTERFACE
AudioOutputUSB            usb;
#endif

AudioConnection a1(audioHihat,   0, mix1,   0);
AudioConnection a2(audioKick,    0, mix1,   1);
AudioConnection a3(audioSnare,   0, mix1,   2);
AudioConnection a4(E2string,     0, mix2,   1);
AudioConnection a5(A2string,     0, mix2,   2);
AudioConnection a6(D3string,     0, mix2,   3);
AudioConnection a7(G3string,     0, mix3,   1);
AudioConnection a8(B3string,     0, mix3,   2);
AudioConnection a9(E4string,     0, mix3,   3);
AudioConnection a10(soloStrings, 0, chorus, 0);
AudioConnection a11(chorus,      0, mix4,   1);

AudioConnection m1(mix1,  0, mix2, 0);
AudioConnection m2(mix2,  0, mix3, 0);
AudioConnection m3(mix3,  0, mix4, 0);
AudioConnection out(mix4, 0, dac,  0);
#ifdef AUDIO_INTERFACE
AudioConnection USBoutL(mix4, 0, usb,  0);
AudioConnection USBoutR(mix4, 0, usb,  1);
#endif

const float GUITAR_CHORDS_GAIN = 0.05;
short delayline[CHORUS_DELAY_LENGTH];
int n_chorus = 1;
elapsedMillis usageTimer;

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    while (!Serial);
    delay(700);
    Serial.println("Start...");
    AudioMemory(26);
    /***Hihat Drum***/
    mix1.gain(0, 0.07);
    /***Kick Drum***/
    mix1.gain(1, .25);
    /***Snare Drum***/
    mix1.gain(2, 0.23);
    /****Guitar Chords***/
    mix2.gain(1, GUITAR_CHORDS_GAIN);
    mix2.gain(2, GUITAR_CHORDS_GAIN);
    mix2.gain(3, GUITAR_CHORDS_GAIN);
    mix3.gain(1, GUITAR_CHORDS_GAIN);
    mix3.gain(2, GUITAR_CHORDS_GAIN);
    mix3.gain(3, GUITAR_CHORDS_GAIN);
    /****Guitar Solo****/
    mix4.gain(1, .18);
    
    chorus.voices(n_chorus);
    chorus.begin(delayline, CHORUS_DELAY_LENGTH, n_chorus);
    
    AudioProcessorUsageMaxReset();
    AudioMemoryUsageMaxReset();
    
    // if using prop shield?
    pinMode(PROP_AMP_ENABLE, OUTPUT);
    digitalWrite(PROP_AMP_ENABLE, HIGH);// Enable Amplifier
    // Master Track begin() starts all tracks.
    Imagine.begin();
    usageTimer = 0;
}

/*
 Loop is considered a Thread too but use it for your non Thread music code.
 Make sure you call "yield();" in any for or while loops so to not starve the
 the Threads of cpu time.
 */
void loop() {
    if (usageTimer >= 5000) {
        Serial.print("Processor Usage: ");
        Serial.print(AudioProcessorUsage());
        Serial.print(" | Processor Usage Max: ");
        Serial.print(AudioProcessorUsageMax());
        Serial.print(" | Memory Usage: ");
        Serial.print(AudioMemoryUsage());
        Serial.print(" | Memory Usage Max: ");
        Serial.println(AudioMemoryUsageMax());
        usageTimer = 0;
    }
    
    int c;
    if (Serial.available()) {
        c = Serial.read();
    }
    
    if (c >= 0) {
        if (c == '1') {
            Serial.println("Pausing Hihat Drum");
            DrumHiHat.pause();
        }
        else if (c == '2') {
            Serial.println("Pausing Kick Drum");
            DrumKick.pause();
        }
        else if (c == '3') {
            Serial.println("Pausing Snare Drum");
            DrumSnare.pause();
        }
        else if (c == '4') {
            Serial.println("Pausing Rhythm Guitar");
            Guitar.pause();
        }
        else if (c == '5') {
            Serial.println("Pausing Guitar Solo");
            SoloGuitar.pause();
        }
        else if (c == '6') {
            Serial.println("Pausing Master Track");
            Imagine.pause();
        }
        else if (c == '7') {
            Serial.println("Stopping Master Track");
            Imagine.pause();
        }
        else if (c == 'A') {
            Serial.println("Resuming Hihat Drum");
            DrumHiHat.resume();
        }
        else if (c == 'B') {
            Serial.println("Resuming Kick Drum");
            DrumKick.resume();
        }
        else if (c == 'C') {
            Serial.println("Resuming Snare Drum");
            DrumSnare.resume();
        }
        else if (c == 'D') {
            Serial.println("Resuming Rhythm Guitar");
            Guitar.resume();
        }
        else if (c == 'E') {
            Serial.println("Resuming Guitar Solo");
            SoloGuitar.resume();
        }
        else if (c == 'F') {
            Serial.println("Resuming all Paused Tracks");
            Imagine.resume();
        }
        else if (c == 'G') {
            Serial.println("Restarting all Tracks from beginning");
            Imagine.restart();
        }
        else if (c == '+') {
            Imagine.tempo(++myTempo);
            Serial.print("Increasing Tempo to: ");
            Serial.println(myTempo);
        }
        else if (c == '-') {
            Imagine.tempo(--myTempo);
            Serial.print("Decreasing Tempo to: ");
            Serial.println(myTempo);
        }
    }
    // even loop needs to yield to keep the 'context switch' happy.
    yield();
}
// -----------------------------------------------------------------------------