#include <LiquidCrystal.h>

class Display : public LiquidCrystal {
    private:
        static const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
        static constexpr uint8_t lcd_nbr_line = 2;
        static constexpr uint8_t lcd_nbr_char = 16;

    public:
        Display::Display();
        Display::startup();
        Display::print_temp(float);
        Display::print_altitude(float);
        Display::print_move(const String &);
};