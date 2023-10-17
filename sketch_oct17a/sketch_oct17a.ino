#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLEAdvertising.h>

bool advertising = true;  // Initial state is advertising

class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    Serial.println("Device connected");
    advertising = false; // Set to false when connected
  }

  void onDisconnect(BLEServer* pServer) {
    Serial.println("Device disconnected");
    advertising = true;  // Set to true when disconnected
  }
};

// Unique name and service UUID
#define DEVICE_NAME "MobileESP32"
#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"

BLEServer* pServer;
BLEAdvertising* pAdvertising;

void setup() {
  Serial.begin(115200);
  BLEDevice::init(DEVICE_NAME);

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Start advertising
  pAdvertising = pServer->getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // 6ms
  pAdvertising->setMinPreferred(0x12);  // 12ms

  pService->start();
  pAdvertising->start();
  Serial.println("Waiting for connections...");
}

void loop() {
  // Check if the device is still advertising (not connected)
  if (advertising) {
    Serial.println("Advertising...");
    pAdvertising->start(); // Re-advertise if not connected
  } else {
    Serial.println("Connected...");
  }

  // You can perform other tasks here
}
