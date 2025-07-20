# Cache Simulator - Two-Level Performance Analysis

A comprehensive cache simulator implementing a realistic two-level cache hierarchy to analyze the performance impact of different cache line sizes and memory access patterns on overall system performance.

## Overview

This project implements a two-level cache simulator that models realistic CPU cache behavior with L1 and L2 caches, incorporating actual miss penalties and quantifying performance through effective cycles per instruction (CPI). The simulator systematically tests different configurations to identify optimal cache parameters and understand the relationship between spatial locality and cache performance.

## Authors

- **Yomna Othman**
- **Ayla Saleh** 
- **Jacques Elia**

*CSCE 2303: Computer Organization - Dr. Mohamed Shalan - Summer 2025*

## Features

- **Two-Level Cache Hierarchy**: L1 (16KB, 4-way) and L2 (128KB, 8-way) cache simulation
- **Variable Line Sizes**: Test L1 cache with 16B, 32B, 64B, and 128B line sizes
- **Multiple Access Patterns**: 5 different memory access patterns for comprehensive analysis
- **Realistic Performance Modeling**: Includes write-back functionality, dirty bit tracking, and realistic timing
- **Comprehensive Analysis**: Automated graph generation and statistical analysis
- **Performance Metrics**: Detailed CPI calculations and cache effectiveness measurements

## Architecture Specifications

### L1 Cache
- **Size**: 16KB
- **Associativity**: 4-way set associative
- **Line Sizes**: Variable (16B, 32B, 64B, 128B)
- **Hit Time**: 1 cycle
- **Replacement Policy**: Random

### L2 Cache
- **Size**: 128KB
- **Associativity**: 8-way set associative
- **Line Size**: Fixed 64B
- **Hit Time**: 10 cycles
- **Replacement Policy**: Random

### Main Memory
- **Size**: 64GB DRAM
- **Access Time**: 50 cycles
- **Write-back Policy**: Implemented with dirty bit tracking

## Memory Access Patterns

1. **Sequential**: Sequential access across entire memory space
2. **Random 24KB**: Random access within 24KB working set
3. **Random Full**: Random access across entire memory space
4. **Sequential 4KB**: Sequential access within 4KB working set
5. **Strided**: Regular strided access pattern (32-byte stride)

## Project Structure

```
cache-sim/
├── src/
│   ├── cache.cpp          # Main simulation program
│   ├── memory.cpp         # Cache hierarchy implementation
│   └── memory.h           # Cache class definitions
├── tests/
│   └── test1.cpp          # Unit tests
├── Makefile               # Build and automation system
├── generate_graphs.py     # Graph generation script
├── create_report_summary.py # Analysis summary script
├── simulation_results.csv # Generated simulation data
├── *.png                  # Generated performance graphs
└── README.md             # This file
```

## Prerequisites

### System Requirements
- **Compiler**: GCC with C++11 support
- **Python**: 3.x with the following packages:
  - pandas
  - matplotlib
  - seaborn
  - numpy

### Windows-Specific
- MSYS2/MinGW64 environment (for GCC)
- Python 3.x installed and accessible via `python` command

## Installation & Setup

1. **Clone the repository**:
   ```bash
   git clone https://github.com/jacquesjeanelia/cache-sim.git
   cd cache-sim
   ```

2. **Install Python dependencies**:
   ```bash
   make install-deps
   ```
   Or manually:
   ```bash
   pip install pandas matplotlib seaborn numpy
   ```

3. **Check project status**:
   ```bash
   make status
   ```

## Usage

### Quick Start - Run Everything
```bash
make all
```
This will compile, run simulations, and generate all graphs and analysis.

### Step-by-Step Execution

1. **Compile the simulator**:
   ```bash
   make cache_sim
   ```

2. **Run simulation**:
   ```bash
   make simulate
   ```

3. **Generate analysis and graphs**:
   ```bash
   make analyze
   ```

### Available Make Targets

| Target | Description |
|--------|-------------|
| `make all` | Run complete workflow (compile, simulate, analyze) |
| `make simulate` | Compile and run simulation |
| `make analyze` | Generate graphs and analysis from existing data |
| `make run` | Run simulation only |
| `make graphs` | Generate graphs from existing data |
| `make test` | Run cache simulator tests |
| `make install-deps` | Install required Python packages |
| `make status` | Show project status |
| `make clean` | Remove all generated files |
| `make help` | Show help message |

## Output Files

After running the simulation, the following files are generated:

- **`simulation_results.csv`**: Raw performance data for all configurations
- **`cpi_vs_line_size.png`**: Line graph showing CPI vs line size trends
- **`cpi_heatmap.png`**: Heatmap visualization of all configurations
- **`individual_patterns.png`**: Individual pattern performance analysis

## Key Results & Findings

### Performance Highlights
- **Best Performance**: 1.0032 CPI (128B lines, Sequential 4KB pattern)
- **Worst Performance**: 56.5103 CPI (16B lines, Random Full pattern)
- **Performance Range**: Over 56x difference between best and worst cases
- **Optimal Line Size**: 128B provides best average performance (19% improvement over 16B)

### Design Implications
1. **Spatial Locality is Critical**: 56x performance difference between good and poor access patterns
2. **Larger Line Sizes Generally Better**: 128B lines provide optimal performance for most workloads
3. **Cache Hierarchy Effectiveness**: L2 cache provides significant benefits for moderate locality workloads
4. **Software Optimization Crucial**: Access pattern optimization more impactful than hardware improvements alone

## Testing

Run the test suite to verify simulator correctness:
```bash
make test
```

## Development

### Modifying Cache Parameters
Edit the constants in `src/cache.cpp`:
```cpp
#define L2_CACHE_SIZE    (128*1024)  // L2 cache size
#define L1_CACHE_SIZE    (16*1024)   // L1 cache size
const int L1_LINE_SIZES[] = {16, 32, 64, 128}; // Line sizes to test
```

### Adding New Access Patterns
1. Implement new memory generator function in `src/cache.cpp`
2. Add function pointer to `memGens[]` array
3. Add descriptive name to `memGenNames[]` array
4. Update `NUM_MEM_GENS` constant

### Customizing Analysis
Modify `generate_graphs.py` and `create_report_summary.py` to customize:
- Graph styling and colors
- Statistical analysis methods
- Output formats and file names

## Performance Analysis Framework

The simulator provides comprehensive performance analysis through:

- **Statistical Analysis**: Mean, min, max CPI calculations
- **Visual Analysis**: Multiple graph types for trend identification
- **Comparative Analysis**: Performance comparison across all configurations
- **Cache Effectiveness Metrics**: Hit rates and access distribution analysis

## Troubleshooting

### Common Issues

1. **Compilation Errors**:
   - Ensure GCC with C++11 support is installed
   - Check that MSYS2/MinGW64 is properly configured on Windows

2. **Python Import Errors**:
   - Install missing packages: `pip install pandas matplotlib seaborn numpy`
   - Verify Python 3.x is being used

3. **Graph Generation Fails**:
   - Ensure `simulation_results.csv` exists (run simulation first)
   - Check Python script permissions and dependencies

### Debug Mode
Enable debug output by setting `DBG 1` in `src/cache.cpp` and recompiling.

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/new-feature`)
3. Commit your changes (`git commit -am 'Add new feature'`)
4. Push to the branch (`git push origin feature/new-feature`)
5. Create a Pull Request

## Resources & Links

- **Source Code**: [GitHub Repository](https://github.com/jacquesjeanelia/cache-sim)
- **Analysis Data**: [Google Sheets](https://docs.google.com/spreadsheets/d/1BaQfZ4zI6AHMglTwkRq3I1hvB3c_4frG/edit?usp=sharing&ouid=109798990182149835238&rtpof=true&sd=true)
- **Documentation**: See project report for detailed analysis and findings

## License

This project is developed as part of academic coursework for CSCE 2303: Computer Organization.

## Acknowledgments

- Dr. Mohamed Shalan for project guidance and requirements
- Python libraries: matplotlib, pandas, seaborn, numpy for analysis capabilities
- MSYS2/MinGW64 for Windows development environment

---

*For detailed performance analysis and findings, refer to the complete project report included in this repository.*
