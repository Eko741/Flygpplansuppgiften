#pragma once
#include "Data.h"
#include "FlightD.h"
class QueueObject {
public:
	QueueObject(FlightD* ptr) : data(ptr){}
	~QueueObject() { delete data; }
	void Next(QueueObject* ptr) { next = ptr; }
	void Prev(QueueObject* ptr) { prev = ptr; }
	void setData(Data* ptr) { data = ptr; }
	QueueObject* Next() { return next; }
	QueueObject* Prev() { return prev; }
	Data* getData() { return data; }

private:
	QueueObject* next = nullptr, * prev = nullptr;
	Data* data;
};