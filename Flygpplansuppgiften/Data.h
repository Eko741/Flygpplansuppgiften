#pragma once
class Data {
public: 
	Data() {}
	~Data() {}
	virtual void info() = 0;
	virtual bool tick() = 0;
};