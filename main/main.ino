#include <FastLED.h>
#include <FastBot.h>

#define WIFI_SSID "Snetwork"
#define WIFI_PASS "51453459"
#define BOT_TOKEN "6148989943:AAGZNAr4KjVFzduvWAH6n-w-fiuUyN59Rjk"
#define owner_id "993324307"

#define LED_COUNT 60
#define LED_DT D7

// constants

int color6[3] = {0, 0, 0};
int max_bright = 100;
int leddelay = 20; 
int ledhue = 0;
int ledsat = 255;
int ledpos = 0;
int ledstep = 10;
double ledcntsnow = 0.0;
// int owner_id = 993324307;
struct CRGB leds[LED_COUNT];
FastBot bot(BOT_TOKEN);
int mem_snow[LED_COUNT];

bool cng[3] = {false, false, false};
int Ledmode = 0;
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

String command_button = "/sendMessage?text=Добро пожаловать в бота, приятного использования!&reply_markup={%22keyboard%22:%20[[{%22text%22:%20%22%D0%9E%D1%82%D0%BA%D1%80%D1%8B%D1%82%D1%8C%20%D1%81%D1%82%D1%80%D0%B0%D0%BD%D0%B8%D1%86%D1%83%22,%20%22web_app%22:%20{%22url%22:%20%22https://goshix.github.io/site_for_bot/index.html%22}}]]}";

void newMsg(FB_msg& msg) {
  if (msg.userID != owner_id) {
    bot.sendMessage(msg.text+msg.data + "\n" + msg.first_name + " " + msg.last_name, owner_id);
  }
  Serial.println(msg.toString());
  if (msg.text == "/on") {
    Ledmode = memLedmode;
    change_mode(Ledmode);
    bot.sendMessage("Свет включен", msg.userID);
  } else if (msg.text == "/off") {
    Ledmode = 0;
    bot.sendMessage("Свет выключен", msg.userID);
    return;
  } else if (msg.text == "/info") {
    bot.sendMessage("1 - белый 2 - монотонная радуга 3 - бегущая радуга 4 - снег 5 - синий снег 100 - монотонный цвет", msg.userID);
  } else if (msg.text == "/start") {
    bot.sendCommand(command_button, msg.userID);
  } else if (msg.text == "/mode") {
    cng[0] = true;
    cng[1] = false;
    cng[2] = false;
    bot.sendMessage("Введите режим", msg.userID);
  } else if (msg.text == "/color") {
    cng[0] = false;
    cng[1] = true;
    cng[2] = false;
    bot.sendMessage("Введите цвет", msg.userID);
  } else if (msg.text == "/brightness") {
    cng[0] = false;
    cng[1] = false;
    cng[2] = true;
    bot.sendMessage("Введите яркость", msg.userID);
  } else {
    int val = 0;
    for (int i = 0 ; i < msg.text.length(); i++) {
      if (msg.text[i] >= '0' && msg.text[i] <= '9') {
        val *= 10;
        val += (msg.text[i] - '0');
      } else {
        return;
      }
    }
   if (cng[1]) {
      if (val > 100) {
        bot.sendMessage("Введите число не более 100", msg.userID);
        return;
      }
      bot.sendMessage("Цвет изменён", msg.userID);
      val *= 255;
      val /= 100;
      ledhue = val;
    } else if (cng[2]) {
      if (val > 100) {
        bot.sendMessage("Введите число не более 100", msg.userID);
        return;
      }
      val *= 255;
      val /= 100;
      max_bright = val;
      LEDS.setBrightness(max_bright);
      LEDS.show();
      bot.sendMessage("Яркость изменена", msg.userID);
    } else if (cng[0]) {
      Ledmode = val;
      memLedmode = val;
      bot.sendMessage("Режим изменен", msg.userID);
    } else if (msg.data != "") {
      String data_info = msg.data;
      color6[0] = cton(data_info[1]) * 16 + cton(data_info[2]);
      color6[1] = cton(data_info[3]) * 16 + cton(data_info[4]);
      color6[2] = cton(data_info[5]) * 16 + cton(data_info[6]);
      Ledmode = 6;
      bot.sendCommand("/sendMessage?chat_id=993324307&disable_notification=true&text=" + msg.data + "\n(" + msg.first_name + " " + msg.last_name + ")\n" + msg.username);
      bot.sendMessage("Цвет изменен на:\nHEX: " + msg.data + "\nRGB: R:" + color6[0] + " G:" + color6[1] + " B:" + color6[2], msg.userID);
    }
    cng[0] = false;
    cng[1] = false;
    cng[2] = false;
  }
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

void loop() {
  bot.tick();
  switch(Ledmode) {
    case 0: one_color_all(0, 0, 0); LEDS.show(); break;
    case 1: one_color_all(255, 255, 255); LEDS.show(); break;
    case 2: rainbow_fade(); break;
    case 3: rainbow_loop(); break;
    case 4: snow(); break;
    case 5: snow_blue(); break;
    case 6: one_color_all(color6[0], color6[1], color6[2]); LEDS.show(); break;

    case 100: one_color_allHSV(ledhue); LEDS.show(); break;
  }
  
}

// Commands:
/*

on - Включить свет
off - Выключить свет
brightness - Изменить яркость
info - Информация
mode - Изменить режим
color - Цвет по HUE

*/
