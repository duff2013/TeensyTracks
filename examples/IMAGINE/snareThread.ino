/*
   Snare Drum Track Thread -
*/
static void snare(void *arg) {
  while (1) {

    // 'onBar' is true when the MasterTrack signals the
    // current beat is bar also. Must clear with 'clearBar'
    // method at the end of the if statement.
    uint8_t bar = Drum3.getBar();
    if (Drum1.onBar()) {
      // get current bar
      uint8_t bar = Drum1.getBar();
      switch (bar) {
        case 12:
          TRACK_DELAY(Imagine, WHOLE_BAR);
          break;
        case 20:
          // Track delay some fraction of the bar corresponding to
          // musical note duration.
          TRACK_DELAY(Imagine, HALF_BAR);
          TRACK_DELAY(Imagine, QUARTER_BAR);
          TRACK_DELAY(Imagine, EIGHTH_BAR);
          // Track delay take into account for how long this Audio Libaray
          // function takes. How long this fuction takes to excute must be less
          // the than this Track_Delay, which in this case is 1/16 of the bar.
          // This allows you too play the next snare hit in musical time for the
          // tempo you are using.
          TRACK_DELAY(Imagine, SIXTEENTH_BAR) audioSnare.play(snareRaw);
          TRACK_DELAY(Imagine, SIXTEENTH_BAR) audioSnare.play(snareRaw);
          audioSnare.play(snareRaw);
          break;
        default:

          break;
      }
      Drum1.clearBar();
    }
    
    // 'onBeat' is true when the MasterTrack signals on the
    // current beat. Must clear with 'clearBeat' method at
    // the end of the if statement.
    if (Drum3.onBeat()) {
      // get current beat count
      uint8_t beat = Drum3.getBeat();
      // play the snare on the 2 and 4 beat of the bar.
      switch (beat) {
        case 1:
          break;
        case 2:
          audioSnare.play(snareRaw);
          break;
        case 3:
          break;
        case 4:
          // a little sizzle to you playing
          TRACK_DELAY(Imagine, SIXTEENTH_BAR) audioSnare.play(snareRaw);
          audioSnare.play(snareRaw);
          break;
      }
      // clearing so we can get next 'onBeat' signal.
      Drum3.clearBeat();
    }
    // all tracks must yield every iteration of the while loop
    yield();
  }
}
