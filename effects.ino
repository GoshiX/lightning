void rainbow_fade() {
  ledhue++;
  if (ledhue > 255) {
    ledhue = 0;
  }
  one_color_allHSV(ledhue);
  LEDS.show();
  delay(leddelay);
}

void rainbow_loop() {
  ledpos++;
  ledhue = ledhue + ledstep;
  if (ledpos >= LED_COUNT) {
    ledpos = 0;
  }
  if (ledhue > 255) {
    ledhue = 0;
  }
  leds[ledpos] = CHSV(ledhue, ledsat, 255);
  LEDS.show();
  delay(leddelay); 
}

void snow() {
  ledcntsnow += (double) LED_COUNT / 400;
  Serial.println(ledcntsnow);
  for (int i = 0 ; i < LED_COUNT; i++) {
    mem_snow[i]--;
    mem_snow[i] = max(mem_snow[i], 0);
  }  
  while (ledcntsnow >= 1) {
    ledcntsnow -= 1;
    mem_snow[random(0, LED_COUNT)] = 70;
  }
  for (int i = 0 ; i < LED_COUNT; i++) {
    leds[i].r = 255 * mem_snow[i] / 70;
    leds[i].g = 255 * mem_snow[i] / 70;
    leds[i].b = 255 * mem_snow[i] / 70;
  }
  LEDS.show();
  delay(leddelay);
}

void snow_blue() {
  ledcntsnow += (double) LED_COUNT / 400;
//  Serial.println(ledcntsnow);
  for (int i = 0 ; i < LED_COUNT; i++) {
    mem_snow[i]--;
    mem_snow[i] = max(mem_snow[i], 0);
  }
  while (ledcntsnow >= 1) {
    ledcntsnow -= 1;
    mem_snow[random(0, LED_COUNT)] = 70;
  }
  for (int i = 0 ; i < LED_COUNT; i++) {
    leds[i].r = 255 * mem_snow[i] / 70;
    leds[i].g = 255 * mem_snow[i] / 70;
    leds[i].b = min(255, 10 + 255 * mem_snow[i] / 70);
  }
  LEDS.show();
  delay(leddelay);
}
