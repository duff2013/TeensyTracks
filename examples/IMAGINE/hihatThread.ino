/*
   Hihat Track Thread -
*/
static void hihat(void *arg) {
  while (1) {

    if (Drum1.onBar()) {
      // get current bar
      uint8_t bar = Drum1.getBar();
      switch (bar) {
        case 12:
          TRACK_DELAY(Imagine, WHOLE_BAR);
          break;
        case 20:
          TRACK_DELAY(Imagine, WHOLE_BAR);
          break;
        default:
        
          break;
      }
      Drum1.clearBar();
    }

    // 'onBeat' is true when the MasterTrack signals on the
    // current beat. Must clear with 'clearBeat' method at
    // the end of the if statement.
    if (Drum1.onBeat()) {
      // get current beat count
      uint8_t beat = Drum1.getBeat();
      // play the snare on the 2 and 4 beat of the bar.
      switch (beat) {
        case 1:
          // Track delay take into account for how long this Audio Libaray
          // function takes. How long this fuction takes to excute must be less
          // the than this Track_Delay, which in this case is 1/16 of the bar.
          // This allows you too play the next snare hit in musical time for the
          // tempo you are using.
          TRACK_DELAY(Imagine, EIGHTH_BAR) audioHihat.play(hihatRaw);
          audioHihat.play(hihatRaw);
          break;
        case 2:
          TRACK_DELAY(Imagine, EIGHTH_BAR) audioHihat.play(hihatRaw);
          audioHihat.play(hihatRaw);
          break;
        case 3:
          TRACK_DELAY(Imagine, EIGHTH_BAR) audioHihat.play(hihatRaw);
          audioHihat.play(hihatRaw);
          break;
        case 4:
          TRACK_DELAY(Imagine, EIGHTH_BAR) audioHihat.play(hihatRaw);
          audioHihat.play(hihatRaw);
          break;
      }
      // clearing so we can get next 'onBeat' signal.
      Drum1.clearBeat();
    }
    // all tracks must yield every iteration of the while loop
    yield();
  }
}
