#include <iostream>
#include  <iomanip>
#include <cstdint>
#include <vector>

using namespace std;

#define		DBG				1
#define		DRAM_SIZE		(64*1024*1024*1024)
#define		L2_CACHE_SIZE		(128*1024)
#define		L1_CACHE_SIZE		(16*1024)

enum cacheResType {MISS=0, HIT=1};

/* The following implements a random number generator */
unsigned int m_w = 0xABABAB55;    /* must not be zero, nor 0x464fffff */
unsigned int m_z = 0x05080902;    /* must not be zero, nor 0x9068ffff */
unsigned int rand_()
{
    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    return (m_z << 16) + m_w;  /* 32-bit result */
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

vector<uint8_t> DRAM(DRAM_SIZE);
vector<uint8_t> L1Cache(L1_CACHE_SIZE);
vector<uint8_t> L2Cache(L2_CACHE_SIZE);


// Direct Mapped Cache Simulator
cacheResType sim_level1(unsigned int addr)
{	
	// This function accepts the memory address for the memory transaction and 
	// returns whether it caused a cache miss or a cache hit

	// The current implementation assumes there is no cache; so, every transaction is a miss
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
    cout << "Average Cycles Per Instruction (CPI) = " << CPI << endl;
}
