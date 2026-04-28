#include <ArduinoBLE.h>

BLEService SphereService("19b10000-e8f2-537e-4f6c-d104768a1214");

// BLE Characteristics for different directions
BLEIntCharacteristic leftCharacteristic("19b10001-e8f2-537e-4f6c-d104768a1214", BLERead | BLEWrite | BLENotify);
BLEIntCharacteristic rightCharacteristic("19b10002-e8f2-537e-4f6c-d104768a1214", BLERead | BLEWrite | BLENotify);
BLEIntCharacteristic forwardCharacteristic("19b10003-e8f2-537e-4f6c-d104768a1214", BLERead | BLEWrite | BLENotify);
BLEIntCharacteristic backwardCharacteristic("19b10004-e8f2-537e-4f6c-d104768a1214", BLERead | BLEWrite | BLENotify);
BLEIntCharacteristic stopCharacteristic("19b10005-e8f2-537e-4f6c-d104768a1214", BLERead | BLEWrite | BLENotify);  // Stop characteristic

void setup() {
  // Start the serial monitor for debugging
  Serial.begin(9600);

  // Start BLE
  if (!BLE.begin()) {
    while (1);  // Wait for BLE to initialize
  }

  BLE.setLocalName("MechSphere");  // Advertise name
  BLE.setAdvertisedService(SphereService); // Set SphereService UUID
  SphereService.addCharacteristic(leftCharacteristic);  // Add left direction characteristic
  SphereService.addCharacteristic(rightCharacteristic); // Add right direction characteristic
  SphereService.addCharacteristic(forwardCharacteristic); // Add forward direction characteristic
  SphereService.addCharacteristic(backwardCharacteristic); // Add backward direction characteristic
  SphereService.addCharacteristic(stopCharacteristic); // Add stop characteristic
  BLE.addService(SphereService);  // Add service to BLE
  BLE.advertise();  // Start advertising

  // Print out a message indicating BLE initialization is complete
  Serial.println("BLE Initialized and Advertising...");
}

void loop() {
  BLEDevice central = BLE.central();  // Listen for BLE devices to connect

  if (central) {  // If a central device is connected
    Serial.println("Device connected");

    while (central.connected()) {  // While still connected

      // Check if a characteristic was written and perform corresponding actions
      if (leftCharacteristic.written()) {
        Serial.println("Left command performed");
        // Perform action for left command
      }
      if (rightCharacteristic.written()) {
        Serial.println("Right command performed");
        // Perform action for right command
      }
      if (forwardCharacteristic.written()) {
        Serial.println("Forward command performed");
        // Perform action for forward command
      }
      if (backwardCharacteristic.written()) {
        Serial.println("Backward command performed");
        // Perform action for backward command
      }
      if (stopCharacteristic.written()) {
        Serial.println("Stop command performed");
        // Perform action for stop command (e.g., stop motors)
      }
    }

    Serial.println("Device disconnected");
  }
}
