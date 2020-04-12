#pragma once
#include <Arduino.h>
#include "esp_adc_cal.h"
#define V_REF 1100  // ADC reference voltage


class ESP32AnalogRead {
private:
	int myPin=0;
	 esp_adc_cal_characteristics_t characteristics;
	 adc_channel_t channel ;
	 boolean attached=false;
public :
	ESP32AnalogRead(int pin=-1);
	~ESP32AnalogRead(){}
	void attach(int pin);
	float readVoltage();
	uint32_t readMiliVolts();
};
