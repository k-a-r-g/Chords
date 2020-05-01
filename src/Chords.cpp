/*************************************************** 
  This is a library to analyse chords in MIDI notes

  For notaion, check examples on
  https://en.wikipedia.org/wiki/Chord_(music)
 
  Written by Karg (Timm Schlegelmilch)
  publushed under GNU license, 
  all text above must be included in any redistribution
 ****************************************************/

#include "Arduino.h"
#include "Chords.h"



// Public functions

Chords::Chords(void) {
  for (uint8_t i=0; i<140; i++) notes[i] = false;
}



void Chords::addNote(uint8_t note) {
  notes[note] = true;
}



void Chords::removeNote(uint8_t note) {
  notes[note] = false;
}



Notes Chords::generateChord(Chord chord) {
  Notes _notes;
  if(chord.quality > NUM_QUALITIES) return _notes; // wrong input, return empty notes
  if(chord.quality < NUM_TRIADS) {		           // generate a triad
	  if (chord.position > 2) return _notes;       // wrong input, return empty notes
	  _notes.numNotes = 3;
      if (chord.position) _notes.note[0] = chord.root + 12 - TRIADS[chord.quality][chord.position][2];
	  else _notes.note[0] = chord.root;
      _notes.note[1] = _notes.note[0] + TRIADS[chord.quality][chord.position][0];
      _notes.note[2] = _notes.note[0] + TRIADS[chord.quality][chord.position][1];
  } else {								           // generate a tetrad
	  if (chord.position > 3) return _notes;	   // wrong input, return empty notes
//	  Serial.println(TETRADS[chord.quality][chord.position][0]);
	  _notes.numNotes = 4;
      if (chord.position) _notes.note[0] = chord.root + 12 - TETRADS[chord.quality][chord.position][3];
	  else _notes.note[0] = chord.root;// -TRIADS[chord.quality][chord.position][3];
      _notes.note[1] = _notes.note[0] + TETRADS[chord.quality][chord.position][0];
      _notes.note[2] = _notes.note[0] + TETRADS[chord.quality][chord.position][1];
      _notes.note[3] = _notes.note[0] + TETRADS[chord.quality][chord.position][2];   	
  }
  return _notes;
}



Chord Chords::analyseChord(boolean pseudoChord) {
  Chord _chord;
  uint8_t n = 0;	

  // Analyse (pure?) chords (consecutive notes)
  while (n<128) {                      				            // cycle through all notes
    if (notes[n])
	  if (analyse(&notes[n], &_chord) != NO_CHORD) break;	    // check if a chord structure follows
	n++;
  }
  
  if (n<127) {	
    chordNotes.pseudochord = false;                             // save the identified notes in chordNotes
  	setChordNotes(n, &_chord);
	 
	_chord.root = ((n+_chord.root)%12)+1;                       // set the return variables
	_chord.octave = (n/12)-1;
	return _chord;  
  }

  // Analyse pseudo chords (notes spread over all octaves)  
  else if (pseudoChord) {
	n = 0;
    boolean oneOctave[24];				        				// yes, that is two octaves, but we will circle through the notes of only the first 
	for (uint8_t i=0; i<24; i++) oneOctave[i] = false;
	
	while (n<128) {                                   			// cycle through all notes and condense them into one octaves
	  if (notes[n]) oneOctave[n%12] = true;
	  n++;
	}	
	n = 0;	
	while (n<12) {                                    			// check for chord structures again
	  if (oneOctave[n]) 
		if (analyse(&oneOctave[n], &_chord) != NO_CHORD) break;
	  n++;
	}
	if (n<12) {
	  chordNotes.pseudochord = true;                             // save the identified notes in chordNotes
	  setChordNotes(n, &_chord);
		
	  _chord.root = n + _chord.root + 1;
      _chord.position = PSEUDO_CHORD;
	  _chord.octave = PSEUDO_CHORD;
	}	
  }
  return _chord;
  
}


Notes Chords::getChordNotes(void) {  	
  return chordNotes;
}



ChordName Chords::getChordName(Chord chord) {
  ChordName name = {'\0', '\0', '\0', '\0\0\0'};
  if (chord.root != NO_CHORD) {
	if (chord.root > ROOT_B) chord.root = (chord.root % 12) + 1;
	name.name = NAMES_SHARPS[chord.root-1][0];
    switch(chord.root) {
      case ROOT_A_SHARP:
	    if ((name.quality == MINOR_TRIAD) || (name.quality == MAJOR_TRIAD)) {
		  name.name = NAMES_FLATS[chord.root-1][0];
          name.accidental = 'b';
	    }
		break;
      case ROOT_F_SHARP:
        name.accidental = '#';
        break;
      case ROOT_C_SHARP:
      case ROOT_D_SHARP:
	  case ROOT_G_SHARP:
	    if (name.quality == MINOR_TRIAD) {
		  name.name = NAMES_FLATS[chord.root-1][0];
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
	  case AUGMENTED_MINOR_SEVENTH:
	    name.quality = '+';	
	    strncpy(name.superscript, "m7", sizeof(name.superscript));
	    break;
  	  case AUGMENTED_MAJOR_SEVENTH:
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
      case MINOR_SEVENTH:
        name.quality = 'm';
	    strncpy(name.superscript, "7", sizeof(name.superscript));	    
		break;
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



void Chords::setChordNotes(uint8_t baseNote, Chord *chord) {  
  if (chord->quality < NUM_TRIADS) {
    chordNotes.numNotes = 3;
    chordNotes.note[0] = baseNote+chord->root-TRIADS[chord->quality][chord->position][2];
    chordNotes.note[1] = chordNotes.note[0]+TRIADS[chord->quality][chord->position][0];
    chordNotes.note[2] = chordNotes.note[0]+TRIADS[chord->quality][chord->position][1];
  }
  else {	
    chordNotes.numNotes = 4;
    chordNotes.note[0] = baseNote+chord->root-TETRADS[chord->quality-NUM_TRIADS][chord->position][3];
    chordNotes.note[1] = chordNotes.note[0]+TETRADS[chord->quality-NUM_TRIADS][chord->position][0];
    chordNotes.note[2] = chordNotes.note[0]+TETRADS[chord->quality-NUM_TRIADS][chord->position][1];
    chordNotes.note[3] = chordNotes.note[0]+TETRADS[chord->quality-NUM_TRIADS][chord->position][2];
  }		
}



String Chords::getChordString(Chord chord) {
  ChordName chordName;
  chordName = getChordName(chord);
  String string;
  string = String(chordName.name);
  if (chordName.accidental) string += chordName.accidental; 
  if (chordName.quality) string += chordName.quality; 
  if (chordName.superscript) string += chordName.superscript; 
  return string;
}



String Chords::getNoteString(uint8_t midiNote) {
  return NAMES_SHARPS[midiNote%12]; 
}



// Private functions

uint8_t Chords::analyse(boolean *notes, Chord *chord) {
  // cycle through tetrads
  for (int _quality = 0; _quality < NUM_TETRADS; _quality++) {
    for (int _position = 0; _position < 4; _position++) {
      if ((notes[TETRADS[_quality][_position][0]] && notes[TETRADS[_quality][_position][1]]) && notes[TETRADS[_quality][_position][2]]) {
        chord->root = TETRADS[_quality][_position][3]; 
  		chord->quality = _quality+NUM_TRIADS; 
  		chord->position = _position; 
        return true;
  	  }
    }
  }

  // cycle through triads
  for (int _quality = 0; _quality < NUM_TRIADS; _quality++) {
    for (int _position = 0; _position < 3; _position++) {
      if (notes[TRIADS[_quality][_position][0]] && notes[TRIADS[_quality][_position][1]]) {
        chord->root = TRIADS[_quality][_position][2]; 
		chord->quality = _quality; 
		chord->position = _position; 
		return true;
	  }
    }
  }

  // no chord identified
  return NO_CHORD;
}

