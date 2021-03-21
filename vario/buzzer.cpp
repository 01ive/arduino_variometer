#include "buzzer.h"
#include <Arduino.h>

Buzzer::startup() {
  for(int note=0; note < this->startup_nbr_note; note++) {
    tone(this->buzzer_pin, this->startup_notes[note], this->startup_notes_durations[note]*this->startup_tempo);
    delay(this->startup_blank);   
  }
}

Buzzer::up(float vertical_speed) {
    int note = vertical_speed * this->NOTE_AS2;
    tone(this->buzzer_pin, note, this->buzzer_duration);
}

Buzzer::down() {
    tone(this->buzzer_pin, this->down_note, this->buzzer_duration);
}

Buzzer::play_note(t_buzzer_sound note) {
  tone(this->buzzer_pin, note, this->buzzer_duration);
}
