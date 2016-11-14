/*
 Hihat Track Thread -
 */
static void hihat(void *arg) {
    
    // get current bar
    uint8_t bar = DrumHiHat.getBar();
    switch (bar) {
        case 12:
            yield();
            return;
            break;
        case 20:
            yield();
            return;
            break;
        default:
            
            break;
    }
    
    // get current beat count
    uint8_t beat = DrumHiHat.getBeat();
    
    // play the snare on the 2 and 4 beat of the bar.
    switch (beat) {
        case 1:
            // Track delay take into account for how long this Audio Libaray
            // function takes. How long this fuction takes to excute must be less
            // the than this Track_Delay, which in this case is 1/16 of the bar.
            // This allows you too play the next snare hit in musical time for the
            // tempo you are using.
            TRACK_DELAY(DrumHiHat, EIGHTH_BAR) audioHihat.play(hihatRaw);
            audioHihat.play(hihatRaw);
            break;
        case 2:
            TRACK_DELAY(DrumHiHat, EIGHTH_BAR) audioHihat.play(hihatRaw);
            audioHihat.play(hihatRaw);
            break;
        case 3:
            TRACK_DELAY(DrumHiHat, EIGHTH_BAR) audioHihat.play(hihatRaw);
            audioHihat.play(hihatRaw);
            break;
        case 4:
            TRACK_DELAY(DrumHiHat, EIGHTH_BAR) audioHihat.play(hihatRaw);
            audioHihat.play(hihatRaw);
            break;
    }
    // all tracks must yield every iteration of the while loop
    yield();
}