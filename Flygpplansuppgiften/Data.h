#pragma once
class Data {
public: 
	Data() {}
	~Data() {}
	virtual bool tick() = 0;
};