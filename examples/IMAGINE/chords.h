// Bass Notes
#define NOTE_B0  30.87
#define NOTE_C1  32.70
#define NOTE_Cs1 34.65
#define NOTE_D1  36.71
#define NOTE_Ds1 38.89
#define NOTE_E1  41.20
#define NOTE_F1  43.65
#define NOTE_Fs1 46.25
#define NOTE_G1  49.00
#define NOTE_Gs1 51.91
#define NOTE_A1  55.00
#define NOTE_As1 58.27
#define NOTE_B1  61.74
#define NOTE_C2  65.41
#define NOTE_Cs2 69.30
#define NOTE_D2  73.42
#define NOTE_Ds2 77.78

// Guitar Notes
#define NOTE_E2   82.41
#define NOTE_F2   87.31
#define NOTE_Fs2  92.50
#define NOTE_G2   98.00
#define NOTE_Gs2 103.82
#define NOTE_A2  110.00
#define NOTE_As2 116.54
#define NOTE_B2  123.47
#define NOTE_C3  130.81
#define NOTE_Cs3 138.59
#define NOTE_D3  146.83
#define NOTE_Ds3 155.56
#define NOTE_E3  164.81
#define NOTE_F3  174.61
#define NOTE_Fs3 185.00
#define NOTE_G3  196.00
#define NOTE_Gs3 207.65
#define NOTE_A3  220.00
#define NOTE_As3 233.08
#define NOTE_B3  246.94
#define NOTE_C4  261.63
#define NOTE_Cs4 277.18
#define NOTE_D4  293.66
#define NOTE_Ds4 311.13
#define NOTE_E4  329.63
#define NOTE_F4  349.23
#define NOTE_Fs4 369.99
#define NOTE_G4  392.00
#define NOTE_Gs4 415.30
#define NOTE_A4  440.00
#define NOTE_As4 466.16
#define NOTE_B4  493.88

#define NOTE_Bb3 233.1
// according to http://www.guitar-chords.org.uk/
// and http://www.8notes.com/guitar_chord_chart/c.asp
//
              // open   =  NOTE_E2  NOTE_A2  NOTE_D3  NOTE_G3  NOTE_B3  NOTE_E4
const float Cmajor[6]   = {      0, NOTE_C3, NOTE_E3, NOTE_G3, NOTE_C4, NOTE_E4};  // C - E - G
const float Dmajor[6]   = {      0,       0, NOTE_D3, NOTE_A3, NOTE_D4, NOTE_Fs4}; // D - F# - A
const float Emajor[6]   = {NOTE_E2, NOTE_B2, NOTE_E3, NOTE_Gs3,NOTE_B3, NOTE_E4};  // E - G# - B
const float Fmajor[6]   = {      0, NOTE_A2, NOTE_F3, NOTE_A3, NOTE_C4, NOTE_F4};  // F - A - C
const float Gmajor[6]   = {NOTE_G2, NOTE_B2, NOTE_D3, NOTE_G3, NOTE_B3, NOTE_E4};  // G - B - D
const float Amajor[6]   = {      0, NOTE_A2, NOTE_E3, NOTE_A3, NOTE_Cs4,NOTE_E4};  // A - C# - E
const float Bmajor[6]   = {      0, NOTE_B2, NOTE_Fs3,NOTE_B3, NOTE_Ds4,NOTE_Fs4}; // B - D# - F#
const float Cminor[6]   = {      0, NOTE_C3, NOTE_G3, NOTE_C4, NOTE_Ds4,NOTE_G4};  // C - D# - G
const float Dminor[6]   = {      0,       0, NOTE_D3, NOTE_A3, NOTE_D4, NOTE_F4};  // D - F - A
const float Eminor[6]   = {NOTE_E2, NOTE_B2, NOTE_E3, NOTE_G3, NOTE_B3, NOTE_E4};  // E - G - B
const float Fminor[6]   = {NOTE_F2, NOTE_C3, NOTE_F3, NOTE_Gs3,NOTE_C4, NOTE_F4};  // F - G# - C
const float Gminor[6]   = {NOTE_G2, NOTE_D3, NOTE_G3, NOTE_As3,NOTE_D3, NOTE_G4};  // G - A# - D
const float Aminor[6]   = {      0, NOTE_A2, NOTE_E3, NOTE_A3, NOTE_C4, NOTE_E4};  // A - C - E
const float Bminor[6]   = {      0, NOTE_B2, NOTE_Fs3,NOTE_B3, NOTE_D4, NOTE_Fs4}; // B - D - F#

const float Cmajor7[6]  = {      0, NOTE_C3, NOTE_E2, NOTE_Bb3, NOTE_C4, NOTE_E4};  // C,E,G,B♭
const float Fmajor7[6]   = {     0, NOTE_C3, NOTE_F3, NOTE_A3,  NOTE_C4, NOTE_E4};  // C F A C E

const float A_7[6]      = {      0, NOTE_A2, NOTE_E3, NOTE_G3, NOTE_Cs4,NOTE_E4};  // A - C# - E
const float D_7[6]      = {      0, NOTE_A2, NOTE_D3, NOTE_A3, NOTE_C4, NOTE_Fs4}; // D - F# - A
const float E_7[6]      = {NOTE_E2, NOTE_B2, NOTE_E3, NOTE_Gs3,NOTE_D4, NOTE_E4};  // E - G# - B

//                   E2, F2, F2#, G2, G2#, A2, A2#, B2
// C3, C3#, D3, D3#, E3, F3, F3#, G3, G3#, A3, A3#, B3
// C4, C4#, D4, D4#, E4, F4, F4#, G4, G4#, A4, A4#, B4


