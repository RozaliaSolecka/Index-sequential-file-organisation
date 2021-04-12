#include<iostream>
#include<fstream>
#include<math.h>
#include"File.h"
#include<time.h>
#include<algorithm>
#include<tuple>

using namespace std;

File::File(const char* name, int sizeOfPrimaryArea, int sizeOfOverflowArea) {
	this->name = name;
	this->sizeOfOverflowArea = sizeOfOverflowArea;
	this->sizeOfPrimaryArea = sizeOfPrimaryArea;

	this->artificialIndexBuffer = new Index[INDEX_BLOCK_RATE * sizeof(Index)];
	this->artificialBuffer = new Record[BLOCK_RATE * sizeof(Record)];
	this->records = new Record[BLOCK_RATE * sizeof(Record)];
	this->recordsOverflow = new Record[BLOCK_RATE * sizeof(Record)];
	this->recordsCop = new Record[BLOCK_RATE * sizeof(Record)];
	this->recordsCopy = new Record[BLOCK_RATE * sizeof(Record)];
	this->recordsFile = new Record[BLOCK_RATE * sizeof(Record)];
	this->r = new Record[BLOCK_RATE * sizeof(Record)];
	this->next = new Record[BLOCK_RATE * sizeof(Record)];
	this->index = new Index[INDEX_BLOCK_RATE * sizeof(Index)];
	this->positionToRemember = MY_NULL;
	this->fileToRemember = "" ;
	this->bufferToRemember = artificialBuffer;
	this->indexBufferToRemember = artificialIndexBuffer;

	fstream file(name, ios::out | ios::in | ios::binary);
	file.close();
}

void File::clearFile(const char* name)
{
	ofstream ofs;
	ofs.open(name, ios::out | ios::trunc);
	ofs.close();
}

//konwersja pojedyñczych stron z bin na typ Record/Index
void File::readPage(const char* fileName, int position, Record* record)
{
	
	if (position != positionToRemember || fileName != fileToRemember || bufferToRemember != record)
	{
		fstream file(fileName, ios::binary | ios::in | ios::out);

		file.seekg(position, ios::beg);
		file.read(reinterpret_cast<char*>(record), sizeof(Record) * BLOCK_RATE);
		numberOfReads++;


		file.close();
		fileToRemember = fileName;
		positionToRemember = position;
		bufferToRemember = record;
		
	}
}
void File::readIndexPage(const char* fileName, int position, Index* index)
{
	if (position != positionToRemember || fileName != fileToRemember || artificialIndexBuffer!= index)
	{
		fstream file(fileName, ios::binary | ios::in | ios::out);

		file.seekg(position, ios::beg);
		file.read(reinterpret_cast<char*>(index), sizeof(Index) * INDEX_BLOCK_RATE);
		numberOfReads++;
		
		file.close();
		fileToRemember = fileName;
		positionToRemember = position;	
		indexBufferToRemember = index;
	}
	
}
void File::initialValues()
{
	ofstream pagedFile(name, ios::out | ios::app | ios::binary);

	int numberOfPages = sizeOfPrimaryArea + sizeOfOverflowArea;

	Record* page = new Record[BLOCK_RATE]();

	page[0].key = -1;   // sztuczny klucz na pierwszej stronie
	page[0].deleted = 1;

	for (int i = 0; i < numberOfPages; i++)
	{
		pagedFile.write(reinterpret_cast<const char*>(page), sizeof(Record) * BLOCK_RATE);
		numberOfWrites++;

		page[0].key = MY_NULL;
		page[0].deleted = 0;
	}

	delete[] page;
	pagedFile.close();
}

void File::createIndex()
{
	int position = 0;
	
	

	clearFile(indexFile); // czyszczenie indeksu

	fstream pagedFile(name, ios::binary | ios::in | ios::out);
	fstream indexFile(indexFile, ios::binary | ios::app | ios::out);


	for (int k = 0; k < INDEX_BLOCK_RATE; k++) // czyszczenie bufora indeksu
	{
		index[k].key = MY_NULL;
		index[k].page = MY_NULL;
	}

	int j = 0;
	for (int i = 0; i < sizeOfPrimaryArea; i++)
	{
		pagedFile.seekg(position, ios::beg);
		readPage(name, position, records);
		position += sizeof(Record) * BLOCK_RATE;

		index[j].key = records[0].key;   //pobranie pierwszego klucza ze strony
		index[j].page = i;
		j++;


		if (j == INDEX_BLOCK_RATE)  //zapisanie strony indeksu
		{
			indexFile.write(reinterpret_cast<const char*>(index), sizeof(Index) * INDEX_BLOCK_RATE);
			numberOfWrites++;
			j = 0;
			for (int k = 0; k < INDEX_BLOCK_RATE; k++) // czyszczenie bufora indeksu
			{
				index[k].key = MY_NULL;
				index[k].page = MY_NULL;
			}
		}
	}
	if (j < INDEX_BLOCK_RATE && j>0) // zapis strony jeœli niezape³niona
	{
		indexFile.write(reinterpret_cast<const char*>(index), sizeof(Index) * INDEX_BLOCK_RATE);
		numberOfWrites++;
	}
	pagedFile.close();
	indexFile.close();

}

void File::readFile(const char* fileToRead)
{
	int position = 0;


	cout << "PRIMARY AREA:" << endl;

	for (int i = 0; i < sizeOfPrimaryArea; i++)
	{
		readPage(fileToRead, position, records);
		position += (sizeof(Record) * BLOCK_RATE);

		for (int k = 0; k < BLOCK_RATE; k++)
		{
			cout << i << "." << k; records[k].displayParameters();
		}
		
	}
	

	cout << "OVERFLOW AREA:" << endl;

	for (int i = 0; i < sizeOfOverflowArea; i++)
	{
		readPage(fileToRead, position, records);
		position += (sizeof(Record) * BLOCK_RATE);

		for (int k = 0; k < BLOCK_RATE; k++)
		{
			cout << i << "." << k; records[k].displayParameters();
		}
		
	}
}
void File::readIndex()
{
	int position = 0;

	
	int numberOfPages = (int)ceil((double)sizeOfPrimaryArea / (double)INDEX_BLOCK_RATE);

	cout << "INDEX FILE:" << endl;

	for (int i = 0; i < numberOfPages; i++)
	{
		readIndexPage(indexFile, position, index);
		position += (INDEX_BLOCK_RATE * sizeof(Index));

		for (int k = 0; k < INDEX_BLOCK_RATE; k++)
		{
			if (index[k].key == MY_NULL && index[k].page == MY_NULL)
			{
				cout << i << "." << k << " key: NULL page: NULL" << endl;
			}
			else
			{
				cout << i << "." << k << " key: " << index[k].key << " page: " << index[k].page << endl;
			}
		}
		
	}
}

int File::getPageNumber(int key)
{
	int position = 0;

	
	Index previousIndex;
	previousIndex.key = -1;
	previousIndex.page = 0;

	int numberOfPages = (int)ceil((double)sizeOfPrimaryArea / (double)INDEX_BLOCK_RATE);

	for (int i = 0; i < numberOfPages; i++)
	{
		readIndexPage(indexFile, position, index);
		position += (INDEX_BLOCK_RATE * sizeof(Index));

		for (int j = 0; j < INDEX_BLOCK_RATE; j++)
		{
			if (previousIndex.key <= key && index[j].key > key)
			{
				return previousIndex.page;
			}
			else if (previousIndex.key <= key && index[j].key == MY_NULL)
			{
				return previousIndex.page;
			}
			previousIndex = index[j];
		}
		
	}
	return previousIndex.page;
}
void File::copyFile(const char* derivation, const char* destination)
{

	int position = 0;
	ofstream des(destination, ios::out | ios::app | ios::binary);

	for (int i = 0; i < sizeOfPrimaryArea + sizeOfOverflowArea; i++)
	{
		readPage(derivation, position, records);
		position += (sizeof(Record) * BLOCK_RATE);
		des.write(reinterpret_cast<const char*>(records), sizeof(Record) * BLOCK_RATE);
		numberOfWrites++;
		
	}
	des.close();
}

//reorganizacja
void File::findRecordInOverflowArea(Record& rec, int* p, int* positionInFile, int page, int position, int sizeOfPrimaryCop) {

	fstream file(pagedFile, ios::binary | ios::in | ios::out);
	Record* records = &rec;
	
	int pos = (sizeOfPrimaryCop + page) * sizeof(Record) * BLOCK_RATE;
	readPage(fileCopy, pos, recordsCop);

	for (int i = 0; i < BLOCK_RATE; i++)
	{
		if (i == position && *p < (int)ceil((double)BLOCK_RATE * (double)ALFA))
		{
			if (recordsCop[i].deleted == 0)
			{
				records[*p] = recordsCop[i];
				records[*p].pointerToPage = MY_NULL;   // czysczenie wskaznikow
				records[*p].pointerToPosition = MY_NULL;
				*p = *p + 1;
			}

			if (*p == (int)ceil((double)BLOCK_RATE * (double)ALFA))
			{
				file.seekp(*positionInFile);
				file.write(reinterpret_cast<const char*>(records), sizeof(Record) * BLOCK_RATE);
				numberOfWrites++;
				*positionInFile += sizeof(Record) * BLOCK_RATE;
				*p = 0;
			}
			if (recordsCop[i].pointerToPage != MY_NULL)
			{
				if (*p == 0)   // je¿eli w overflow jest kolejne wskazanie i trzeba pobraæ now¹ stronê
				{
					readPage(pagedFile, *positionInFile, r);  // czytam pust¹ strone 

					for (int y = 0; y < BLOCK_RATE; y++)
					{
						records[y] = r[y];   // aktualna strona jest zape³niana pust¹ stron¹
					}
					
				}
				findRecordInOverflowArea(*records, &(*p), &(*positionInFile), recordsCop[i].pointerToPage, recordsCop[i].pointerToPosition, sizeOfPrimaryCop);

			}
			
			return;
		}
	}
	
}
void File::reorganise()
{
	int sizeOfPrimaryCopy = sizeOfPrimaryArea;
	int sizeOfOverflowCopy = sizeOfOverflowArea;
	int totalNumberOfRecordsInPrimaryCopy = totalNumberOfRecordsInPrimaryArea;
	int totalNumberOfRecordsInOverflowCopy = totalNumberOfRecordsInOverflowArea;
	int positionCopy = 0;
	int positionFile = 0;

	
	int p = 0; //recordsFile iterator
	int k;

	copyFile(pagedFile, fileCopy);
	clearFile(pagedFile);                                                       //-deleted
	sizeOfPrimaryArea = (int)ceil((double)(totalNumberOfRecordsInPrimaryArea + totalNumberOfRecordsInOverflowArea - numberOfDeletedRecords) / (double)((int)ceil((double)BLOCK_RATE * (double)ALFA)));
	sizeOfOverflowArea = (int)ceil((double)VN_RATIO * (double)sizeOfPrimaryArea); //nowe przestrzenie w stronach

	initialValues();
	readPage(pagedFile, positionFile, recordsFile);
	fstream file(pagedFile, ios::binary | ios::in | ios::out);

	for (int i = 0; i < sizeOfPrimaryCopy; i++)  // liczba stron w primary
	{
		readPage(fileCopy, positionCopy, recordsCopy);
		positionCopy += sizeof(Record) * BLOCK_RATE;

		for (k = 0; k < BLOCK_RATE; k++)
		{
			if (recordsCopy[k].key != MY_NULL && p < (int)ceil((double)BLOCK_RATE * (double)ALFA))
			{
				if (recordsCopy[k].deleted == 0 || recordsCopy[k].key == -1)
				{
					recordsFile[p] = recordsCopy[k];
					recordsFile[p].pointerToPage = MY_NULL;   // czysczenie wskaznikow
					recordsFile[p].pointerToPosition = MY_NULL;
					p++;
				}

				if (p == (int)ceil((double)BLOCK_RATE * (double)ALFA))
				{
					file.seekp(positionFile);
					file.write(reinterpret_cast<const char*>(recordsFile), sizeof(Record) * BLOCK_RATE);
					numberOfWrites++;
					positionFile += sizeof(Record) * BLOCK_RATE;

					

					readPage(pagedFile, positionFile, recordsFile);
					p = 0;
				}
			}
			if (recordsCopy[k].pointerToPage != MY_NULL)  // jezeli jest wskaznik na overflow area
			{
				findRecordInOverflowArea(*recordsFile, &p, &positionFile, recordsCopy[k].pointerToPage, recordsCopy[k].pointerToPosition, sizeOfPrimaryCopy);

				if (p == 0)
				{
					
					readPage(pagedFile, positionFile, recordsFile);
				}
			}
		}
		
	}
	if (p > 0 && p < (int)ceil((double)BLOCK_RATE * (double)ALFA)) // gdy jest to ostatni ze strony i nie zapelni³ po³owy
	{
		file.seekp(positionFile);
		file.write(reinterpret_cast<const char*>(recordsFile), sizeof(Record) * BLOCK_RATE);
		numberOfWrites++;
	}
	
	file.close();
	clearFile(fileCopy);
	totalNumberOfRecordsInPrimaryArea += (totalNumberOfRecordsInOverflowArea - numberOfDeletedRecords);  //-deleted
	totalNumberOfRecordsInOverflowArea = 0;  // liczba rekordów w overflow = 0
	numberOfDeletedRecords = 0;

	createIndex();
	reo_n++;
	reo += numberOfReads + numberOfWrites;
}
void File::sortRecords(Record* records, int number)
{
	Record tempRecord;
	for (int j = number; j >= 0; j--)
	{
		for (int i = 0; i < j; i++)
		{
			if (records[i].key > records[i + 1].key) // niewlasciwa kolejnosc
			{
				tempRecord = records[i];
				records[i] = records[i + 1];
				records[i + 1] = tempRecord;
			}
		}
	}
}
//wstawianie
tuple<int, int> File::findFreePositionInOverflowArea()
{
	int pageNumber = totalNumberOfRecordsInOverflowArea / BLOCK_RATE;
	int positionNumber = totalNumberOfRecordsInOverflowArea % BLOCK_RATE;

	return make_tuple(pageNumber, positionNumber);
}
void File::findPredecessorInOverflowArea(int pageNum, int positionNum, int pageNumber, int positionNumber, int key, Record& rec, int iteratorToChangeInPrimary, Record* newRecord) { //szukaj poprzednika o overflowArea

	int position = (sizeOfPrimaryArea + pageNum) * BLOCK_RATE * sizeof(Record);
	
	Record* recordsFromPrimary = &rec;
	
	int nextPos;

	fstream fil(pagedFile, ios::binary | ios::in | ios::out);
	readPage(pagedFile, position, recordsOverflow);

	for (int k = 0; k < BLOCK_RATE; k++) // jedna strona
	{
		if (key == recordsOverflow[k].key && recordsOverflow[k].deleted == 0)
		{
			cout << "Record with given key already exists. " << endl;
			AddToOverflowArea = false;
			fil.close();
			return;
		}
		if (recordsOverflow[k].key == key && recordsOverflow[k].deleted == 1) { // rekord jest usuniety, dodaje nowy rekord, ale w tym samym miejscu
			recordsOverflow[k].a0 = newRecord->a0;
			recordsOverflow[k].a1 = newRecord->a1;
			recordsOverflow[k].a2 = newRecord->a2;
			recordsOverflow[k].a3 = newRecord->a3;
			recordsOverflow[k].a4 = newRecord->a4;
			recordsOverflow[k].x = newRecord->x;
			recordsOverflow[k].deleted = 0;
			numberOfDeletedRecords--;
			AddToOverflowArea = false;
			fil.seekp(position, ios::beg);
			fil.write(reinterpret_cast<const char*>(recordsOverflow), sizeof(Record) * BLOCK_RATE);  // wpisanie odpowiedniej strony do primaryArea
			numberOfWrites++;
			return;
		}
		if (k == positionNum) {

			if (recordsOverflow[k].pointerToPage != MY_NULL)
			{
				nextPos = (sizeOfPrimaryArea + recordsOverflow[k].pointerToPage) * BLOCK_RATE * sizeof(Record);   // je¿eli jestem pomiêdzy mniejszym a wiêkszym
				readPage(pagedFile, nextPos, next);

				if (k == positionNum && key > recordsOverflow[k].key && next[(recordsOverflow[k].pointerToPosition)].key > key && recordsOverflow[k].pointerToPage != MY_NULL) {  // wskaznik na wskaznik a nowy rekord pomiedzy
					newRecord->pointerToPage = recordsOverflow[k].pointerToPage;
					newRecord->pointerToPosition = recordsOverflow[k].pointerToPosition;
					recordsOverflow[k].pointerToPage = pageNumber;
					recordsOverflow[k].pointerToPosition = positionNumber;

					fil.seekp(position, ios::beg);
					fil.write(reinterpret_cast<const char*>(recordsOverflow), sizeof(Record) * BLOCK_RATE);
					numberOfWrites++;
					fil.close();
					
					return;
				}
			}
			
			if ((k == positionNum && key < recordsOverflow[k].key))     // je¿eli nowy klucz jest mniejszy ni¿ ten wskazany
			{
				recordsFromPrimary[iteratorToChangeInPrimary].pointerToPage = pageNumber;  // zmiana wskazania w primary area na ten mniejszy
				recordsFromPrimary[iteratorToChangeInPrimary].pointerToPosition = positionNumber;
				newRecord->pointerToPage = pageNum;                         // zmiana wska¿nika w nowym rekordzie na ten kolejny wiêkszy
				newRecord->pointerToPosition = positionNum;
				savePrimaryArea = true;

				fil.seekp(position, ios::beg);
				fil.write(reinterpret_cast<const char*>(recordsOverflow), sizeof(Record) * BLOCK_RATE);
				numberOfWrites++;
				fil.close();
				return;
			}
			else if (k == positionNum && recordsOverflow[k].pointerToPage == MY_NULL) {

				recordsOverflow[k].pointerToPage = pageNumber;
				recordsOverflow[k].pointerToPosition = positionNumber;

				fil.seekp(position, ios::beg);
				fil.write(reinterpret_cast<const char*>(recordsOverflow), sizeof(Record) * BLOCK_RATE);
				numberOfWrites++;
				fil.close();
				return;
			}
			else if (k == positionNum && recordsOverflow[k].pointerToPage != MY_NULL) {
				fil.close();
				findPredecessorInOverflowArea(recordsOverflow[k].pointerToPage, recordsOverflow[k].pointerToPosition, pageNumber, positionNumber, key, *recordsFromPrimary, iteratorToChangeInPrimary, newRecord);
			}
		}
	}
	

}
tuple<int, int> File::findPredecessor(Record& rec, Record* newRecord) {  //szukaj poprzednika w primaryArea

	int key = newRecord->key;
	int pageNumber, positionNumber;
	int pageNum, positionNum;
	Record* records = &rec;
	int iteratorToChangeInPrimary = MY_NULL;

	tie(pageNumber, positionNumber) = findFreePositionInOverflowArea(); // ten wskaznik mam wstawiæ

	for (int i = 0; i < BLOCK_RATE - 1; i++)
	{
		if ((records[i].key < key && records[i + 1].key > key && records[i].pointerToPage == MY_NULL) || //znajdŸ porzednika w primaryArea
			(records[i].key < key && records[i + 1].key == MY_NULL && records[i].pointerToPage == MY_NULL))
		{
			records[i].pointerToPage = pageNumber;                              // zmiana pointera w primaryArea
			records[i].pointerToPosition = positionNumber;

			savePrimaryArea = true;
		}

		else if ((records[i].key < key && records[i + 1].key > key && records[i].pointerToPage != MY_NULL) ||
			(records[i].key < key && records[i + 1].key == MY_NULL && records[i].pointerToPage != MY_NULL)) // znajdz poprzednika w overflowArea;
		{
			pageNum = records[i].pointerToPage;
			positionNum = records[i].pointerToPosition;
			iteratorToChangeInPrimary = i;
			findPredecessorInOverflowArea(pageNum, positionNum, pageNumber, positionNumber, key, *records, iteratorToChangeInPrimary, newRecord);  // idŸ do tego, zapis ten
		}
	}
	int i = BLOCK_RATE - 1;
	if ((records[i].key < key && records[i].pointerToPage == MY_NULL) || //znajdŸ porzednika w primaryArea
		(records[i].key < key && records[i].pointerToPage == MY_NULL))
	{
		records[i].pointerToPage = pageNumber;                              // zmiana pointera w primaryArea
		records[i].pointerToPosition = positionNumber;
		numberOfWrites++;
		savePrimaryArea = true;
	}

	else if ((records[i].key < key && records[i].pointerToPage != MY_NULL) ||
		(records[i].key < key && records[i].pointerToPage != MY_NULL)) // znajdz poprzednika w overflowArea;
	{
		pageNum = records[i].pointerToPage;
		positionNum = records[i].pointerToPosition;
		iteratorToChangeInPrimary = i;
		findPredecessorInOverflowArea(pageNum, positionNum, pageNumber, positionNumber, key, *records, iteratorToChangeInPrimary, newRecord);
	}

	return make_tuple(pageNumber, positionNumber);

}

void File::addRecordToOverflowArea(Record* record, int pageNumber, int positionNumber)
{


	int position = (sizeOfPrimaryArea + pageNumber) * BLOCK_RATE * sizeof(Record);

	fstream fil(pagedFile, ios::binary | ios::in | ios::out);
	readPage(pagedFile, position, recordsOverflow);

	for (int k = 0; k < BLOCK_RATE; k++) // jedna strona
	{
		if (k == positionNumber) {
			recordsOverflow[k] = *record;
			totalNumberOfRecordsInOverflowArea++;  //zwiêkszam liczbê rekordów w overflowArea
		}
	}
	fil.seekp(position, ios::beg);
	fil.write(reinterpret_cast<const char*>(recordsOverflow), sizeof(Record) * BLOCK_RATE);  // wpisanie odpowiedniej strony do pliku
	numberOfWrites++;
	fil.close();
	

}
void File::addRecord(Record* record)
{
	if (record->key < 0)
	{
		cout << "info: Key is below zero. Invalid Record " << endl;
		return;
	}

	//if (((double)totalNumberOfRecordsInOverflowArea / (double)totalNumberOfRecordsInPrimaryArea >= BETA)
	//	|| (totalNumberOfRecordsInOverflowArea == BLOCK_RATE * sizeOfOverflowArea))
	if (totalNumberOfRecordsInOverflowArea == BLOCK_RATE * sizeOfOverflowArea)
	{
		cout << "info: Automatic reorganisation " << endl;
		reorganise();
		numberOfReads = 0;
		numberOfWrites = 0;
	}

	
	int pageToRead = getPageNumber(record->key);  // numer strony do zmiany
	int position = pageToRead * BLOCK_RATE * sizeof(Record);
	int numberOfRecords = 0;
	int pageNumber, positionNumber;

	fstream der(pagedFile, ios::binary | ios::in | ios::out);

	readPage(pagedFile, position, records);

	int k = 0;
	for (int k = 0; k < BLOCK_RATE; k++) // jedna strona
	{
		if (records[k].key == record->key && records[k].deleted == 0) { cout << "info: Record with the given key aready exists. Unable to add record " << endl; der.close();  return; }
		if (records[k].key == record->key && records[k].deleted == 1) { // rekord jest usuniety, dodaje nowy rekord, ale w tym samym miejscu
		records[k].a0 = record->a0; 
		records[k].a1 = record->a1; 
		records[k].a2 = record->a2;
		records[k].a3 = record->a3;
		records[k].a4 = record->a4;
		records[k].x = record->x;
		records[k].deleted = 0;
		numberOfDeletedRecords--;
		der.seekp(position, ios::beg);
		der.write(reinterpret_cast<const char*>(records), sizeof(Record) * BLOCK_RATE);  // wpisanie odpowiedniej strony do primaryArea
		numberOfWrites++;
		return;
		}

		if (records[k].key != MY_NULL) { numberOfRecords++; }  //liczba zajêtych pozycji na stronie
	}

	if (numberOfRecords < BLOCK_RATE)  //strona do zmiany nie jest zape³niona , mo¿na tu wstawiæ rekord
	{
		records[numberOfRecords] = *record;
		totalNumberOfRecordsInPrimaryArea++;  //ca³kowita liczba rekordów w primaryArea
		sortRecords(records, numberOfRecords); //sortowanie strony
		savePrimaryArea = true;

	}
	else //dodajê do overflow area
	{
		tie(pageNumber, positionNumber) = findPredecessor(*records, record);  //znajdŸ powi¹zanie i zapisz pointer
		if (AddToOverflowArea) { addRecordToOverflowArea(record, pageNumber, positionNumber); }
		else { AddToOverflowArea = true; }  //wstaw do overflowArea
	}
	if (savePrimaryArea)
	{
		der.seekp(position, ios::beg);
		der.write(reinterpret_cast<const char*>(records), sizeof(Record) * BLOCK_RATE);  // wpisanie odpowiedniej strony do primaryArea
		numberOfWrites++;
		//updateIndex(records[0].key, pageToRead);  //aktualizacja indeksu
		savePrimaryArea = false;
	}

	der.close();
	
}

//odczyt sekwencji rekordow
void File::readSequenceInOverflowArea(int page, int position) {

	
	int pos = (sizeOfPrimaryArea + page) * sizeof(Record) * BLOCK_RATE;
	readPage(pagedFile, pos, recordsOverflow);

	for (int i = 0; i < BLOCK_RATE; i++)
	{
		if (i == position && recordsOverflow[i].deleted == 0)
		{
			recordsOverflow[i].displayRecord();
		}

		if (i == position && recordsOverflow[i].pointerToPage != MY_NULL)
		{
			readSequenceInOverflowArea(recordsOverflow[i].pointerToPage, recordsOverflow[i].pointerToPosition);
			
			return;
		}

	}
	
}
void File::readSequence()
{
	
	int position = 0;
	cout << "Sequence of records: " << endl;

	for (int i = 0; i < sizeOfPrimaryArea; i++)  // przejœcie po stronach primary area
	{
		readPage(pagedFile, position, records);
		position += BLOCK_RATE * sizeof(Record);

		for (int k = 0; k < BLOCK_RATE; k++)
		{
			if (records[k].deleted == 0 && records[k].key != MY_NULL)
			{
				records[k].displayRecord();
			}
			if (records[k].pointerToPage != MY_NULL)  // jezeli jest wskaznik na overflow area
			{
				readSequenceInOverflowArea(records[k].pointerToPage, records[k].pointerToPosition);
			}
		}
		
	}
}

//odczyt rekordu
void File::readRecordInOverflowArea(int key, int page, int position) {

	
	int pos = (sizeOfPrimaryArea + page) * sizeof(Record) * BLOCK_RATE;
	readPage(pagedFile, pos, recordsOverflow);

	if (recordsOverflow[position].key == key && recordsOverflow[position].deleted == 0)
	{
		recordsOverflow[position].displayRecord();
		recordFound = true;
		return;
	}
	else if (recordsOverflow[position].pointerToPage != MY_NULL)
	{
		readRecordInOverflowArea(key, recordsOverflow[position].pointerToPage, recordsOverflow[position].pointerToPosition);
	}
	
}
void File::readRecord(int key)
{
	recordFound = false;

	if (key < 0)
	{
		cout << "info: Key is below zero. Invalid Record " << endl;
		return;
	}

	
	int pageToRead = getPageNumber(key);  // numer strony do odczytu
	int position = pageToRead * BLOCK_RATE * sizeof(Record);
	readPage(pagedFile, position, records);
	Record previousRecord = records[0];

	for (int i = 0; i < BLOCK_RATE; i++)  // przejœcie po stronie
	{
		if (records[i].key == key && records[i].deleted == 0)
		{
			recordFound = true;
			records[i].displayRecord();
			return;
		}
		else if (previousRecord.key < key && records[i].key > key && previousRecord.pointerToPage != MY_NULL)
		{
			readRecordInOverflowArea(key, previousRecord.pointerToPage, previousRecord.pointerToPosition);
		}
		previousRecord = records[i];
	}
	if (records[BLOCK_RATE - 1].key < key && records[BLOCK_RATE - 1].pointerToPage != MY_NULL)
	{
		readRecordInOverflowArea(key, records[BLOCK_RATE - 1].pointerToPage, records[BLOCK_RATE - 1].pointerToPosition);
	}

	if (!recordFound)
	{
		cout << "info: Record with the given key does not exist or has been deleted " << endl;
	}
	

}
//usuwanie rekordu 
void File::deleteRecordInOverflowArea(int key, int page, int position) {

	
	int pos = (sizeOfPrimaryArea + page) * sizeof(Record) * BLOCK_RATE;
	readPage(pagedFile, pos, recordsOverflow);

	if (recordsOverflow[position].key == key && recordsOverflow[position].deleted == 0)
	{
		recordsOverflow[position].tickDeleted();
		numberOfDeletedRecords++;
		recordFound = true;

		fstream fil(pagedFile, ios::binary | ios::in | ios::out);
		fil.seekp(pos, ios::beg);
		fil.write(reinterpret_cast<const char*>(recordsOverflow), sizeof(Record) * BLOCK_RATE);
		numberOfWrites++;
		fil.close();

		return;
	}
	else if (recordsOverflow[position].pointerToPage != MY_NULL)
	{
		deleteRecordInOverflowArea(key, recordsOverflow[position].pointerToPage, recordsOverflow[position].pointerToPosition);
	}
	
}
void File::deleteRecord(int key) {
	recordFound = false;

	if (key < 0)
	{
		cout << "info: Key is below zero. Invalid Record " << endl;
		return;
	}

	
	int pageToRead = getPageNumber(key);  // numer strony do odczytu
	int position = pageToRead * BLOCK_RATE * sizeof(Record);
	readPage(pagedFile, position, records);
	Record previousRecord = records[0];

	for (int i = 0; i < BLOCK_RATE; i++)  // przejœcie po stronie
	{
		if (records[i].key == key && records[i].deleted == 0)
		{
			recordFound = true;
			records[i].tickDeleted();
			numberOfDeletedRecords++;

			fstream fil(pagedFile, ios::binary | ios::in | ios::out);
			fil.seekp(position, ios::beg);
			fil.write(reinterpret_cast<const char*>(records), sizeof(Record) * BLOCK_RATE);
			numberOfWrites++;
			fil.close();

			return;
		}
		else if (previousRecord.key < key && records[i].key > key && previousRecord.pointerToPage != MY_NULL)
		{
			deleteRecordInOverflowArea(key, previousRecord.pointerToPage, previousRecord.pointerToPosition);
		}
		previousRecord = records[i];
	}
	if (records[BLOCK_RATE - 1].key < key && records[BLOCK_RATE - 1].pointerToPage != MY_NULL)
	{
		deleteRecordInOverflowArea(key, records[BLOCK_RATE - 1].pointerToPage, records[BLOCK_RATE - 1].pointerToPosition);
	}

	if (!recordFound)
	{
		cout << "info: Record with the given key does not exist or has been deleted " << endl;
	}
	
}
//aktualizacja rekordu

void File::updateRecordInOverflowArea(int key, int page, int position, Record* record) {

	
	int pos = (sizeOfPrimaryArea + page) * sizeof(Record) * BLOCK_RATE;
	readPage(pagedFile, pos, recordsOverflow);

	if (recordsOverflow[position].key == key && recordsOverflow[position].deleted == 0)
	{
		recordsOverflow[position].update(record);
		recordFound = true;

		fstream fil(pagedFile, ios::binary | ios::in | ios::out);
		fil.seekp(pos, ios::beg);
		fil.write(reinterpret_cast<const char*>(recordsOverflow), sizeof(Record) * BLOCK_RATE);
		numberOfWrites++;
		fil.close();

	
		return;
	}
	else if (recordsOverflow[position].pointerToPage != MY_NULL)
	{
		updateRecordInOverflowArea(key, recordsOverflow[position].pointerToPage, recordsOverflow[position].pointerToPosition, record);
	}
	
}
void File::updateRecord(int key, Record* record) {
	recordFound = false;

	
	int pageToRead = getPageNumber(key);  // numer strony do odczytu
	int position = pageToRead * BLOCK_RATE * sizeof(Record);
	readPage(pagedFile, position, records);
	Record previousRecord = records[0];

	for (int i = 0; i < BLOCK_RATE; i++)  // przejœcie po stronie
	{
		if (records[i].key == key && records[i].deleted == 0)
		{
			recordFound = true;
			records[i].update(record);

			fstream fil(pagedFile, ios::binary | ios::in | ios::out);
			fil.seekp(position, ios::beg);
			fil.write(reinterpret_cast<const char*>(records), sizeof(Record) * BLOCK_RATE);
			numberOfWrites++;
			fil.close();

			
			return;
		}
		else if (previousRecord.key < key && records[i].key > key && previousRecord.pointerToPage != MY_NULL)
		{
			updateRecordInOverflowArea(key, previousRecord.pointerToPage, previousRecord.pointerToPosition, record);
		}
		previousRecord = records[i];
	}
	if (records[BLOCK_RATE - 1].key < key && records[BLOCK_RATE - 1].pointerToPage != MY_NULL)
	{
		updateRecordInOverflowArea(key, records[BLOCK_RATE - 1].pointerToPage, records[BLOCK_RATE - 1].pointerToPosition, record);
	}

	if (!recordFound)
	{
		cout << "info: Record with the given key does not exist or has been deleted " << endl;
	}
	

}
void File::updateFunction(int key, Record* record) {

	if (key < 0)
	{
		cout << "info: Key is below zero. Invalid Record " << endl;
		return;
	}

	if (record->key == key)  // je¿eli bez zmiany klucza
	{
		updateRecord(key, record);
	}
	else  // ze zmian¹ klucza
	{
		readRecord(record->key);  // czy nowy klucz do wstawienia ju¿ istnieje
		if (!recordFound)  // jêsli nie, to próbuj aktualizowaæ
		{
			deleteRecord(key);
			if (recordFound) { addRecord(record); } // je¿eli rekord istnieje i zosta³ usuniety

		}
		else {  // jêsli taki klucz istnieje, to nie aktualizuj
			cout << "-info: Can't update record, because record with new given key already exists" << endl;
		}
		
	}

}

File::~File()
{
	delete[] records;
	delete[] recordsOverflow;
	delete[] recordsCop;
	delete[] recordsCopy;
	delete[] recordsFile;
	delete[] r;
	delete[] next;
	delete[] index;
	delete[] artificialBuffer;
	delete[] artificialIndexBuffer;
}