#include "buzzer.h"
#include <Arduino.h>

Buzzer::startup() {
  for(int note=0; note < this->startup_nbr_note; note++) {
    tone(this->buzzer_pin, this->startup_notes[note], this->startup_notes_durations[note]*this->startup_tempo);
    delay(this->startup_blank);   
  }
}

Buzzer::up() {
    tone(this->buzzer_pin, this->up_note, this->buzzer_duration);
}

Buzzer::down() {
    tone(this->buzzer_pin, this->down_note, this->buzzer_duration);
}
