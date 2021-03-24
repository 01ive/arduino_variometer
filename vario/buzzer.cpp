#include "buzzer.h"
#include <Arduino.h>

Buzzer::startup() {
  const long startup_tempo = 400;
  const long startup_blank = 200;
  const int startup_nbr_note = 4;
  const t_buzzer_bip startup_notes[4] = {this->NOTE_D6, this->NOTE_C6, this->NOTE_B5, this->NOTE_AS5};
  const long startup_notes_durations[4] = {2, 3, 3, 3};

  for(int note=0; note < startup_nbr_note; note++) {
    tone(this->buzzer_pin, startup_notes[note], startup_notes_durations[note]*startup_tempo);
    delay(startup_blank);   
  }
}

Buzzer::sound_tick() {
  unsigned long time_ms = millis();

  if(this->p_current_sound != NULL) {
    if( time_ms - this->sound_start_time_ms > this->p_current_sound->duration ) {
      tone( this->buzzer_pin, 
            this->p_current_sound->bip_note, 
            this->p_current_sound->bip_duration);
      this->sound_start_time_ms = time_ms;
    }
  }
}

Buzzer::repeat_sound(t_buzzer_sound *p_sound) {
  this->p_current_sound = p_sound;
}

Buzzer::stop_sound() {
  this->p_current_sound = NULL;
}