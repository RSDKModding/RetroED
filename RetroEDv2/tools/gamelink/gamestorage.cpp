#include "include.hpp"

void initStorage(DataStorage *storages)
{
    if (!storages)
        return;

    // storage limits
    storages[DATASET_STG].storageLimit = 0x1800000;
    storages[DATASET_STR].storageLimit = 0x200000;
    storages[DATASET_TMP].storageLimit = 0x800000;

    for (int s = 0; s < DATASET_MAX; ++s) {
        storages[s].memPtr      = (int *)malloc(storages[s].storageLimit);
        storages[s].usedStorage = 0;
        storages[s].entryCount  = 0;
        storages[s].unknown     = 0;
    }
}

void releaseStorage(DataStorage *storages)
{
    if (!storages)
        return;

    for (int s = 0; s < DATASET_MAX; ++s) {
        if (storages[s].memPtr)
            free(storages[s].memPtr);
        storages[s].memPtr      = NULL;
        storages[s].usedStorage = 0;
        storages[s].entryCount  = 0;
        storages[s].unknown     = 0;
    }
}

// MemPtr format:
// active
// data set
// start Offset
// size
// [storage bytes]

void allocateStorage(DataStorage *storages, uint size, void **dataPtr, StorageDataSets dataSet,
                     bool32 clear)
{
    if (!storages)
        return;

    int **data = (int **)dataPtr;
    *data      = NULL;

    if (dataSet < DATASET_MAX) {
        DataStorage *storage = &storages[dataSet];

        if ((size & -4) < size)
            size = (size & -4) + sizeof(int);

        if (storage->entryCount < 0x1000) {
            if (size + sizeof(int) * storage->usedStorage >= storage->storageLimit) {
                clearUnusedStorage(storages, dataSet);

                if (size + sizeof(int) * storage->usedStorage >= storage->storageLimit) {
                    if (storage->entryCount >= 0x1000)
                        cleanEmptyStorage(storages, dataSet);

                    if (*data && clear) {
                        memset(*data, 0, size);
                    }
                }
                else {
                    storage->memPtr[storage->usedStorage++] = true;
                    storage->memPtr[storage->usedStorage++] = dataSet;

                    storage->memPtr[storage->usedStorage++] = storage->usedStorage + 2;
                    storage->memPtr[storage->usedStorage++] = size;
                    *data                                   = &storage->memPtr[storage->usedStorage];
                    storage->usedStorage += size / sizeof(int);
                    storage->startPtrs1[storage->entryCount] = data;
                    storage->startPtrs2[storage->entryCount] = *data;
                }
            }
            else {
                storage->memPtr[storage->usedStorage++] = true;
                storage->memPtr[storage->usedStorage++] = dataSet;
                storage->memPtr[storage->usedStorage++] = storage->usedStorage + 2;
                storage->memPtr[storage->usedStorage++] = size;
                *data                                   = &storage->memPtr[storage->usedStorage];
                storage->usedStorage += size / sizeof(int);
                storage->startPtrs1[storage->entryCount] = data;
                storage->startPtrs2[storage->entryCount] = *data;
            }
            ++storage->entryCount;
            if (storage->entryCount >= 0x1000)
                cleanEmptyStorage(storages, dataSet);

            if (*data && clear) {
                memset(*data, 0, size);
            }
        }
    }
}

void removeStorageEntry(DataStorage *storages, void **dataPtr)
{
    if (!storages)
        return;

    if (dataPtr) {
        if (*dataPtr) {
            int **data = (int **)dataPtr;
            int *ptr   = *data;

            int set = *(ptr - 3);

            for (uint e = 0; e < storages[set].entryCount; ++e) {
                if (data == storages[set].startPtrs1[e]) {
                    storages[set].startPtrs1[e] = NULL;
                }
            }

            uint c = 0;
            for (uint e = 0; e < storages[set].entryCount; ++e) {
                if (storages[set].startPtrs1[e]) {
                    if (e != c) {
                        storages[set].startPtrs1[c] = storages[set].startPtrs1[e];
                        storages[set].startPtrs2[c] = storages[set].startPtrs2[e];
                        storages[set].startPtrs1[e] = NULL;
                        storages[set].startPtrs2[e] = NULL;
                    }
                    c++;
                }
            }
            storages[set].entryCount = c;

            for (int e = storages[set].entryCount; e < 0x1000; ++e) {
                storages[set].startPtrs1[e] = NULL;
                storages[set].startPtrs2[e] = NULL;
            }
            *(ptr - 4) = false;
        }
    }
}

void clearUnusedStorage(DataStorage *storages, StorageDataSets set)
{
    if (!storages)
        return;

    ++storages[set].unknown;
    cleanEmptyStorage(storages, set);

    if (storages[set].usedStorage) {
        int totalSizeA  = 0;
        int totalSizeB  = 0;
        int usedStorage = 0;
        int *memPtr2    = storages[set].memPtr;
        int *memPtr     = storages[set].memPtr;

        for (uint c = 0; c < storages[set].usedStorage;) {
            int startOffset = memPtr2[2];
            int size        = ((uint)memPtr2[3] >> 2) + 4;
            *memPtr2        = false;
            int *dataPtr    = &storages[set].memPtr[startOffset];

            if (!storages[set].entryCount) {
                memPtr2 += size;
                c           = size + totalSizeA;
                usedStorage = size + totalSizeA;
                totalSizeB += size;
                totalSizeA += size;
            }
            else {
                for (uint e = 0; e < storages[set].entryCount; ++e) {
                    if (dataPtr == storages[set].startPtrs2[e])
                        *memPtr2 = true;
                }

                if (*memPtr2) {
                    c = size + totalSizeA;
                    totalSizeA += size;
                    if (memPtr2 <= memPtr) {
                        memPtr += size;
                        memPtr2 += size;
                    }
                    else {
                        for (; size; --size) {
                            int store = *memPtr2;
                            ++memPtr2;
                            *memPtr = store;
                            ++memPtr;
                        }
                    }
                    usedStorage = totalSizeB;
                }
                else {
                    memPtr2 += size;
                    c           = size + totalSizeA;
                    usedStorage = size + totalSizeB;
                    totalSizeB += size;
                    totalSizeA += size;
                }
            }
        }

        if (usedStorage) {
            bool32 flag = storages[set].usedStorage == (uint)usedStorage;
            storages[set].usedStorage -= usedStorage;
            int *memPtr = storages[set].memPtr;

            if (!flag) {
                for (uint offset = 0; offset < storages[set].usedStorage;) {
                    int *ptr = &storages[set].memPtr[memPtr[2]];
                    int size = ((uint)memPtr[3] >> 2) + 4;

                    for (uint c = 0; c < storages[set].entryCount; ++c) {
                        if (ptr == storages[set].startPtrs2[c]) {
                            *storages[set].startPtrs1[c] = memPtr + 4;
                            storages[set].startPtrs2[c]  = memPtr + 4;
                        }
                    }

                    memPtr[2] = offset + 4; // offset
                    offset += size;
                    memPtr += size;
                }
            }
        }
    }
}

void copyStorage(DataStorage *storages, int **src, int **dst)
{
    if (!storages)
        return;

    if (src) {
        int *dstPtr = *dst;
        *src        = *dst;
        int dstSet  = dstPtr[-3];

        if (storages[dstSet].entryCount < 0x1000) {
            storages[dstSet].startPtrs1[storages[dstSet].entryCount] = src;
            storages[dstSet].startPtrs2[storages[dstSet].entryCount] = *src;

            dstSet = dstPtr[-3];
            if (storages[dstSet].entryCount >= 0x1000)
                cleanEmptyStorage(storages, (StorageDataSets)dstSet);
        }
    }
}

void cleanEmptyStorage(DataStorage *storages, StorageDataSets set)
{
    if (!storages)
        return;

    if (set < DATASET_MAX) {
        DataStorage *storage = &storages[set];

        for (uint e = 0; e < storage->entryCount; ++e) {
            if (storage->startPtrs1[e] && *storage->startPtrs1[e] != storage->startPtrs2[e])
                storage->startPtrs1[e] = NULL;
        }

        uint c = 0;
        for (uint e = 0; e < storage->entryCount; ++e) {
            if (storage->startPtrs1[e]) {
                if (e != c) {
                    storage->startPtrs1[c] = storage->startPtrs1[e];
                    storage->startPtrs2[c] = storage->startPtrs2[e];
                    storage->startPtrs1[e] = NULL;
                    storage->startPtrs2[e] = NULL;
                }
                c++;
            }
        }
        storage->entryCount = c;

        for (int e = storage->entryCount; e < 0x1000; ++e) {
            storage->startPtrs1[e] = NULL;
            storage->startPtrs2[e] = NULL;
        }
    }
}
