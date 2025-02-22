#include <SD.h>
// Make sure these two variables are correct for your setup
const int chipSelect = 4;

int scale = 3;               // 3 (±3g) for ADXL337, 200 (±200g) for ADXL377
boolean micro_is_5V = true;  // Set to true if using a 5V microcontroller such as the Arduino Uno, false if using a 3.3V microcontroller, this affects the interpretation of the sensor data
char* filename = "first.txt";
unsigned start_time_seconds = millis() / 1000;
int counter = 0;
File dataFile;


void setup() {
  
  // Initialize serial communication at 115200 baud
    Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect.
  }

  //Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  /*pinMode(SS, OUTPUT);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1) ;
  }

  Serial.println("card initialized.");

  // Open up the file we're going to log to!

   dataFile = SD.open(filename, FILE_WRITE);

  if (! dataFile) {
    Serial.println("error opening dataFile");
    // Wait forever since we cant write data
    while (1) ;
  }
  
  pinMode(7, OUTPUT);

  //Flash the an LED 5 times before beginning and then keep the LED on during measurement
  for (int i = 0; i < 5; i++) {
    digitalWrite(7, LOW);
    delay(500);
    digitalWrite(7, HIGH);
    delay(500);
  }*/
}

void loop() {

  if (millis() / 1000 - start_time_seconds < 60) {

    //***********3 g accelerometer

    // Get raw accelerometer data for each axis
    int rawX = analogRead(A0);
    int rawY = analogRead(A1);
    int rawZ = analogRead(A2);

    // Scale accelerometer ADC readings into common units
    float scaledX, scaledY, scaledZ;  // Scaled values for each axis

    scaledX = mapf(rawX, 0, 675, -3, 3);  // 3.3/5 * 1023 =~ 675
    scaledY = mapf(rawY, 0, 675, -3, 3);
    scaledZ = mapf(rawZ, 0, 675, -3, 3);

    //************200 g accelerometer

    // Get raw accelerometer data for each axis
    int rawX2 = analogRead(A3);
    int rawY2 = analogRead(A4);
    int rawZ2 = analogRead(A5);

    // Scale accelerometer ADC readings into common units
    // Scale map depends on if using a 5V or 3.3V microcontroller
    float scaledX2, scaledY2, scaledZ2;        // Scaled values for each axis
    scaledX2 = mapf(rawX, 0, 675, -200, 200);  // 3.3/5 * 1023 =~ 675
    scaledY2 = mapf(rawY, 0, 675, -200, 200);
    scaledZ2 = mapf(rawZ, 0, 675, -200, 200);


    // Print out scaled X,Y,Z accelerometer readings

    //printf("%f,%f,%f,%f,%f,%f\n", scaledX, scaledY, scaledZ, scaledX2, scaledY2, scaledZ2);
    //Serial.println("=============");
    Serial.print(scaledX);
   // Serial.print(scaledY);
   // Serial.print(scaledZ);//, scaledX2, scaledY2, scaledZ2);
    Serial.println();
    delay(10);  // Minimum delay of 2 milliseconds between sensor reads (500 Hz)

  } else {
    digitalWrite(7, LOW);
    //dataFile.close();
    while (1)
      ;
  }
}

// Same functionality as Arduino's standard map function, except using floats
float mapf(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
