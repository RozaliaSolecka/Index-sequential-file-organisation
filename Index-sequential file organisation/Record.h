#pragma once
#include"globals.h"

class Record
{
public:
	int key, a0, a1, a2, a3, a4, x;
	int deleted;
	int pointerToPage;
	int pointerToPosition;
	
	Record();
	Record(int a0, int a1, int a2, int a3, int a4, int x);
	void displayRecord();
	void tickDeleted();
	void update(Record* record);
	void displayParameters();
	void displayParam();
	
};
