#pragma once
#include"Record.h"
#include"Index.h"
#include"globals.h"
#include<tuple>
using namespace std;


class File
{
public:
	const char* name;
	int sizeOfPrimaryArea; // liczba stron w Obszarze G≥Ûwnym
	int sizeOfOverflowArea; // liczba stron w Obszarze Nadmiarowym
	
	Record* records;
	Record* recordsOverflow;
	Record* recordsCop ;
	Record* recordsCopy ;
	Record* recordsFile ;
	Record* r ;
	Record* next ;
	Index* index ;
	int positionToRemember;
	const char* fileToRemember;
	Record* bufferToRemember;
	Index* indexBufferToRemember;
	Record* artificialBuffer;
	Index* artificialIndexBuffer;

	File(const char* name, int sizeOfPrimaryArea, int sizeOfOverflowArea);
	void initialValues();
	void addRecord(Record* record);
	void createIndex();
	void readFile(const char* fileToRead);
	void readIndex();
	int getPageNumber(int key);
	void copyFile(const char* derivation, const char* destination);
	void clearFile(const char* name);
	void sortRecords(Record* record, int number);
	tuple<int, int> findPredecessor(Record& records, Record* newRecord);
	void findPredecessorInOverflowArea(int pageNum, int positionNum, int pageNumber, int positionNumber, int key, Record& rec, int iteratorToChangeInPrimary, Record* newRecord); // idü do tego wskaünika, zapisz ten wskaünik
	tuple<int, int> findFreePositionInOverflowArea();
	void addRecordToOverflowArea(Record* record, int pageNumber, int positionNumber);
	void reorganise();
	void findRecordInOverflowArea(Record& recordsToWrite, int* p, int* positionInFile, int page, int position, int sizeOfPrimaryCop);
	void readPage(const char* fileName, int position, Record* page);
	void readIndexPage(const char* fileName, int position, Index* page);
	void readSequence();
	void readSequenceInOverflowArea(int page, int position);
	void readRecord(int key);
	void readRecordInOverflowArea(int key, int page, int position);
	void deleteRecord(int key);
	void deleteRecordInOverflowArea(int key, int page, int position);
	void updateFunction(int key, Record* record);
	void updateRecord(int key, Record* record);
	void updateRecordInOverflowArea(int key, int page, int position, Record* record);
	~File();


};