#include <Chords.h>



Chords         chordAnalyser;
Chord          chord;
ChordName      chordName;



void setup() {
//  chordAnalyser.addNote(48);  // C  3
//  chordAnalyser.addNote(49);  // C# 3
//  chordAnalyser.addNote(50);  // D  3
//  chordAnalyser.addNote(51);  // D# 3
//  chordAnalyser.addNote(52);  // E  3
//  chordAnalyser.addNote(53);  // F  3
//  chordAnalyser.addNote(54);  // F# 3
//  chordAnalyser.addNote(55);  // G  3
//  chordAnalyser.addNote(56);  // G# 3
//  chordAnalyser.addNote(57);  // A  3
//  chordAnalyser.addNote(58);  // A# 3
//  chordAnalyser.addNote(59);  // B  3
  chordAnalyser.addNote(60);  // C  4
//  chordAnalyser.addNote(61);  // C# 4
//  chordAnalyser.addNote(62);  // D  4
//  chordAnalyser.addNote(63);  // D# 4
  chordAnalyser.addNote(64);  // E  4
//  chordAnalyser.addNote(65);  // F  4
//  chordAnalyser.addNote(66);  // F# 4
//  chordAnalyser.addNote(67);  // G  4
  chordAnalyser.addNote(68);  // G# 4
//  chordAnalyser.addNote(69);  // A  4
  chordAnalyser.addNote(70);  // A# 4
//  chordAnalyser.addNote(71);  // B  4
//  chordAnalyser.addNote(72);  // C  5

  Serial.begin(115200);
  chord = chordAnalyser.analyseChord();
  if (chord.root == NO_CHORD) chord = chordAnalyser.analyseChord(true);
  if (chord.root != NO_CHORD) {
    chordName = chordAnalyser.getChordName(chord);

    Serial.print("Root note:   ");
    Serial.print(chordName.name);
    Serial.println(chordName.accidental);

    Serial.print("Quality:     ");
    Serial.println(chordName.quality);
 
    Serial.print("Superscript: ");
    Serial.println(chordName.superscript);

    Serial.print("\nPosition:    "); 
    switch (chord.position) {
      case ROOT_POSITION:
        Serial.println("root");
        break;
      case INVERSION_1:
        Serial.println("1st inversion");
        break;
      case INVERSION_2:
        Serial.println("2nd inversion");
        break;
      case INVERSION_3:
        Serial.println("3rd inversion");
        break;  
    }
    
    Serial.print("Octave:      ");
    Serial.println(chord.octave);
  } else Serial.println("no chord found");
}



void loop() {
}
