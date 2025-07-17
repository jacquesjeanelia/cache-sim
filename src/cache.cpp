#include <iostream>
#include  <iomanip>
#include <cstdint>
#include <cmath>
#include <vector>

using namespace std;

#define		DBG				1
#define		DRAM_SIZE		(1024*1024*1024)
#define		L2_CACHE_SIZE		(128*1024)
#define		L1_CACHE_SIZE		(16*1024)
uint64_t     L1lineSize	;	

enum cacheResType {MISS=0, HIT=1};

/* The following implements a random number generator */
uint32_t m_z = 362436069;
uint32_t m_w = 521288629;

uint64_t rand_() {
    m_z = 36969 * (m_z & 0xFFFF) + (m_z >> 16);
    m_w = 18000 * (m_w & 0xFFFF) + (m_w >> 16);
    uint64_t result = ((uint64_t(m_z) << 16) + m_w); // 32-bit
    result = (result << 4) ^ (result >> 28);         // stretch to 36 bits with mixing
    return result & 0xFFFFFFFFF; // Mask to 36 bits (9 hex digits)
}


unsigned int memGen1()
{
	static unsigned int addr=0;
	return (addr++)%(DRAM_SIZE);
}

unsigned int memGen2()
{
	static unsigned int addr=0;
	return  rand_()%(24*1024);
}

unsigned int memGen3()
{
	return rand_()%(DRAM_SIZE);
}

unsigned int memGen4()
{
	static unsigned int addr=0;
	return (addr++)%(4*1024);
}

unsigned int memGen5()
{
	static unsigned int addr=0;
	return (addr+=32)%(64*16*1024);
}

vector<vector<uint64_t>> L1CacheTag (4,vector<uint64_t> (1)); // Assuming 4 bytes per line
vector<vector<bool>> L1CacheV (4,vector<bool> (1)); // Assuming 4 bytes per line

// Direct Mapped Cache Simulator
cacheResType sim_level1(uint64_t addr)
{	
    int offsetbits = log2(L1lineSize);

    int numLines = L1_CACHE_SIZE / (L1lineSize * 4); 
    //cout << "num of lines: "<< numLines << endl;
    int linebits = log2(numLines);

    //split into components
    uint64_t offset = addr & ((1 << offsetbits) - 1);
    //cout << "Offset: " << offset << "\tbits: " << offsetbits << endl;

    uint64_t line = (addr >> offsetbits) & ((1 << linebits) - 1);
    cout << "line: " << line << "\tbits: " << linebits << endl;

    uint64_t tag = addr >> (offsetbits + linebits);
    cout << "tag: " << tag << endl;

    if (L1CacheTag[0].size() == 1){
        // Resize the cache tag and valid vectors to accommodate the number of lines
        for (int i = 0; i < 4; ++i) {
            L1CacheTag[i].resize(numLines, 0); // Initialize with 0
            L1CacheV[i].resize(numLines, false); // Initialize with false
        }
    }

    //check line if valid
    for (int i = 0; i < 4; ++i) {
        if (L1CacheV[i][line]) {                    // If the line is valid
            if (L1CacheTag[i][line] == tag) {       // If the tag matches
                cout << "Cache Hit: Line " << line << " with tag " << tag << endl;
                return HIT; 
            }
        }
    }

    //no hit -> check for invalid 
    for (int i = 0; i < 4; ++i) {
        if (!L1CacheV[i][line]) {           // If the line is not valid
            L1CacheTag[i][line] = tag;      // set tag
            L1CacheV[i][line] = 1;          // mark as valid
            cout << "Cache Miss: Line " << line << " set with tag " << tag << endl;
            return MISS; 
        }
    }

    //all valid -> Capacity miss/Conflict miss
    int ind = rand_() % 4;                  // rand select to replace

    L1CacheTag[ind][line] = tag;            // set tag
    L1CacheV[ind][line] = 1;                // mark as valid
    //cout << "Cache Miss: Line " << line << " set with tag " << tag << endl;
    return MISS; 
}

cacheResType sim_level2(unsigned int addr)
{
    // This function simulates a direct-mapped cache
    // It accepts the memory address for the memory transaction and 
    // returns whether it caused a cache miss or a cache hit

    // The current implementation assumes there is no cache; so, every transaction is a miss
    return sim_level1(addr);
}



char *msg[2] = {"Miss","Hit"};

#define		NO_OF_Iterations	100.0		// CHange to 1,000,000

int main(){
    L1lineSize = 64; // Assuming a line size of 64 bytes
    sim_level1(0x00000540);
sim_level1(0x0000AA40);
sim_level1(0x00015440);
sim_level1(0x0001FD40);

    /*
    int cycles = 0;
    for (int i = 0; i < NO_OF_Iterations; ++i) {
        float randomValue = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        if(randomValue <= 0.35){
            unsigned int address = memGen2();
            cacheResType result1 = sim_level1(address);
            if (result1 == HIT) {
                cycles += 1;
            } else{
                cacheResType result2 = sim_level2(address);
                if (result2 == HIT) {
                    cycles += 10;
                } else {
                    cycles += 50;
                }
            }
        }
        else cycles += 1;    
    }
    float CPI = cycles / NO_OF_Iterations;
    cout << "Average Cycles Per Instruction (CPI) = " << CPI << endl;*/
}
