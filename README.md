# TeensyTracks v0.1.0

<h4>TeensyTracks is a multi track library inspired by Apple's Garage Band.</h4>

---
TeensyTracks provides the timing for laying multiple audio tracks to play in unison. Each track runs independent from each other but all tracks are linked to the Master Track which handles tempo, key and time signature. With TeensyTracks you can pause each individual track and resume it or you can pause, stop, resume or restart the Master Track in which all Tracks will follow. Also you can dynamically change the tempo of the Master Track which controls the tempo of all tracks. This library can be used with Paul's Audio Library but is not bound to it so you can use it for other audio software if desired. At the current moment TeensyTracks only handles 4/4 time signature and the key is only for reference. Some more features I want to integrate into this library:

1. Live recording and playback, in effect make a looper track using the Audio Library.
2. Make tracks dynamic in the sense I can add or subtract tracks from the sketch in realtime. Not related to dynamic memory allocations.
3. Incorperate a specialized version of Zlich into TeensyTracks, no need to install Zlich also. (Done!)
4. Fast forward and rewind. (Done!)
5. Allow an 2 beat intro for tracks.

---
<h4>TeensyTracks Usage</h4>
TeensyTracks uses a simple scheduler library for Teensy's and since you need this requirement you must not use any dynamic memory operations since they will create havoc with the stack allocations, so no String, malloc, printf and such also! While some people might ask why use a threaded environment for Tracks it makes it much easier to have independent tracks play along with each other like Garage Band does. Without these threads your programs would become in my opinion horribly complicated to manage and syncing all the Tracks to each other would be a nightmare.

---
<h4>How it works - </h4>
With a piece of sheet music like below:
![alt text](http://www.guitarnick.com/images/london-bridge-easy-guitar-sheet-music.png "London Bridge sheet music")<br>
You can see what notes make up the melody of this song, the time signature it uses and how many bars. Teensy Tracks works the same way using each of those parameters along with what tempo you want. There are three concepts that must be explained for you to layout your tracks to sound in time with each other.

1. Master Track - Provides the timing and produces timing signals for tracks (bar and beat).
2. Tracks - Where you play instrument samples, sounds etc.. using timing from the Master Track.
3. Track Delays - These are what makes your instrument play in logical and musical manner.

---
<h4>Master Track</h4>
Uses the IntervalTimer method to fire every 1/32 of one beat at a given tempo. The MasterTrack class uses this to provide the timing and signals to the tracks for any instrument delays and/or signals on the beat and/or bar. 

---
<h4>Tracks</h4>
Inherits from the MasterTrack class a multi threaded enviroment along with the timing signals to run the track threads in time with each other. These threads (Tracks) need to 'yield' to other threads(Tracks) that use a round robin 'context switch' with no preemption but the switch between themselves is very fast. This makes it seem as each track thread is running in parallel with each other and since musical timing is so slow compared to the 'context switch' between threads, instruments sound in time. For example say you want to play a blues type snare. You create track and use the Wav snare clip and monitor for the "onBar" signal and have it play at the beginning of bar 2 and 4. 

---
<h4>Track Delays</h4>
Provides inner bar delays that correspond to standard musical note timings. Say for example you want to play a certain guitar chord using Audio Library's AudioSynthKarplusStrong feature in which you want it to strum 4 times per bar. This would correspond to strumming some chord 4 Quarter Note delays in one bar. This is the way music is played, this allow you to layout music like you would write it for sheet music.

[Zilch]:https://github.com/duff2013/Zilch
