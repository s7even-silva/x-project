#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "logo.h"

// OLED
#define I2C_ADDRESS   0x3C
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// DS18B20
const int ONE_WIRE_BUS = 4;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature dsSensor(&oneWire);

// TDS Gravity
const int tdsPin     = A0;
const int numSamples = 30;
int analogBuffer[numSamples];
int bufIndex = 0;
const float VREF    = 5.0;
const int   ADC_RES = 1023;

// Botón
const int buttonPin      = 6;
int       buttonState    = LOW;
int       lastButtonState= LOW;

void setup() {
  Serial.begin(115200);

  // Muestra el logo al encender el equipo
  delay(250);
  display.begin(I2C_ADDRESS, true);
  display.clearDisplay();
  {
    int16_t x = (SCREEN_WIDTH  - LOGO_WIDTH ) / 2;
    int16_t y = (SCREEN_HEIGHT - LOGO_HEIGHT) / 2;
    display.drawBitmap(x, y, epd_bitmap_logo, LOGO_WIDTH, LOGO_HEIGHT, SH110X_WHITE);
    display.display();
    delay(2000);
    display.clearDisplay();
  }

  // Inicializando el sensor de temperatura, el sensor TDS, y el botón.
  // Además limpiando el contenido de analogBuffer por seguridad.
  dsSensor.begin();
  for (int i = 0; i < numSamples; i++) analogBuffer[i] = 0;
  pinMode(buttonPin, INPUT);
  pinMode(tdsPin, INPUT);
}

void loop() {

  // Espera a que se presione el botón para hacer
  // una llamada a la función hacerMediciones()
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH && lastButtonState == LOW) {
    hacerMediciones();
  }
  lastButtonState = buttonState;
  delay(50);
}

void hacerMediciones() {

  // Declarando constantes para el uso posterior
  const int totalMed = 20;
  float sumaTdsComp = 0.0;
  float sumaTemp    = 0.0;

  // Muestra en la pantalla la palabra "Midiendo"
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);
  display.setCursor((SCREEN_WIDTH - 16*6)/2, 0);
  display.print("Midiendo");
  int barY = 32;
  int barH = 10;

  // Muestra una barra de progreso que se actualiza
  // según la cantidad de mediciones realizadas.
  display.drawRect(0, barY, SCREEN_WIDTH, barH, SH110X_WHITE);
  display.display();

  bufIndex = 0;
  for (int i = 0; i < numSamples; i++) analogBuffer[i] = 0;

  for (int i = 1; i <= totalMed; i++) {
    // Medición de temperatura
    dsSensor.requestTemperatures();
    float tempC = dsSensor.getTempCByIndex(0);
    if (tempC == DEVICE_DISCONNECTED_C) tempC = 25.0;
    sumaTemp += tempC;

    // Medición de TDS
    analogBuffer[bufIndex++] = analogRead(tdsPin);
    if (bufIndex >= numSamples) bufIndex = 0;
    
    long sumBuf = 0;
    for (int j = 0; j < numSamples; j++) sumBuf += analogBuffer[j];
    float avgAdc  = sumBuf / float(numSamples);
    float voltage = avgAdc * VREF / ADC_RES;
    float tdsRaw  = 133.42 * pow(voltage, 3)
                   - 255.86 * pow(voltage, 2)
                   + 857.39 * voltage; // Fórmula para calcular TDS

    float tdsComp = tdsRaw / (1.0 + 0.02 * (tempC - 25.0)); // Compensación por temperatura
    tdsComp = 1.6442 * tdsComp; // Compensación por mediciones experimentales

    sumaTdsComp += tdsComp;

    // Actualización de la barra de progreso
    int barWidth = map(i, 0, totalMed, 0, SCREEN_WIDTH);
    display.fillRect(1, barY+1, barWidth-2, barH-2, SH110X_WHITE);
    display.display();

    delay(50);
  }

  // Cálculo de los promedios
  float promedioTds = sumaTdsComp / totalMed;
  float promedioT   = sumaTemp    / totalMed;

  // Mostrado de los resultados en pantalla
  // y en el monitor serial
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);

  display.setCursor(0, 24);
  display.print("TDS:");
  display.print(int(promedioTds));
  display.print(" ppm");

  display.setCursor(0, 0);
  display.print("T:");
  display.print(promedioT, 1);
  display.print("C");

  display.display();

  Serial.print("Promedio TDS comp: ");
  Serial.print(promedioTds);
  Serial.println(" ppm");
}
