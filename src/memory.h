#include <iostream>
#include <cstddef>
#include <cstdint>
#include <vector>
#include <memory>
#include <cmath>
using namespace std;

#define		DBG				1
#define		DRAM_SIZE		(1024*1024*1024)
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
    vector<vector<long>> TAG;
    vector<vector<bool>> V;    
};

enum cacheResType {MISS=0, HIT=1};

class Memory{
    friend class Cache;
public:
    Memory(int lineSize);
    ~Memory();
    cacheResType simulate(long addr);
    int getCycles() const { return cycles; }

private:
    Cache L1;
    Cache L2;
    cacheResType sim_level(Cache cache, long addr, int storeCycles);
    int cycles;
};