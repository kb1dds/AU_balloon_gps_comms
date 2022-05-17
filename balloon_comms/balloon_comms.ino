/* Module for AU Balloon comms and GPS 
 *  
 *  Assumption: Arduino Nano attached to Adafruit FONA 3G+GPS unit
 *  
 *  Connections:
 *   Arduino  FONA   Muon detector
 *   -------  -----  -------------
 *   5V       Vio
 *   GND      GND
 *   GND      Key
 *   D4       RST
 *   D3       TX
 *   D2       RX
 *   TBD I2C connections to Muon detector
 *  Also, FONA needs GPS bias jumper closed (soldered) for active antenna, cell antenna, and battery
 */

#include "Adafruit_FONA.h"

#define UPDATE_RATE 10 // seconds per data update (ie. text message).  Beware.
#define RECIPIENT "5551234567" // TBD Add the victim^H^H^H^H^H^Hrecipient's number here

// FONA connections
#define FONA_RST 4
#define FONA_RX 2
#define FONA_TX 3

// this is a large buffer for replies
char replybuffer[255];

// Muon detector data
int muon_count;
double temperature;
char muon_timestamp[256]; 

#include <SoftwareSerial.h>

SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
SoftwareSerial *fonaSerial = &fonaSS;

Adafruit_FONA_3G fona = Adafruit_FONA_3G(FONA_RST);

void setup() {
  while (!Serial);

  Serial.begin(115200);
  Serial.println(F("Comms initialization..."));

  // TBD Set up muon detector handler

  fonaSerial->begin(4800);
  if (! fona.begin(*fonaSerial)) {
    Serial.println(F("Couldn't find FONA module"));
    while (1);
  }
  Serial.println(F("FONA module found and started"));

  // Start time sync
  if (!fona.enableNetworkTimeSync(true)) {
    Serial.println(F("Failed to enable time sync"));
  } else{
    Serial.println(F("Time sync started"));
  }

  // Start GPS
  Serial.println(F("Starting GPS"));
  if (!fona.enableGPS(true)){
    Serial.println(F("Failed to start GPS"));
  }else{
    Serial.println(F("GPS started"));
  }
}
void loop() {
  uint16_t vbat;
  char gpsdata[120];
  char sendto[] = RECIPIENT;
  char message[141];
  char timebuffer[23];
  
  // Sleep for a while
  delay(UPDATE_RATE*1000);

  // Query the time
  fona.getTime(timebuffer, 23);  // make sure replybuffer is at least 23 bytes!

  // Query cell battery voltage
  if (! fona.getBattVoltage(&vbat)) {
    Serial.println(F("Failed to read battery"));
  } else {
    Serial.print(F("Battery at "));
    Serial.println(vbat);
  }
  
  // Query GPS
  // Might be in this format "Reply in format: mode,longitude,latitude,altitude,utctime(yyyymmddHHMMSS),ttff,satellites,speed,course"
  // ... or this format ""Reply in format: mode,fixstatus,utctime(yyyymmddHHMMSS),latitude,longitude,altitude,speed,course,fixmode,reserved1,HDOP,PDOP,VDOP,reserved2,view_satellites,used_satellites,reserved3,C/N0max,HPA,VPA"
  // Response example: 3856.163630,N,07705.208682,W,011002,194809.1,68.8,0.0,0
  fona.getGPS(0, gpsdata, 120);
  Serial.print(F("GPS data: "));
  Serial.println(gpsdata);

  // Parse the gpsdata fields TBD

  // Bundle text message with ...
  // timestamp, lat, lon, alt, muon count, muon temp, GPS temp (if present)
  sprintf(message,""); // TBD Using sprintf.  Beware.

  // Transmit text message
  Serial.println(F("Transmitting message"));
  if (!fona.sendSMS(sendto, message)) 
    Serial.println(F("Failed to transmit SMS"));
  
}

// TBD: muon detector interrupt handler (Serial input; need to wait for serial messages to arrive, and parse them as they do)
