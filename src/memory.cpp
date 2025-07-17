#include "memory.h"

/* The following implements a random number generator */
uint32_t m_z = 362436069;
uint32_t m_w = 521288629;

long rand_() {
    m_z = 36969 * (m_z & 0xFFFF) + (m_z >> 16);
    m_w = 18000 * (m_w & 0xFFFF) + (m_w >> 16);
    long result = ((long(m_z) << 16) + m_w); // 32-bit
    result = (result << 4) ^ (result >> 28);         // stretch to 36 bits with mixing
    return result & 0xFFFFFFFFF; // Mask to 36 bits (9 hex digits)
}

Cache::Cache(long size, int lineSize, int associativity)
    : lineSize(lineSize), TAG(associativity, vector<long>(size / (lineSize * associativity), 0)), V(associativity, vector<bool>(size / (lineSize * associativity), false)) {
}

Cache::~Cache() {}

Memory::Memory(int lineSize)
    : L1(DRAM_SIZE, lineSize, 4), L2(L2_CACHE_SIZE, 64, 8), cycles(0) {
}

Memory::~Memory() {}

cacheResType Memory::sim_level1(long addr) {
    int offsetbits = log2(L1.lineSize);
    int numLines = L1_CACHE_SIZE / (L1.lineSize * 4);
    //cout << "num of lines: "<< numLines << endl;
    int linebits = log2(numLines);

    //split into components
    long offset = addr & ((1 << offsetbits) - 1);
    //cout << "Offset: " << offset << "\tbits: " << offsetbits << endl;

    long line = (addr >> offsetbits) & ((1 << linebits) - 1);
    cout << "line: " << line << "\tbits: " << linebits << endl;

    long tag = addr >> (offsetbits + linebits);
    cout << "tag: " << tag << endl;

    if (L1.TAG[0].size() == 1){
        // Resize the cache tag and valid vectors to accommodate the number of lines
        for (int i = 0; i < 4; ++i) {
            L1.TAG[i].resize(numLines, 0); // Initialize with 0
            L1.V[i].resize(numLines, false); // Initialize with false
        }
    }

    //check line if valid
    for (int i = 0; i < 4; ++i) {
        if (L1.V[i][line]) {                    // If the line is valid
            if (L1.TAG[i][line] == tag) {       // If the tag matches
                cout << "Cache Hit: Line " << line << " with tag " << tag << endl;
                return cacheResType::HIT;
            }
        }
    }

    //no hit -> check for invalid 
    for (int i = 0; i < 4; ++i) {
        if (!L1.V[i][line]) {           // If the line is not valid
            L1.TAG[i][line] = tag;      // set tag
            L1.V[i][line] = 1;          // mark as valid
            cout << "Cache cacheResType::MISS: Line " << line << " set with tag " << tag << endl;
            return cacheResType::MISS; 
        }
    }

    //all valid -> Capacity cacheResType::MISS/Conflict cacheResType::MISS
    int ind = rand_() % 4;                  // rand select to replace

    L1.TAG[ind][line] = tag;            // set tag
    L1.V[ind][line] = 1;                // mark as valid
    //cout << "Cache cacheResType::MISS: Line " << line << " set with tag " << tag << endl;
    return cacheResType::MISS; 
}

cacheResType Memory::sim_level2(long addr) {
    return cacheResType::MISS;
}