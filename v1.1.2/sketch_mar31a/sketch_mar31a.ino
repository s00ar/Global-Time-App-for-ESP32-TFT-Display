#include <WiFi.h>
#include <TFT_eSPI.h>
#include <time.h>

TFT_eSPI tft = TFT_eSPI();

const char* ssid     = "";
const char* password = "";

int simulated_offset = 0;

void setup() {
  pinMode(32, OUTPUT);
  digitalWrite(32, HIGH);

  Serial.begin(115200);
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  configTime(0, 0, "pool.ntp.org");
  while (time(nullptr) < 100000) {
    delay(100);
  }

  pinMode(0, INPUT_PULLUP);
}

String formatHora(int h, int m) {
  char buf[6];
  sprintf(buf, "%02d:%02d", h % 24, m);
  return String(buf);
}

void mostrarColumna(String nombre, int hora_base, int min, int x) {
  tft.setTextDatum(TC_DATUM);

  // Nombre del país
  tft.drawString(nombre, x, 10, 2);

  // Hora +1 (arriba)
  tft.setTextSize(2);
  tft.drawString(formatHora(hora_base + 1, min), x, 55);

  // Hora actual (grande)
  tft.setTextSize(3);
  tft.drawString(formatHora(hora_base, min), x, 85);

  // Hora -1 (abajo)
  tft.setTextSize(2);
  tft.drawString(formatHora(hora_base - 1 + 24, min), x, 135);
}

void loop() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) return;

  int utc_h = (timeinfo.tm_hour + simulated_offset) % 24;
  int min = timeinfo.tm_min;

  // Horarios locales
  int arg_h = (utc_h - 3 + 24) % 24;
  int ita_h = (utc_h + 2) % 24;   // Italia en horario de verano
  int ecu_h = (utc_h - 5 + 24) % 24;

  tft.fillScreen(TFT_BLACK);

  // Mostrar columnas
  mostrarColumna("Arg", arg_h, min, 53);
  mostrarColumna("Ita", ita_h, min, 160);
  mostrarColumna("Ecu", ecu_h, min, 267);

  // Separadores entre columnas
  tft.drawLine(107, 0, 107, 135, TFT_WHITE);
  tft.drawLine(213, 0, 213, 135, TFT_WHITE);

  // Esperar botón
  int contador = 0;
  while (contador < 100) {
    if (digitalRead(0) == LOW) {
      simulated_offset = (simulated_offset + 1) % 24;
      delay(500);
      break;
    }
    delay(50);
    contador++;
  }
}
