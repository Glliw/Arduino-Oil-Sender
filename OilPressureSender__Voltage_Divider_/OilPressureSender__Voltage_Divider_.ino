// This sketch finds the resistance of an unknown resistor based on the principles of a voltage divider circuit.
// The intent is to display oil pressure using a 35-244 ohm, 0-100 psi 2-terminal sender.
// Vout = Vin * R2 / (R1 + R2 )
// R2 = Vout * R1 / (Vin – Vout)


int analogPin = 0;
int a2dpin_input = 0;
int Vin = 5;  //voltage at 5v pin of arduino
float Vout = 0; //voltage at A0 pin of arduino
float R1 = 98;
float R2 = 0;
float buffer = 0;
float OilPres = 0;


void setup(){
Serial.begin(9600);
}

void loop(){
  a2dpin_input = analogRead(analogPin); // stores analog input as a digital value
  if(a2dpin_input){
    buffer = a2dpin_input * Vin;
    Vout = (buffer)/1024.0; //The Arduino Uno ADC is of 10-bit resolution (so the integer values from 0 - 2^10 = 1024 values). This means that it will map input voltages between 0 and 5 volts into integer values between 0 and 1023. So if we multiply input anlogValue to (buffer/1024), then we get the digital value of input voltage.
    //buffer =  Vout/(Vin-Vout); // original code had incorretly: (Vin/Vout) - 1;
    buffer = (Vin/Vout) - 1;
    R2 = R1 * buffer;
    OilPres = (0.0015 * (R2 * R2)) - (0.8815 * R2) + 127.14;


    
    Serial.print("a2dpin_input: ");
    Serial.println(a2dpin_input);
    Serial.print("buffer: ");
    Serial.println(buffer);
    Serial.print("Vout: ");
    Serial.println(Vout);
    Serial.print("R2: ");
    Serial.println(R2);
    Serial.print("Oil Pressure: ");
    Serial.println(OilPres);
    
    delay(1000);

    
  }
}
