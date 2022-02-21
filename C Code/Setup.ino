void setup() {

  //camera setup
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();
  pinMode(PIN_LED, OUTPUT); 

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
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  
  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.
  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

#if defined(CAMERA_MODEL_ESP_EYE)
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
#endif

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t * s = esp_camera_sensor_get();
  // initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1); // flip it back
    s->set_brightness(s, 1); // up the brightness just a bit
    s->set_saturation(s, -2); // lower the saturation
  }
  // drop down frame size for higher initial frame rate
  s->set_framesize(s, FRAMESIZE_QVGA);

#if defined(CAMERA_MODEL_M5STACK_WIDE) || defined(CAMERA_MODEL_M5STACK_ESP32CAM)
  s->set_vflip(s, 1);
  s->set_hmirror(s, 1);
#endif

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  startCameraServer();

  Serial.print("Camera Ready! Use 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' to connect");


// motor setup
pinMode(Left_Mot_Forw,OUTPUT);
pinMode(Left_Mot_Backw,OUTPUT);
pinMode(Right_Mot_Forw,OUTPUT);
pinMode(Right_Mot_Backw,OUTPUT);
digitalWrite(Left_Mot_Forw,LOW);
digitalWrite(Left_Mot_Backw,LOW);
digitalWrite(Right_Mot_Forw,LOW);
digitalWrite(Right_Mot_Backw,LOW);
Serial.begin(115200);
setup_wifi();
client.setServer(mqtt_server, 1883);
client.setCallback(callback1);

// DUST sensor

Serial2.begin(9600, SERIAL_8N1, 2, -1); //setting the device with UART
Serial.begin(115200);

//ENVIRONMENT sensor

  Serial.begin(115200);
  Serial.println("sgp40 is starting, the reading can be taken after 10 seconds...");
  wire2.begin(33, 32);
  /*
   * Sensor preheat time: 10s
   * duration: init wait time. Unit: ms. It is suggested: duration>=10000ms
   */
  while(mySgp40.begin(/*duration = */500) !=true){
    Serial.println("failed to init chip, please check if the chip connection is fine");
    delay(500);
  }
  Serial.println("----------------------------------------------");
  Serial.println("sgp40 initialized successfully!");
  Serial.println("----------------------------------------------");
  /* 
   * Set the relative humidity and temperature of current environment 
   * The sensor has internal temerpature & humidity calibration. For more accurate VOC index, please open the function setRhT().
   * relativeHumidity：ambient relative humidity, refer to the moisture content in air. Range：0-90, unit: %RH，e.g. 50%
   * temperatureC：ambient temperature. Range: -10~50, unit: °C, e.g. 20°C
   */
  //mySgp40.setRhT(/*relativeHumidity = */ 50, /*temperatureC = */ 20);
  
}

void loop(){
  motor_system();
  camera();
  dust_sensor();
  environment_sensor();
}
