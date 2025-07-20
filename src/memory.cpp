#include "memory.h"


Cache::Cache(long size, int lineSize, int associativity, int hitTime)
    : size(size), lineSize(lineSize), associativity(associativity), hitTime(hitTime), TAG(associativity, vector<long>(size / (lineSize * associativity), 0)), V(associativity, vector<bool>(size / (lineSize * associativity), false)), W(associativity, vector<bool>(size / (lineSize * associativity), false)) {
}

Cache::~Cache() {}

Memory::Memory(int lineSize)
    : L1(L1_CACHE_SIZE, lineSize, 4, 1), L2(L2_CACHE_SIZE, 64, 8, 10), cycles(0) {
}

Memory::~Memory() {}

cacheResType Memory::sim_level(Cache &cache, long addr, int storeCycles, bool write, int level) {
    int offsetbits = log2(cache.lineSize);
    int numLines = cache.size / (cache.lineSize * cache.associativity);
    //cout << "num of lines: "<< numLines << endl;
    int linebits = log2(numLines);

    //split into components
    long offset = addr & ((1 << offsetbits) - 1);
    //cout << "Offset: " << offset << "\tbits: " << offsetbits << endl;

    long line = (addr >> offsetbits) & ((1 << linebits) - 1);
    //cout << "line: " << line << endl << "bits: " << linebits << endl;

    long tag = addr >> (offsetbits + linebits);
    //cout << "tag: " << tag << endl;

    //printLine(cache, line); // Print the current state of the cache line

    //check line if valid
    for (int i = 0; i < cache.associativity; ++i) {
        if (cache.V[i][line]) {                    // If the line is valid
            if (cache.TAG[i][line] == tag) {       // If the tag matches
                if (write) 
                    cache.W[i][line] = true;        // Mark as written if it's a write operation  
                //cout << "Cache Hit: Line " << line << " with tag " << tag << endl;
                return cacheResType::HIT;
            }
        }
    }

    //no hit -> check for invalid 
    for (int i = 0; i < cache.associativity; ++i) {
        if (!cache.V[i][line]) {           // If the line is not valid
            cache.TAG[i][line] = tag;      // set tag
            cache.V[i][line] = 1;          // mark as valid
            if (write) 
                cache.W[i][line] = true;
            //cout << "Cache Miss: Line " << line << " with tag " << tag << endl;
            return cacheResType::MISS; 
        }
    }

    //all valid -> Capacity cacheResType::MISS/Conflict cacheResType::MISS
    int ind = rand() % cache.associativity;                  // rand select to replace
    cout << "rand = " << ind << endl;
    if (cache.W[ind][line]){  // If the line is dirty, we need to write it back
        cycles += storeCycles; // Add cycles for writing back
        if (level == 1){ 
            long oldAddr = (cache.TAG[ind][line] << (linebits + offsetbits)) | (line << offsetbits); // reconstruct old addres
            sim_level(L2, oldAddr, 50, true, 2); // Write back to L2
        }
    }

    cache.TAG[ind][line] = tag;          // set tag
    cache.V[ind][line] = 1;                // mark as valid
    if (write) 
        cache.W[ind][line] = true;
    else
        cache.W[ind][line] = false; 
    //cout << "Cache Miss: Line " << line << " set with tag " << tag << endl;
    return cacheResType::MISS; 
}

void Memory::printLine (Cache &cache, int line){
    cout << "Cache State:" << endl;
    for (int i = 0; i < cache.associativity; ++i) {
        cout << "Set " << i << ": ";
        if (cache.V[i][line])
            cout << cache.TAG[i][line] << endl;   
        else
            cout << "Invalid" << endl;         
    }
}

cacheResType Memory::simulate(long addr, bool write) {
    cacheResType res = sim_level(L1, addr, 10, write, 1); // Simulate L1 cache
    if (res == HIT){
        cycles += 1; // Add cycles for L1 hit
        cout << "\t\t\t\t found at L1"<<endl;
    }
    else {
        res = sim_level(L2, addr, 50, write, 2); // Simulate L2 cache
        if (res == HIT) {
            cycles += 10; // Add cycles for L2 hit
                    cout << "\t\t\t\t found at L2"<<endl;

        } else {
            cycles += 100; // Add cycles for DRAM access
            cout << "\t\t\t\t found at DRAM"<<endl;

        }
    }
    return res;
}
