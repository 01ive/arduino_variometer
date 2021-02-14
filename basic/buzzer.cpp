#include "buzzer.h"
#include <Arduino.h>

Buzzer::up() {
    tone(this->buzzer_pin, this->up_note, this->buzzer_duration);
}

Buzzer::down() {
    tone(this->buzzer_pin, this->down_note, this->buzzer_duration);
}
