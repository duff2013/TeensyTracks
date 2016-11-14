/*
 Rhythm Guitar Track Thread -
 */
const int finger_delay = 5;
// -----------------------------------------------------------------------------
void strum_up(const float * chord, float velocity) {
    if (chord[0] > 20.0) E2string.noteOn(chord[0], velocity);
    delay(finger_delay);
    if (chord[1] > 20.0) A2string.noteOn(chord[1], velocity);
    delay(finger_delay);
    if (chord[2] > 20.0) D3string.noteOn(chord[2], velocity);
    delay(finger_delay);
    if (chord[3] > 20.0) G3string.noteOn(chord[3], velocity);
    delay(finger_delay);
    if (chord[4] > 20.0) B3string.noteOn(chord[4], velocity);
    delay(finger_delay);
    if (chord[5] > 20.0) E4string.noteOn(chord[5], velocity);
    delay(finger_delay);
}

void strum_dn(const float * chord, float velocity) {
    if (chord[5] > 20.0) E2string.noteOn(chord[5], velocity);
    delay(finger_delay);
    if (chord[4] > 20.0) A2string.noteOn(chord[4], velocity);
    delay(finger_delay);
    if (chord[3] > 20.0) D3string.noteOn(chord[3], velocity);
    delay(finger_delay);
    if (chord[2] > 20.0) G3string.noteOn(chord[2], velocity);
    delay(finger_delay);
    if (chord[1] > 20.0) B3string.noteOn(chord[1], velocity);
    delay(finger_delay);
    if (chord[0] > 20.0) E4string.noteOn(chord[0], velocity);
    delay(finger_delay);
}
//--------------------------------------------------------------
void C_Major_Strum(void) {
    TRACK_DELAY(Guitar, QUARTER_BAR)   strum_dn(Cmajor, .6);
    TRACK_DELAY(Guitar, EIGHTH_BAR)    strum_dn(Cmajor, .6);
    TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_up(Cmajor, .6);
    TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_dn(Cmajor, .6);
    TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_up(Cmajor, .6);
    TRACK_DELAY(Guitar, EIGHTH_BAR) ;
    TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_up(Cmajor, .6);
    TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_dn(Cmajor, .6);
    TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_up(Cmajor, .6);
    TRACK_DELAY(Guitar, SIXTEENTH_BAR)
    TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_up(Cmajor, .6);
    strum_up(Cmajor, .6);
}
//--------------------------------------------------------------
void F_Major7_Strum(void) {
    TRACK_DELAY(Guitar, QUARTER_BAR)   strum_dn(Fmajor7, .6);
    TRACK_DELAY(Guitar, EIGHTH_BAR)    strum_dn(Fmajor7, .6);
    TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_up(Fmajor7, .6);
    TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_dn(Fmajor7, .8);
    TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_up(Fmajor7, .6);
    TRACK_DELAY(Guitar, EIGHTH_BAR) ;
    TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_up(Fmajor7, .6);
    TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_dn(Fmajor7, .8);
    TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_up(Fmajor7, .6);
    TRACK_DELAY(Guitar, SIXTEENTH_BAR)
    TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_up(Fmajor7, .6);
    strum_up(Fmajor7, .6);
}
//--------------------------------------------------------------
void D_Minor_Strum(void) {
    TRACK_DELAY(Guitar, QUARTER_BAR)   strum_dn(Dminor, .8);
    TRACK_DELAY(Guitar, EIGHTH_BAR)    strum_dn(Dminor, .8);
    TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_up(Dminor, .6);
    TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_dn(Dminor, .8);
    TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_up(Dminor, .6);
    TRACK_DELAY(Guitar, EIGHTH_BAR) ;
    TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_up(Dminor, .6);
    TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_dn(Dminor, .8);
    TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_up(Dminor, .6);
    TRACK_DELAY(Guitar, SIXTEENTH_BAR)
    TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_up(Dminor, .6);
    strum_up(Dminor, .6);
}
//--------------------------------------------------------------
void G_Major_Strum(void) {
    TRACK_DELAY(Guitar, QUARTER_BAR)   strum_dn(Gmajor, .8);
    TRACK_DELAY(Guitar, EIGHTH_BAR)    strum_dn(Gmajor, .8);
    TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_up(Gmajor, .6);
    TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_dn(Gmajor, .8);
    TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_up(Gmajor, .6);
    TRACK_DELAY(Guitar, EIGHTH_BAR) ;
    TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_up(Gmajor, .6);
    TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_dn(Gmajor, .8);
    TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_up(Gmajor, .6);
    TRACK_DELAY(Guitar, SIXTEENTH_BAR)
    TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_up(Gmajor, .6);
    strum_up(Gmajor, .6);
}
//--------------------------------------------------------------
static void guitar(void *arg) {
    
    uint8_t bar = Guitar.getBar();
    
    switch (bar) {
        case 1:
            C_Major_Strum();
            break;
        case 2:
            F_Major7_Strum();
            break;
        case 3:
            C_Major_Strum();
            break;
        case 4:
            F_Major7_Strum();
            break;
        case 5:
            C_Major_Strum();
            break;
        case 6:
            F_Major7_Strum();
            break;
        case 7:
            C_Major_Strum();
            break;
        case 8:
            F_Major7_Strum();
            break;
        case 9:
            C_Major_Strum();
            break;
        case 10:
            D_Minor_Strum();
            break;
        case 11:
            G_Major_Strum();
            break;
        case 12:
            strum_dn(Gmajor, .8);
            break;
        case 13:
            TRACK_DELAY(Guitar, EIGHTH_BAR)    strum_dn(Fmajor, .8);
            TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_up(Fmajor, .6);
            TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_dn(Fmajor, .8);
            TRACK_DELAY(Guitar, EIGHTH_BAR)    strum_up(Fmajor, .6);
            
            TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_up(Gmajor, .6);
            TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_dn(Gmajor, .8);
            TRACK_DELAY(Guitar, EIGHTH_BAR)    strum_up(Gmajor, .6);
            strum_up(Gmajor, .6);
            break;
        case 14:
            TRACK_DELAY(Guitar, EIGHTH_BAR)    strum_dn(Cmajor, .8);
            TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_up(Cmajor, .6);
            TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_dn(Cmajor, .8);
            TRACK_DELAY(Guitar, EIGHTH_BAR)    strum_up(Cmajor, .6);
            TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_up(Emajor, .6);
            TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_dn(Emajor, .8);
            TRACK_DELAY(Guitar, EIGHTH_BAR)    strum_up(Emajor, .6);
            strum_up(Emajor, .6);
            break;
        case 15:
            TRACK_DELAY(Guitar, EIGHTH_BAR)    strum_dn(Fmajor, .8);
            TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_up(Fmajor, .6);
            TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_dn(Fmajor, .8);
            TRACK_DELAY(Guitar, EIGHTH_BAR)    strum_up(Fmajor, .6);
            TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_up(Gmajor, .6);
            TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_dn(Gmajor, .8);
            TRACK_DELAY(Guitar, EIGHTH_BAR)    strum_up(Gmajor, .6);
            strum_up(Gmajor, .6);
            break;
        case 16:
            TRACK_DELAY(Guitar, EIGHTH_BAR)    strum_dn(Cmajor, .8);
            TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_up(Cmajor, .6);
            TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_dn(Cmajor, .8);
            TRACK_DELAY(Guitar, EIGHTH_BAR)    strum_up(Cmajor, .6);
            TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_up(Emajor, .6);
            TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_dn(Emajor, .8);
            TRACK_DELAY(Guitar, EIGHTH_BAR)    strum_up(Emajor, .6);
            strum_up(Emajor, .6);
            break;
        case 17:
            TRACK_DELAY(Guitar, EIGHTH_BAR)    strum_dn(Fmajor, .8);
            TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_up(Fmajor, .6);
            TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_dn(Fmajor, .8);
            TRACK_DELAY(Guitar, EIGHTH_BAR)    strum_up(Fmajor, .6);
            TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_up(Gmajor, .6);
            TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_dn(Gmajor, .8);
            TRACK_DELAY(Guitar, EIGHTH_BAR)    strum_up(Gmajor, .6);
            strum_up(Gmajor, .6);
            break;
        case 18:
            TRACK_DELAY(Guitar, EIGHTH_BAR)    strum_dn(Cmajor, .8);
            TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_up(Cmajor, .6);
            TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_dn(Cmajor, .8);
            TRACK_DELAY(Guitar, EIGHTH_BAR)    strum_up(Cmajor, .6);
            TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_up(Emajor, .6);
            TRACK_DELAY(Guitar, SIXTEENTH_BAR) strum_dn(Emajor, .8);
            TRACK_DELAY(Guitar, EIGHTH_BAR)    strum_up(Emajor, .6);
            strum_up(Emajor, .6);
            break;
        case 19:
            TRACK_DELAY(Guitar, EIGHTH_BAR)     strum_dn(Fmajor, .8);
            TRACK_DELAY(Guitar, SIXTEENTH_BAR)  strum_up(Fmajor, .6);
            TRACK_DELAY(Guitar, SIXTEENTH_BAR)  strum_dn(Fmajor, .8);
            TRACK_DELAY(Guitar, EIGHTH_BAR)     strum_up(Fmajor, .6);
            TRACK_DELAY(Guitar, SIXTEENTH_BAR)  strum_up(Gmajor, .6);
            TRACK_DELAY(Guitar, SIXTEENTH_BAR)  strum_dn(Gmajor, .8);
            TRACK_DELAY(Guitar, EIGHTH_BAR)     strum_up(Gmajor, .6);
            strum_up(Gmajor, .6);
            break;
        case 20:
            strum_dn(Cmajor, .8);
            break;
        default:
            break;
    }
    // must yield for other tracks to run
    yield();
}