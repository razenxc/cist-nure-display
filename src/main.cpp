#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <vector>

#include "models.h"
#include "misc.h"

#include <sstream>

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include "fonts/NotoSans_Condensed_Bold20pt7b.h"

#define TFT_CS 5
#define TFT_DC 21
#define TFT_RST 16

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

const char* ntpServer = "pool.ntp.org";
// const char* ntpBackupServer = "0.ua.pool.ntp.org";
const long gmtOffset_sec = 2 * 3600;  // adjust for your timezone
const int daylightOffset_sec = 3600;

// Constants
const char* wifi_ssid = "";
const char* wifi_password = "";

void setup_wifi()
{
  WiFi.begin(wifi_ssid, wifi_password);
  Serial.print("[WiFi] Connecting...");
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print('.');
  }
  Serial.println("");
  Serial.print("[WiFi] Connected: ");
  Serial.println(WiFi.localIP());
}

std::vector<Lesson> getLessons()
{
  HTTPClient http;
  std::vector<Lesson> lessons;

  const char* groupName = "";
  time_t now = getCurrentUnixTimestamp();
  struct tm* modif = localtime(&now);
  modif->tm_hour=0;
  modif->tm_min=0;
  modif->tm_mday+=1;

  time_t twelveOClock = mktime(modif);

  time_t startTime = now;
  time_t endTime = twelveOClock;

  std::stringstream ss;
  ss << "https://nure-time.runasp.net/api/v2/Lessons/GetByName?name=" << groupName <<
  "&startTime=" << startTime <<
  "&endTime=" << endTime;

  std::string serverPath = ss.str();

  Serial.printf("[Get lessons] %s \n", serverPath.c_str());

  http.begin(serverPath.c_str());
  int httpResponseCode = http.GET();
      
  if (httpResponseCode>0) {
    JsonDocument doc;
    deserializeJson(doc, http.getString());
    
    for(size_t i = 0; i < doc.size(); i++)
    {
      Lesson lesson(doc[i]["title"], doc[i]["brief"], doc[i]["startTime"], doc[i]["endTime"]);
      lessons.push_back(lesson);
    }
    // [BUG] somehow request body appears in doc[i]["title"] and "brief"
  }
  else {
    Serial.print("[Get Lessons] Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return lessons;
}

enum status 
{
  negative,
  positive
};

enum status countTillNearestLesson()
{
  time_t now = getCurrentUnixTimestamp();
  std::vector<Lesson> lessons = getLessons();
  size_t nearestIndex = lessons.size();

  for (size_t i = 0; i < lessons.size(); i++)
  {
    if (now < lessons[i].startTime)
    {
      nearestIndex = i;
      break;
    }
  }

  if (nearestIndex == lessons.size()) {
    tft.setCursor(0,30);
    tft.setTextColor(ST7735_GREEN);
    tft.setTextWrap(1);
    tft.println("No upcoming lessons");
    
    Serial.println("[Counter] No upcoming lessons");
    return status::negative;
  }

  time_t nearestClassStartTime = lessons[nearestIndex].startTime;
  time_t nearestClassEndTime = lessons[nearestIndex].endTime;
  const char* placeholder = "in";

  while (now < nearestClassStartTime)
  {
    now = getCurrentUnixTimestamp();

    time_t diff;

    if (nearestClassStartTime >= now <= nearestClassEndTime)
    {
      diff = nearestClassStartTime - now;
    }

    // if (nearestClassStartTime <= now <= nearestClassEndTime) 
    // {
    //   diff = nearestClassEndTime - now;
    //   placeholder = "ends in";
    // }

    if (now >= nearestClassEndTime)
    {
      break;
    }

    int hours = diff / 3600;
    int minutes = (diff % 3600) / 60;
    int seconds = diff % 60;

    tft.setCursor(0,30);
    tft.setTextColor(ST7735_WHITE);
    tft.setTextWrap(1);
    tft.printf("%s %s", translitUA(lessons[nearestIndex].brief), placeholder);
    tft.setCursor(10, 100);
    tft.printf("%02d:%02d:%02d", hours, minutes, seconds);

    Serial.printf("[Counter] %s %s",translitUA( lessons[nearestIndex].brief), placeholder);
    Serial.printf(" %02d:%02d:%02d \n", hours, minutes, seconds);

    delay(1000);
    tft.fillScreen(ST7735_BLACK);
  }

  return status::positive;
}

// ===============================
// LOOP & SETUP
// ===============================

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("");
  setup_wifi();

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  tft.initR(INITR_BLACKTAB); 
  tft.fillScreen(ST7735_BLACK);
  tft.setRotation(3);
  tft.setFont(&NotoSans_Condensed_Bold20pt7b);
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("[WiFi] Lost connections. Reconnecting:");
  }

  status status = countTillNearestLesson(); // if no upcoming lessons return <somthing> and check it with if and set delay X ms
  if (status == status::negative)
  {
    long pause = 1800000;
    Serial.printf("[Loop] %zu minutes delay...", pause / 60000);
    delay(pause);
  }
}