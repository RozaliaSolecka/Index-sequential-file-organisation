#pragma once
#define ALFA 0.5  //zape³nienie stron do po³owy  0.5
#define BETA 0.2  // wspó³czynnik v/n kiedy robiæ reorganizacjê  0.2
#define BLOCK_RATE 4
#define INDEX_BLOCK_RATE 20
#define MY_NULL -2
#define VN_RATIO 0.2  // wspó³czynnik na liczbê stron w overflow area po reorganizacji
//extern int sizeOfPrimaryArea; // liczba stron w Obszarze G³ównym
//extern int sizeOfOverflowArea; // liczba stron w Obszarze Nadmiarowym
extern const char* pagedFile;
extern const char* indexFile;
extern const char* fileCopy;

extern int totalNumberOfRecordsInPrimaryArea; //liczba rekordów w primary area
extern int totalNumberOfRecordsInOverflowArea;  //liczba rekordów w overflow area
extern int numberOfReads;  //liczba odczytów stron dyskowych
extern int numberOfWrites;  //liczba zapisow stron dyskowych
extern bool savePrimaryArea;
extern bool recordFound;
extern int numberOfDeletedRecords;
extern bool AddToOverflowArea;  //zmienna ustawiana, je¿eli o ovweflow istnieje ju¿ rekord o danym kluczu 

	// eksperyment
extern int ad, up, del, read, reo, seq, ind;
extern int ad_n , up_n , del_n , read_n , reo_n, seq_n, ind_n ;

