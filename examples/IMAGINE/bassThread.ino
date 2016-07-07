/*
 * Bass drum Track Thread - Not done yet, just follows the solo guitar!!!
 */
static void bass(void *arg) {
    unsigned long t;
    elapsedMicros time = 0;
    while (1) {
        if (Bass.onBar()) {
            uint8_t bar = Bass.getBar();
            switch (bar) {
                case 1:
                    TRACK_DELAY(Imagine, QUARTER_BAR);
                    TRACK_DELAY(Imagine, EIGHTH_BAR)  bassStrings.noteOn(NOTE_G1, .2);
                    TRACK_DELAY(Imagine, EIGHTH_BAR)  bassStrings.noteOn(NOTE_E1, .2);
                    TRACK_DELAY(Imagine, QUARTER_BAR) bassStrings.noteOn(NOTE_G1, .2);
                    TRACK_DELAY(Imagine, EIGHTH_BAR)  bassStrings.noteOn(NOTE_B1, .2);
                    bassStrings.noteOn(NOTE_B1, .2);
                    break;
                case 2:
                    TRACK_DELAY(Imagine, EIGHTH_BAR)  bassStrings.noteOn(NOTE_B1, .2);
                    bassStrings.noteOn(NOTE_A1, .2);
                    break;
                case 3:
                    TRACK_DELAY(Imagine, QUARTER_BAR);
                    TRACK_DELAY(Imagine, EIGHTH_BAR)  bassStrings.noteOn(NOTE_G1, .2);
                    TRACK_DELAY(Imagine, EIGHTH_BAR)  bassStrings.noteOn(NOTE_E1, .2);
                    TRACK_DELAY(Imagine, QUARTER_BAR) bassStrings.noteOn(NOTE_G1, .2);
                    TRACK_DELAY(Imagine, EIGHTH_BAR)  bassStrings.noteOn(NOTE_B1, .2);
                    TRACK_DELAY(Imagine, EIGHTH_BAR)  bassStrings.noteOn(NOTE_B1, .2);
                    break;
                case 4:
                    bassStrings.noteOn(NOTE_A1, .2);
                    break;
                case 5:
                    TRACK_DELAY(Imagine, QUARTER_BAR);
                    TRACK_DELAY(Imagine, EIGHTH_BAR)  bassStrings.noteOn(NOTE_G1, .2);
                    TRACK_DELAY(Imagine, EIGHTH_BAR)  bassStrings.noteOn(NOTE_E1, .2);
                    TRACK_DELAY(Imagine, QUARTER_BAR) bassStrings.noteOn(NOTE_G1, .2);
                    TRACK_DELAY(Imagine, EIGHTH_BAR)  bassStrings.noteOn(NOTE_B1, .2);
                    bassStrings.noteOn(NOTE_B1, .2);
                    break;
                case 6:
                    TRACK_DELAY(Imagine, EIGHTH_BAR)  bassStrings.noteOn(NOTE_B1, .2);
                    bassStrings.noteOn(NOTE_A1, .2);
                    break;
                case 7:
                    TRACK_DELAY(Imagine, QUARTER_BAR);
                    TRACK_DELAY(Imagine, EIGHTH_BAR)  bassStrings.noteOn(NOTE_G1, .2);
                    TRACK_DELAY(Imagine, EIGHTH_BAR)  bassStrings.noteOn(NOTE_E1, .2);
                    TRACK_DELAY(Imagine, QUARTER_BAR) bassStrings.noteOn(NOTE_G1, .2);
                    TRACK_DELAY(Imagine, EIGHTH_BAR)  bassStrings.noteOn(NOTE_B1, .2);
                    bassStrings.noteOn(NOTE_B1, .2);
                    break;
                case 8:
                    bassStrings.noteOn(NOTE_A1, .2);
                    break;
                case 9:
                    TRACK_DELAY(Imagine, QUARTER_BAR);
                    TRACK_DELAY(Imagine, EIGHTH_BAR)  bassStrings.noteOn(NOTE_C2, .2);
                    TRACK_DELAY(Imagine, EIGHTH_BAR)  bassStrings.noteOn(NOTE_A1, .2);
                    TRACK_DELAY(Imagine, QUARTER_BAR) bassStrings.noteOn(NOTE_C2, .2);
                    TRACK_DELAY(Imagine, EIGHTH_BAR)  bassStrings.noteOn(NOTE_E2, .2);
                    bassStrings.noteOn(NOTE_E2, .2);
                    break;
                case 10:
                    TRACK_DELAY(Imagine, EIGHTH_BAR)  bassStrings.noteOn(NOTE_D2, .2);
                    TRACK_DELAY(Imagine, EIGHTH_BAR)  bassStrings.noteOn(NOTE_C2, .2);
                    bassStrings.noteOn(NOTE_A1, .2);
                    break;
                case 11:
                    TRACK_DELAY(Imagine, EIGHTH_BAR)  bassStrings.noteOn(NOTE_B1, .2);
                    TRACK_DELAY(Imagine, EIGHTH_BAR)  bassStrings.noteOn(NOTE_B1, .2);
                    TRACK_DELAY(Imagine, QUARTER_BAR);
                    TRACK_DELAY(Imagine, QUARTER_BAR) bassStrings.noteOn(NOTE_B1, .2);
                    TRACK_DELAY(Imagine, EIGHTH_BAR)  bassStrings.noteOn(NOTE_C2, .2);
                    bassStrings.noteOn(NOTE_D2, .2);
                    break;
                case 12:
                    TRACK_DELAY(Imagine, HALF_BAR);
                    TRACK_DELAY(Imagine, EIGHTH_BAR)  bassStrings.noteOn(NOTE_E2, .2);
                    TRACK_DELAY(Imagine, EIGHTH_BAR)  bassStrings.noteOn(NOTE_G2, .2);
                    TRACK_DELAY(Imagine, EIGHTH_BAR);
                    TRACK_DELAY(Imagine, SIXTEENTH_BAR)  bassStrings.noteOn(NOTE_E2, .2);
                    bassStrings.noteOn(NOTE_D2, .2);
                    break;
                case 13:
                    TRACK_DELAY(Imagine, QUARTER_BAR) bassStrings.noteOn(NOTE_C2, .2);
                    TRACK_DELAY(Imagine, EIGHTH_BAR)  bassStrings.noteOn(NOTE_A1, .2);
                    TRACK_DELAY(Imagine, EIGHTH_BAR)  bassStrings.noteOn(NOTE_C2, .2);
                    TRACK_DELAY(Imagine, QUARTER_BAR) bassStrings.noteOn(NOTE_B1, .2);
                    TRACK_DELAY(Imagine, EIGHTH_BAR) bassStrings.noteOn(NOTE_A1, .2);
                    bassStrings.noteOn(NOTE_B1, .2);
                    break;
                case 14:
                    TRACK_DELAY(Imagine, EIGHTH_BAR)  bassStrings.noteOn(NOTE_C2, .2);
                    bassStrings.noteOn(NOTE_C2, .2);
                    break;
                case 15:
                    TRACK_DELAY(Imagine, QUARTER_BAR);
                    TRACK_DELAY(Imagine, EIGHTH_BAR)  bassStrings.noteOn(NOTE_A1, .2);
                    TRACK_DELAY(Imagine, EIGHTH_BAR)  bassStrings.noteOn(NOTE_C2, .2);
                    TRACK_DELAY(Imagine, QUARTER_BAR) bassStrings.noteOn(NOTE_B1, .2);
                    TRACK_DELAY(Imagine, EIGHTH_BAR)  bassStrings.noteOn(NOTE_A1, .2);
                    bassStrings.noteOn(NOTE_G1, .2);
                    break;
                case 16:
                    bassStrings.noteOn(NOTE_E1, .2);
                    break;
                case 17:
                    TRACK_DELAY(Imagine, QUARTER_BAR);
                    TRACK_DELAY(Imagine, EIGHTH_BAR)  bassStrings.noteOn(NOTE_A1, .2);
                    TRACK_DELAY(Imagine, EIGHTH_BAR)  bassStrings.noteOn(NOTE_C2, .2);
                    TRACK_DELAY(Imagine, QUARTER_BAR) bassStrings.noteOn(NOTE_B1, .2);
                    TRACK_DELAY(Imagine, EIGHTH_BAR)  bassStrings.noteOn(NOTE_A1, .2);
                    bassStrings.noteOn(NOTE_B1, .2);
                    break;
                case 18:
                    TRACK_DELAY(Imagine, EIGHTH_BAR)  bassStrings.noteOn(NOTE_D2, .2);
                    bassStrings.noteOn(NOTE_C2, .2);
                    break;
                case 19:
                    TRACK_DELAY(Imagine, QUARTER_BAR);
                    TRACK_DELAY(Imagine, EIGHTH_BAR)  bassStrings.noteOn(NOTE_A1, .2);
                    TRACK_DELAY(Imagine, EIGHTH_BAR)  bassStrings.noteOn(NOTE_C2, .2);
                    TRACK_DELAY(Imagine, QUARTER_BAR)  bassStrings.noteOn(NOTE_D2, .2);
                    TRACK_DELAY(Imagine, EIGHTH_BAR)  bassStrings.noteOn(NOTE_C2, .2);
                    bassStrings.noteOn(NOTE_D2, .2);
                    break;
                case 20:
                    TRACK_DELAY(Imagine, SIXTEENTH_BAR)  bassStrings.noteOn(NOTE_E2, .2);
                    TRACK_DELAY(Imagine, SIXTEENTH_BAR)  bassStrings.noteOn(NOTE_D2, .2);
                    bassStrings.noteOn(NOTE_C2, .2);
                    break;
                default:
                    
                    break;
            }
            // must clear the bar so a new bar signal can be captured
            Bass.clearBar();
        }
        // must yield for other tracks to run
        yield();
    }
}
