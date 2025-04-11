#include <Arduino.h>
#include <Adafruit_ADS1X15.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

int adc0;
float volt;

int tare_pin = 17;
int mean_n = 100;

bool first = true;

// float_t tare = 0.19818819110185473; // All data
float_t tare = 0.20928980710714182; // All data RIDGE
// float_t tare = 0.20925706474448935; // Just Screws RIDGE

// float_t slope = 7.987488384490883e-05; // All data
float_t slope = 8.114590129318831e-05; // All data RIDGE
// float_t slope = 8.193766988377718e-05; // Just Screws RIDGE


Adafruit_ADS1115 ads;

Adafruit_SSD1306 display(128, 64, &Wire, -1);

void setup() {

	Serial.begin(115200);

	display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
	display.clearDisplay();

	display.setTextSize(1);
	display.setTextColor(WHITE);
	display.setCursor(0, 0);
	display.print("oh, es sans anderteil");
	display.display();

	ads.setGain(GAIN_EIGHT);

	pinMode(tare_pin, INPUT_PULLDOWN);
}

void loop() {

	if (!ads.begin()) {
		Serial.println("No se encontro el ADS1115");
	}

	float temp = 0;  
	for (size_t i = 0; i < mean_n; i++)
	{
		adc0 = ads.readADC_Differential_0_1();
		volt = ads.computeVolts(adc0);
		temp += volt;
	}

	float mean_volt = temp / mean_n;

	Serial.print("Voltaje: ");
	Serial.print(mean_volt, 5);
	Serial.println(" V");
  
	float mass = (mean_volt - (tare)) /(slope);

	if (digitalRead(tare_pin) == HIGH || first == true)
	{
		first = false;
		Serial.println("Tara activada");
		tare = mean_volt;
		delay(100);
	}


  	Serial.print("Masa: ");
	Serial.print(mass, 0);
	Serial.println(" g");
	Serial.println("========================================");

	// Display
	display.clearDisplay();
	display.setTextSize(1);
	display.setTextColor(WHITE);
	display.setCursor(0, 0);
	display.print("Voltaje: ");
	display.print(mean_volt, 5);
	display.println(" V");
	display.println("=====================");
	display.print("Masa: ");
	display.print(mass, 0);
	display.println(" g");
	display.display();

	delay(100);
}