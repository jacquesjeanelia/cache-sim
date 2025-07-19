#include <iostream>
#include <cassert>
#include "../src/memory.h"

using namespace std;

// Simple test to verify cache simulator functionality
int main() {
    cout << "Running Cache Simulator Tests..." << endl;
    
    // Test 1: Create memory with 64B line size
    Memory memory(64);
    cout << "✓ Memory object created successfully" << endl;
    
    // Test 2: Simulate a few memory accesses
    memory.simulate(0x1000);
    memory.simulate(0x1004);
    memory.simulate(0x1008);
    
    int cycles = memory.getCycles();
    cout << "✓ Memory simulation completed. Total cycles: " << cycles << endl;
    
    // Test 3: Verify cycles are reasonable (should be > 0)
    assert(cycles > 0);
    cout << "✓ Cycle count is valid" << endl;
    
    cout << "All tests passed!" << endl;
    return 0;
}
