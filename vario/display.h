#include <LiquidCrystal.h>

#define LCD_NBR_LINE 2
#define LCD_NBR_CHAR 16

class Display : public LiquidCrystal {
    static const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;

    public:
        Display::Display();
        Display::startup();
        Display::print_temp(float);
        Display::print_altitude(float);
        Display::print_move(const String &);
};