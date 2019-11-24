/* ----------------------------------------------------------------------------
// General constants
 ---------------------------------------------------------------------------- */
bool DEBUG;

// analog inputs
int a0 = 0;
int a1 = 0;
int a2 = 0;


int last_a0 = 0;
int last_a1 = 0;
int last_a2 = 0;


/* ----------------------------------------------------------------------------
// LCD constants
 ---------------------------------------------------------------------------- */
#include <LiquidCrystal.h>

// You could use other kind of LCD screens.
// Just make sure it has at least 16 cols and at least 2 rows :).
#define COLS 16
#define ROWS 2
#define LCD_DELAY 120
 
//LiquidCrystal(rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(19, 18, 5, 4, 3, 2);

String statusString;


/* ----------------------------------------------------------------------------
// Keypad constants
 -----------------------------------------------------------------------------*/
#include <Keypad.h>

const byte ROWS = 3; 
const byte COLS = 2; 


char hexaKeys[ROWS][COLS] = {
  {'2', '1'},
  {'3', '4'},
  {'5', '6'},
};


byte rowPins[ROWS] = {6, 7, 8}; 
byte colPins[COLS] = {16, 17}; 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
char customKey;
char last_key = ' ';


/* ----------------------------------------------------------------------------
// MIDI constants
 ---------------------------------------------------------------------------- */
// Control Change message
// Channel followed by CC command (0xB)
// Channel set to 16
int controlChange = 0xB0;
int noteChange = 0x90;

// Controller number
// this is the second byte on the midi message
// CC value
int cca0 = 110;
int cca1 = 111;
int cca2 = 112;

// Key controller addr
// set to 90 for base
int cck = 90;

int pressedKey = 91;


/* ----------------------------------------------------------------------------
// RGB Led constants
 ---------------------------------------------------------------------------- */
int red_pin= 11;
int green_pin = 10;
int blue_pin = 9;

unsigned long last_check = 0;

int color_counter = 0;

// RGB map for the colors
int colorArray[9][3] = {
  {0, 0, 0},
  {255, 0, 0},
  {0, 255, 0},
  {0, 0, 255},
  {255, 255, 125},
  {0, 255, 255},
  {255, 0, 255},
  {255, 255, 0},
  {255, 255, 255},
};

int r,g,b;


/* ----------------------------------------------------------------------------
// Setup
 ---------------------------------------------------------------------------- */
void setup() {

  DEBUG = false;

  if (DEBUG == false) {
    // enable midi communicaiton
    Serial.begin(31250);
  } else {
    // enable debug mode
    Serial.begin(9600);
  }
 
  // start LCD
  lcd.begin(COLS, ROWS);
  lcd.clear();
  lcd.setCursor(0, 0);
  String headerString = " A0  A1  A2   K";
  lcd.print(headerString);


  // RGB configuration
  pinMode(red_pin, OUTPUT);
  pinMode(green_pin, OUTPUT);
  pinMode(blue_pin, OUTPUT);

}


/* ----------------------------------------------------------------------------
// Main loop
 ---------------------------------------------------------------------------- */
void loop() {

  // read and send keys
  customKey = customKeypad.getKey();

  // just in case a key is pressed
  // TODO: find a better routine to detect if the key is still pressed 
  if (customKey) {

    pressedKey = cck + (customKey - '0');

    // set RGB Led
    if (pressedKey == 91) { setColor(1); } 
    else if (pressedKey == 92) { setColor(2); } 
    else if (pressedKey == 93) { setColor(3); } 
    else if (pressedKey == 94) { setColor(4); } 
    else if (pressedKey == 95) { setColor(5); } 
    else if (pressedKey == 96) { setColor(6); }

    sendCC(controlChange, pressedKey, 127);
    sendCC(controlChange, pressedKey, 0);

  }

  // set the status string on the LCD
  lcd.setCursor(0, 1);

  statusString = getStatusString(a0, a1, a2, pressedKey);
  lcd.print(statusString);

  readAnalogInputs();

  // send the value only if different
  // TODO: find a better debounce for this
  if (last_a0 != a0) {
    setColor(7);
    sendCC(controlChange, cca0, a0);
    last_a0 = a0;
  }

  if (last_a1 != a1) {
    setColor(8);
    sendCC(controlChange, cca1, a1);
    last_a1 = a1;
  }

  if (last_a2 != a2) {
    setColor(9);
    sendCC(controlChange, cca2, a2);
    last_a2 = a2;
  }

  // LCD delay, otherwise looks weird
  delay(LCD_DELAY);

}


/* ----------------------------------------------------------------------------
// General functions
 ---------------------------------------------------------------------------- */
void readAnalogInputs(){
  // set to a 0 - 127 MIDI compatible range.
  // TODO: try to use the map function for this
  a0 = analogRead(A0) / 8;
  a1 = analogRead(A6) / 8;
  a2 = analogRead(A7) / 8;
  
}


/* ----------------------------------------------------------------------------
// LCD functions
 ---------------------------------------------------------------------------- */
String getStatusString(int a0, int a1, int a2, int pressedKey) {
  // Format the status string to the LCD
  if (a0 < 10) { statusString = "  " + String(a0); }
  else if (a0 < 100) { statusString = " " + String(a0); } 
  else { statusString = "" + String(a0); }

  if (a1 < 10) { statusString += "   " + String(a1); }
  else if (a1 < 100) { statusString += "  " + String(a1); }
  else { statusString += " " + String(a1); }

  if (a2 < 10) { statusString += "   " + String(a2); }
  else if (a2 < 100) { statusString += "  " + String(a2); } 
  else { statusString += " " + String(a2); }

  if (pressedKey > 0) { statusString += "  " + String(pressedKey); }
  else { statusString += "   " + String(pressedKey); }

  return statusString;
}


/* ----------------------------------------------------------------------------
// MIDI Functions
 ---------------------------------------------------------------------------- */
void sendCC(int statusByte, int dataByte1, int dataByte2) {
  Serial.write(statusByte);
  Serial.write(dataByte1);
  Serial.write(dataByte2);
}


void noteOn(int cmd, int pitch, int velocity) {
  Serial.write(cmd);
  Serial.write(pitch);
  Serial.write(velocity);
}


/* ----------------------------------------------------------------------------
// RGB LED functions
 ---------------------------------------------------------------------------- */
void setColor(int index) {
      r = colorArray[index][0];
      g = colorArray[index][1];
      b = colorArray[index][2];
      RGB_color(r, g, b);
}

void RGB_color(int red_light_value, int green_light_value, int blue_light_value)
 {
  analogWrite(red_pin, red_light_value);
  analogWrite(green_pin, green_light_value);
  analogWrite(blue_pin, blue_light_value);
}
