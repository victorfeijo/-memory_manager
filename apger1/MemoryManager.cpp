/*
 * File:   MemoryManager.cpp
 * Authors: INSERT YOUR NAME HERE
 * Strategy: INSERT THE MEMORY MANAGEMENT SCHEMA HERE (THE ALGORITHM YOU ARE IMPLEMENTING -- RANDOMLY GENERATED BY MOODLE AS A VARIATION)
 *
 * Created on ...
 */

#include <iostream>
#include <queue>
#include "MemoryManager.h"

MemoryManager::MemoryManager(unsigned int size) {
    // do not change
    this->_size = size;
    // INSERT YOUR CODE TO INITIALIZE OTHER PRIVATE ATTRIBUTES
    // ...
    //PARTIÇÕES CRIADAS ESTÁTICAMENTE POR CRITÉRIO PRÓPRIO
    //6 PARTIÇÕES COM TAMANHO CRESCENTE

    unsigned int lastEnd = 0;
    unsigned int actualSize;
    for(int i = 0; i < NUMBER_PARTITIONS-1; i++) {
        actualSize = _size*(i+1)/21;
        _partitions[i] = new Partition(lastEnd, lastEnd+actualSize-1);
        lastEnd += actualSize;
    }
    _partitions[NUMBER_PARTITIONS] = new Partition(lastEnd, _size-1);

}

MemoryManager::MemoryManager(const MemoryManager& orig) {
}

MemoryManager::~MemoryManager() {
}

void MemoryManager::allocateMemoryForProcess(Process* process) {
    // INSERT YOUR CODE TO ALLOCATE MEMOTY FOR THE PROCESS
    // ...
    unsigned int begin = process->getBeginMemory();
    unsigned int end = process->getEndMemory();
    unsigned int processSize = end - begin;
    for (int i = 0; i < NUMBER_PARTITIONS; i++) {
        if (_partitions[i]->getLength() > processSize) {
            if(!(_partitions[i]->hasProcess())) {
                _partitions[i]->setProcess(process);
            } else {
                _partitions[i]->getProcessQueue().push(process);
            }
        }
        else {
            throw "não foi possível alocar";
        }
    }

}

void MemoryManager::deallocateMemoryOfProcess(Process* process) {
    // INSERT YOUR CODE TO DEALLOCATE MEMORY OF THE PROCESS
    // ...


}

unsigned intMemoryManager::getNumPartitions() {
    // INSERT YOUR CODE TO RETURN THE QUANTITY OF ALLOCATED PARTITIONS
    // ...

    return NUMBER_PARTITIONS; // CHANGE TO THE CORRET RETURN VALUE
}

Partition* MemoryManager::getPartition(unsigned int index) {
    // INSERT YOUR CODE TO RETURN THE PARTITION AT INDEX index
    // ...

    return _partitions[index];  // CHANGE TO THE CORRET RETURN VALUE
}


void MemoryManager::showAllocatedMemory() {
    unsigned int beginAllocatedAddress, endAllocatedAddress, totalAllocated, processId, internalFragmentation, externalFragmentation;
    // INSERT YOUR CODE TO SHOW EACH ONE OF THE ALLOCATED MEMORY PARTITIONS, INCLUDING INTERNAL AND EXTERNAL (THE FOLLOWING) FRAGMENTATION
    // ...

    // for ... {  // for each partition...
    // ...

    beginAllocatedAddress = 0;
    endAllocatedAddress = 0;
    totalAllocated = 0;
    processId = 0;
    internalFragmentation = 0;
    externalFragmentation = 0;

    // no not change the next line (the way information are shown)
    std::cout << "\tAllocd: " << "B=" << (beginAllocatedAddress) << ", \tE=" << (endAllocatedAddress) << ", \tT=" << (totalAllocated) << ", \tPID=" << (processId)
            << ", \tIF=" << (internalFragmentation) << ", \tEF=" << (externalFragmentation) << "\n";
    // }
}

unsigned int MemoryManager::getSize() const {
    return _size;
}
