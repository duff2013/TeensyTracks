/*
 * Kick Drum Track Thread -
 */
static void kick(void *arg) {
    
    while (!Drum2.EndTrack) {
        // 'onBeat' is true when the MasterTrack signals on the
        // current beat. Must clear with 'clearBeat' method at
        // the end of the if statement.
        if (Drum2.onBeat()) {
            // get current beat count
            uint8_t beat = Drum2.getBeat();
            // play the snare on the 2 and 4 beat of the bar.
            switch (beat) {
                case 1:
                    // Track delay some fraction of the bar corresponding to
                    // musical note duration. This one 1/8 of the bar.
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
            // clearing so we can get next 'onBeat' signal.
            Drum2.clearBeat();
        }
        // all tracks must yield every iteration of the while loop
        yield();
    }
}
