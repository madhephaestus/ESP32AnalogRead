/*
 * ESP32AnalogRead.cpp
 *
 *  Created on: Apr 10, 2020
 *      Author: hephaestus
 *      https://github.com/madhephaestus/ESPMutexDemo/blob/DSPTest/ESPMutexDemo.ino
 */

#include "ESP32AnalogRead.h"

#undef ADC_WIDTH_BIT_DEFAULT
#define ADC_WIDTH_BIT_DEFAULT   ((adc_bits_width_t) ((int)ADC_WIDTH_MAX-1))

ESP32AnalogRead::ESP32AnalogRead(int pinNum)
{
	if (!(pinNum < 0))
	{
		attach(pinNum);
	}
}
void ESP32AnalogRead::attach(int pin)
{
	myPin = pin;
	channel = (adc_channel_t)digitalPinToAnalogChannel(myPin);

	attached = true;
}

float ESP32AnalogRead::readVoltage()
{
	float mv = readMiliVolts();

	return mv * 0.001;
}
/**
 * typedef enum {
 ADC1_CHANNEL_0 = 0, !< ADC1 channel 0 is GPIO36
 ADC1_CHANNEL_1,     !< ADC1 channel 1 is GPIO37
 ADC1_CHANNEL_2,     !< ADC1 channel 2 is GPIO38
 ADC1_CHANNEL_3,     !< ADC1 channel 3 is GPIO39
 ADC1_CHANNEL_4,     !< ADC1 channel 4 is GPIO32
 ADC1_CHANNEL_5,     !< ADC1 channel 5 is GPIO33
 ADC1_CHANNEL_6,     !< ADC1 channel 6 is GPIO34
 ADC1_CHANNEL_7,     !< ADC1 channel 7 is GPIO35
 ADC1_CHANNEL_MAX,
 } adc1_channel_t;
 typedef enum {
 ADC2_CHANNEL_0 = 0, !< ADC2 channel 0 is GPIO4
 ADC2_CHANNEL_1,     !< ADC2 channel 1 is GPIO0
 ADC2_CHANNEL_2,     !< ADC2 channel 2 is GPIO2
 ADC2_CHANNEL_3,     !< ADC2 channel 3 is GPIO15
 ADC2_CHANNEL_4,     !< ADC2 channel 4 is GPIO13
 ADC2_CHANNEL_5,     !< ADC2 channel 5 is GPIO12
 ADC2_CHANNEL_6,     !< ADC2 channel 6 is GPIO14
 ADC2_CHANNEL_7,     !< ADC2 channel 7 is GPIO27
 ADC2_CHANNEL_8,     !< ADC2 channel 8 is GPIO25
 ADC2_CHANNEL_9,     !< ADC2 channel 9 is GPIO26
 ADC2_CHANNEL_MAX,
 } adc2_channel_t;
 */
uint32_t ESP32AnalogRead::readMiliVolts()
{
#if defined(CONFIG_IDF_TARGET_ESP32S2) || defined(CONFIG_IDF_TARGET_ESP32S3)
	if (!attached)
		return 0;
	analogRead(myPin);
	// Configure ADC
	adc_unit_t unit;
	if (myPin < 11)
	{
		adc1_config_width(ADC_WIDTH_BIT_DEFAULT);
		adc1_channel_t chan = ADC1_CHANNEL_0;
		unit = ADC_UNIT_1;
		switch (myPin)
		{

		case 1:
			chan = ADC1_CHANNEL_0;
			break;
		case 2:
			chan = ADC1_CHANNEL_1;
			break;
		case 3:
			chan = ADC1_CHANNEL_2;
			break;
		case 4:
			chan = ADC1_CHANNEL_3;
			break;
		case 5:
			chan = ADC1_CHANNEL_4;
			break;
		case 6:
			chan = ADC1_CHANNEL_5;
			break;
		case 7:
			chan = ADC1_CHANNEL_6;
			break;
		case 8:
			chan = ADC1_CHANNEL_7;
			break;
		case 9:
			chan = ADC1_CHANNEL_8;
			break;
		case 10:
			chan = ADC1_CHANNEL_9;
			break;
		}
		adc1_channel = chan;
		adc1_config_channel_atten(chan, ADC_ATTEN_11db);
	}
	else
	{
		adc2_channel_t chan = ADC2_CHANNEL_0;
		unit = ADC_UNIT_2;
		switch (myPin)
		{
		case 11:
			chan = ADC2_CHANNEL_0;
			break;
		case 12:
			chan = ADC2_CHANNEL_1;
			break;
		case 13:
			chan = ADC2_CHANNEL_2;
			break;
		case 14:
			chan = ADC2_CHANNEL_3;
			break;
		case 15:
			chan = ADC2_CHANNEL_4;
			break;
		case 16:
			chan = ADC2_CHANNEL_5;
			break;
		case 17:
			chan = ADC2_CHANNEL_6;
			break;
		case 18:
			chan = ADC2_CHANNEL_7;
			break;
		case 19:
			chan = ADC2_CHANNEL_8;
			break;
		case 20:
			chan = ADC2_CHANNEL_9;
			break;
		}
		adc2_channel = chan;
		adc2_config_channel_atten(chan, ADC_ATTEN_11db);
	}

	// Calculate ADC characteristics i.e. gain and offset factors
#ifdef ESP_IDF_VERSION
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(3, 1, 0)
	esp_adc_cal_characterize(unit,
							 ADC_ATTEN_DB_11,
							 ADC_WIDTH_BIT_DEFAULT,
							 V_REF,
							 &characteristics);
#else
	esp_adc_cal_get_characteristics(V_REF, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, &characteristics);
#endif
#else
	esp_adc_cal_get_characteristics(V_REF, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, &characteristics);
#endif
	int32_t raw = 0;
	uint32_t voltage = 0;
	// Read ADC and obtain result in mV
	if (unit == ADC_UNIT_1)
	{
		raw = adc1_get_raw(adc1_channel);
	}
	else
	{
		adc2_get_raw(adc2_channel, ADC_WIDTH_BIT_DEFAULT, &raw);
	}
	voltage = esp_adc_cal_raw_to_voltage(raw, &characteristics);
	return voltage;
#elif defined(CONFIG_IDF_TARGET_ESP32)
	if (!attached)
		return 0;
	analogRead(myPin);
	// Configure ADC
	adc_unit_t unit;
	if (myPin > 27)
	{
		adc1_config_width(ADC_WIDTH_12Bit);
		adc1_channel_t chan = ADC1_CHANNEL_0;
		unit = ADC_UNIT_1;
		switch (myPin)
		{

		case 36:
			chan = ADC1_CHANNEL_0;
			break;
		case 37:
			chan = ADC1_CHANNEL_1;
			break;
		case 38:
			chan = ADC1_CHANNEL_2;
			break;
		case 39:
			chan = ADC1_CHANNEL_3;
			break;
		case 32:
			chan = ADC1_CHANNEL_4;
			break;
		case 33:
			chan = ADC1_CHANNEL_5;
			break;
		case 34:
			chan = ADC1_CHANNEL_6;
			break;
		case 35:
			chan = ADC1_CHANNEL_7;
			break;
		}
		adc1_channel = chan;
		adc1_config_channel_atten(chan, ADC_ATTEN_11db);
	}
	else
	{
		adc2_channel_t chan = ADC2_CHANNEL_0;
		unit = ADC_UNIT_2;
		switch (myPin)
		{
		case 4:
			chan = ADC2_CHANNEL_0;
			break;
		case 0:
			chan = ADC2_CHANNEL_1;
			break;
		case 2:
			chan = ADC2_CHANNEL_2;
			break;
		case 15:
			chan = ADC2_CHANNEL_3;
			break;
		case 13:
			chan = ADC2_CHANNEL_4;
			break;
		case 12:
			chan = ADC2_CHANNEL_5;
			break;
		case 14:
			chan = ADC2_CHANNEL_6;
			break;
		case 27:
			chan = ADC2_CHANNEL_7;
			break;
		case 25:
			chan = ADC2_CHANNEL_8;
			break;
		case 26:
			chan = ADC2_CHANNEL_9;
			break;
		}
		adc2_channel = chan;
		adc2_config_channel_atten(chan, ADC_ATTEN_11db);
	}
	// Calculate ADC characteristics i.e. gain and offset factors
#ifdef ESP_IDF_VERSION
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(3, 1, 0)
	esp_adc_cal_characterize(unit,
							 ADC_ATTEN_DB_11,
							 ADC_WIDTH_BIT_12,
							 V_REF,
							 &characteristics);
#else
	esp_adc_cal_get_characteristics(V_REF, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, &characteristics);
#endif
#else
	esp_adc_cal_get_characteristics(V_REF, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, &characteristics);
#endif

	int32_t raw = 0;
	uint32_t voltage = 0;
	// Read ADC and obtain result in mV
	if (unit == ADC_UNIT_1)
	{
		raw = adc1_get_raw(adc1_channel);
	}
	else
	{
		adc2_get_raw(adc2_channel, ADC_WIDTH_12Bit, &raw);
	}
	voltage = esp_adc_cal_raw_to_voltage(raw, &characteristics);
	return voltage;
    
#elif defined(CONFIG_IDF_TARGET_ESP32C3)
	if (!attached)
		return 0;
	analogRead(myPin);
	// Configure ADC
	adc_unit_t unit;
	if (myPin < 5)
	{
		adc1_config_width(ADC_WIDTH_12Bit);
		adc1_channel_t chan = ADC1_CHANNEL_0;
		unit = ADC_UNIT_1;
		switch (myPin)
		{

		case 0:
			chan = ADC1_CHANNEL_0;
			break;
		case 1:
			chan = ADC1_CHANNEL_1;
			break;
		case 2:
			chan = ADC1_CHANNEL_2;
			break;
		case 3:
			chan = ADC1_CHANNEL_3;
			break;
		case 4:
			chan = ADC1_CHANNEL_4;
			break;
		}
		adc1_channel = chan;
		adc1_config_channel_atten(chan, ADC_ATTEN_11db);
	}
	else
	{
		adc2_channel_t chan = ADC2_CHANNEL_0;
		unit = ADC_UNIT_2;
		switch (myPin)
		{
	case 5:
			chan = ADC2_CHANNEL_0;
			break;
		}
		adc2_channel = chan;
		adc2_config_channel_atten(chan, ADC_ATTEN_11db);
	}
	// Calculate ADC characteristics i.e. gain and offset factors
#ifdef ESP_IDF_VERSION
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(3, 1, 0)
	esp_adc_cal_characterize(unit,
							 ADC_ATTEN_DB_11,
							 ADC_WIDTH_BIT_12,
							 V_REF,
							 &characteristics);
#else
	esp_adc_cal_get_characteristics(V_REF, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, &characteristics);
#endif
#else
	esp_adc_cal_get_characteristics(V_REF, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, &characteristics);
#endif

    int raw = 0;
	uint32_t voltage = 0;
	// Read ADC and obtain result in mV
	if (unit == ADC_UNIT_1)
	{
		raw = adc1_get_raw(adc1_channel);
	}
	else
	{
		adc2_get_raw(adc2_channel, ADC_WIDTH_12Bit, &raw);
	}
	voltage = esp_adc_cal_raw_to_voltage(raw, &characteristics);
	return voltage;
#endif
}

uint16_t ESP32AnalogRead::readRaw()
{
#if defined(CONFIG_IDF_TARGET_ESP32S2) || defined(CONFIG_IDF_TARGET_ESP32S3)
	if (!attached)
		return 0;
	analogRead(myPin);
	// Configure ADC
	adc_unit_t unit;
	if (myPin < 11)
	{
		adc1_config_width(ADC_WIDTH_BIT_DEFAULT);
		adc1_channel_t chan = ADC1_CHANNEL_0;
		unit = ADC_UNIT_1;
		switch (myPin)
		{

		case 1:
			chan = ADC1_CHANNEL_0;
			break;
		case 2:
			chan = ADC1_CHANNEL_1;
			break;
		case 3:
			chan = ADC1_CHANNEL_2;
			break;
		case 4:
			chan = ADC1_CHANNEL_3;
			break;
		case 5:
			chan = ADC1_CHANNEL_4;
			break;
		case 6:
			chan = ADC1_CHANNEL_5;
			break;
		case 7:
			chan = ADC1_CHANNEL_6;
			break;
		case 8:
			chan = ADC1_CHANNEL_7;
			break;
		case 9:
			chan = ADC1_CHANNEL_8;
			break;
		case 10:
			chan = ADC1_CHANNEL_9;
			break;
		}
		adc1_channel = chan;
		adc1_config_channel_atten(chan, ADC_ATTEN_11db);
	}
	else
	{
		adc2_channel_t chan = ADC2_CHANNEL_0;
		unit = ADC_UNIT_2;
		switch (myPin)
		{
		case 11:
			chan = ADC2_CHANNEL_0;
			break;
		case 12:
			chan = ADC2_CHANNEL_1;
			break;
		case 13:
			chan = ADC2_CHANNEL_2;
			break;
		case 14:
			chan = ADC2_CHANNEL_3;
			break;
		case 15:
			chan = ADC2_CHANNEL_4;
			break;
		case 16:
			chan = ADC2_CHANNEL_5;
			break;
		case 17:
			chan = ADC2_CHANNEL_6;
			break;
		case 18:
			chan = ADC2_CHANNEL_7;
			break;
		case 19:
			chan = ADC2_CHANNEL_8;
			break;
		case 20:
			chan = ADC2_CHANNEL_9;
			break;
		}
		adc2_channel = chan;
		adc2_config_channel_atten(chan, ADC_ATTEN_11db);
	}
	// Calculate ADC characteristics i.e. gain and offset factors
#ifdef ESP_IDF_VERSION
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(3, 1, 0)
	esp_adc_cal_characterize(unit,
							 ADC_ATTEN_DB_11,
							 ADC_WIDTH_BIT_DEFAULT,
							 V_REF,
							 &characteristics);
#else
	esp_adc_cal_get_characteristics(V_REF, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, &characteristics);
#endif
#else
	esp_adc_cal_get_characteristics(V_REF, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, &characteristics);
#endif

	int32_t raw = 0;
	// Read ADC and obtain result in mV
	if (unit == ADC_UNIT_1)
	{
		raw = adc1_get_raw(adc1_channel);
	}
	else
	{
		adc2_get_raw(adc2_channel, ADC_WIDTH_BIT_DEFAULT, &raw);
	}
	return raw;
#elif defined(CONFIG_IDF_TARGET_ESP32)
	if (!attached)
		return 0;
	analogRead(myPin);
	// Configure ADC
	adc_unit_t unit;
	if (myPin > 27)
	{
		adc1_config_width(ADC_WIDTH_12Bit);
		adc1_channel_t chan = ADC1_CHANNEL_0;
		unit = ADC_UNIT_1;
		switch (myPin)
		{

		case 36:
			chan = ADC1_CHANNEL_0;
			break;
		case 37:
			chan = ADC1_CHANNEL_1;
			break;
		case 38:
			chan = ADC1_CHANNEL_2;
			break;
		case 39:
			chan = ADC1_CHANNEL_3;
			break;
		case 32:
			chan = ADC1_CHANNEL_4;
			break;
		case 33:
			chan = ADC1_CHANNEL_5;
			break;
		case 34:
			chan = ADC1_CHANNEL_6;
			break;
		case 35:
			chan = ADC1_CHANNEL_7;
			break;
		}
		adc1_channel = chan;
		adc1_config_channel_atten(chan, ADC_ATTEN_11db);
	}
	else
	{
		adc2_channel_t chan = ADC2_CHANNEL_0;
		unit = ADC_UNIT_2;
		switch (myPin)
		{
		case 4:
			chan = ADC2_CHANNEL_0;
			break;
		case 0:
			chan = ADC2_CHANNEL_1;
			break;
		case 2:
			chan = ADC2_CHANNEL_2;
			break;
		case 15:
			chan = ADC2_CHANNEL_3;
			break;
		case 13:
			chan = ADC2_CHANNEL_4;
			break;
		case 12:
			chan = ADC2_CHANNEL_5;
			break;
		case 14:
			chan = ADC2_CHANNEL_6;
			break;
		case 27:
			chan = ADC2_CHANNEL_7;
			break;
		case 25:
			chan = ADC2_CHANNEL_8;
			break;
		case 26:
			chan = ADC2_CHANNEL_9;
			break;
		}
		adc2_channel = chan;
		adc2_config_channel_atten(chan, ADC_ATTEN_11db);
	}
	// Calculate ADC characteristics i.e. gain and offset factors
#ifdef ESP_IDF_VERSION
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(3, 1, 0)
	esp_adc_cal_characterize(unit,
							 ADC_ATTEN_DB_11,
							 ADC_WIDTH_BIT_12,
							 V_REF,
							 &characteristics);
#else
	esp_adc_cal_get_characteristics(V_REF, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, &characteristics);
#endif
#else
	esp_adc_cal_get_characteristics(V_REF, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, &characteristics);
#endif

	int32_t raw = 0;
	// Read ADC and obtain result in mV
	if (unit == ADC_UNIT_1)
	{
		raw = adc1_get_raw(adc1_channel);
	}
	else
	{
		adc2_get_raw(adc2_channel, ADC_WIDTH_12Bit, &raw);
	}
	return raw;
    
#elif defined(CONFIG_IDF_TARGET_ESP32C3)
	if (!attached)
		return 0;
	analogRead(myPin);
	// Configure ADC
	adc_unit_t unit;
	if (myPin < 5)
	{
		adc1_config_width(ADC_WIDTH_12Bit);
		adc1_channel_t chan = ADC1_CHANNEL_0;
		unit = ADC_UNIT_1;
		switch (myPin)
		{

		case 0:
			chan = ADC1_CHANNEL_0;
			break;
		case 1:
			chan = ADC1_CHANNEL_1;
			break;
		case 2:
			chan = ADC1_CHANNEL_2;
			break;
		case 3:
			chan = ADC1_CHANNEL_3;
			break;
		case 4:
			chan = ADC1_CHANNEL_4;
			break;
		}
		adc1_channel = chan;
		adc1_config_channel_atten(chan, ADC_ATTEN_11db);
	}
	else
	{
		adc2_channel_t chan = ADC2_CHANNEL_0;
		unit = ADC_UNIT_2;
		switch (myPin)
		{
	case 5:
			chan = ADC2_CHANNEL_0;
			break;
		}
		adc2_channel = chan;
		adc2_config_channel_atten(chan, ADC_ATTEN_11db);
	}
	// Calculate ADC characteristics i.e. gain and offset factors
#ifdef ESP_IDF_VERSION
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(3, 1, 0)
	esp_adc_cal_characterize(unit,
							 ADC_ATTEN_DB_11,
							 ADC_WIDTH_BIT_12,
							 V_REF,
							 &characteristics);
#else
	esp_adc_cal_get_characteristics(V_REF, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, &characteristics);
#endif
#else
	esp_adc_cal_get_characteristics(V_REF, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, &characteristics);
#endif

	int raw = 0;
	// Read ADC and obtain result in mV
	if (unit == ADC_UNIT_1)
	{
		raw = adc1_get_raw(adc1_channel);
	}
	else
	{
		adc2_get_raw(adc2_channel, ADC_WIDTH_12Bit, &raw);
	}
	return raw;
#endif
}
