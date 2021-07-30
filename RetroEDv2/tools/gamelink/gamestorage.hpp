#ifndef GAMESTORAGE_HPP
#define GAMESTORAGE_HPP

enum StorageDataSets {
    DATASET_STG,
    DATASET_STR,
    DATASET_TMP,
    DATASET_MAX, // used to signify limits
};

struct DataStorage {
    int *memPtr;
    uint usedStorage;
    uint storageLimit;
    int **startPtrs1[0x1000];
    int *startPtrs2[0x1000];
    uint entryCount;
    uint unknown;
};

void InitStorage(DataStorage *dataStorage);
void ReleaseStorage(DataStorage *dataStorage);

void AllocateStorage(DataStorage *dataStorage, uint size, void **dataPtr, StorageDataSets dataSet,
                     bool32 clear);
void ClearUnusedStorage(DataStorage *dataStorage, StorageDataSets set);
void RemoveStorageEntry(DataStorage *dataStorage, void **dataPtr);
void CopyStorage(DataStorage *dataStorage, int **src, int **dst);
void CleanEmptyStorage(DataStorage *dataStorage, StorageDataSets dataSet);

#endif // GAMESTORAGE_HPP
