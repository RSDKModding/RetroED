#pragma once

#define STORAGE_ENTRY_COUNT (0x1000)
#define STORAGE_HEADER_SIZE (sizeof(DataStorageHeader) / sizeof(int))

enum StorageDataSets {
    DATASET_STG,
    DATASET_STR,
    DATASET_TMP,
    DATASET_MAX, // used to signify limits
};

struct DataStorage {
    int *memoryTable;
    uint usedStorage;
    uint storageLimit;
    int **dataEntries[STORAGE_ENTRY_COUNT];   // pointer to the actual variable
    int *storageEntries[STORAGE_ENTRY_COUNT]; // pointer to the storage in "memoryTable"
    uint entryCount;
    uint clearCount;
};

struct DataStorageHeader {
    bool32 active;
    int setID;
    int dataOffset;
    int dataSize;
    // there are "dataSize" int's following this header, but they are omitted from the struct cuz they
    // don't need to be here
};

void initStorage(DataStorage *dataStorage);
void releaseStorage(DataStorage *dataStorage);

void allocateStorage(DataStorage *dataStorage, uint size, void **dataPtr, StorageDataSets dataSet,
                     bool32 clear);
void clearUnusedStorage(DataStorage *dataStorage, StorageDataSets set);
void removeStorageEntry(DataStorage *dataStorage, void **dataPtr);
void copyStorage(DataStorage *dataStorage, int **src, int **dst);
void cleanEmptyStorage(DataStorage *dataStorage, StorageDataSets dataSet);


