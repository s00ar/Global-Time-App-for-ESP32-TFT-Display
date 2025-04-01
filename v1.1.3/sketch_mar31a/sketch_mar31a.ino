#include <WiFi.h>
#include <TFT_eSPI.h>
#include <time.h>

TFT_eSPI tft = TFT_eSPI();

const char* ssid     = "YoNoTengoWifi";
const char* password = "yateladije";

#define BACKLIGHT_PIN 32
#define SYNC_INTERVAL 300000  // cada 5 minutos

int simulated_offset = 0;
int last_minute = -1;

unsigned long last_interaction = 0;
unsigned long last_sync_time = 0;
bool backlight_on = true;

void conectarWiFiYSync() {
  Serial.println("Conectando WiFi para sincronizar hora...");
  WiFi.begin(ssid, password);
  unsigned long startAttempt = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 10000) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi conectado. Sincronizando hora...");
    configTime(0, 0, "pool.ntp.org");
    while (time(nullptr) < 100000) {
      delay(100);
    }
    Serial.println("Hora sincronizada.");
    WiFi.disconnect(true);  // Desconectar WiFi para ahorrar batería
    Serial.println("WiFi desconectado.");
  } else {
    Serial.println("\nError al conectar WiFi para NTP.");
  }

  last_sync_time = millis();
}

void setup() {
  Serial.begin(115200);

  // Encender retroiluminación
  pinMode(BACKLIGHT_PIN, OUTPUT);
  digitalWrite(BACKLIGHT_PIN, HIGH);

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);

  conectarWiFiYSync();  // Primera sincronización

  pinMode(0, INPUT_PULLUP);
  last_interaction = millis();
}

String formatHora(int h, int m) {
  char buf[6];
  sprintf(buf, "%02d:%02d", h % 24, m);
  return String(buf);
}

void mostrarColumna(String nombre, int hora_base, int min, int x) {
  tft.setTextDatum(TC_DATUM);

  tft.drawString(nombre, x, 10, 2);
  tft.setTextSize(2);
  tft.drawString(formatHora(hora_base + 1, min), x, 55);
  tft.setTextSize(3);
  tft.drawString(formatHora(hora_base, min), x, 85);
  tft.setTextSize(2);
  tft.drawString(formatHora(hora_base - 1 + 24, min), x, 135);
}

void loop() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) return;

  // Sincronizar hora cada 5 minutos
  if (millis() - last_sync_time > SYNC_INTERVAL) {
    conectarWiFiYSync();
  }

  int utc_h = (timeinfo.tm_hour + simulated_offset) % 24;
  int min = timeinfo.tm_min;

  if (min != last_minute) {
    last_minute = min;

    int arg_h = (utc_h - 3 + 24) % 24;
    int ita_h = (utc_h + 2) % 24;
    int ecu_h = (utc_h - 5 + 24) % 24;

    tft.fillScreen(TFT_WHITE);
    mostrarColumna("Arg", arg_h, min, 53);
    mostrarColumna("Ita", ita_h, min, 160);
    mostrarColumna("Ecu", ecu_h, min, 267);

    tft.drawLine(107, 0, 107, 135, TFT_BLACK);
    tft.drawLine(213, 0, 213, 135, TFT_BLACK);
  }

  // Botón para ajustar zona horaria simulada
  if (digitalRead(0) == LOW) {
    simulated_offset = (simulated_offset + 1) % 24;
    last_interaction = millis();

    if (!backlight_on) {
      digitalWrite(BACKLIGHT_PIN, HIGH);
      backlight_on = true;
    }

    delay(500);
  }

  // Apagar pantalla tras 1 minuto sin interacción
  if (backlight_on && (millis() - last_interaction > 60000)) {
    digitalWrite(BACKLIGHT_PIN, LOW);
    backlight_on = false;
  }

  delay(500);
}
