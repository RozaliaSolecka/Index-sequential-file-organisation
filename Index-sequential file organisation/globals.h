#pragma once
#define ALFA 0.5  //zape�nienie stron do po�owy  0.5
#define BETA 0.2  // wsp�czynnik v/n kiedy robi� reorganizacj�  0.2
#define BLOCK_RATE 4
#define INDEX_BLOCK_RATE 20
#define MY_NULL -2
#define VN_RATIO 0.2  // wsp�czynnik na liczb� stron w overflow area po reorganizacji
//extern int sizeOfPrimaryArea; // liczba stron w Obszarze G��wnym
//extern int sizeOfOverflowArea; // liczba stron w Obszarze Nadmiarowym
extern const char* pagedFile;
extern const char* indexFile;
extern const char* fileCopy;

extern int totalNumberOfRecordsInPrimaryArea; //liczba rekord�w w primary area
extern int totalNumberOfRecordsInOverflowArea;  //liczba rekord�w w overflow area
extern int numberOfReads;  //liczba odczyt�w stron dyskowych
extern int numberOfWrites;  //liczba zapisow stron dyskowych
extern bool savePrimaryArea;
extern bool recordFound;
extern int numberOfDeletedRecords;
extern bool AddToOverflowArea;  //zmienna ustawiana, je�eli o ovweflow istnieje ju� rekord o danym kluczu 

	// eksperyment
extern int ad, up, del, read, reo, seq, ind;
extern int ad_n , up_n , del_n , read_n , reo_n, seq_n, ind_n ;

