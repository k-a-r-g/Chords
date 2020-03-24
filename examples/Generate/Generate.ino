#include <Chords.h>



Chords         chordGenerator;
Chord          chord;
ChordName      chordName;
Notes          notes;



void setup() {
  Serial.begin(115200);
  chord.root = 60;                  // midi number for C3
  chord.quality = MAJOR_SEVENTH;
  chord.position = INVERSION_2;
  
  notes = chordGenerator.generateChord(chord);
  if (notes.numNotes) {
    Serial.print("Chord:  ");
    Serial.println(chordGenerator.getChordString(chord));

    Serial.print("Octave: ");
    Serial.println((notes.note[0]/12)-2);
    Serial.println();
    
    for(int i=0; i<notes.numNotes; i++) {
      Serial.print("Note ");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(chordGenerator.getNoteString(notes.note[i]));
    }
  } else Serial.println("Wrong input: cannot generate chord");
}



void loop() {
}
