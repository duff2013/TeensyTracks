/*
 * Solo Guitar Track Thread
 */
static void soloGuitar(void *arg) {
  unsigned long t;
  elapsedMicros time = 0;
  while (!SoloGuitar.EndTrack) {
    if (SoloGuitar.onBar()) {
      uint8_t bar = SoloGuitar.getBar();
      switch (bar) {
        case 1:
          TRACK_DELAY(Imagine, QUARTER_BAR);
          TRACK_DELAY(Imagine, EIGHTH_BAR)  soloStrings.noteOn(NOTE_G3, .6);
          TRACK_DELAY(Imagine, EIGHTH_BAR)  soloStrings.noteOn(NOTE_E3, .6);
          TRACK_DELAY(Imagine, QUARTER_BAR) soloStrings.noteOn(NOTE_G3, .6);
          TRACK_DELAY(Imagine, EIGHTH_BAR)  soloStrings.noteOn(NOTE_B3, .6);
          soloStrings.noteOn(NOTE_B3, .6);
          //Serial.println("Solo Bar 1");
          break;
        case 2:
          TRACK_DELAY(Imagine, EIGHTH_BAR)  soloStrings.noteOn(NOTE_B3, .6);
          soloStrings.noteOn(NOTE_A3, .6);
          //Serial.println("Solo Bar 2");
          break;
        case 3:
          TRACK_DELAY(Imagine, QUARTER_BAR);
          TRACK_DELAY(Imagine, EIGHTH_BAR)  soloStrings.noteOn(NOTE_G3, .6);
          TRACK_DELAY(Imagine, EIGHTH_BAR)  soloStrings.noteOn(NOTE_E3, .6);
          TRACK_DELAY(Imagine, QUARTER_BAR) soloStrings.noteOn(NOTE_G3, .6);
          TRACK_DELAY(Imagine, EIGHTH_BAR)  soloStrings.noteOn(NOTE_B3, .6);
          TRACK_DELAY(Imagine, EIGHTH_BAR)  soloStrings.noteOn(NOTE_B3, .6);
          //Serial.println("Bar 3");
          break;
        case 4:
          soloStrings.noteOn(NOTE_A2, .6);
          //Serial.println("Bar 4");
          break;
        case 5:
          TRACK_DELAY(Imagine, QUARTER_BAR);
          TRACK_DELAY(Imagine, EIGHTH_BAR)  soloStrings.noteOn(NOTE_G3, .6);
          TRACK_DELAY(Imagine, EIGHTH_BAR)  soloStrings.noteOn(NOTE_E3, .6);
          TRACK_DELAY(Imagine, QUARTER_BAR) soloStrings.noteOn(NOTE_G3, .6);
          TRACK_DELAY(Imagine, EIGHTH_BAR)  soloStrings.noteOn(NOTE_B3, .6);
          soloStrings.noteOn(NOTE_B3, .6);
          //Serial.println("Bar 5");
          break;
        case 6:
          TRACK_DELAY(Imagine, EIGHTH_BAR)  soloStrings.noteOn(NOTE_B3, .6);
          soloStrings.noteOn(NOTE_A3, .6);
          //Serial.println("Bar 6");
          break;
        case 7:
          TRACK_DELAY(Imagine, QUARTER_BAR);
          TRACK_DELAY(Imagine, EIGHTH_BAR)  soloStrings.noteOn(NOTE_G3, .6);
          TRACK_DELAY(Imagine, EIGHTH_BAR)  soloStrings.noteOn(NOTE_E3, .6);
          TRACK_DELAY(Imagine, QUARTER_BAR) soloStrings.noteOn(NOTE_G3, .6);
          TRACK_DELAY(Imagine, EIGHTH_BAR)  soloStrings.noteOn(NOTE_B3, .6);
          soloStrings.noteOn(NOTE_B3, .6);
          //Serial.println("Bar 7");
          break;
        case 8:
          soloStrings.noteOn(NOTE_A3, .6);
          //Serial.println("Bar 8");
          break;
        case 9:
          TRACK_DELAY(Imagine, QUARTER_BAR);
          TRACK_DELAY(Imagine, EIGHTH_BAR)  soloStrings.noteOn(NOTE_C4, .6);
          TRACK_DELAY(Imagine, EIGHTH_BAR)  soloStrings.noteOn(NOTE_A3, .6);
          TRACK_DELAY(Imagine, QUARTER_BAR) soloStrings.noteOn(NOTE_C4, .6);
          TRACK_DELAY(Imagine, EIGHTH_BAR)  soloStrings.noteOn(NOTE_E4, .6);
          soloStrings.noteOn(NOTE_E4, .6);
          //Serial.println("Bar 9");
          break;
        case 10:
          TRACK_DELAY(Imagine, EIGHTH_BAR)  soloStrings.noteOn(NOTE_D4, .6);
          TRACK_DELAY(Imagine, EIGHTH_BAR)  soloStrings.noteOn(NOTE_C4, .6);
          soloStrings.noteOn(NOTE_A3, .6);
          //Serial.println("Bar 10");
          break;
        case 11:
          TRACK_DELAY(Imagine, EIGHTH_BAR)  soloStrings.noteOn(NOTE_B3, .6);
          TRACK_DELAY(Imagine, EIGHTH_BAR)  soloStrings.noteOn(NOTE_B3, .6);
          TRACK_DELAY(Imagine, QUARTER_BAR);
          TRACK_DELAY(Imagine, QUARTER_BAR) soloStrings.noteOn(NOTE_B3, .6);
          TRACK_DELAY(Imagine, EIGHTH_BAR)  soloStrings.noteOn(NOTE_C4, .6);
          soloStrings.noteOn(NOTE_D4, .6);
          //Serial.println("Bar 11");
          break;
        case 12:
          TRACK_DELAY(Imagine, HALF_BAR);
          TRACK_DELAY(Imagine, EIGHTH_BAR)  soloStrings.noteOn(NOTE_E4, .6);
          TRACK_DELAY(Imagine, EIGHTH_BAR)  soloStrings.noteOn(NOTE_G4, .6);
          TRACK_DELAY(Imagine, EIGHTH_BAR);
          TRACK_DELAY(Imagine, SIXTEENTH_BAR)  soloStrings.noteOn(NOTE_E4, .6);
          soloStrings.noteOn(NOTE_D4, .6);
          //Serial.println("Bar 12");
          break;
        case 13:
          TRACK_DELAY(Imagine, QUARTER_BAR) soloStrings.noteOn(NOTE_C4, .6);

          TRACK_DELAY(Imagine, EIGHTH_BAR)  soloStrings.noteOn(NOTE_A3, .6);
          TRACK_DELAY(Imagine, EIGHTH_BAR)  soloStrings.noteOn(NOTE_C4, .6);

          TRACK_DELAY(Imagine, QUARTER_BAR) soloStrings.noteOn(NOTE_B3, .6);

          TRACK_DELAY(Imagine, EIGHTH_BAR) soloStrings.noteOn(NOTE_A3, .6);
          soloStrings.noteOn(NOTE_B3, .6);
          //Serial.println("Bar .63");
          break;
        case 14:
          TRACK_DELAY(Imagine, EIGHTH_BAR)  soloStrings.noteOn(NOTE_C4, .6);
          soloStrings.noteOn(NOTE_C4, .6);
          //Serial.println("Bar 14");
          break;
        case 15:
          TRACK_DELAY(Imagine, QUARTER_BAR);

          TRACK_DELAY(Imagine, EIGHTH_BAR)  soloStrings.noteOn(NOTE_A3, .6);
          TRACK_DELAY(Imagine, EIGHTH_BAR)  soloStrings.noteOn(NOTE_C4, .6);

          TRACK_DELAY(Imagine, QUARTER_BAR) soloStrings.noteOn(NOTE_B3, .6);

          TRACK_DELAY(Imagine, EIGHTH_BAR)  soloStrings.noteOn(NOTE_A3, .6);
          soloStrings.noteOn(NOTE_G3, .6);
          //          Serial.println("Bar 15");
          break;
        case 16:
          soloStrings.noteOn(NOTE_E3, .6);
          //          Serial.println("Bar 16");
          break;
        case 17:
          TRACK_DELAY(Imagine, QUARTER_BAR);
          TRACK_DELAY(Imagine, EIGHTH_BAR)  soloStrings.noteOn(NOTE_A3, .6);
          TRACK_DELAY(Imagine, EIGHTH_BAR)  soloStrings.noteOn(NOTE_C4, .6);

          TRACK_DELAY(Imagine, QUARTER_BAR) soloStrings.noteOn(NOTE_B3, .6);

          TRACK_DELAY(Imagine, EIGHTH_BAR)  soloStrings.noteOn(NOTE_A3, .6);
          soloStrings.noteOn(NOTE_B3, .6);
          //          Serial.println("Bar 17");
          break;
        case 18:
          TRACK_DELAY(Imagine, EIGHTH_BAR)  soloStrings.noteOn(NOTE_D4, .6);
          soloStrings.noteOn(NOTE_C4, .6);
          //          Serial.println("Bar 18");
          break;
        case 19:
          TRACK_DELAY(Imagine, QUARTER_BAR);
          TRACK_DELAY(Imagine, EIGHTH_BAR)  soloStrings.noteOn(NOTE_A3, .6);
          TRACK_DELAY(Imagine, EIGHTH_BAR)  soloStrings.noteOn(NOTE_C4, .6);

          TRACK_DELAY(Imagine, QUARTER_BAR)  soloStrings.noteOn(NOTE_D4, .6);

          TRACK_DELAY(Imagine, EIGHTH_BAR)  soloStrings.noteOn(NOTE_C4, .6);
          soloStrings.noteOn(NOTE_D4, .6);
          //          Serial.println("Bar 19");
          break;
        case 20:
          TRACK_DELAY(Imagine, SIXTEENTH_BAR)  soloStrings.noteOn(NOTE_E4, .6);
          TRACK_DELAY(Imagine, SIXTEENTH_BAR)  soloStrings.noteOn(NOTE_D4, .6);
          soloStrings.noteOn(NOTE_C4, .6);
          //          Serial.println("Bar 20");
          break;
        default:

          break;
      }
      SoloGuitar.clearBar();
    }
    yield();
  }
}
