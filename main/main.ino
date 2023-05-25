#include <FastLED.h>
#include <FastBot.h>

#define WIFI_SSID "Redmi_7130"
#define WIFI_PASS "90105487"
#define BOT_TOKEN "6233933552:AAF1r7_tHBwXas8KqNzcoEps-xwGd5sfEEo"
#define owner_id "993324307"

#define LED_COUNT 60      
#define LED_DT 2

// constants

int max_bright = 100;
int leddelay = 20; 
int ledhue = 0;
int ledsat = 255;
int ledpos = 0;
int ledstep = 10;
double ledcntsnow = 0.0;
//int owner_id = 993324307;
struct CRGB leds[LED_COUNT];
FastBot bot(BOT_TOKEN);
int mem_snow[LED_COUNT];

String last_command = "";

void newMsg(FB_msg& msg) {
  // выводим ID чата, имя юзера и текст сообщения
  Serial.print(msg.chatID);     // ID чата 
  Serial.print(", ");
  Serial.print(msg.username);   // логин
  Serial.print(", ");
  Serial.println(msg.text);     // текст
//  Serial.print(", ");
//  Serial.println(msg.query);
  last_command = msg.text;
//  bot.sendMessage("OK", msg.chatID);
}

void setup()
{
  Serial.begin(115200);
  Serial.println();    
  connectWiFi();
  bot.attach(newMsg);
  LEDS.setBrightness(max_bright);

  LEDS.addLeds<WS2812, LED_DT, GRB>(leds, LED_COUNT);
  one_color_all(0, 0, 0);
  LEDS.show();
  randomSeed(analogRead(0));
}

bool cng[3] = {false, false, false};
int Ledmode = 1;
void change_mode(int newmode) {
  one_color_all(0, 0, 0); LEDS.show();
  switch (newmode) {
    case 2: leddelay = 40; break;
    case 3: leddelay = 20; ledpos = 0; ledstep = 10; break;
    case 4: leddelay = 50; ledstep = 500; for (int i = 0 ; i < LED_COUNT; i++) mem_snow[i] = 0; break;
    case 5: leddelay = 50; ledstep = 500; for (int i = 0 ; i < LED_COUNT; i++) mem_snow[i] = 0; break;
    
  }
}

int memLedmode = 1;

void action() {
  if (last_command == "") {
    return;
  }
  if (last_command == "/on") {
    Ledmode = memLedmode;
    change_mode(Ledmode);
    bot.sendMessage("Свет включен", owner_id);
  } else if (last_command == "/off") {
    Ledmode = 0;
    bot.sendMessage("Свет выключен", owner_id);
    last_command = "";
    return;
  } else if (last_command == "/info") {
    bot.sendMessage("1 - белый 2 - монотонная радуга 3 - бегущая радуга 4 - снег 5 - синий снег 100 - монотонный цвет", owner_id);
  } else if (last_command == "/mode") {
    cng[0] = true;
    cng[1] = false;
    cng[2] = false;
    bot.sendMessage("Введите режим", owner_id);
  } else if (last_command == "/color") {
    cng[0] = false;
    cng[1] = true;
    cng[2] = false;
    bot.sendMessage("Введите цвет", owner_id);
  } else if (last_command == "/brightness") {
    cng[0] = false;
    cng[1] = false;
    cng[2] = true;
    bot.sendMessage("Введите яркость", owner_id);
  } else {
    int val = 0;
    for (int i = 0 ; i < last_command.length(); i++) {
      if (last_command[i] >= '0' && last_command[i] <= '9') {
        val *= 10;
        val += (last_command[i] - '0');
      } else {
        last_command = "";
        return;
      }
    }
   if (cng[1]) {
      if (val > 100) {
        bot.sendMessage("Введите число не более 100", owner_id);
        last_command = "";
        return;
      }
      bot.sendMessage("Цвет изменён", owner_id);
      val *= 255;
      val /= 100;
      ledhue = val;
    } else if (cng[2]) {
      if (val > 100) {
        bot.sendMessage("Введите число не более 100", owner_id);
        last_command = "";
        return;
      }
      val *= 255;
      val /= 100;
      max_bright = val;
      LEDS.setBrightness(max_bright);
      LEDS.show();
      bot.sendMessage("Яркость изменена", owner_id);
    } else {
      bot.sendMessage("Режим изменён", owner_id);
      Ledmode = val;
      memLedmode = val;
      change_mode(val);
    }
    cng[0] = false;
    cng[1] = false;
    cng[2] = false;
  }
  last_command = "";
}

void loop() {
  bot.tick();
  action();
  switch(Ledmode) {
    case 0: one_color_all(0, 0, 0); LEDS.show(); break;
    case 1: one_color_all(255, 255, 255); LEDS.show(); break;
    case 2: rainbow_fade(); break;
    case 3: rainbow_loop(); break;
    case 4: snow(); break;
    case 5: snow_blue(); break;

    case 100: one_color_allHSV(ledhue); LEDS.show();
  }
  
}
