#include <SD.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 68, 175);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);
char v[15];

String output_string; 

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

char* get_pump_status(){
  return "\"off\"";
}  
/*   List* data = create_list(2);
   
  for (int i = 0; i < 10; i++) {
      //***********3 g accelerometer
      int rawX = analogRead(A0);
      float scaledX = mapf(rawX, 0, 675, -3, 3);  // 3.3/5 * 1023 =~ 675
      append(data,scaledX);
      delay(100);
  }

    float stdev=standard_deviation(data);
    if (stdev>0.05){return "\"on\"";}
    else {return "\"off\"";}
}*/

void setup() {

    analogReference(EXTERNAL);
   // Initialize serial communication at 115200 baud
    Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect.
  }
  Serial.println("Serial port initialized");

    // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();

          Serial.println("About to fetch pump status");

          char* pump_status = get_pump_status();

          Serial.println(pump_status);
          
          output_string="";
          output_string.concat("{\"pump_status\":");
          output_string.concat(pump_status);
          output_string.concat("}");
          client.print(output_string);
          //client.println("<br />");
          //client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(2);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}


