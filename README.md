# TeensyTracks v0.0.2

<h4>TeensyTracks is a multi track library inspired by Apple's Garage Band software using Arduino for Teensy 3.x.</h4>

---
TeensyTracks provides the timing for laying multiple audio tracks to play in unison. Each track is independent from each other but all tracks are linked to the Master Track which handles tempo, key and time signature. With TeensyTracks you can pause each individual track and resume it or you can pause, end, resume or restart the Master Track in which all Tracks will follow. Also you can dynamically change the tempo of the Master Track which controls the tempo of all tracks. This library can be used with Paul's Audio Library but is not bound to it so you can use it for other audio software if desired. At the current moment TeensyTracks only handles 4/4 time signature and the key is only for reference.
<br>
---
<h4>TeensyTracks Usage</h4>
To use TeensyTracks you also must install my [Zilch] library which is a simple scheduler library for Teensy's. Since you need this requirement you must not use any dynamic memory operations since they will play havoc with Zlich library, all memory usage must be static. While some people might ask why use a threaded environment for Tracks it makes it much easier to have independent tracks play along with each other like Garage Band does. Without these threads your programs would become in my opinion horribly complicated to manage and syncing all the Tracks to each other would be a nightmare.
<br>
<h4>How it works - </h4>
Mostly a way more pain in the ass way of having very simple garage band functionalities, but really there are three concepts that must be explained for you to layout your music score instruments to sound in time with each other.

1. Master Track - Provides the timing and produces timing signals for tracks.
2. Tracks - Where you make music using the timing from the MasterTrack.
3. Track Delays - These are what makes your instrument play in logical and musical manner in a track.

---
<h4>Master Track</h4>
Uses the IntervalTimer method to fire every 1/32 of one beat at a given tempo. The MasterTrack Class uses this to provide the timing and signals to the tracks for any instrument delays and/or signals on the beat and/or bar. It also keeps the current beat and bar for use in tracks and repeats after so many bar have elapsed in which you choose. 

---
<h4>Tracks</h4>
Is linked to the MasterTrack Class and uses a multi threaded library (Zlich) to run the track threads. These threads need to 'yield' to other threads and use a round robin 'context switch' with no preemption but switch between themselves very fast. This makes it seem as each track thread is running in parallel and since musical timing is so slow compared to the 'context switch' between threads, instruments sound in time with other track instruments. For example say you want to play a snare drum wav sample at the beginning of the 2 and 4 bars for a blues piece. You would use the 'onBar' signal from the master track  to tell you when the 2 or 4 bar happens and then play your snare sample.

---
<h4>Track Delays</h4>
Provides inner beat delays that correspond to standard musical note timings. Say for example you want to play a certain guitar chord using Audio Library's AudioSynthKarplusStrong feature in which you want it to strum 4 times per beat. This would correspond to strumming some chord 4 Quarter Note delays in one beat. This is the way music is played, this allow you to synthesize real music with these delays.

[Zilch]:https://github.com/duff2013/Zilch