# Two-Level Cache Simulator

A comprehensive cache simulator implementation for analyzing two-level cache performance with different line sizes and memory access patterns.

## Project Overview

This project implements a two-level cache simulator to analyze the performance impact of different cache line sizes and memory access patterns on overall system performance. The simulator models a modern CPU cache hierarchy and quantifies performance through effective Cycles Per Instruction (CPI).

## Cache Hierarchy Specifications

- **L1 Cache**: 16KB, 4-way associative, variable line size (16B, 32B, 64B, 128B), 1 cycle hit time
- **L2 Cache**: 128KB, 8-way associative, 64B line size (fixed), 10 cycle hit time  
- **Main Memory (DRAM)**: 64GB, 50 cycle access penalty
- **Assumptions**: 35% of instructions are loads/stores, write-back policy, random replacement

## Memory Access Patterns

The simulator includes five different memory access patterns:

1. **Sequential**: Sequential access across entire memory space
2. **Random 24KB**: Random access within a 24KB working set
3. **Random Full**: Random access across the entire memory space
4. **Sequential 4KB**: Sequential access within a 4KB working set
5. **Strided**: Strided access pattern with 32-byte increments

## Quick Start

### Compilation
```bash
make clean && make
```

### Running Simulations
```bash
./cache_sim
```

This will run all 20 combinations (4 line sizes × 5 memory generators) and generate:
- Console output with results
- `simulation_results.csv` with all data
- Performance graphs (see below)

### Generating Analysis
```bash
python3 generate_graphs.py
python3 create_report_summary.py
```

## Project Structure

```
cache-sim/
├── src/
│   ├── cache.cpp          # Main simulation logic
│   ├── memory.cpp         # Cache hierarchy implementation
│   └── memory.h           # Header file with class definitions
├── docs/
│   └── Project2_Report.md # Comprehensive project report
├── tests/
│   └── test1.cpp          # Test file (empty)
├── Makefile               # Build configuration
├── generate_graphs.py     # Data visualization script
├── create_report_summary.py # Results summary script
├── simulation_results.csv # Generated simulation data
├── cpi_vs_line_size.png   # Main performance graph
├── cpi_heatmap.png        # Performance heatmap
└── individual_patterns.png # Individual pattern analysis
```

## Key Findings

### Best Performance
- **Line Size**: 128B
- **Pattern**: Sequential 4KB
- **CPI**: 1.0032 (nearly ideal performance)

### Worst Performance
- **Line Size**: 128B
- **Pattern**: Random Full
- **CPI**: 56.6901 (significant cache thrashing)

### Performance Trends
- Larger line sizes (128B) generally improve performance
- Spatial locality dramatically affects cache performance
- Sequential 4KB pattern shows near-ideal performance
- Random Full pattern suffers from cache thrashing
- Strided access benefits significantly from larger lines

## Data Analysis

The project includes comprehensive data analysis with:

1. **Statistical Summary**: Best/worst performers, averages by category
2. **Visualization**: Multiple graph types showing performance trends
3. **Heatmap Analysis**: Visual representation of all combinations
4. **Individual Pattern Analysis**: Detailed breakdown by memory access pattern

## Report

A comprehensive report is available in `docs/Project2_Report.md` covering:
- Implementation details
- Experimental methodology
- Results and analysis
- Conclusions and recommendations
- Future work suggestions

## Requirements

- C++11 compatible compiler (g++)
- Python 3 with pandas, matplotlib, seaborn
- Make utility

## Usage Examples

### Run Single Test
```bash
# Compile and run
make
./cache_sim
```

### Generate Analysis
```bash
# Generate graphs and statistics
python3 generate_graphs.py
python3 create_report_summary.py
```

### Clean Build
```bash
make clean
make
```

## Performance Results Summary

| Line Size | Sequential | Random 24KB | Random Full | Sequential 4KB | Strided |
|-----------|------------|-------------|-------------|----------------|---------|
| 16B       | 2.0689     | 3.2487      | 56.4309     | 1.0081         | 32.0855 |
| 32B       | 1.8674     | 3.2536      | 56.5857     | 1.0069         | 32.0120 |
| 64B       | 1.7627     | 3.2610      | 56.4520     | 1.0063         | 28.7360 |
| 128B      | 1.3836     | 3.2598      | 56.6901     | 1.0032         | 14.8160 |

## Contributing

This is a course project submission. For questions or issues, please refer to the project specification and course materials.

## License

This project is part of a Computer Architecture course assignment.