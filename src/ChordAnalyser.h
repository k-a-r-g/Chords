/*************************************************** 
  This is a library to analyse chords in MIDI notes

  For notaion, check examples on
  https://en.wikipedia.org/wiki/Chord_(music)
 
  Written by Karg (Timm Schlegelmilch)
  publushed under GNU license, 
  all text above must be included in any redistribution
 ****************************************************/


#ifndef _Chord_Analyser_H_
#define _Chord_Analyser_H_

#include "Arduino.h"


const uint8_t NO_CHORD                = 0;
const uint8_t ROOT_C                  = 1;
const uint8_t ROOT_C_SHARP            = 2;
const uint8_t ROOT_D_FLAT             = 2;
const uint8_t ROOT_D                  = 3;
const uint8_t ROOT_D_SHARP            = 4;
const uint8_t ROOT_E_FLAT             = 4;
const uint8_t ROOT_E                  = 5;
const uint8_t ROOT_F                  = 6;
const uint8_t ROOT_F_SHARP            = 7;
const uint8_t ROOT_G_FLAT             = 7;
const uint8_t ROOT_G                  = 8;
const uint8_t ROOT_G_SHARP            = 9;
const uint8_t ROOT_A_FLAT             = 9;
const uint8_t ROOT_A                  = 10;
const uint8_t ROOT_A_SHARP            = 11;
const uint8_t ROOT_B_FLAT             = 11;
const uint8_t ROOT_B                  = 12;


const uint8_t MAJOR_TRIAD             = 0;
const uint8_t MINOR_TRIAD             = 1;
const uint8_t AUGMENTED_TRIAD         = 2;
const uint8_t DIMINISHED_TRIAD        = 3;
const uint8_t MAJOR_SIXTH             = 4;
const uint8_t MINOR_SIXTH             = 5;
const uint8_t DOMINANT_SEVENTH        = 6;
const uint8_t MAJOR_SEVENTH           = 7; 
const uint8_t AUGMENTED_SEVENTH       = 8;
const uint8_t MINOR_SEVENTH           = MAJOR_SIXTH;
const uint8_t MINOR_MAJOR_SEVENTH     = 10;
const uint8_t DIMINISHED_SEVENTH      = 11;
const uint8_t HALF_DIMINISHED_SEVENTH = 12;
 
const uint8_t ROOT_POSITION           = 0;
const uint8_t INVERSION_1             = 1;
const uint8_t INVERSION_2             = 2;
const uint8_t INVERSION_3             = 3;

const uint8_t PSEUDO_CHORD		      = 127; 

const char    namesSharps[12]         = {'C', 'C', 'D', 'D', 'E', 'F', 'F', 'G', 'G', 'A', 'A', 'B'};
const char    namesFlats[12]          = {'C', 'D', 'D', 'E', 'E', 'F', 'G', 'G', 'A', 'A', 'B', 'B'};



struct Chord {
  uint8_t root = NO_CHORD;		
  uint8_t quality = 0;
  uint8_t position = 0;
  int8_t octave = 0;
};

struct ChordName {
  char name;							// e.g. F
  char accidental;						// e.g. #
  char quality;							// e.g. m for a minor chord
  char superscript[3];					// e.g. 7 for a minor seventh
  char octave[2];		
  // and the rest....
};



class ChordAnalyser {
public:
  ChordAnalyser(void);
  void addNote(uint8_t note);			// input: note as midi note number
  void removeNote(uint8_t note);
  Chord getChord(void);
  Chord getPseudoChord(void);
  ChordName getName(Chord chord);
  
private:  
  boolean notes[140]; 					// 128 notes + 12 for chord analysis (balance between memory and code simplicity)
  boolean analyse(boolean *_notes, Chord *_chord);  
};

#endif
