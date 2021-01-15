#include "esphome.h"
#include "FastLED.h"

    #define LED_PIN  D4
    #define COLOR_ORDER GRB
    #define CHIPSET     WS2812B
    const uint8_t kMatrixWidth = 14;
    const uint8_t kMatrixHeight = 14;
    const bool    kMatrixSerpentineLayout = true;
    #define NUM_LEDS (kMatrixWidth * kMatrixHeight)
    CRGB leds_plus_safety_pixel[ NUM_LEDS + 1];
    CRGB* const leds( leds_plus_safety_pixel + 1);

class WordclockLightOutput : public PollingComponent, public LightOutput
{
public:

    WordclockLightOutput() : PollingComponent(1000) {}
    CRGB currentColor;
    CRGB dayColor;
    CRGB monthColor;
    int test = 0;

    void setup() override
    {
        // This will be called by App.setup()
        FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
    }
    LightTraits get_traits() override
    {
        // return the traits this light supports
        auto traits = LightTraits();
        traits.set_supports_brightness(true);
        traits.set_supports_rgb(true);
        traits.set_supports_rgb_white_value(false);
        traits.set_supports_color_temperature(false);
        return traits;
    }

    void write_state(LightState *state) override
    {
        ESP_LOGD("Wordclock", "write_state aufgerufen");
        // This will be called by the light to get a new state to be written.
        // use any of the provided current_values methods
        float red, green, blue;
        state->current_values_as_rgb(&red, &green, &blue);
        ESP_LOGD("Wordclock", "r: %f, g: %f, b: %d", red, green*255.0, byte(blue*255.0));
        currentColor.red = red*255.0;
        currentColor.green = green*255.0;
        currentColor.blue = blue*255.0;
        CHSV complementary = rgb2hsv_approximate(currentColor);
        complementary.hue += 128;
        hsv2rgb_rainbow(complementary, dayColor);
        hsv2rgb_rainbow(complementary, monthColor);
        // Write red, green and blue to HW
        // ...
        
        setPixels();
        
        FastLED.show();
    }

    // void write_time(int Hour, int Minute)
    void update() override
    {
        //ESP_LOGD("Wordclock", "update: H:%d M:%d M5:%d D:%d M:%d", Hour, Minute, MinuteMod5, Day, Month);

        for(int x = 0; x < kMatrixHeight; x++)
        {
            for(int y = 0; y < kMatrixWidth; y++)
            {
                leds[XYsafe(x, y)] = CRGB::Black;
            }
        }

        setPixels();
        
        FastLED.show();
    }

    void setPixels()
    {
        auto time = HA_time->now();
        int Hour, Minute, MinuteMod5, Day, Month;
        Hour = time.hour;
        Minute = time.minute;
        MinuteMod5 = time.minute % 5;
        Day = time.day_of_month;
        Month = time.month;

        switch (Day)
        {
        case 1:
            tag_1();
            break;
        case 2:
            tag_2();
            break;
        case 3:
            tag_3();
            break;
        case 4:
            tag_4();
            break;
        case 5:
            tag_5();
            break;
        case 6:
            tag_6();
            break;
        case 7:
            tag_7();
            break;
        case 8:
            tag_8();
            break;
        case 9:
            tag_9();
            break;
        case 10:
            tag_0();
            tag_10();
            break;
        case 11:
            tag_1();
            tag_10();
            break;
        case 12:
            tag_2();
            tag_10();
            break;
        case 13:
            tag_3();
            tag_10();
            break;
        case 14:
            tag_4();
            tag_10();
            break;
        case 15:
            tag_5();
            tag_10();
            break;
        case 16:
            tag_6();
            tag_10();
            break;
        case 17:
            tag_7();
            tag_10();
            break;
        case 18:
            tag_8();
            tag_10();
            break;
        case 19:
            tag_9();
            tag_10();
            break;
        case 20:
            tag_0();
            tag_20();
            break;
        case 21:
            tag_1();
            tag_20();
            break;
        case 22:
            tag_2();
            tag_20();
            break;
        case 23:
            tag_3();
            tag_20();
            break;
        case 24:
            tag_4();
            tag_20();
            break;
        case 25:
            tag_5();
            tag_20();
            break;
        case 26:
            tag_6();
            tag_20();
            break;
        case 27:
            tag_7();
            tag_20();
            break;
        case 28:
            tag_8();
            tag_20();
            break;
        case 29:
            tag_9();
            tag_20();
            break;
        case 30:
            tag_0();
            tag_30();
            break;
        case 31:
            tag_1();
            tag_30();
            break;
        default:
            break;
        }

        switch (Month)
        {
        case 1:
            januar();
            break;
        case 2:
            februar();
            break;
        case 3:
            maerz();
            break;
        case 4:
            april();
            break;
        case 5:
            mai();
            break;
        case 6:
            juni();
            break;
        case 7:
            juli();
            break;
        case 8:
            august();
            break;
        case 9:
            september();
            break;
        case 10:
            oktober();
            break;
        case 11:
            november();
            break;
        case 12:
            dezember();
            break;
        default:
            break;
        }

        es();

        if ((MinuteMod5 == 1 || MinuteMod5 == 2) && (Minute != 0))
        {
            war();
        }
        else
        {
            ist();
        }

        if(MinuteMod5 == 3 || MinuteMod5 == 4)
        {
            gleich();
        }

        if(Minute == 0)
        {
            genau();
        }

        if((MinuteMod5 == 0 || MinuteMod5 == 1 || MinuteMod5 == 2) && (Minute != 0))
        {
            gerade();
        }

        if ((Minute >= 3 && Minute <= 7) ||
            (Minute >= 23 && Minute <= 27) ||
            (Minute >= 33 && Minute <= 37) ||
            (Minute >= 53 && Minute <= 57))
        {
            m_fuenf();
        }
        if ((Minute >= 13 && Minute <= 17) ||
            (Minute >= 43 && Minute <= 47))
        {
            m_viertel();
        }
        if ((Minute >= 8 && Minute <= 12) ||
            (Minute >= 48 && Minute <= 52))
        {
            m_zehn();
        }
        if ((Minute >= 18 && Minute <= 22) ||
            (Minute >= 38 && Minute <= 42))
        {
            m_zwanzig();
        }
        if ((Minute >= 3 && Minute <= 22) ||
            (Minute >= 33 && Minute <= 37))
        {
            m_nach();
        }
        if ((Minute >= 23 && Minute <= 27) ||
            (Minute >= 38 && Minute <= 57))
        {
            m_vor();
        }
        if (Minute >= 23 && Minute <= 37)
        {
            m_halb();
        }
        if (((Hour == 1 || Hour == 13) && Minute <= 22) ||
            ((Hour == 0 || Hour == 12) && Minute >= 23))
        {
            eins();
        }
        if (((Hour == 2 || Hour == 14) && Minute <= 22) ||
            ((Hour == 1 || Hour == 13) && Minute >= 23))
        {
            zwei();
        }
        if (((Hour == 3 || Hour == 15) && Minute <= 22) ||
            ((Hour == 2 || Hour == 14) && Minute >= 23))
        {
            drei();
        }
        if (((Hour == 4 || Hour == 16) && Minute <= 22) ||
            ((Hour == 3 || Hour == 15) && Minute >= 23))
        {
            vier();
        }
        if (((Hour == 5 || Hour == 17) && Minute <= 22) ||
            ((Hour == 4 || Hour == 16) && Minute >= 23))
        {
            fuenf();
        }
        if (((Hour == 6 || Hour == 18) && Minute <= 22) ||
            ((Hour == 5 || Hour == 17) && Minute >= 23))
        {
            sechs();
        }
        if (((Hour == 7 || Hour == 19) && Minute <= 22) ||
            ((Hour == 6 || Hour == 18) && Minute >= 23))
        {
            sieben();
        }
        if (((Hour == 8 || Hour == 20) && Minute <= 22) ||
            ((Hour == 7 || Hour == 19) && Minute >= 23))
        {
            acht();
        }
        if (((Hour == 9 || Hour == 21) && Minute <= 22) ||
            ((Hour == 8 || Hour == 20) && Minute >= 23))
        {
            neun();
        }
        if (((Hour == 10 || Hour == 22) && Minute <= 22) ||
            ((Hour == 9 || Hour == 21) && Minute >= 23))
        {
            zehn();
        }
        if (((Hour == 11 || Hour == 23) && Minute <= 22) ||
            ((Hour == 10 || Hour == 22) && Minute >= 23))
        {
            elf();
        }
        if (((Hour == 12 || Hour == 0) && Minute <= 22) ||
            ((Hour == 11 || Hour == 23) && Minute >= 23))
        {
            zwoelf();
        }
        if (Minute >= 58 || Minute <= 2)
        {
            uhr();
        }
        if ((Hour == 3 && Minute >= 23) ||
            (Hour >= 4 && Hour <= 8) ||
            (Hour == 9 && Minute <= 22))
        {
            morgens();
        }
        if ((Hour == 9 && Minute >= 23) ||
            (Hour == 10) ||
            (Hour == 11 && Minute <= 22))
        {
            vor();
            mittags();
        }
        if ((Hour == 11 && Minute >= 23) ||
            (Hour == 12) ||
            (Hour == 13 && Minute <= 22))
        {
            mittags();
        }
        if ((Hour == 13 && Minute >= 23) ||
            (Hour >= 14 && Hour <= 17) ||
            (Hour == 18 && Minute <= 22))
        {
            nach();
            mittags();
        }
        if ((Hour == 18 && Minute >= 23) ||
            (Hour >= 19 && Hour <= 21) ||
            (Hour == 22 && Minute <= 22))
        {
            abends();
        }
        if ((Hour == 22 && Minute >= 23) ||
            (Hour >= 23 || Hour <= 2) ||
            (Hour == 3 && Minute <= 22))
        {
            nachts();
        }
    }

    uint16_t XY( uint8_t x, uint8_t y)
    {
        uint16_t i;
        
        if( kMatrixSerpentineLayout == false) {
            i = (y * kMatrixWidth) + x;
        }

        if( kMatrixSerpentineLayout == true) {
            if( y & 0x01) {
            // Odd rows run backwards
            uint8_t reverseX = (kMatrixWidth - 1) - x;
            i = (y * kMatrixWidth) + reverseX;
            } else {
            // Even rows run forwards
            i = (y * kMatrixWidth) + x;
            }
        }
        
        return i;
    }

    uint16_t XYsafe( uint8_t x, uint8_t y)
    {
        if( x >= kMatrixWidth) return -1;
        if( y >= kMatrixHeight) return -1;
        return XY(x,y);
    }

    void es(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->currentColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(13, 0)] = color;
        leds[this->XYsafe(13, 1)] = color;
    }

    void war(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->currentColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(13, 7)] = color;
        leds[this->XYsafe(13, 8)] = color;
        leds[this->XYsafe(13, 9)] = color;
    }

    void ist(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->currentColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(13, 11)] = color;
        leds[this->XYsafe(13, 12)] = color;
        leds[this->XYsafe(13, 13)] = color;
    }

    void wird(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->currentColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(12, 0)] = color;
        leds[this->XYsafe(12, 1)] = color;
        leds[this->XYsafe(12, 2)] = color;
        leds[this->XYsafe(12, 3)] = color;
    }

    void gerade(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->currentColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(12, 8)] = color;
        leds[this->XYsafe(12, 9)] = color;
        leds[this->XYsafe(12, 10)] = color;
        leds[this->XYsafe(12, 11)] = color;
        leds[this->XYsafe(12, 12)] = color;
        leds[this->XYsafe(12, 13)] = color;
    }

    void genau(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->currentColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(11, 0)] = color;
        leds[this->XYsafe(11, 1)] = color;
        leds[this->XYsafe(11, 2)] = color;
        leds[this->XYsafe(11, 3)] = color;
        leds[this->XYsafe(11, 4)] = color;
    }

    void gleich(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->currentColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(11, 8)] = color;
        leds[this->XYsafe(11, 9)] = color;
        leds[this->XYsafe(11, 10)] = color;
        leds[this->XYsafe(11, 11)] = color;
        leds[this->XYsafe(11, 12)] = color;
        leds[this->XYsafe(11, 13)] = color;
    }

    void m_fuenf(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->currentColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(10, 0)] = color;
        leds[this->XYsafe(10, 1)] = color;
        leds[this->XYsafe(10, 2)] = color;
        leds[this->XYsafe(10, 3)] = color;
    }

    void m_viertel(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->currentColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(10, 7)] = color;
        leds[this->XYsafe(10, 8)] = color;
        leds[this->XYsafe(10, 9)] = color;
        leds[this->XYsafe(10, 10)] = color;
        leds[this->XYsafe(10, 11)] = color;
        leds[this->XYsafe(10, 12)] = color;
        leds[this->XYsafe(10, 13)] = color;
    }

    void m_zehn(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->currentColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(9, 0)] = color;
        leds[this->XYsafe(9, 1)] = color;
        leds[this->XYsafe(9, 2)] = color;
        leds[this->XYsafe(9, 3)] = color;
    }

    void m_zwanzig(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->currentColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(9, 7)] = color;
        leds[this->XYsafe(9, 8)] = color;
        leds[this->XYsafe(9, 9)] = color;
        leds[this->XYsafe(9, 10)] = color;
        leds[this->XYsafe(9, 11)] = color;
        leds[this->XYsafe(9, 12)] = color;
        leds[this->XYsafe(9, 13)] = color;
    }

    void m_nach(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->currentColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(8, 0)] = color;
        leds[this->XYsafe(8, 1)] = color;
        leds[this->XYsafe(8, 2)] = color;
        leds[this->XYsafe(8, 3)] = color;
    }

    void m_vor(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->currentColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(8, 6)] = color;
        leds[this->XYsafe(8, 7)] = color;
        leds[this->XYsafe(8, 8)] = color;
    }

    void m_halb(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->currentColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(8, 10)] = color;
        leds[this->XYsafe(8, 11)] = color;
        leds[this->XYsafe(8, 12)] = color;
        leds[this->XYsafe(8, 13)] = color;
    }

    void eins(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->currentColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(7, 0)] = color;
        leds[this->XYsafe(7, 1)] = color;
        leds[this->XYsafe(7, 2)] = color;
        leds[this->XYsafe(7, 3)] = color;
    }

    void zwei(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->currentColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(7, 6)] = color;
        leds[this->XYsafe(7, 7)] = color;
        leds[this->XYsafe(7, 8)] = color;
        leds[this->XYsafe(7, 9)] = color;
    }

    void drei(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->currentColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(7, 10)] = color;
        leds[this->XYsafe(7, 11)] = color;
        leds[this->XYsafe(7, 12)] = color;
        leds[this->XYsafe(7, 13)] = color;
    }

    void vier(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->currentColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(6, 0)] = color;
        leds[this->XYsafe(6, 1)] = color;
        leds[this->XYsafe(6, 2)] = color;
        leds[this->XYsafe(6, 3)] = color;
    }

    void fuenf(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->currentColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(6, 10)] = color;
        leds[this->XYsafe(6, 11)] = color;
        leds[this->XYsafe(6, 12)] = color;
        leds[this->XYsafe(6, 13)] = color;
    }

    void sechs(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->currentColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(5, 0)] = color;
        leds[this->XYsafe(5, 1)] = color;
        leds[this->XYsafe(5, 2)] = color;
        leds[this->XYsafe(5, 3)] = color;
        leds[this->XYsafe(5, 4)] = color;
    }

    void sieben(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->currentColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(5, 8)] = color;
        leds[this->XYsafe(5, 9)] = color;
        leds[this->XYsafe(5, 10)] = color;
        leds[this->XYsafe(5, 11)] = color;
        leds[this->XYsafe(5, 12)] = color;
        leds[this->XYsafe(5, 13)] = color;
    }

    void acht(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->currentColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(4, 0)] = color;
        leds[this->XYsafe(4, 1)] = color;
        leds[this->XYsafe(4, 2)] = color;
        leds[this->XYsafe(4, 3)] = color;
    }

    void neun(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->currentColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(4, 6)] = color;
        leds[this->XYsafe(4, 7)] = color;
        leds[this->XYsafe(4, 8)] = color;
        leds[this->XYsafe(4, 9)] = color;
    }

    void zehn(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->currentColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(4, 10)] = color;
        leds[this->XYsafe(4, 11)] = color;
        leds[this->XYsafe(4, 12)] = color;
        leds[this->XYsafe(4, 13)] = color;
    }

    void elf(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->currentColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(3, 0)] = color;
        leds[this->XYsafe(3, 1)] = color;
        leds[this->XYsafe(3, 2)] = color;
    }

    void zwoelf(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->currentColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(3, 9)] = color;
        leds[this->XYsafe(3, 10)] = color;
        leds[this->XYsafe(3, 11)] = color;
        leds[this->XYsafe(3, 12)] = color;
        leds[this->XYsafe(3, 13)] = color;
    }

    void uhr(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->currentColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(2, 0)] = color;
        leds[this->XYsafe(2, 1)] = color;
        leds[this->XYsafe(2, 2)] = color;
    }

    void vor(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->currentColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(2, 7)] = color;
        leds[this->XYsafe(2, 8)] = color;
        leds[this->XYsafe(2, 9)] = color;
    }

    void nach(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->currentColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(2, 10)] = color;
        leds[this->XYsafe(2, 11)] = color;
        leds[this->XYsafe(2, 12)] = color;
        leds[this->XYsafe(2, 13)] = color;
    }

    void morgens(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->currentColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(1, 0)] = color;
        leds[this->XYsafe(1, 1)] = color;
        leds[this->XYsafe(1, 2)] = color;
        leds[this->XYsafe(1, 3)] = color;
        leds[this->XYsafe(1, 4)] = color;
        leds[this->XYsafe(1, 5)] = color;
        leds[this->XYsafe(1, 6)] = color;
    }

    void mittags(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->currentColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(1, 7)] = color;
        leds[this->XYsafe(1, 8)] = color;
        leds[this->XYsafe(1, 9)] = color;
        leds[this->XYsafe(1, 10)] = color;
        leds[this->XYsafe(1, 11)] = color;
        leds[this->XYsafe(1, 12)] = color;
        leds[this->XYsafe(1, 13)] = color;
    }

    void nachts(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->currentColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(0, 0)] = color;
        leds[this->XYsafe(0, 1)] = color;
        leds[this->XYsafe(0, 2)] = color;
        leds[this->XYsafe(0, 3)] = color;
        leds[this->XYsafe(0, 4)] = color;
        leds[this->XYsafe(0, 5)] = color;
    }

    void abends(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->currentColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(0, 8)] = color;
        leds[this->XYsafe(0, 9)] = color;
        leds[this->XYsafe(0, 10)] = color;
        leds[this->XYsafe(0, 11)] = color;
        leds[this->XYsafe(0, 12)] = color;
        leds[this->XYsafe(0, 13)] = color;
    }

    void tag_0(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->dayColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(13, 6)] = color;
    }

    void tag_1(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->dayColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(12, 6)] = color;
    }

    void tag_2(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->dayColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(11, 6)] = color;
    }

    void tag_3(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->dayColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(10, 6)] = color;
    }

    void tag_4(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->dayColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(9, 6)] = color;
    }

    void tag_5(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->dayColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(10, 4)] = color;
    }

    void tag_6(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->dayColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(9, 4)] = color;
    }

    void tag_7(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->dayColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(8, 4)] = color;
    }

    void tag_8(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->dayColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(7, 4)] = color;
    }

    void tag_9(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->dayColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(6, 4)] = color;
    }

    void tag_10(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->dayColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(13, 2)] = color;
    }

    void tag_20(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->dayColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(13, 3)] = color;
    }

    void tag_30(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->dayColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(13, 4)] = color;
    }

    void januar(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->monthColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(13, 5)] = color;
    }

    void februar(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->monthColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(12, 5)] = color;
    }

    void maerz(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->monthColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(11, 5)] = color;
    }

    void april(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->monthColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(10, 5)] = color;
    }

    void mai(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->monthColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(9, 5)] = color;
    }

    void juni(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->monthColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(8, 5)] = color;
    }

    void juli(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->monthColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(7, 5)] = color;
    }

    void august(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->monthColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(6, 5)] = color;
    }

    void september(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->monthColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(5, 5)] = color;
    }

    void oktober(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->monthColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(4, 5)] = color;
    }

    void november(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->monthColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(3, 5)] = color;
    }

    void dezember(bool on = true)
    {
        CRGB color;
        if(on)
        {
            color = this->monthColor;
        }
        else
        {
            color = CRGB::Black;
        }
        leds[this->XYsafe(2, 5)] = color;
    }

};