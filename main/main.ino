// library digunakan
#include <SPI.h>
#include <Ethernet.h>
#include <HttpClient.h>
#include <EthernetClient.h>

// Mode Develop
#define DEV_MODE // Komentari baris ini untuk saat menjalankan device tanpa terkoneksi dengan port usb
#ifdef DEV_MODE
#define SERIAL_PRINT(value) Serial.print(value)
#define SERIAL_PRINTLN(value) Serial.println(value)
#else
#define SERIAL_PRINT(value)
#define SERIAL_PRINTLN(value)
#endif

// Name of the server we want to connect to
const char kHostname[] = "127.0.0.1"; // Remove the port from hostname 
uint16_t Kport = 5600; // set port here
// Path to download (this is the bit after the hostname in the URL
// that you want to download
const char kPath[] = "/sensor-detected";

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 43, 159); //satatic IP microcontroller

#define SENSOR 5 // sensor read pin
byte repeat = 0; // variable for repeat condition

void setup() {
  Ethernet.init(10); // configure the CS pin

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial)

  SERIAL_PRINTLN("Ethernet WebServer Example");

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    SERIAL_PRINTLN("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    SERIAL_PRINTLN("Ethernet cable is not connected.");
  }

  SERIAL_PRINT("server is at ");
  SERIAL_PRINTLN(Ethernet.localIP()); // print IP
  SERIAL_PRINTLN("Device ready to use :");

  pinMode(SENSOR, INPUT_PULLUP); // set pin to use internal pull up resistor
}


void loop() {
  int err = 0; // variable to detect error network
  int sensorValue = digitalRead(SENSOR); // read sensor

  EthernetClient c;
  HttpClient http(c);
  
  /* Run With Sensor */
  if(sensorValue == 0 && repeat == 0) {
    err = http.post(kHostname, Kport, kPath); // send request
    SERIAL_PRINT(err);
    SERIAL_PRINTLN(" => Vehicle detected");
    repeat = 1;
  }else if (sensorValue == 1 && repeat == 1) {
    SERIAL_PRINTLN(" No Vehicle");
    repeat = 0;
  }else{}

  /*------------------------------*/
  /* test without sensor */
  // err = http.post(kHostname, Kport, kPath);
  // delay(1000);
  // err = http.responseStatusCode();
  // SERIAL_PRINTLN(err);
  /*----------------------------*/
  
  http.stop(); // reset connection
  delay(500);
}
