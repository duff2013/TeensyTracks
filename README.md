# TeensyTracks v0.0.1

<h4>TeensyTracks is a multi track library inspired by Apple's Garage Band software.</h4>

---
TeensyTracks provides the timing for laying multiple audio tracks to play in unison. Each track is independent from each other but all tracks are linked to the Master Track which handles tempo, key and time signature. With TeensyTracks you can pause each individual track and resume it or you can pause, end, resume or restart the Master Track in which all Tracks will follow. Also you can dynamically change the tempo of the Master Track which controls the tempo of all tracks. This library can be used with Paul's Audio Library but is not bound to it so you can use it for other audio software if desired. At the current moment TeensyTracks only handles 4/4 time signature and the key is only for reference.
<br>
---
<h4>TeensyTracks Usage</h4>
To use TeensyTracks you also must install my [Zilch] library which is a simple scheduler library for Teensy's. Since you need this requirement you must not use any dynamic memory operations since they will play havoc with Zlich library, all memory usage must be static. While some people might ask why use a threaded environment for Tracks it makes it much easier to have independent tracks play along with each other like Garage Band does. Without these threads your programs would become in my opinion horribly complicated to manage and syncing all the Tracks to each other would be a nightmare.
<br>
[Zilch]:https://github.com/duff2013/Zilch