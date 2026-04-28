#include "esp_camera.h"
#include <WiFi.h>
#define CAMERA_MODEL_XIAO_ESP32S3 
#include "camera_pins.h"

#include <ArduinoBLE.h>

BLEService SphereService("19b10000-e8f2-537e-4f6c-d104768a1214");
BLEIntCharacteristic leftCharacteristic("19b10001-e8f2-537e-4f6c-d104768a1214", BLERead | BLEWrite | BLENotify);
BLEIntCharacteristic rightCharacteristic("19b10002-e8f2-537e-4f6c-d104768a1214", BLERead | BLEWrite | BLENotify);
BLEIntCharacteristic forwardCharacteristic("19b10003-e8f2-537e-4f6c-d104768a1214", BLERead | BLEWrite | BLENotify);
BLEIntCharacteristic backwardCharacteristic("19b10004-e8f2-537e-4f6c-d104768a1214", BLERead | BLEWrite | BLENotify);
BLEIntCharacteristic stopCharacteristic("19b10005-e8f2-537e-4f6c-d104768a1214", BLERead | BLEWrite | BLENotify);

const char *ssid = "Akshay";
const char *password = "aks142004";

void startCameraServer();
void setupLedFlash(int pin);

// http://192.168.45.109:81/stream   
// Akshays laptop ip for camera stream

void setup() 
{
  Serial.begin(9600);
  Serial.setDebugOutput(true);
  Serial.println();

  if (!BLE.begin()) 
  {
    while (1);  
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
  Serial.println("BLE Initialized and Advertising...");
  
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_UXGA;
  config.pixel_format = PIXFORMAT_JPEG;  // for streaming
  //config.pixel_format = PIXFORMAT_RGB565; // for face detection/recognition
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  if (config.pixel_format == PIXFORMAT_JPEG) 
  {
    if (psramFound()) 
    {
      config.jpeg_quality = 10;
      config.fb_count = 2;
      config.grab_mode = CAMERA_GRAB_LATEST;
    } 
    else 
    {
      // Limit the frame size when PSRAM is not available
      config.frame_size = FRAMESIZE_SVGA;
      config.fb_location = CAMERA_FB_IN_DRAM;
    }
  } 
  else 
  {
    // Best option for face detection/recognition
    config.frame_size = FRAMESIZE_240X240;
#if CONFIG_IDF_TARGET_ESP32S3
    config.fb_count = 2;
#endif
  }

#if defined(CAMERA_MODEL_ESP_EYE)
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
#endif

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) 
  {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t *s = esp_camera_sensor_get();
  
  if (s->id.PID == OV3660_PID) 
  {
    s->set_vflip(s, 1);        // flip it back
    s->set_brightness(s, 1);   // up the brightness just a bit
    s->set_saturation(s, -2);  // lower the saturation
  }
  // drop down frame size for higher initial frame rate
  if (config.pixel_format == PIXFORMAT_JPEG) 
  {
    s->set_framesize(s, FRAMESIZE_QVGA);
  }

#if defined(CAMERA_MODEL_M5STACK_WIDE) || defined(CAMERA_MODEL_M5STACK_ESP32CAM)
  s->set_vflip(s, 1);
  s->set_hmirror(s, 1);
#endif

#if defined(CAMERA_MODEL_ESP32S3_EYE)
  s->set_vflip(s, 1);
#endif

#if defined(LED_GPIO_NUM)
  setupLedFlash(LED_GPIO_NUM);
#endif

  WiFi.begin(ssid, password);
  WiFi.setSleep(false);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  startCameraServer();

  Serial.print("Camera Ready! Use 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' to connect");
}

void loop() 
{
  BLEDevice central = BLE.central(); 

  if (central) 
  {  
    Serial.println("Device connected");

    while (central.connected()) 
    {  
      if (leftCharacteristic.written()) 
      {
        Serial.println("Left command performed");
        // Perform action for left command
      }
      if (rightCharacteristic.written()) 
      {
        Serial.println("Right command performed");
        // Perform action for right command
      }
      if (forwardCharacteristic.written()) 
      {
        Serial.println("Forward command performed");
        // Perform action for forward command
      }
      if (backwardCharacteristic.written()) 
      {
        Serial.println("Backward command performed");
        // Perform action for backward command
      }
      if (stopCharacteristic.written()) 
      {
        Serial.println("Stop command performed");
        // Perform action for stop command (e.g., stop motors)
      }
    }

    Serial.println("Device disconnected");
  }
 }
