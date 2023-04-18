
#include <SPI.h>
#include <RH_RF95.h>


// ------------- RADIO SETUP ------------------ //

#define RADIO_FREQ    915.0
#define RADIO_RST     14
#define RADIO_CS      10
#define RADIO_INT     digitalPinToInterrupt(2) // Have to set this, but not using it

// Create driver
RH_RF95 driver(RADIO_CS, RADIO_INT);

void setup() 
{

  // Initialize serial communication for logging
  Serial.begin(115200);

  /* DEBUG

  // Don't proceed until we've connected the serial monitor, so we can
  // see logs from the initialization process
  while (!Serial) { 
    delay(1); 
  } 

  */

  // (From docs) Ensure serial flash is not interfering with radio communication on SPI bus
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);

  // Configure the radio reset pin as an output and set low initially
  pinMode(RADIO_RST, OUTPUT);
  digitalWrite(RADIO_RST, 1);

  // Toggle the reset pin high, then low again to reset the device
  digitalWrite(RADIO_RST, 0);
  delay(10);
  digitalWrite(RADIO_RST, 1);
  delay(10);

  // --- RADIO CONFIG --- //

  // Initialize the radio
  if (!driver.init()) {
    Serial.println("Failed to initialize radio");
    while (1);
  }
  Serial.println("Radio initialized successfully");

  // Set the radio frequency to 915 MHz
  if (!driver.setFrequency(RADIO_FREQ)) {
    Serial.println("Failed to set the radio frequency");
  }

  // Set the radio power level (range 5-23 dBm)
  driver.setTxPower(23, false);
 
}


void loop()
{

  // Send a message at a 1 second interval
  Serial.println("Transmitting message");

  char messageBytes[] = "Hello from Jake!";
  if (driver.send((uint8_t *)messageBytes, strlen(messageBytes))) {
    Serial.println("transmission successful");
  } else {
    Serial.println("transmission failed");
  }

  // Sleep for 1 second
  delay(1000);

}
