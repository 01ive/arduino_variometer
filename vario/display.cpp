#include "display.h"

Display::Display() : LiquidCrystal(Display::rs, Display::en, Display::d4, Display::d5, Display::d6, Display::d7) {
    this->begin(LCD_NBR_CHAR, LCD_NBR_LINE);
}

Display::startup() {
    this->print("Variometer");
}

Display::print_temp(float temp) {
    String text = String(temp) + "C";
    this->setCursor(0, 0);
    this->print(text);
}

Display::print_altitude(float altitude) {
    String text = String(altitude) + "m/s";
    this->setCursor(16 - text.length(), 0);
    this->print(text);
}

Display::print_move(const String &text) {
    this->setCursor(0, 1);
    this->print(text);
}