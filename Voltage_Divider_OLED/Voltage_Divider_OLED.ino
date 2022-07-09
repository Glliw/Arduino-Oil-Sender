// This sketch finds the resistance of an unknown resistor based on the principles of a voltage divider circuit.
// The intent is to display oil pressure using a 35-244 ohm, 0-100 psi 2-terminal sender.
// Vout = Vin * R2 / (R1 + R2 )
// R2 = Vout * R1 / (Vin – Vout)
// After completing the calculation, it displays to a 128x64 OLED display.
// 06/27/22 - Code works but is a matter of whether the output is accurate.  Need to test with a pressure source.
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int analogPin = 0;
int a2dpin_input = 0;
int Vin = 5;  //voltage at 5v pin of arduino
float Vout = 0; //voltage at A0 pin of arduino
float R1 = 98;
float R2 = 0;
float buffer = 0;
float OilPres = 0;

void setup() {
  Serial.begin(9600);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
}

// You MUST call display() after drawing commands to make them visible on screen!
  display.display();
  delay(250); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
  display.display();
}
void loop() {
  a2dpin_input = analogRead(analogPin); // stores analog input as a digital value
  if(a2dpin_input){
    buffer = a2dpin_input * Vin;
    Vout = (buffer)/1024.0; //The Arduino Uno ADC is of 10-bit resolution (so the integer values from 0 - 2^10 = 1024 values). This means that it will map input voltages between 0 and 5 volts into integer values between 0 and 1023. So if we multiply input anlogValue to (buffer/1024), then we get the digital value of input voltage.
    //buffer =  Vout/(Vin-Vout); // original code had incorretly: (Vin/Vout) - 1;
    buffer = (Vin/Vout) - 1;
    R2 = R1 * buffer;
    OilPres = (0.0015 * (R2 * R2)) - (0.8815 * R2) + 127.14;
  
//    Serial.print("a2dpin_input: ");
//    Serial.println(a2dpin_input);
//    Serial.print("buffer: ");
//    Serial.println(buffer);
//    Serial.print("Vout: ");
//    Serial.println(Vout);
//    Serial.print("R2: ");
//    Serial.println(R2);
//    Serial.print("Oil Pressure: ");
//    Serial.println(OilPres,1);
    
    delay(50);
    
  }
      
  display.clearDisplay();  // Clear display buffer
  display.setTextSize(2); //Normal 1:1 pixel scale; 2 = 2pixel height
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0,0); // start at top left corner
  display.cp437(true);  // use full 256 char 'code page 437' font
  display.write("Oil Pres: ");
  display.setCursor(0,40); // start at top left corner
  display.println(OilPres,1);
  display.setTextSize(2);
  display.setCursor(55,40); // start at top left corner
  display.print("psi");
  display.display();

}
