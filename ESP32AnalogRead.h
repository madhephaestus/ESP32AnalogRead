#pragma once

class ESP32AnalogRead {
public :
	ESP32AnalogRead();
	~ESP32AnalogRead(){}
	void attach(int pin);
};
