#include<iostream>
#include<fstream>
#include"Record.h"
#include"File.h"
#include"Index.h"
#include"globals.h"
using namespace std;

void readFromFile(File *file)
{
	int choice;
	bool esc = true;
	int a0, a1, a2, a3, a4, x, key;
	Record* record;
	char operation;

	numberOfReads = 0;
	numberOfWrites = 0;

	string fileName;
	cout << "Enter the name of the test file " << endl;
	cin >> fileName;

	ifstream test(fileName, ios::in);

	while (esc)
	{
		test >> operation;

		switch (operation)
		{
		case 'a':
			cout << " a - add record ";
			test >> a0 >> a1 >> a2 >> a3 >> a4 >> x;
			record = new Record(a0, a1, a2, a3, a4, x);
			record->displayRecord();
			file->addRecord(record);
			cout << "info: READS: " << numberOfReads << " WRITES: " << numberOfWrites << endl << endl;
			ad_n++;
			ad += numberOfReads + numberOfWrites;
			numberOfReads = 0;
			numberOfWrites = 0;
			delete record;
			break;
		case 'u':
			cout << " u - update record ";
			test >> key;
			test >> a0 >> a1 >> a2 >> a3 >> a4 >> x;
			record = new Record(a0, a1, a2, a3, a4, x);
			file->updateFunction(key, record);
			cout << "info: READS: " << numberOfReads << " WRITES: " << numberOfWrites << endl << endl;
			up_n++;
			up += numberOfReads + numberOfWrites;
			numberOfReads = 0;
			numberOfWrites = 0;
			delete record;
			break;
		case 'r':
			cout << " r - read record ";
			test >> key;
			cout << "key: " << key << endl;
			file->readRecord(key);
			cout << "info: READS: " << numberOfReads << " WRITES: " << numberOfWrites << endl << endl;
			read_n++;
			read += numberOfReads + numberOfWrites;
			numberOfReads = 0;
			numberOfWrites = 0;
			break;
		case 'd':
			cout << " d - delete record " << endl;
			test >> key;
			file->deleteRecord(key);
			cout << "info: READS: " << numberOfReads << " WRITES: " << numberOfWrites << endl << endl;
			del_n++;
			del += numberOfReads + numberOfWrites;
			numberOfReads = 0;
			numberOfWrites = 0;
			break;
		case 'f':
			cout << " f - read file " << endl;
			file->readFile(pagedFile);
			cout << "info: READS: " << numberOfReads << " WRITES: " << numberOfWrites << endl << endl;
			numberOfReads = 0;
			numberOfWrites = 0;
			break;
		case 'i':
			cout << " i - read index " << endl;
			file->readIndex();
			cout << "info: READS: " << numberOfReads << " WRITES: " << numberOfWrites << endl << endl;
			ind_n++;
			ind += numberOfReads + numberOfWrites;
			numberOfReads = 0;
			numberOfWrites = 0;
			break;
		case 's':
			cout << " s - sequence of records " << endl;
			file->readSequence();
			cout << "info: READS: " << numberOfReads << " WRITES: " << numberOfWrites << endl << endl;
			seq_n++;
			seq += numberOfReads + numberOfWrites;
			numberOfReads = 0;
			numberOfWrites = 0;
			break;
		case 'o':
			cout << " o - reorganise " << endl;
			file->reorganise();
			cout << "info: READS: " << numberOfReads << " WRITES: " << numberOfWrites << endl << endl;
			numberOfReads = 0;
			numberOfWrites = 0;
			break;
		case 'e':
			cout << " e - end " << endl;
			esc = false;
			break;
		}
	}
	/*
	cout << "EKSPERYMENT average disk operation:" << endl;
	cout << " add " << ad / ad_n <<endl;
	cout << " delete " << del / del_n << endl;
	cout << " update " << up / up_n << endl;
	cout << " read Record " << read / read_n <<endl;
	cout << " reorganise " << reo / reo_n << endl;
	cout << " sequence " << seq / seq_n << endl;
	cout << " index " << ind / ind_n << endl << endl;
	*/
	
	test.close();
}
int main()
{	
	int choice;
	bool esc = true;
	int a0, a1, a2, a3, a4, x, key;
	Record* record;
	char operation;

	File* file = new File(pagedFile, 1, 1);
	file->initialValues();
	file->createIndex();
	numberOfReads = 0;
	numberOfWrites = 0;
	
	cout << "Choose option " << endl;
	cout << "1. Enter data form keyboard" << endl;
	cout << "2. Read data form file " << endl;
	cin >> choice;

	if (choice == 2)  // z pliku
	{
		readFromFile(file);
	}
	else   // z klawiatury
	{
		while (esc)
		{
			cout << "Choose operation " << endl;
			cout << " a - add record " << endl;
			cout << " u - update record " << endl;
			cout << " r - read record " << endl;
			cout << " d - delete record " << endl;
			cout << " f - read file " << endl;
			cout << " i - read index " << endl;
			cout << " s - sequence of records " << endl;
			cout << " o - reorganise " << endl;
			cout << " t - read from test file " << endl;
			cout << " e - end " << endl;
			cin >> operation;

			switch (operation)
			{
			case 'a':
				cout << "Enter Record (a0 a1 a2 a3 a4 x)" << endl;
				cin >> a0 >> a1 >> a2 >> a3 >> a4 >> x;
				record = new Record(a0, a1, a2, a3, a4, x);
				file->addRecord(record);
				cout << "info: READS: " << numberOfReads << " WRITES: " << numberOfWrites << endl << endl;
				numberOfReads = 0;
				numberOfWrites = 0;
				delete record;
				break;
			case 'u':
				cout << "Enter the key of the Record to be changed "<< endl;
				cin >> key;
				cout << "Enter new Record(a0 a1 a2 a3 a4 x) " << endl;
				cin >> a0 >> a1 >> a2 >> a3 >> a4 >> x;
				record = new Record(a0, a1, a2, a3, a4, x);
				file->updateFunction(key, record);
				cout << "info: READS: " << numberOfReads << " WRITES: " << numberOfWrites << endl << endl;
				numberOfReads = 0;
				numberOfWrites = 0;
				delete record;
				break;
			case 'r':
				cout << "Enter the key" << endl;
				cin >> key;
				file->readRecord(key);
				cout << "info: READS: " << numberOfReads << " WRITES: " << numberOfWrites << endl << endl;
				numberOfReads = 0;
				numberOfWrites = 0;
				break;
			case 'd':
				cout << "Enter the key" << endl;
				cin >> key;
				file->deleteRecord(key);
				cout << "info: READS: " << numberOfReads << " WRITES: " << numberOfWrites << endl << endl;
				numberOfReads = 0;
				numberOfWrites = 0;
				break;
			case 'f':
				file->readFile(pagedFile);
				cout << "info: READS: " << numberOfReads << " WRITES: " << numberOfWrites << endl << endl;
				numberOfReads = 0;
				numberOfWrites = 0;
				break;
			case 'i':
				file->readIndex();
				cout << "info: READS: " << numberOfReads << " WRITES: " << numberOfWrites << endl << endl;
				numberOfReads = 0;
				numberOfWrites = 0;
				break;
			case 's':
				file->readSequence();
				cout << "info: READS: " << numberOfReads << " WRITES: " << numberOfWrites << endl << endl;
				numberOfReads = 0;
				numberOfWrites = 0;
				break;
			case 'o':
				file->reorganise();
				cout << "info: READS: " << numberOfReads << " WRITES: " << numberOfWrites << endl << endl;
				numberOfReads = 0;
				numberOfWrites = 0;
				break;
			case 't':
				readFromFile(file);
				break;
			case 'e':
				esc = false;
				break;
			}
		}
		
	}
	// EKSPERYMENT 
	/*
	ifstream mainFile("pagedFile.bin", ios::beg | ios::binary);
	mainFile.seekg(0, ios::end);
	cout << "Size of main file in bytes: " << mainFile.tellg() << endl;
	mainFile.close();

	ifstream indexFil("indexFile.bin", ios::beg | ios::binary);
	indexFil.seekg(0, ios::end);
	cout << "Size of index file in bytes: " << indexFil.tellg() << endl;
	indexFil.close();
	*/


	file->clearFile(fileCopy);
	file->clearFile(pagedFile);
	file->clearFile(indexFile);

	system("pause");
	return 0;
}