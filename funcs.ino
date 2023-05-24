void connectWiFi() {
  delay(1000);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (millis() > 15000) ESP.restart();
  }
  Serial.println("Connected");
}

void one_color_allHSV(int ahue) {
  for (int i = 0 ; i < LED_COUNT; i++ ) {
    leds[i] = CHSV(ahue, ledsat, 255);
  }
}

void one_color_all(int cred, int cgrn, int cblu) { 
  for (int i = 0 ; i < LED_COUNT; i++ ) {
    leds[i].r = cred;
    leds[i].g = cgrn;
    leds[i].b = cblu;
  }
}
