/*
 * This example plays the Song Imagine by John Lennon using
 * the Audio library's AudioSynthKarplusStrong for the guitar
 * and bass and WAV drum samples using the prop shield.
 *
 * It expands on the "Guitar" and "SamplePlayer" examples
 * from the Audio library examples. TeensyTracks keeps
 * everything in time so you can play songs from a collection
 * of instruments you choose. Since each of the Track threads
 * inherit from the MasterTrack class you can do things like
 * pause, restart or speed or slow down the song. You can also
 * pause and resume individual tracks. This example shows you
 * how to do that. Look at the loop function for this.
 *
 * Since this library uses a scheduling library for the Track
 * Threads do not use any dynamic memory operations. Things like
 * String, prinf and such should not be used sorry.
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
// independent of each other by using the scheduling library Zilch. These
// Tracks get there timing from the MasterTrack Class.
static void hihat(void *arg);
static void kick(void *arg);
static void snare(void *arg);
static void guitar(void *arg);
static void soloGuitar(void *arg);
static void bass(void *arg);
// Number of measure this song has
#define NUM_MEASURES 20
// The tempo of the song in Beats Per Minute (BPM)
uint8_t myTempo = 85;
// Master Track class needs the tempo, key (for reference only now) number of measures or (bars),
// and Time Signatures this one uses 4/4
MasterTrack Imagine(myTempo, C, NUM_MEASURES, 4, 4);
// Track Threads, first argument is the static void function and the amount of stack space it
// needs. If you run into problems with the Tracks not playing weird things happening it could
// be the stack space needs to increase.
Track Drum1(hihat, 1000);
Track Drum2(kick, 1000);
Track Drum3(snare, 1000);
Track Guitar(guitar, 1000);
Track SoloGuitar(soloGuitar, 1000);
Track Bass(bass, 1000);

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

AudioConnection a1(audioHihat, 0, mix1, 0);
AudioConnection a2(audioKick,  0, mix1, 1);
AudioConnection a3(audioSnare, 0, mix1, 2);
AudioConnection a4(E2string,  0, mix2, 1);
AudioConnection a5(A2string,  0, mix2, 2);
AudioConnection a6(D3string,  0, mix2, 3);
AudioConnection a7(G3string, 0, mix3, 1);
AudioConnection a8(B3string, 0, mix3, 2);
AudioConnection a9(E4string, 0, mix3, 3);
AudioConnection a10(soloStrings, 0, chorus, 0);
AudioConnection a11(bassStrings, 0, bassBiquad, 1);
AudioConnection a12(bassBiquad, 0, mix4, 1);
AudioConnection a13(chorus, 0, mix4, 2);

AudioConnection m1(mix1,  0, mix2, 0);
AudioConnection m2(mix2,  0, mix3, 0);
AudioConnection m3(mix3,  0, mix4, 0);
AudioConnection out(mix4, 0, dac,  0);

const float GUITAR_CHORDS_GAIN = 0.07;
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
    mix1.gain(1, .75);
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
    mix4.gain(2, .25);
    /****Bass Guitar****/
    mix4.gain(1, .45);
    
    // set low pass for bass guitar.
    //bassBiquad.setLowpass(0, 110, 0.707);
    bassBiquad.setLowpass(0, 110, 0.54);
    bassBiquad.setLowpass(1, 110, 1.3);
    bassBiquad.setLowpass(2, 110, 0.54);
    bassBiquad.setLowpass(3, 110, 1.3);
    
    chorus.voices(n_chorus);
    chorus.begin(delayline, CHORUS_DELAY_LENGTH, n_chorus);
    
    AudioProcessorUsageMaxReset();
    AudioMemoryUsageMaxReset();
    
    // if using prop shield?
    pinMode(PROP_AMP_ENABLE, OUTPUT);
    digitalWrite(PROP_AMP_ENABLE, HIGH);// Enable Amplifier
    
    Drum1.begin(); // Start hihat Track Thread
    Drum2.begin(); // Start kick drum Track Thread
    Drum3.begin(); // Start snare drum Track Thread
    Guitar.begin(); // Start rhythm guitar Track Thread
    SoloGuitar.begin(); // Start solo guitar Track Thread
    Bass.begin(); // Start bass guitar Track Thread
    Imagine.begin(); // Start Master Track Timer
    usageTimer = 0;
}

// Loop is concidered a Thread too.  
void loop() {
    if (usageTimer >= 100) {
        Serial.print(AudioProcessorUsage());
        Serial.print(" ");
        Serial.print(AudioProcessorUsageMax());
        Serial.print(" ");
        Serial.print(AudioMemoryUsage());
        Serial.print(" ");
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
            Drum1.pause();
        }
        else if (c == '2') {
            Serial.println("Pausing Kick Drum");
            Drum2.pause();
        }
        else if (c == '3') {
            Serial.println("Pausing Snare Drum");
            Drum3.pause();
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
            Imagine.end();
        }
        else if (c == 'A') {
            Serial.println("Resuming Hihat Drum");
            Drum1.resume();
        }
        else if (c == 'B') {
            Serial.println("Resuming Kick Drum");
            Drum2.resume();
        }
        else if (c == 'C') {
            Serial.println("Resuming Snare Drum");
            Drum3.resume();
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
            Serial.println("Resuming Guitar Solo");
            Imagine.resume();
        }
        else if (c == 'G') {
            Serial.println("Restarting Master Track");
            Imagine.restart();
        }
        else if (c == '+') {
            Imagine.tempo(++myTempo);
            Serial.print("Increasing Tempo: ");
            Serial.println(myTempo);
        }
        else if (c == '-') {
            Imagine.tempo(--myTempo);
            Serial.print("Decreasing Tempo: ");
            Serial.println(myTempo);
        }
    }
    // even loop needs to yield to keep the 'contex switch' happy.
    yield();
}
// -----------------------------------------------------------------------------
