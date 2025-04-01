#include <WiFi.h>
#include <TFT_eSPI.h>
#include <time.h>

TFT_eSPI tft = TFT_eSPI();

const char* ssid     = "YoNoTengoWifi";
const char* password = "yateladije";

int simulated_offset = 0;  // Para avanzar horas manualmente

void setup() {
  pinMode(32, OUTPUT);
  digitalWrite(32, HIGH);

  Serial.begin(115200);
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  // Conectar WiFi
  WiFi.begin(ssid, password);
  tft.setCursor(0, 10);
  tft.println("Conectando WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  tft.println("WiFi OK");

  // Configurar hora NTP
  configTime(0, 0, "pool.ntp.org");
  while (time(nullptr) < 100000) {
    delay(100);
    Serial.print("*");
  }

  // Configurar pin del botón (BOOT en muchas ESP32)
  pinMode(0, INPUT_PULLUP);
}

void loop() {
  // Leer hora actual UTC
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    tft.println("Error al obtener hora");
    return;
  }

  // Simular el avance horario
  timeinfo.tm_hour = (timeinfo.tm_hour + simulated_offset) % 24;

  // Calcular horas locales
  int hora_italia = (timeinfo.tm_hour + 1) % 24;  // UTC+1
  int hora_argentina = (timeinfo.tm_hour - 3 + 24) % 24;  // UTC-3

  tft.fillScreen(TFT_BLACK);
  tft.setCursor(10, 30);
  tft.setTextSize(2);
  tft.printf("Italia: %02d:%02d\n", hora_italia, timeinfo.tm_min);

  tft.setCursor(10, 70);
  tft.setTextSize(2);
  tft.printf("Argentina: %02d:%02d", hora_argentina, timeinfo.tm_min);

  // Esperar botón para simular avance
  int contador = 0;
  while (contador < 100) {  // espera 5 segundos
    if (digitalRead(0) == LOW) {  // botón presionado
      simulated_offset = (simulated_offset + 1) % 24;
      delay(500);  // evitar rebotes
      break;
    }
    delay(50);
    contador++;
  }
}