# Project 2: Two-Level Cache Performance Analysis
## Cache Simulator Implementation and Performance Evaluation

**Course:** Computer Architecture  
**Project:** Two-Level Cache Simulator  
**Date:** July 2024  
**Team Members:** [Your Team Names]

---

## 1. Introduction

This project implements a two-level cache simulator to analyze the performance impact of different cache line sizes and memory access patterns on overall system performance. The simulator models a modern CPU cache hierarchy with L1 and L2 caches, incorporating realistic miss penalties and quantifying performance through effective Cycles Per Instruction (CPI).

### 1.1 Cache Hierarchy Specifications

- **L1 Cache**: 16KB, 4-way associative, variable line size (16B, 32B, 64B, 128B), 1 cycle hit time
- **L2 Cache**: 128KB, 8-way associative, 64B line size (fixed), 10 cycle hit time  
- **Main Memory (DRAM)**: 64GB, 50 cycle access penalty
- **Assumptions**: 35% of instructions are loads/stores, write-back policy, random replacement

---

## 2. Implementation Overview

### 2.1 Cache Simulator Architecture

The simulator implements a hierarchical cache system with the following key components:

- **Cache Class**: Manages individual cache levels with configurable size, line size, associativity, and hit time
- **Memory Class**: Orchestrates the two-level cache hierarchy and tracks total cycles
- **Memory Generators**: Five different access patterns to simulate various workload characteristics

### 2.2 Memory Access Patterns

1. **Sequential**: Sequential access across entire memory space
2. **Random 24KB**: Random access within a 24KB working set
3. **Random Full**: Random access across the entire memory space
4. **Sequential 4KB**: Sequential access within a 4KB working set
5. **Strided**: Strided access pattern with 32-byte increments

---

## 3. Experimental Methodology

### 3.1 Test Configuration

- **Total Simulations**: 20 combinations (4 line sizes × 5 memory generators)
- **Iterations per Simulation**: 1,000,000 instructions
- **Memory Access Probability**: 35% (loads/stores)
- **Performance Metric**: Effective CPI (Cycles Per Instruction)

### 3.2 Data Collection Process

The simulator systematically tests all combinations and outputs results to a CSV file for analysis. Each simulation runs for 1M iterations to ensure statistical significance.

---

## 4. Results and Data Analysis

### 4.1 Overall Performance Summary

| Line Size | Sequential | Random 24KB | Random Full | Sequential 4KB | Strided |
|-----------|------------|-------------|-------------|----------------|---------|
| 16B       | 2.0689     | 3.2487      | 56.4309     | 1.0081         | 32.0855 |
| 32B       | 1.8674     | 3.2536      | 56.5857     | 1.0069         | 32.0120 |
| 64B       | 1.7627     | 3.2610      | 56.4520     | 1.0063         | 28.7360 |
| 128B      | 1.3836     | 3.2598      | 56.6901     | 1.0032         | 14.8160 |

### 4.2 Key Performance Findings

**Best Performance**: 
- Line Size: 128B
- Pattern: Sequential 4KB
- CPI: 1.0032 (nearly ideal performance)

**Worst Performance**:
- Line Size: 128B  
- Pattern: Random Full
- CPI: 56.6901 (significant cache thrashing)

### 4.3 Average Performance by Category

**By Line Size**:
- 16B: 18.97 CPI
- 32B: 18.95 CPI  
- 64B: 18.24 CPI
- 128B: 15.43 CPI

**By Memory Pattern**:
- Sequential: 1.77 CPI
- Sequential 4KB: 1.01 CPI
- Random 24KB: 3.26 CPI
- Strided: 26.91 CPI
- Random Full: 56.54 CPI

---

## 5. Analysis and Conclusions

### 5.1 Impact of Cache Line Size

**General Trend**: Larger line sizes generally improve performance, with 128B showing the best average CPI (15.43 vs 18.97 for 16B).

**Exceptions**: 
- Random Full pattern shows minimal improvement with larger lines due to poor spatial locality
- Sequential 4KB shows minimal variation (1.00-1.01 CPI) across all line sizes

**Optimal Line Size**: 128B provides the best overall performance across most access patterns.

### 5.2 Memory Access Pattern Analysis

**Spatial Locality Patterns**:
- **Sequential 4KB**: Excellent performance (CPI ~1.00) due to high spatial locality within cache capacity
- **Sequential**: Good performance (CPI ~1.4-2.1) with improvement from larger lines
- **Strided**: Moderate performance with significant improvement from larger lines (32.1 → 14.8 CPI)

**Random Access Patterns**:
- **Random 24KB**: Consistent moderate performance (CPI ~3.2) across all line sizes
- **Random Full**: Poor performance (CPI ~56) due to cache thrashing and poor locality

### 5.3 Cache Hierarchy Effectiveness

**L1 Cache Hit Rates**: 
- High for patterns with good locality (Sequential 4KB, Sequential)
- Low for random patterns, leading to frequent L2 and DRAM accesses

**L2 Cache Benefits**:
- Provides significant performance improvement for moderate locality patterns
- Limited benefit for patterns with very poor locality (Random Full)

### 5.4 Design Implications

1. **Line Size Selection**: 128B provides optimal performance for most workloads
2. **Memory Access Optimization**: Spatial locality is crucial for cache performance
3. **Working Set Management**: Keeping frequently accessed data within cache capacity dramatically improves performance
4. **Cache Hierarchy**: Two-level design effectively handles moderate locality patterns

---

## 6. Conclusions

### 6.1 Key Insights

1. **Line Size Matters**: Larger cache lines (128B) provide better performance for most access patterns
2. **Locality is Critical**: Spatial locality has a dramatic impact on cache performance
3. **Working Set Size**: Access patterns that fit within cache capacity show near-ideal performance
4. **Random Access Penalty**: Poor locality patterns suffer significant performance degradation

### 6.2 Performance Recommendations

1. **Optimal Configuration**: 128B line size for general-purpose workloads
2. **Memory Access Patterns**: Optimize for spatial locality when possible
3. **Working Set Management**: Design algorithms to maintain data locality within cache capacity
4. **Cache-Aware Programming**: Consider cache behavior when designing data structures and algorithms

### 6.3 Future Work

- Analyze impact of different associativity levels
- Study temporal locality effects
- Evaluate prefetching strategies
- Consider multi-core cache coherence overhead

---

## 7. References

1. Hennessy, J. L., & Patterson, D. A. (2017). Computer Architecture: A Quantitative Approach (6th ed.). Morgan Kaufmann.
2. Cache Performance and Memory Hierarchy Design. Computer Architecture Course Materials.
3. Two-Level Cache Simulator Project Specification.

---

**Appendix**: Complete simulation data and graphs available in the project repository. 