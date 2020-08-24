/*
 * Arduino tonestack controller
 * Controlls ToneStack app (iOS) via MIDI messages
 * Might prove useful for live presentations.
 *
 * Version 0.1 February, 2020
 * 2019 Javier Mejias
 * https://worldprogproject.com
 */
 

/* ----------------------------------------------------------------------------
// General constants
 ---------------------------------------------------------------------------- */
bool DEBUG;

// Feel free to re-arrange any pin to your taste

// physical analog inputs
int ANALOG_IN_0 = A7;
int ANALOG_IN_1 = A6;
int ANALOG_IN_2 = A5;

// variables for values
int a0 = 0;
int a1 = 0;
int a2 = 0;

// buffer for comparison.
int last_a0 = 0;
int last_a1 = 0;
int last_a2 = 0;


// physical digital I/O
// keyboard colums
int KPC1_IN = 13;
int KPC2_IN = 14;
int KPC3_IN = 15;
int KPC4_IN = 16;

// keyboard rows
int KPR1_IN = 17;
int KPR2_IN = 18;


/* ----------------------------------------------------------------------------
// Keypad constants
 -----------------------------------------------------------------------------*/
#include <Keypad.h>

// Keyboard configuration
const byte ROWS = 2; 
const byte COLS = 4; 

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', '4'},
  {'5', '6', '7', '8'},
};

byte colPins[COLS] = {KPC1_IN, KPC2_IN, KPC3_IN, KPC4_IN}; 
byte rowPins[ROWS] = {KPR1_IN, KPR2_IN}; 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
char customKey;
char last_key = ' ';


/* ----------------------------------------------------------------------------
// LCD constants
 ---------------------------------------------------------------------------- */
#include <LiquidCrystal.h>

//LiquidCrystal(rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

// You could use other kind of LCD screens.
// Just make sure it has at least 16 cols and at least 2 rows :).
#define LCD_COLS 16
#define LCD_ROWS 2
#define LCD_DELAY 120.0..........................................................................................................................................................................................


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
int cck = 0;

int pressedKey = 0;


/* ----------------------------------------------------------------------------
// Setup
 ---------------------------------------------------------------------------- */
void setup() {

  DEBUG = false;
  //DEBUG = true;

  if (DEBUG == false) {
    // enable midi communicaiton
    Serial.begin(31250);
  } else {
    // enable debug mode
    Serial.begin(9600);
  }
 
  // start LCD
  lcd.begin(LCD_COLS, LCD_ROWS);
  lcd.clear();
  lcd.setCursor(0, 0);

  // initialize the LCD
  lcd.setCursor(0, 0);
  String headerString = " A0  A1  A2   K";
  lcd.print(headerString);
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

    sendKey(pressedKey);
  }

  readAnalogInputs();
  // readDistanceSensor();


  // set the status string on the LCD
  lcd.setCursor(0, 1);

  statusString = getStatusString();
  lcd.print(statusString);

  // send the value only if different
  // TODO: find a better debounce for this
  if (last_a0 != a0) {
    sendCC(controlChange, cca0, a0);
    last_a0 = a0;
  }

  if (last_a1 != a1) {
    sendCC(controlChange, cca1, a1);
    last_a1 = a1;
  }

  if (last_a2 != a2) {
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
  a0 = 127 - analogRead(ANALOG_IN_0) / 8;
  a1 = analogRead(ANALOG_IN_1) / 8;
  a2 = analogRead(ANALOG_IN_2) / 8;
  
}


/* ----------------------------------------------------------------------------
// LCD functions
 ---------------------------------------------------------------------------- */
String getStatusString() {
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

  
  statusString += "   " + String(pressedKey);
  
  return statusString;
}


/* ----------------------------------------------------------------------------
// MIDI Functions
 ---------------------------------------------------------------------------- */
void sendCC(int statusByte, int dataByte1, int dataByte2) {

  if (DEBUG == false) {
    Serial.write(statusByte);
    Serial.write(dataByte1);
    Serial.write(dataByte2);
  } else {
    Serial.print("sendCC: ");
    Serial.print(statusByte);
    Serial.print(" ");
    Serial.print(dataByte1);
    Serial.print(" ");
    Serial.println(dataByte2);
   }
}

void noteOn(int cmd, int pitch, int velocity) {
  if (DEBUG == false) {
    Serial.write(cmd);
    Serial.write(pitch);
    Serial.write(velocity);
  } else {
    Serial.print("noteOn: ");
    Serial.print(cmd);
    Serial.print(" ");
    Serial.print(pitch);
    Serial.print(" ");
    Serial.println(velocity); 
  }
}

void sendKey(int pressedKey) {
  // TODO: find a better calibrate sensor key and mapping
  if (DEBUG == false) {
    sendCC(controlChange, pressedKey, 127);
    sendCC(controlChange, pressedKey, 0);
   } else {

    Serial.print("keyOn: ");
    Serial.println(pressedKey); 
  }
}
