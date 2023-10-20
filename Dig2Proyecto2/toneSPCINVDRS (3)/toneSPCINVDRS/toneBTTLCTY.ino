#include "pitches.h"

//Pine del buzzer
int buzzerPin = 2;

//Orden de las notas
int melody[] = {
  NOTE_C4, NOTE_D4, NOTE_DS4, 
  NOTE_C4, NOTE_D4, NOTE_DS4, 
  NOTE_DS4, NOTE_F4, NOTE_G4, 
  NOTE_DS4, NOTE_F4, NOTE_G4, 
  NOTE_F4, NOTE_G4, NOTE_A4, 
  NOTE_F4, NOTE_G4, NOTE_A4, 
  NOTE_GS4, NOTE_AS4, NOTE_C5, 
  NOTE_GS4, NOTE_AS4, NOTE_C5, 
  NOTE_C5, NOTE_C5, NOTE_C5, NOTE_C5, NOTE_C5
};

// Duracion de las notas
int noteDurations[] = {
  8, 8, 8, 
  8, 8, 8, 
  8, 8, 8, 
  8, 8, 8, 
  8, 8, 8, 
  8, 8, 8, 
  8, 8, 8, 
  8, 8, 8, 
  3, 8, 8, 8, 2
};

void setup() {
  //Seteo pin del buzzer como output
  pinMode(buzzerPin, OUTPUT);

  //Iteracion de las notas
  for (int thisNote = 0; thisNote < 29; thisNote++) {

    //La duracion de cada nota divide 1 segundo en segmentos
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(buzzerPin, melody[thisNote], noteDuration);

    //Se pausa 30% de la duracion de la nota entre notas
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    //Deja de sonar
    noTone(buzzerPin);
  }
}

void loop() {
  //No se repite la cancion
  }
