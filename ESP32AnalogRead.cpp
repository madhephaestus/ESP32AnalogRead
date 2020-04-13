/*
 * ESP32AnalogRead.cpp
 *
 *  Created on: Apr 10, 2020
 *      Author: hephaestus
 *      https://github.com/madhephaestus/ESPMutexDemo/blob/DSPTest/ESPMutexDemo.ino
 */


#include "ESP32AnalogRead.h"

ESP32AnalogRead::ESP32AnalogRead(int pinNum){
	if(!(pinNum<0)){
		attach(pinNum);
	}
}
void ESP32AnalogRead::attach(int pin){
	myPin=pin;
	channel = (adc_channel_t)digitalPinToAnalogChannel(myPin);
	attached=true;
}

float ESP32AnalogRead::readVoltage(){
	float mv= readMiliVolts();

	return mv*0.001;
}
uint32_t  ESP32AnalogRead::readMiliVolts(){
	if(!attached)
		return 0;
	analogRead(myPin);
    // Configure ADC
    adc1_config_width(ADC_WIDTH_12Bit);
    adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_11db);

    // Calculate ADC characteristics i.e. gain and offset factors
    esp_adc_cal_get_characteristics(V_REF, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, &characteristics);
    uint32_t voltage=0;
    // Read ADC and obtain result in mV
    esp_adc_cal_get_voltage(channel, &characteristics,&voltage);
    return voltage-50;
}
