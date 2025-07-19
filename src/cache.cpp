#include <iostream>
#include  <iomanip>
#include <cstdint>
#include <cmath>
#include <vector>
#include "memory.h"

using namespace std;

#define		DBG				1
#define		DRAM_SIZE	    (1024*1024*1024) // still need to multiply by 64
#define		L2_CACHE_SIZE		(128*1024)
#define		L1_CACHE_SIZE		(16*1024)
uint64_t     L1lineSize	;	


/* The following implements a random number generator */
uint32_t mz = 362436069;
uint32_t mw = 521288629;

uint64_t rand1() {
    mz = 36969 * (mz & 0xFFFF) + (mz >> 16);
    mw = 18000 * (mw & 0xFFFF) + (mw >> 16);
    uint64_t result = ((uint64_t(mz) << 16) + mw); // 32-bit
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
	return  rand1()%(24*1024);
}

unsigned int memGen3()
{
	return rand1()%(DRAM_SIZE);
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

char *msg[2] = {"Miss","Hit"};

#define		NO_OF_Iterations	100.0		// CHange to 1,000,000

int main(){
    srand(time(0));
    Memory memory(64); // variable line size 
    int cycles = 0;
    for (int i = 0; i < NO_OF_Iterations; ++i) {
        float randomValue = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        if(randomValue <= 0.35 && randomValue > 0.175)  //load
            memory.simulate(memGen1(), 0);
        else if (randomValue <= 0.175)                  //store
            memory.simulate(memGen1(), 1);
        else cycles += 1;    
    }
    cycles += memory.getCycles();
    float CPI = cycles / NO_OF_Iterations;
    cout << "Average Cycles Per Instruction (CPI) = " << CPI << endl; 
}
