/*
 * Hihat Track Thread
 */
static void hihat(void *arg) {
  while (!Drum1.EndTrack) {
    uint8_t bar = Drum1.getBar();
    if  (Drum1.onBar()) {
      if (bar == 12 || bar == 20) {
        TRACK_DELAY(Imagine, WHOLE_BAR);
        Drum1.clearBar();
        continue;
      }
      Drum1.clearBar();
    }

    if (Drum1.onBeat()) {
      uint8_t beat = Drum1.getBeat();
      switch (beat) {
        case 1:
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
      Drum1.clearBeat();
    }
    yield();
  }
}
