/*
 * Kick Drum Track Thread
 */
static void kick(void *arg) {
  while (!Drum2.EndTrack) {
    uint8_t bar = Drum2.getBar();
    if  (Drum2.onBar()) {
      if (bar == 12 || bar == 20) {
        TRACK_DELAY(Imagine, WHOLE_BAR);
        Drum2.clearBar();
        continue;
      }
      Drum2.clearBar();
    }

    if (Drum2.onBeat()) {
      uint8_t beat = Drum2.getBeat();
      switch (beat) {
        case 1:
          TRACK_DELAY(Imagine, EIGHTH_BAR) audioKick.play(kickRaw);
          audioKick.play(kickRaw);
          break;
        case 2:
          audioKick.play(kickRaw);
          break;
        case 3:
          TRACK_DELAY(Imagine, EIGHTH_BAR) audioKick.play(kickRaw);
          audioKick.play(kickRaw);
          break;
        case 4:
          audioKick.play(kickRaw);
          break;
      }
      Drum2.clearBeat();
    }
    yield();
  }
}
