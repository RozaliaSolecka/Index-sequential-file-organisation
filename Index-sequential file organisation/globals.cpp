#pragma once
#include"globals.h"
const char* pagedFile = "pagedFile.bin";
const char* indexFile = "indexFile.bin";
const char* fileCopy = "fileCopy.bin";

int totalNumberOfRecordsInPrimaryArea = 1;
int totalNumberOfRecordsInOverflowArea = 0;
int numberOfReads = 0;  
extern int numberOfWrites = 0;
bool savePrimaryArea = false;
bool recordFound = false;
int numberOfDeletedRecords = 0;
bool AddToOverflowArea = true;


// eksperyment
int ad = 0, up = 0, del = 0, read = 0, reo = 0, seq = 0, ind = 0;
int ad_n = 0, up_n = 0, del_n = 0, read_n = 0, reo_n = 0, seq_n = 0, ind_n = 0;