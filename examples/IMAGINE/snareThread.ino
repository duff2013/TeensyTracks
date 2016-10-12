/*
   Snare Drum Track Thread -
*/
static void snare(void *arg) {
  // get current beat count
  uint8_t beat = Drum3.getBeat();
  // get current bar
  uint8_t bar = Drum3.getBar();

  Serial.print("snare bar: ");
  Serial.println(bar);

  switch (bar) {
    case 12:
      yield();
      return;
      break;
    case 20:
      // 
      if (beat == 4) {
        // Track_Delay's are a fraction of the bar corresponding to a
        // musical notes duration and accounts for how long an Audio Library
        // function takes. How long this function takes to execute (audioSnare.play)
        // must be less the than the Track_Delay, which in this case is 1/16 of 
        // the bar. This allows you too play the next snare hit in musical time 
        // for the tempo you are using. Since this track executes every beat and
        // played in 4/4 time, one beat is only a Quarter Note long. So a Track_Delay
        // longer than this will be cut off when the next beat is scheduled to
        // play.
        TRACK_DELAY(Imagine, SIXTEENTH_BAR) audioSnare.play(snareRaw);
        TRACK_DELAY(Imagine, SIXTEENTH_BAR) audioSnare.play(snareRaw);
        audioSnare.play(snareRaw);
      }
      yield();
      return;
      break;
    default:

      break;
  }

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
      /*if (bar == 20) {
        TRACK_DELAY(Imagine, SIXTEENTH_BAR);
        TRACK_DELAY(Imagine, SIXTEENTH_BAR) audioSnare.play(snareRaw);
        TRACK_DELAY(Imagine, SIXTEENTH_BAR) audioSnare.play(snareRaw);
        audioSnare.play(snareRaw);
        yield();
        return;
        }*/
      // a little sizzle to you playing
      TRACK_DELAY(Imagine, SIXTEENTH_BAR) audioSnare.play(snareRaw);
      audioSnare.play(snareRaw);
      break;
  }
  // all tracks must yield every iteration of the while loop
  yield();
}
