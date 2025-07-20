#include "memory.h"


Cache::Cache(long size, int lineSize, int associativity, int hitTime)
    : size(size), lineSize(lineSize), associativity(associativity), hitTime(hitTime) {
    offsetbits = log2(lineSize);
    numLines = size / (lineSize * associativity);
    linebits = log2(numLines);
    TAG.resize(associativity, vector<long>(numLines, 0)); 
    V.resize(associativity, vector<bool>(numLines, false)); 
    W.resize(associativity, vector<bool>(numLines, false)); 
}

Cache::~Cache() {}

Memory::Memory(int lineSize)
    : L1(L1_CACHE_SIZE, lineSize, 4, 1), L2(L2_CACHE_SIZE, 64, 8, 10), cycles(0) {
}

Memory::~Memory() {}

cacheResType Memory::sim_level(Cache &cache, long addr, int storeCycles, bool write, int level) {
    //cout << "num of lines: "<< numLines << endl;
    int linebits = log2(cache.numLines);

    //split into components
    long offset = addr & ((1 << cache.offsetbits) - 1);
    //cout << "Offset: " << offset << "\tbits: " << cache.offsetbits << endl;

    long line = (addr >> cache.offsetbits) & ((1 << linebits) - 1);
    //cout << "line: " << line << endl << "bits: " << linebits << endl;

    long tag = addr >> (cache.offsetbits + linebits);
    //cout << "tag: " << tag << endl;

    // printLine(cache, line); // Print the current state of the cache line (disabled for performance)

    //check line if valid
    for (int i = 0; i < cache.associativity; ++i) {
        if (cache.V[i][line]) {                    // If the line is valid
            if (cache.TAG[i][line] == tag) {       // If the tag matches
                if (write) 
                    cache.W[i][line] = true;        // Mark as written if it's a write operation  
                // cout << "Cache Hit: Line " << line << " with tag " << tag << endl;
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
            // cout << "Cache Miss: Line " << line << " with tag " << tag << endl;
            return cacheResType::MISS; 
        }
    }

    //all valid -> Capacity cacheResType::MISS/Conflict cacheResType::MISS
    int ind = rand() % cache.associativity;                  // rand select to replace
    // cout << "rand = " << ind << endl;
    if (cache.W[ind][line])  // If the line is dirty, we need to write it back
        cycles += storeCycles; // Add cycles for writing back
    
    cache.TAG[ind][line] = tag;          // set tag
    cache.V[ind][line] = 1;                // mark as valid
    if (write){ 
        cache.W[ind][line] = true;
        if (level == 1){ 
            long oldAddr = (cache.TAG[ind][line] << (linebits + cache.offsetbits)) | (line << cache.offsetbits); // reconstruct old addres
            sim_level(L2, oldAddr, 50, true, 2); // Write back to L2
        }
    }
    // cout << "Cache Miss: Line " << line << " set with tag " << tag << endl;
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
        // cout << "\t\t\t\t found at L1"<<endl;
    }
    else {
        res = sim_level(L2, addr, 50, write, 2); // Simulate L2 cache
        if (res == HIT) {
            cycles += 10; // Add cycles for L2 hit
            // cout << "\t\t\t\t found at L2"<<endl;

        } else {
            cycles += 50; // Add cycles for DRAM access
            // cout << "\t\t\t\t found at DRAM"<<endl;
        }
    }
    return res;
}
