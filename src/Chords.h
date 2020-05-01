/*************************************************** 
  This is a library to analyse chords in MIDI notes

  For notaion, check examples on
  https://en.wikipedia.org/wiki/Chord_(music)
 
  Written by Karg (Timm Schlegelmilch)
  publushed under GNU license, 
  all text above must be included in any redistribution
 ****************************************************/


#ifndef _Chords_H_
#define _Chords_H_

#include "Arduino.h"

// notes
const uint8_t NO_CHORD                   = 0;
const uint8_t ROOT_C                     = 1;
const uint8_t ROOT_C_SHARP               = 2;
const uint8_t ROOT_D_FLAT                = 2;
const uint8_t ROOT_D                     = 3;
const uint8_t ROOT_D_SHARP               = 4;
const uint8_t ROOT_E_FLAT                = 4;
const uint8_t ROOT_E                     = 5;
const uint8_t ROOT_F                     = 6;
const uint8_t ROOT_F_SHARP               = 7;
const uint8_t ROOT_G_FLAT                = 7;
const uint8_t ROOT_G                     = 8;
const uint8_t ROOT_G_SHARP               = 9;
const uint8_t ROOT_A_FLAT                = 9;
const uint8_t ROOT_A                     = 10;
const uint8_t ROOT_A_SHARP               = 11;
const uint8_t ROOT_B_FLAT                = 11;
const uint8_t ROOT_B                     = 12;

// triads
const uint8_t MAJOR_TRIAD                = 0;
const uint8_t MINOR_TRIAD                = 1;
const uint8_t AUGMENTED_TRIAD            = 2;
const uint8_t DIMINISHED_TRIAD           = 3;

// tetrads
const uint8_t MAJOR_SIXTH                = 4;
const uint8_t MINOR_SIXTH                = 5;
const uint8_t DOMINANT_SEVENTH           = 6;
const uint8_t MAJOR_SEVENTH              = 7; 
const uint8_t MINOR_SEVENTH              = 8;
const uint8_t MINOR_MAJOR_SEVENTH        = 9;
const uint8_t AUGMENTED_MAJOR_SEVENTH    = 10;
const uint8_t AUGMENTED_MINOR_SEVENTH    = 11;
const uint8_t DIMINISHED_SEVENTH         = 12;
const uint8_t HALF_DIMINISHED_SEVENTH    = 13;
 
// positions
const uint8_t ROOT_POSITION              = 0;
const uint8_t INVERSION_1                = 1;
const uint8_t INVERSION_2                = 2;
const uint8_t INVERSION_3                = 3;

const uint8_t PSEUDO_CHORD		         = 127; 



// Structure definitions
struct Chord {
  uint8_t root     = NO_CHORD;		
  uint8_t quality  = 0;
  uint8_t position = 0;
  int8_t  octave   = 0;
};

struct ChordName {
  char name;											 // e.g. 'F'
  char accidental;										 // e.g. '#'
  char quality;											 // e.g. 'm' for a minor chord
  char superscript[3]                 = "";				 // e.g. 7 for a minor seventh
  char octave[2] 					  = "";		
  // and the rest....
};

struct Notes {
   uint8_t numNotes    = 0;
   uint8_t note[4]     = {0, 0, 0, 0};
   boolean pseudochord = false;
};



class Chords {
public:
  Chords(void);
  void addNote(uint8_t note);							 // input: note as midi note number
  void removeNote(uint8_t note);
  Notes generateChord(Chord chord);
  Chord analyseChord(boolean pseudoChord = false);       // if strictChord = false pseudo chords (spread over multiple octaves) are considered
  Notes getChordNotes(void);
  ChordName getChordName(Chord chord);
  String getChordString(Chord chord);
  String getNoteString(uint8_t midiNote);
  
private:  	
  boolean notes[140]; 					                 // 128 notes + 12 for chord analysis (balance between memory and code simplicity)
  Notes   chordNotes;
  void setChordNotes(uint8_t n, Chord *_chord);
  uint8_t analyse(boolean *notes, Chord *chord);
  
  const char    NAMES_SHARPS[12][3] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
  const char    NAMES_FLATS[12][3]  = {"C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab", "A", "Bb", "B"};
  
  const uint8_t NUM_QUALITIES    = 13;
  const uint8_t NUM_TRIADS       = 4;
  const uint8_t TRIADS[4][3][3]  = // A,  B,      O      -- A+B: intervals, O: root note offset
                                   {{{4,  7,      0},    // 0  - major triads - root
                                     {3,  8,      8},    //      major triad - 1st inversion
                                     {5,  9,      5}},   //      major triad - 2nd inversion
                                    {{3,  7,      0},    // 1  - minor triad - root
                                     {4,  9,      9},    //      minor triad - 1st inversion
                                     {5,  8,      5}},   //      minor triad - 2nd inversion
                                    {{4,  8,      0},    // 2  - augmented triad - root
                                     {4,  8,      0},    //      augmented triad - 1st inversion          = root
                                     {4,  8,      0}},   //      augmented triad - 2nd inversion.         = root
                                    {{3,  6,      0},    // 3  - diminished triad - root
                                     {3,  9,      9},    //      diminished triad - 1st inversion
                                     {6,  9,      6}}};  //      diminished triad - 2nd inversion

const uint8_t NUM_TETRADS        = 10;
const uint8_t TETRADS[10][4][4]  = // A,  B,  C,  O      -- A-C: intervals, O: root note offset 
                                   {{{4,  7,  9,  0},    // 4  - major sixth - root                       = minor seventh - 1st inversion
                                     {3,  5,  8,  8},    //      major sixth - 1st inversion              = minor seventh - 2nd inversion
                                     {2,  5,  9,  5},    //      major sixth - 2nd inversion              = minor seventh - 3rd inversion
                                     {3,  7, 10,  3}},   //      major sixth - 3rd inversion              = minor seventh - root
                                    {{3,  7,  9,  0},    // 5  - minor sixth - root                       = half-diminished seventh - 1st inversion
                                     {4,  6,  9,  9},    //      minor sixth - 1st inversion              = half-diminished seventh - 2nd inversion
                                     {2,  5,  8,  5},    //      minor sixth - 2nd inversion              = half-diminished seventh - 3rd inversion
                                     {3,  6, 10,  3}},   //      minor sixth - 3rd inverion               = half-diminished seventh - root
                                    {{4,  7, 10,  0},    // 6  - dominant seventh - root
                                     {3,  6,  8,  8},    //      dominant seventh - 1st inversion
                                     {3,  5,  9,  5},    //      dominant seventh - 2nd inversion
                                     {2,  6,  9,  2}},   //      dominant seventh - 3rd inversion		                                    
                                    {{4,  7, 11,  0},    // 7  - major seventh - root	
                                     {3,  7,  8,  8},    //      major seventh - 1st inversion
                                     {4,  5,  9,  5},    //      major seventh - 2nd inversion
                                     {1,  5,  8,  1}},   //      major seventh - 3rd inversion
                                    {{3,  7, 10,  0},    // 8  - minor seventh - root                     = major sixth - third inversion
                                     {4,  7,  9,  9},    //      minor seventh - 1st inversion            = major sixth - root
                                     {3,  5,  8,  5},    //      minor seventh - 2nd inversion            = major sixth - 1st inversion
                                     {2,  5,  9,  2}},   //      minor seventh - 3rd inversion            = major sixth - 2nd inversion
                                    {{3,  7, 11,  0},    // 9  - minor-major seventh - root
                                     {4,  8,  9,  9},    //      minor-major seventh - 1st inversion
                                     {4,  5,  8,  5},    //      minor-major seventh - 2nd inversion
                                     {1,  4,  8,  1}},   //      minor-major seventh - 3rd inversion
                                    {{4,  8, 11,  0},    // 10 - augmented major seventh - root  
                                     {4,  7,  8,  8},    //      augmented major seventh - 1st inversion
                                     {3,  4,  8,  4},    //      augmented major seventh - 2nd inversion
                                     {1,  5,  9,  1}},   //      augmented major seventh - 3rd inversion
                                    {{4,  8, 10,  0},    // 11 - augmented minor seventh - root  
                                     {4,  6,  8,  8},    //      augmented minor seventh - 1st inversion
                                     {2,  4,  8,  4},    //      augmented minor seventh - 2nd inversion
                                     {2,  6, 10,  2}},   //      augmented minor seventh - 3rd inversion
                                    {{3,  6,  9,  0},    // 12 - diminished seventh - root
                                     {3,  6,  9,  0},    //      diminished seventh - 1st inversion      = root
                                     {3,  6,  9,  0},    //      diminished seventh - 2nd inversion      = root
                                     {3,  6,  9,  0}},	 //      diminished seventh - 3rd inversion      = root
                                    {{3,  6, 10,  0},    // 13 - half-diminished seventh - root           = minor sixth - 3rd inversion
                                     {3,  7,  9,  9},    //      half-diminished seventh - 1st inversion  = minor sixth - root
                                     {4,  6,  9,  6},    //      half-diminished seventh - 2nd inversion  = minor sixth - 1st inversion 
                                     {2,  5,  8,  2}}};  //      half-diminished seventh - 3rd inversion  = minor sixth - 2nd inversion
};

#endif
