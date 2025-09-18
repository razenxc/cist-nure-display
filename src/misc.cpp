#include "misc.h"

const char* translitUA(String text) {
  text.replace("А", "A");  text.replace("а", "a");
  text.replace("Б", "B");  text.replace("б", "b");
  text.replace("В", "V");  text.replace("в", "v");
  text.replace("Г", "H");  text.replace("г", "h");
  text.replace("Ґ", "G");  text.replace("ґ", "g");
  text.replace("Д", "D");  text.replace("д", "d");
  text.replace("Е", "E");  text.replace("е", "e");
  text.replace("Є", "Ye"); text.replace("є", "ie");
  text.replace("Ж", "Zh"); text.replace("ж", "zh");
  text.replace("З", "Z");  text.replace("з", "z");
  text.replace("И", "Y");  text.replace("и", "y");
  text.replace("І", "I");  text.replace("і", "i");
  text.replace("Ї", "Yi"); text.replace("ї", "i");
  text.replace("Й", "Y");  text.replace("й", "i");
  text.replace("К", "K");  text.replace("к", "k");
  text.replace("Л", "L");  text.replace("л", "l");
  text.replace("М", "M");  text.replace("м", "m");
  text.replace("Н", "N");  text.replace("н", "n");
  text.replace("О", "O");  text.replace("о", "o");
  text.replace("П", "P");  text.replace("п", "p");
  text.replace("Р", "R");  text.replace("р", "r");
  text.replace("С", "S");  text.replace("с", "s");
  text.replace("Т", "T");  text.replace("т", "t");
  text.replace("У", "U");  text.replace("у", "u");
  text.replace("Ф", "F");  text.replace("ф", "f");
  text.replace("Х", "Kh"); text.replace("х", "kh");
  text.replace("Ц", "Ts"); text.replace("ц", "ts");
  text.replace("Ч", "Ch"); text.replace("ч", "ch");
  text.replace("Ш", "Sh"); text.replace("ш", "sh");
  text.replace("Щ", "Shch"); text.replace("щ", "shch");
  text.replace("Ю", "Yu"); text.replace("ю", "iu");
  text.replace("Я", "Ya"); text.replace("я", "ia");

  text.replace("Ь", "'");  text.replace("ь", "'");
  text.replace("’", "'");  text.replace("'", "");

  return text.c_str();
}

time_t getCurrentUnixTimestamp()
{
  struct tm timeinfo;
  while (!getLocalTime(&timeinfo)) {
    Serial.println("[TIME] Failed to obtain time! Retrying!");
    delay(1000);
    // return time(nullptr);
  }
  time_t now = mktime(&timeinfo);
  return now;
}