#include "esphome.h"
#include "Keypad.h"

class KeypadTextSensor : public Component, public TextSensor {

    public:

    std::string keysequenz;

    static const byte n_rows = 4;
    static const byte n_cols = 3;
    
    char keys[n_rows][n_cols] = {
    {'1','2','3'},
    {'4','5','6'},
    {'7','8','9'},
    {'*','0','#'}
    };
    
    byte colPins[n_cols] = {D3, D2, D1};
    byte rowPins[n_rows] = {D7, D6, D5, D4};
    
    Keypad myKeypad = Keypad( makeKeymap(keys), rowPins, colPins, n_rows, n_cols); 

    void setup() override {
    // This will be called by App.setup()
    }
    void loop() override {
    // This will be called by App.loop()
        char myKey = myKeypad.getKey();
        if (myKey != NO_KEY){
            if (myKey == '#'){
                publish_state(keysequenz);
                keysequenz.clear();
            }
            else if (myKey == '*'){
                keysequenz.clear();
                publish_state(keysequenz);
            }
            else {
                keysequenz.push_back(myKey);
            }
        }
    }
};