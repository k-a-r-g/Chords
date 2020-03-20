/*************************************************** 
  This is a library to analyse chords in MIDI notes

  For notaion, check examples on
  https://en.wikipedia.org/wiki/Chord_(music)
 
  Written by Karg (Timm Schlegelmilch)
  publushed under GNU license, 
  all text above must be included in any redistribution
 ****************************************************/

#include "Arduino.h"
#include "ChordAnalyser.h"



ChordAnalyser::ChordAnalyser(void) {
  for (uint8_t i=0; i<140; i++) notes[i] = false;
}



void ChordAnalyser::addNote(uint8_t note) {
  notes[note] = true;
}



void ChordAnalyser::removeNote(uint8_t note) {
  notes[note] = false;
}



Chord ChordAnalyser::getChord(void) {
  Chord _chord;
  uint8_t n = 0;	
  while (n<128) {                      				// cycle through all notes
    if (notes[n]) {
	  if (analyse(&notes[n], &_chord)) break;	    // check if a chord structure follows
	}  
	n++;
  }
  if (n<127) {
    _chord.root = (n%12)+1;
    _chord.octave = (n/12)-1;
  }      
  return _chord;
}



Chord ChordAnalyser::getPseudoChord(void) {
  Chord _pseudoChord;
  boolean oneOctave[24];				            // yes, that is two octaves, but we will circle through the notes of only the first 
  for (uint8_t i=0; i<24; i++) oneOctave[i] = false;
  uint8_t n = 0;	
  while (n<128) {                                   // cycle through all notes and condense them into one octaves
    if (notes[n]) oneOctave[n%12] = true;
    n++;
  }	
  n = 0;	
  while (n<12) {                                    // check for chord structures again
    if (analyse(&oneOctave[n], &_pseudoChord)) break;
    n++;
  }
  if (n<12) {
    _pseudoChord.root = n+1;
    _pseudoChord.octave = PSEUDO_CHORD;
  }
  return _pseudoChord;
}



ChordName ChordAnalyser::getName(Chord chord) {
  ChordName name = {'\0', '\0', '\0', '\0\0\0'};
  if (chord.root != NO_CHORD) {
    name.name = namesSharps[chord.root-1];
    switch(chord.root) {
      case ROOT_A_SHARP:
        name.name = namesFlats[chord.root-1];
        name.accidental = 'b';
        break;
      case ROOT_F_SHARP:
        name.accidental = '#';
        break;
      case ROOT_C_SHARP:
      case ROOT_D_SHARP:
	  case ROOT_G_SHARP:
	    if (name.quality == MINOR_TRIAD) {
		  name.name = namesFlats[chord.root-1];
		  name.accidental = 'b';
	    } else name.accidental = '#';
	    break;	
    }

    switch (chord.quality) {
      case MAJOR_TRIAD:
        name.quality = 0;
	    break;
	  case MAJOR_SIXTH:
        name.quality = 0;
        strncpy(name.superscript, "6", sizeof(name.superscript));      
        break;
	  case DOMINANT_SEVENTH:
	    name.quality = 0;
	    strncpy(name.superscript, "7", sizeof(name.superscript));
	    break;
	  case MAJOR_SEVENTH:
	    name.quality = 'M';
	    strncpy(name.superscript, "7", sizeof(name.superscript));
	    break;
	  case AUGMENTED_TRIAD:
        name.quality = '+';	
        break;
	  case AUGMENTED_SEVENTH:
	    name.quality = '+';	
	    strncpy(name.superscript, "7", sizeof(name.superscript));
	    break;
	  case MINOR_TRIAD:
        name.quality = 'm';
        break;
	  case MINOR_SIXTH:
        name.quality = 'm';
        strncpy(name.superscript, "6", sizeof(name.superscript));      
        break;
	  // case MINOR_SEVENTH:
	  case MINOR_MAJOR_SEVENTH:
	    name.quality = 'm';
	    strncpy(name.superscript, "M7", sizeof(name.superscript));
	    break;
      case DIMINISHED_TRIAD:
        strncpy(name.superscript, "o", sizeof(name.superscript));
        break;
      case DIMINISHED_SEVENTH:
        strncpy(name.superscript, "o7", sizeof(name.superscript));
        break;
	  case HALF_DIMINISHED_SEVENTH:
        strncpy(name.superscript, "ø7", sizeof(name.superscript));
        break;	  
    }
  } else name.name = 0;
  return name;
}



boolean ChordAnalyser::analyse(boolean *_notes, Chord *_chord) {
	
  // major sixth
  if ((_notes[4] && _notes[7]) && _notes[9]) { _chord->quality = MAJOR_SIXTH; _chord->position = ROOT_POSITION; return true; }  // major sixth - root (minor seventh - 1st inversion)
  else if ((_notes[3] && _notes[4]) && _notes[7]) { _chord->quality = MAJOR_SIXTH; _chord->position = INVERSION_1; return true; }  // major sixth - 1st inversion
  else if ((_notes[2] && _notes[4]) && _notes[8]) { _chord->quality = MAJOR_SIXTH; _chord->position = INVERSION_2; return true; }  // major sixth - 2nd inversion
  else if ((_notes[3] && _notes[7]) && _notes[10]) { _chord->quality = MAJOR_SIXTH; _chord->position = INVERSION_3; return true; } // major sixth - 3rd inversion (minor seventh - root)

  // minor sixth
  else if ((_notes[3] && _notes[7]) && _notes[9]) { _chord->quality = MINOR_SIXTH; _chord->position = ROOT_POSITION; return true; }  // minor sixth - root (half-diminished seventh - 1st inversion)
  else if ((_notes[4] && _notes[6]) && _notes[9]) { _chord->quality = MINOR_SIXTH; _chord->position = INVERSION_1; return true; }  // minor sixth - 1st inversion (half-diminished seventh - 2nd inversion)
  else if ((_notes[2] && _notes[5]) && _notes[8]) { _chord->quality = MINOR_SIXTH; _chord->position = INVERSION_2; return true; }  // minor sixth - 2nd inversion
  else if ((_notes[3] && _notes[6]) && _notes[10]) { _chord->quality = MINOR_SIXTH; _chord->position = INVERSION_3; return true; } // minor sixth - 3rd inversion

  // major seventh
  else if ((_notes[4] && _notes[7]) && _notes[11]) { _chord->quality = MAJOR_SEVENTH; _chord->position = ROOT_POSITION; return true; } // major seventh - root	
  else if ((_notes[3] && _notes[7]) && _notes[8]) { _chord->quality = MAJOR_SEVENTH; _chord->position = INVERSION_1; return true; } // major seventh - 1st inversion
  else if ((_notes[4] && _notes[5]) && _notes[9]) { _chord->quality = MAJOR_SEVENTH; _chord->position = INVERSION_2; return true; } // major seventh - 2nd inversion
  else if ((_notes[1] && _notes[5]) && _notes[8]) { _chord->quality = MAJOR_SEVENTH; _chord->position = INVERSION_3; return true; } // major seventh - 3rd inversion

  // minor seventh = major sixth

  // minor-major seventh
  else if ((_notes[3] && _notes[7]) && _notes[11]) { _chord->quality = MINOR_MAJOR_SEVENTH; _chord->position = ROOT_POSITION; return true; } // minor-major seventh - root
  else if ((_notes[4] && _notes[8]) && _notes[9]) { _chord->quality = MINOR_MAJOR_SEVENTH; _chord->position = INVERSION_1; return true; } // minor-major seventh - 1st inversion
  else if ((_notes[4] && _notes[5]) && _notes[8]) { _chord->quality = MINOR_MAJOR_SEVENTH; _chord->position = INVERSION_2; return true; } // minor-major seventh - 2nd inversion
  else if ((_notes[1] && _notes[4]) && _notes[8]) { _chord->quality = MINOR_MAJOR_SEVENTH; _chord->position = INVERSION_3; return true; } // minor-major seventh - 3rd inversion

  // augmented seventh
  else if ((_notes[4] && _notes[8]) && _notes[12]) { _chord->quality = AUGMENTED_SEVENTH; _chord->position = ROOT_POSITION; return true; } // augmented seventh - root  
  else if ((_notes[4] && _notes[6]) && _notes[8]) { _chord->quality = AUGMENTED_SEVENTH; _chord->position = INVERSION_1; return true; } // augmented seventh - 1st inversion
  else if ((_notes[2] && _notes[4]) && _notes[8]) { _chord->quality = AUGMENTED_SEVENTH; _chord->position = INVERSION_2; return true; }  // augmented seventh - 2nd inversion
  else if ((_notes[2] && _notes[6]) && _notes[10]) { _chord->quality = AUGMENTED_SEVENTH; _chord->position = INVERSION_3; return true; } // augmented seventh - 3rd inversion

  // half-diminished seventh
  else if ((_notes[2] && _notes[5]) && _notes[8]) { _chord->quality = HALF_DIMINISHED_SEVENTH; _chord->position = INVERSION_3; return true; } //half-diminished seventh - 3rd inversion

  // dominant seventh
  else if ((_notes[4] && _notes[7]) && _notes[10]) { _chord->quality = DOMINANT_SEVENTH; _chord->position = ROOT_POSITION; return true; } // dominant seventh - root
  else if ((_notes[3] && _notes[6]) && _notes[8]) { _chord->quality = DOMINANT_SEVENTH; _chord->position = INVERSION_1; return true; } // dominant seventh - 1st inversion
  else if ((_notes[3] && _notes[5]) && _notes[9]) { _chord->quality = DOMINANT_SEVENTH; _chord->position = INVERSION_2; return true; } // dominant seventh - 2nd inversion
  else if ((_notes[2] && _notes[6]) && _notes[9]) { _chord->quality = DOMINANT_SEVENTH; _chord->position = INVERSION_3; return true; } // dominant seventh - 3rd inversion

  // diminished seventh
  else if ((_notes[3] && _notes[6]) && _notes[9]) { _chord->quality = DIMINISHED_SEVENTH; _chord->position = ROOT_POSITION; return true; }	// diminished seventh - root
 
  // major triad
  else if (_notes[4] && _notes[7]) { _chord->quality = MAJOR_TRIAD; _chord->position = ROOT_POSITION; return true; } // major triad - root
  else if (_notes[3] && _notes[8]) { _chord->quality = MAJOR_TRIAD; _chord->position = INVERSION_1; return true; } // major triad - 1st inversion
  else if (_notes[5] && _notes[9]) { _chord->quality = MAJOR_TRIAD; _chord->position = INVERSION_2; return true; } // major triad - 2nd inversion

  // minor triad
  else if (_notes[3] && _notes[7]) { _chord->quality = MINOR_TRIAD; _chord->position = ROOT_POSITION; return true; } // minor triad - root
  else if (_notes[4] && _notes[9]) { _chord->quality = MINOR_TRIAD; _chord->position = INVERSION_1; return true; } // minor triad - 1st inversion
  else if (_notes[5] && _notes[8]) { _chord->quality = MINOR_TRIAD; _chord->position = INVERSION_2; return true; } // minor triad - 2nd inversion
	
  // augmented triad
  else if (_notes[4] && _notes[8]) { _chord->quality = AUGMENTED_TRIAD; _chord->position = ROOT_POSITION; return true; } // augmented triad

  // diminished triad
  else if (_notes[3] && _notes[6]) { _chord->quality = DIMINISHED_TRIAD; _chord->position = ROOT_POSITION; return true; } // diminished triad - root
  else if (_notes[3] && _notes[9]) { _chord->quality = DIMINISHED_TRIAD; _chord->position = INVERSION_1; return true; } // diminished triad - 1st inversion
  else if (_notes[6] && _notes[9]) { _chord->quality = DIMINISHED_TRIAD; _chord->position = INVERSION_2; return true; } // diminished triad - 2nd inversion

  // no chord identified
  else return false;
}

