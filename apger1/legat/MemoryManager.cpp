/*
 * File:   MemoryManager.cpp
 * Authors: INSERT YOUR NAME HERE
 * Strategy: Part.fixas:UmaFilaPorParticao
 * Created on september 21, 2015 
 */

#include <iostream>
#include "MemoryManager.h"

MemoryManager::MemoryManager(unsigned int size) { 
    // do not change
    this->_size = size;
    // INSERT YOUR CODE TO INITIALIZE OTHER PRIVATE ATTRIBUTES
    createFixedPartitions(NUMBER_PARTITIONS);
}

MemoryManager::MemoryManager(const MemoryManager& orig) {
}

MemoryManager::~MemoryManager() {
}

void MemoryManager::allocateMemoryForProcess(Process* process) {
    // INSERT YOUR CODE TO ALLOCATE MEMOTY FOR THE PROCESS
    for (unsigned int index = 0; index < NUMBER_PARTITIONS; index++) {
        Partition *partition = getPartition(index);
        if (functions::fits(process, partition)) {
            partition->allocateProcess(process);
            return;
        }
    }
    std::cout << "\nPROCESS DOESN'T FIT IN ANY PARTITION\n";
    //throw 0;
}

void MemoryManager::deallocateMemoryOfProcess(Process* process) {
    // INSERT YOUR CODE TO DEALLOCATE MEMORY OF THE PROCESS
    for (unsigned int index = 0; index < NUMBER_PARTITIONS; index++) {
        Partition *partition = getPartition(index);
        if (partition->hasProcess()) {
            if (partition->getProcess()->getId() == process->getId()) {
                partition->deallocateProcess();
                return;
            }
        }
    }

}

unsigned int MemoryManager::getNumPartitions() {
    return NUMBER_PARTITIONS;
}

Partition* MemoryManager::getPartition(unsigned int index) {
    return _partitions[index];
}


void MemoryManager::showAllocatedMemory() {
    unsigned int beginAllocatedAddress, endAllocatedAddress, totalAllocated, processId, internalFragmentation, externalFragmentation;
    // INSERT YOUR CODE TO SHOW EACH ONE OF THE ALLOCATED MEMORY PARTITIONS, INCLUDING INTERNAL AND EXTERNAL (THE FOLLOWING) FRAGMENTATION
    for (int index = 0; index < NUMBER_PARTITIONS; index++) {  // for each partition...  
        Partition* partition = getPartition(index);
        beginAllocatedAddress = partition->getBeginAddress();
        endAllocatedAddress = partition->getEndAddress();
        totalAllocated = partition->getLength();
        externalFragmentation = 0;
        if (partition->hasProcess()) {
            Process *process = partition->getProcess();
            processId = process->getId();
            internalFragmentation = totalAllocated - functions::getProcessSize(process);
        } else {
            internalFragmentation = totalAllocated;
            processId = 0;
        }
        
        // no not change the next line (the way information are shown)
        std::cout << "\tAllocd: " << "B=" << (beginAllocatedAddress) << ", \tE=" << (endAllocatedAddress) 
                << ", \tT=" << (totalAllocated) << ", \tPID=" << (processId)
                << ", \tIF=" << (internalFragmentation) << ", \tEF=" << (externalFragmentation) << "\n";
    }
    
    beginAllocatedAddress = 0;
    endAllocatedAddress = 0;
    totalAllocated = 0;
    processId = 0;
    internalFragmentation = 0;
    externalFragmentation = 0;
}

unsigned int MemoryManager::getSize() const {
    return _size;
}

void MemoryManager::createFixedPartitions(unsigned int number) {
    // Create fixed partitions of sequencial increasing sizes using fractions
    unsigned int divisor = calculateDivisor(number);
    unsigned int actualStart = 0;
    for (unsigned int index = 0; index < number-1; index++) {
        unsigned int actualSize = (int)(_size * ((double)index+1) / (double)divisor);
        if (index == 0)
            actualSize = (int)(actualSize * 1.6);
        if (index == 1)
            actualSize = (int)(actualSize * 0.94);
        if (index == 2)
            actualSize = (int)(actualSize * 0.75);
        _partitions[index] = new Partition(actualStart, actualStart+actualSize-1);
        actualStart += actualSize;
    }
    // The last partition ends at the end of the memory,
    // ignoring losses caused by integer divisions
    _partitions[number-1] = new Partition(actualStart, _size-1);
}

unsigned int MemoryManager::calculateDivisor(unsigned int number) {
    unsigned int sum = 0;
    for (unsigned int k = 1; k <= number; k++) {
        sum += k;
    }
    return sum;
}


Partition::Partition(unsigned int begin, unsigned int end) {
    _beginAddress = begin;
    _endAddress = end;
}

bool Partition::hasProcess() {
    if (_process == NULL)
        return false;
    else
        return true;
}

void Partition::setProcess(Process *process) {
    _process = process;
}

Process* Partition::getProcess() {
    return _process;
}

void Partition::allocateProcess(Process *process) {
    if (hasProcess()) {
        _waitingQueue.push(process);
    } else {
        process->setBeginMemory(_beginAddress);
        process->setEndMemory(_beginAddress + functions::getProcessSize(process) - 1);
        _process = process;
    }
}

void Partition::deallocateProcess() {
    //delete _process;
    if (_waitingQueue.empty()) {
        _process = NULL;
    } else {
        _process = _waitingQueue.front();
        _waitingQueue.pop();
    }
}


bool functions::fits(Process *process, Partition *partition) {
    unsigned int processSize = getProcessSize(process);
    unsigned int partitionSize = partition->getLength();
    return (processSize <= partitionSize);
}

unsigned int functions::getProcessSize(Process *process) {
    std::list<MemorySegment*> *segList = process->getSegments();
    std::list<MemorySegment*>::iterator iterator;
    unsigned int size = 0;
    for (iterator = segList->begin(); iterator != segList->end(); iterator++)
        size += (*iterator)->getSize();
    return size;
}
