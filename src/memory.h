#include <iostream>
#include <cstddef>
#include <cstdint>
#include <vector>
#include <memory>
#include <cmath>
using namespace std;

#define		DBG				1
#define		L2_CACHE_SIZE		(128*1024)
#define		L1_CACHE_SIZE		(16*1024)

enum cacheResType {MISS=0, HIT=1};

class Cache{
    friend class Memory;
public:
    Cache(long size, int lineSize, int associativity, int hitTime);
    ~Cache();

private:
    long size;
    int lineSize;
    int associativity;
    int hitTime;
    int offsetbits;
    int linebits;
    int numLines;
    vector<vector<long>> TAG;
    vector<vector<bool>> V; 
    vector<vector<bool>> W;    
};

class Memory{
    friend class Cache;
public:
    Memory(int lineSize);
    ~Memory();
    cacheResType simulate(long addr, bool write);
    int getCycles() const { return cycles; }

private:
    Cache L1;
    Cache L2;
    cacheResType sim_level(Cache &cache, long addr, int storeCycles, bool write, int level);
    void printLine (Cache &cache, int line);
    int cycles;
};