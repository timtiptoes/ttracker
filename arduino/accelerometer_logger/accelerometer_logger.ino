#include <SD.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

typedef struct {
    float *data;
    size_t size;
    size_t capacity;
} List;

// Same functionality as Arduino's standard map function, except using floats
float mapf(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// Function to initialize the list
List* create_list(size_t capacity) {
    List* list = (List*)malloc(sizeof(List));
    list->data = (float*)malloc(capacity * sizeof(float));
    list->size = 0;
    list->capacity = capacity;
    return list;
}

// Function to resize the list
void resize_list(List* list, size_t new_capacity) {
    list->data = (float*)realloc(list->data, new_capacity * sizeof(float));
    list->capacity = new_capacity;
}

// Function to append an element
void append(List* list, float value) {
    if (list->size == list->capacity) {
        resize_list(list, list->capacity * 2);
    }
    list->data[list->size++] = value;
}

// Function to insert an element at a given index
void insert(List* list, size_t index, float value) {
    if (index > list->size) return;
    if (list->size == list->capacity) {
        resize_list(list, list->capacity * 2);
    }
    for (size_t i = list->size; i > index; i--) {
        list->data[i] = list->data[i - 1];
    }
    list->data[index] = value;
    list->size++;
}

// Function to remove an element at a given index
void remove_at(List* list, size_t index) {
    if (index >= list->size) return;
    for (size_t i = index; i < list->size - 1; i++) {
        list->data[i] = list->data[i + 1];
    }
    list->size--;
}

// Function to get an element at a given index
float get(List* list, size_t index) {
    if (index >= list->size) exit(EXIT_FAILURE);
    return list->data[index];
}

// Function to set an element at a given index
void set(List* list, size_t index, float value) {
    if (index >= list->size) return;
    list->data[index] = value;
}

// Function to compute the mean value of the list
float mean(List* list) {
    if (list->size == 0) return 0.0;
    float sum = 0.0;
    for (size_t i = 0; i < list->size; i++) {
        sum += list->data[i];
    }
    return sum / list->size;
}

// Function to compute the standard deviation of the list
float standard_deviation(List* list) {
    if (list->size == 0) return 0.0;
    float m = mean(list);
    float sum_sq_diff = 0.0;
    for (size_t i = 0; i < list->size; i++) {
        sum_sq_diff += (list->data[i] - m) * (list->data[i] - m);
    }
    return sqrt(sum_sq_diff / list->size);
}

// Function to get the size of the list
size_t get_size(List* list) {
    return list->size;
}

// Function to print the list
void print_list(List* list) {
    for (size_t i = 0; i < list->size; i++) {
        Serial.print(list->data[i]);
        Serial.print(",");
    //printf("%.2f ", list->data[i]);
    }
    Serial.print("\n");
}

// Function to free the list memory
void free_list(List* list) {
    free(list->data);
    free(list);
}

char* convert_time(long long milliseconds) {
    long long seconds = milliseconds / 1000;
    int days = seconds / 86400;
    seconds %= 86400;
    int hours = seconds / 3600;
    seconds %= 3600;
    int minutes = seconds / 60;
    seconds %= 60;
    
    char* formatted_time = (char*)malloc(50 * sizeof(char));
    if (formatted_time != NULL) {
        snprintf(formatted_time, 50, "%d days %02d:%02d:%02d", days, hours, minutes, (int)seconds);
    }
    return formatted_time;
}

// Make sure these two variables are correct for your setup
const int chipSelect = 4;

int scale = 3;               // 3 (±3g) for ADXL337, 200 (±200g) for ADXL377
boolean micro_is_5V = true;  // Set to true if using a 5V microcontroller such as the Arduino Uno, false if using a 3.3V microcontroller, this affects the interpretation of the sensor data
char* filename = "hottub1.txt";
unsigned start_time_seconds = millis() / 1000;
int n = 10;

unsigned long previousTime = 0;
byte seconds ;
byte minutes ;
byte hours ;
 List* data = create_list(2);
int pump;
int previous_pump;
unsigned long StartTime = millis();
File datafile = SD.open(filename, FILE_WRITE);


void setup() {
  
   // Initialize serial communication at 115200 baud
    Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect.
  }

  //Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(SS, OUTPUT);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1) ;
  }

  Serial.println("card initialized.");

  // Open up the file we're going to log to!

 

  if (! datafile) {
    Serial.println("error opening dataFile");
    // Wait forever since we cant write data
    while (1) ;
  }
  


}

void loop() {

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

    append(data,scaledX);

  

    if (get_size(data)>n) {
          remove_at(data, 0);
    }

      float stdev=standard_deviation(data);

      if (stdev>0.05){
        pump=1;
      }
      else {pump=0;}


    datafile.println("testing 1, 2, 3.");

    if (previous_pump!=pump){
    unsigned long CurrentTime = millis();
    unsigned long ElapsedTime = CurrentTime - StartTime;
    char* result = convert_time(ElapsedTime);
        if (result != NULL) {
        datafile.print(result);
        Serial.print(result);
        free(result);
    }
      if (pump){
        datafile.println(",pump turned on");
        Serial.print(",pump turned on\n");
      }
      else {
        datafile.println(",pump turned off");
        Serial.print(",pump turned off\n");}
      previous_pump=pump;  
    }
    delay(100);  // Minimum delay of 2 milliseconds between sensor reads (500 Hz)

}


