#include<iostream>
#include"Record.h"
#include<time.h>
#include"globals.h"
using namespace std;

Record::Record() {
	this->a0 = MY_NULL;
	this->a1 = MY_NULL;
	this->a2 = MY_NULL;
	this->a3 = MY_NULL;
	this->a4 = MY_NULL;
	this->x = MY_NULL;
	this->key = MY_NULL;
	this->deleted = 0;
	this->pointerToPage = MY_NULL;
	this->pointerToPosition = MY_NULL;
}

Record::Record(int a0, int a1, int a2, int a3, int a4, int x) {
	this->a0 = a0;
	this->a1 = a1;
	this->a2 = a2;
	this->a3 = a3;
	this->a4 = a4;
	this->x = x;
	this->key = a0 + a1 * x + a2 * x * x + a3 * x * x * x + a4 * x * x * x * x;
	this->deleted = 0;
    this->pointerToPage = MY_NULL;
	this->pointerToPosition = MY_NULL;

}
void Record::displayRecord() {
	cout << "-Record: key: " << key << " a0: " << a0 << " a1: " << a1 << " a2: " << a2 << " a3: " << a3 << " a4: " << a4 << " x: " << x << endl;
}
void Record::displayParameters() {

	if (key == MY_NULL)
	{
		cout <<" NULL" << endl;
	}
	else if (key == -1)
	{
		if (pointerToPage == MY_NULL)
		{
			cout << " key: " << key << " pointer: NULL" << " deleted: " << deleted << endl;
		}
		else
		{
			cout << " key: " << key << " pointer: " << pointerToPage << "." << pointerToPosition << " deleted: " << deleted << endl;
		}
	}
	else if (pointerToPage == MY_NULL)
	{
		cout << " key: " << key << " a0: " << a0 << " a1: " << a1 << " a2: " << a2 << " a3: " << a3 << " a4: " << a4 << " x: " << x << " deleted: " << deleted<<" pointer: NULL" << endl;
	}
	else
	{
		cout <<" key: " << key << " a0: " << a0 << " a1: " << a1 << " a2: " << a2 << " a3: " << a3 << " a4: " << a4 << " x: " << x << " deleted: "<< deleted<< " pointer: " << pointerToPage << "." << pointerToPosition <<  endl;
	}
}
void Record::displayParam() {
	if (key == MY_NULL)
	{
		cout << " NULL" << endl;
	}
	else if (pointerToPage == MY_NULL)
	{
		cout << " key: " << key << " pointer: NULL" <<  " deleted: " << deleted  << endl;
	}
	else
	{
		cout << " key: " << key << " pointer: " << pointerToPage << "." << pointerToPosition << " deleted: " << deleted <<  endl;
	}
}
void Record::tickDeleted() {
	deleted = 1;
}
void Record::update(Record* record)
{
	a0 = record->a0;
	a1 = record->a1;
	a2 = record->a2;
	a3 = record->a3;
	a4 = record->a4;
	x = record->x;
}
bool operator<(Record rec1, Record rec2)
{
	return rec1.key < rec2.key;
}
