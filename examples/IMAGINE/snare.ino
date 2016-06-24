/*
 * Snare Drum Track Thread
 */
static void snare(void *arg) {
  while (!Drum3.EndTrack) {
    uint8_t bar = Drum3.getBar();
    if  (Drum3.onBar()) {
      if (bar == 12) {
        TRACK_DELAY(Imagine, WHOLE_BAR);
        Drum3.clearBar();
        continue;
      }
      else if (bar == 20) {
        TRACK_DELAY(Imagine, HALF_BAR);
        TRACK_DELAY(Imagine, QUARTER_BAR);
        TRACK_DELAY(Imagine, EIGHTH_BAR);
        TRACK_DELAY(Imagine, SIXTEENTH_BAR) audioSnare.play(snareRaw);
        TRACK_DELAY(Imagine, SIXTEENTH_BAR) audioSnare.play(snareRaw);
        audioSnare.play(snareRaw);
        Drum3.clearBar();
        continue;
      }
      Drum3.clearBar();
    }

    if (Drum3.onBeat()) {
      uint8_t beat = Drum3.getBeat();
      switch (beat) {
        case 1:
          break;
        case 2:
          audioSnare.play(snareRaw);
          break;
        case 3:
          break;
        case 4:
          TRACK_DELAY(Imagine, SIXTEENTH_BAR) audioSnare.play(snareRaw);
          audioSnare.play(snareRaw);
          break;
      }
      Drum3.clearBeat();
    }
    yield();
  }
}
