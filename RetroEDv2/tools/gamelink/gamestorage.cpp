#include "includes.hpp"

void initStorage(DataStorage *storages)
{
    if (!storages)
        return;

    // storage limits
    storages[DATASET_STG].storageLimit = 32 * 0x100000;
    storages[DATASET_STR].storageLimit = 2 * 0x100000;
    storages[DATASET_TMP].storageLimit = 8 * 0x100000;

    for (int s = 0; s < DATASET_MAX; ++s) {
        storages[s].memoryTable = (int *)malloc(storages[s].storageLimit);

        storages[s].usedStorage = 0;
        storages[s].entryCount  = 0;
        storages[s].clearCount  = 0;
    }
}

void releaseStorage(DataStorage *storages)
{
    if (!storages)
        return;

    for (int s = 0; s < DATASET_MAX; ++s) {
        if (storages[s].memoryTable)
            free(storages[s].memoryTable);

        storages[s].memoryTable = NULL;
        storages[s].usedStorage = 0;
        storages[s].entryCount  = 0;
        storages[s].clearCount  = 0;
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

    if ((uint)dataSet < DATASET_MAX) {
        DataStorage *storage = &storages[dataSet];

        if ((size & -4) < size)
            size = (size & -4) + sizeof(int);

        if (storage->entryCount < STORAGE_ENTRY_COUNT) {
            if (size + sizeof(int) * storage->usedStorage >= storage->storageLimit) {
                clearUnusedStorage(storages, dataSet);

                if (size + sizeof(int) * storage->usedStorage >= storage->storageLimit) {
                    if (storage->entryCount >= STORAGE_ENTRY_COUNT)
                        cleanEmptyStorage(storages, dataSet);

                    if (*data && clear)
                        memset(*data, 0, size);
                }
                else {
                    DataStorageHeader *entry =
                        (DataStorageHeader *)&storage->memoryTable[storage->usedStorage];

                    entry->active     = true;
                    entry->setID      = dataSet;
                    entry->dataOffset = storage->usedStorage + STORAGE_HEADER_SIZE;
                    entry->dataSize   = size;

                    storage->usedStorage += STORAGE_HEADER_SIZE;
                    *data = &storage->memoryTable[storage->usedStorage];
                    storage->usedStorage += size / sizeof(int);

                    storage->dataEntries[storage->entryCount]    = data;
                    storage->storageEntries[storage->entryCount] = *data;
                }
            }
            else {
                DataStorageHeader *entry =
                    (DataStorageHeader *)&storage->memoryTable[storage->usedStorage];

                entry->active     = true;
                entry->setID      = dataSet;
                entry->dataOffset = storage->usedStorage + STORAGE_HEADER_SIZE;
                entry->dataSize   = size;

                storage->usedStorage += STORAGE_HEADER_SIZE;
                *data = &storage->memoryTable[storage->usedStorage];
                storage->usedStorage += size / sizeof(int);

                storage->dataEntries[storage->entryCount]    = data;
                storage->storageEntries[storage->entryCount] = *data;
            }

            ++storage->entryCount;
            if (storage->entryCount >= STORAGE_ENTRY_COUNT)
                cleanEmptyStorage(storages, dataSet);

            if (*data && clear)
                memset(*data, 0, size);
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

            DataStorageHeader *entry = (DataStorageHeader *)(*data - STORAGE_HEADER_SIZE);
            int set                  = entry->setID;

            for (int e = 0; e < storages[set].entryCount; ++e) {
                if (data == storages[set].dataEntries[e])
                    storages[set].dataEntries[e] = NULL;
            }

            int newEntryCount = 0;
            for (int entryID = 0; entryID < storages[set].entryCount; ++entryID) {
                if (storages[set].dataEntries[entryID]) {
                    if (entryID != newEntryCount) {
                        storages[set].dataEntries[newEntryCount] = storages[set].dataEntries[entryID];
                        storages[set].storageEntries[newEntryCount] =
                            storages[set].storageEntries[entryID];
                        storages[set].dataEntries[entryID]    = NULL;
                        storages[set].storageEntries[entryID] = NULL;
                    }

                    newEntryCount++;
                }
            }
            storages[set].entryCount = newEntryCount;

            for (int e = storages[set].entryCount; e < STORAGE_ENTRY_COUNT; ++e) {
                storages[set].dataEntries[e]    = NULL;
                storages[set].storageEntries[e] = NULL;
            }

            entry->active = false;
        }
    }
}

void clearUnusedStorage(DataStorage *storages, StorageDataSets set)
{
    if (!storages)
        return;

    ++storages[set].clearCount;

    cleanEmptyStorage(storages, set);

    if (storages[set].usedStorage) {
        int curStorageSize = 0;
        int newStorageSize = 0;
        uint usedStorage   = 0;

        int *curMemTablePtr = storages[set].memoryTable;
        int *newMemTablePtr = storages[set].memoryTable;

        for (uint memPos = 0; memPos < storages[set].usedStorage;) {
            DataStorageHeader *curEntry = (DataStorageHeader *)curMemTablePtr;

            int size         = ((uint)curEntry->dataSize >> 2) + STORAGE_HEADER_SIZE;
            curEntry->active = false;

            int *dataPtr = &storages[set].memoryTable[curEntry->dataOffset];

            bool32 noCopy = true;
            if (storages[set].entryCount) {
                for (uint e = 0; e < storages[set].entryCount; ++e) {
                    if (dataPtr == storages[set].storageEntries[e])
                        curEntry->active = true;
                }

                if (curEntry->active) {
                    noCopy = false;

                    curStorageSize += size;
                    memPos = curStorageSize;

                    if (curMemTablePtr <= newMemTablePtr) {
                        newMemTablePtr += size;
                        curMemTablePtr += size;
                    }
                    else {
                        for (; size; --size) {
                            *newMemTablePtr++ = *curMemTablePtr++;
                        }
                    }

                    usedStorage = newStorageSize;
                }
            }

            if (noCopy) {
                curMemTablePtr += size;
                newStorageSize += size;
                curStorageSize += size;

                memPos      = curStorageSize;
                usedStorage = newStorageSize;
            }
        }

        if (usedStorage) {
            bool32 noEntriesRemoved = storages[set].usedStorage != usedStorage;
            storages[set].usedStorage -= usedStorage;

            int *memory = storages[set].memoryTable;
            if (noEntriesRemoved) {
                for (uint memPos = 0; memPos < storages[set].usedStorage;) {
                    DataStorageHeader *entry = (DataStorageHeader *)memory;

                    int *dataPtr = &storages[set].memoryTable[entry->dataOffset];
                    int size     = ((uint)entry->dataSize >> 2) + STORAGE_HEADER_SIZE; // size (in ints)

                    for (uint c = 0; c < storages[set].entryCount; ++c) {
                        if (dataPtr == storages[set].storageEntries[c]) {
                            *storages[set].dataEntries[c]   = memory + STORAGE_HEADER_SIZE;
                            storages[set].storageEntries[c] = memory + STORAGE_HEADER_SIZE;
                        }
                    }

                    entry->dataOffset = memPos + STORAGE_HEADER_SIZE;
                    memPos += size;
                    memory += size;
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

        DataStorageHeader *entry = (DataStorageHeader *)(dstPtr - 4);
        int setID                = entry->setID;

        if (storages[setID].entryCount < STORAGE_ENTRY_COUNT) {
            storages[setID].dataEntries[storages[setID].entryCount]    = src;
            storages[setID].storageEntries[storages[setID].entryCount] = *src;

            if (storages[setID].entryCount >= STORAGE_ENTRY_COUNT)
                cleanEmptyStorage(storages, (StorageDataSets)setID);
        }
    }
}

void cleanEmptyStorage(DataStorage *storages, StorageDataSets set)
{
    if (!storages)
        return;

    if ((uint)set < DATASET_MAX) {
        DataStorage *storage = &storages[set];

        for (uint e = 0; e < storage->entryCount; ++e) {
            // So what's happening here is the engine is checking to see if the storage entry
            // (which is the pointer to the "memoryTable" offset that is allocated for this entry)
            // matches what the actual variable that allocated the storage is currently pointing to.
            // if they don't match, the storage entry is considered invalid and marked for removal.

            if (storage->dataEntries[e] && *storage->dataEntries[e] != storage->storageEntries[e])
                storage->dataEntries[e] = NULL;
        }

        int newEntryCount = 0;
        for (uint entryID = 0; entryID < storage->entryCount; ++entryID) {
            if (storage->dataEntries[entryID]) {
                if (entryID != newEntryCount) {
                    storage->dataEntries[newEntryCount]    = storage->dataEntries[entryID];
                    storage->storageEntries[newEntryCount] = storage->storageEntries[entryID];
                    storage->dataEntries[entryID]          = NULL;
                    storage->storageEntries[entryID]       = NULL;
                }

                newEntryCount++;
            }
        }
        storage->entryCount = newEntryCount;

        for (int e = storage->entryCount; e < STORAGE_ENTRY_COUNT; ++e) {
            storage->dataEntries[e]    = NULL;
            storage->storageEntries[e] = NULL;
        }
    }
}
