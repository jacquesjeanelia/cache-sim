#include <iostream>
#include  <iomanip>
#include <cstdint>
#include <cmath>
#include <vector>
#include <fstream>
#include <ctime>
#include "memory.h"

using namespace std;

#define		DBG				1
#define		DRAM_SIZE	    (1024*1024*1024) // still need to multiply by 64
#define		L2_CACHE_SIZE		(128*1024)
#define		L1_CACHE_SIZE		(16*1024)
uint64_t     L1lineSize	;	

// L1 line sizes to test
const int L1_LINE_SIZES[] = {16, 32, 64, 128};
const int NUM_LINE_SIZES = 4;

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

// Function pointers for memory generators
typedef unsigned int (*MemGenFunc)();
MemGenFunc memGens[] = {memGen1, memGen2, memGen3, memGen4, memGen5};
const char* memGenNames[] = {"Sequential", "Random 24KB", "Random Full", "Sequential 4KB", "Strided"};
const int NUM_MEM_GENS = 5;

char *msg[2] = {"Miss","Hit"};

#define		NO_OF_Iterations	1000000.0		// 1M iterations as required

// Function to run simulation for a specific configuration
float runSimulation(int lineSize, MemGenFunc memGen, const char* genName) {
    Memory memory(lineSize);
    int cycles = 0;
    
    for (int i = 0; i < NO_OF_Iterations; ++i) {
        float randomValue = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        if(randomValue <= 0.35 && randomValue > 0.175)  //load
            memory.simulate(memGen(), 0);
        else if (randomValue <= 0.175)                  //store
            memory.simulate(memGen(), 1);
        else cycles += 1;    
    }
    cycles += memory.getCycles();
    float CPI = static_cast<float>(cycles) / NO_OF_Iterations;
    
    cout << "Line Size: " << lineSize << "B, Generator: " << genName 
         << ", CPI: " << fixed << setprecision(4) << CPI << endl;
    
    return CPI;
}

int main(){
    srand(time(0));
    cout << "Cache Simulator - Two-Level Performance Analysis" << endl;
    cout << "================================================" << endl;
    
    // Create results file
    ofstream resultsFile("simulation_results.csv");
    resultsFile << "Line_Size,Memory_Generator,CPI" << endl;
    
    // Run all combinations
    for (int lineSizeIdx = 0; lineSizeIdx < NUM_LINE_SIZES; lineSizeIdx++) {
        int lineSize = L1_LINE_SIZES[lineSizeIdx];
        cout << "\nTesting L1 Line Size: " << lineSize << "B" << endl;
        cout << "----------------------------------------" << endl;
        
        for (int genIdx = 0; genIdx < NUM_MEM_GENS; genIdx++) {
            float cpi = runSimulation(lineSize, memGens[genIdx], memGenNames[genIdx]);
            resultsFile << lineSize << "," << memGenNames[genIdx] << "," << cpi << endl;
        }
    }
    
    resultsFile.close();
    cout << "\nResults saved to simulation_results.csv" << endl;
    cout << "Total simulations run: " << NUM_LINE_SIZES * NUM_MEM_GENS << endl;
    
    return 0;
}
