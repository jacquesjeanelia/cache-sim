CXX = g++
CXXFLAGS = -std=c++11 -O2 -Wall
TARGET = cache_sim
SOURCES = src/cache.cpp src/memory.cpp
HEADERS = src/memory.h
PYTHON = python3

# Default target - run everything
all: run-complete

# Compile the cache simulator
$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

# Run the simulation to generate data
simulate: $(TARGET)
	@echo "Running cache simulation..."
	./$(TARGET)

# Generate graphs and analysis
analyze: simulate
	@echo "Generating graphs and analysis..."
	$(PYTHON) generate_graphs.py
	$(PYTHON) create_report_summary.py

# Run complete workflow (compile, simulate, analyze)
run-complete: analyze
	@echo "Complete workflow finished!"
	@echo "Generated files:"
	@echo "- simulation_results.csv"
	@echo "- cpi_vs_line_size.png"
	@echo "- cpi_heatmap.png"
	@echo "- individual_patterns.png"

# Run just the simulation
run: simulate

# Run just the analysis (if data already exists)
graphs: 
	@echo "Generating graphs from existing data..."
	$(PYTHON) generate_graphs.py
	$(PYTHON) create_report_summary.py

# Test the cache simulator
test: $(TARGET)
	@echo "Running cache simulator tests..."
	g++ -std=c++11 -o test_cache tests/test1.cpp src/memory.cpp
	./test_cache

# Install Python dependencies
install-deps:
	@echo "Installing Python dependencies..."
	pip3 install pandas matplotlib seaborn

# Show project status
status:
	@echo "Project Status:"
	@echo "==============="
	@if [ -f $(TARGET) ]; then echo "✓ Cache simulator compiled"; else echo "✗ Cache simulator not compiled"; fi
	@if [ -f simulation_results.csv ]; then echo "✓ Simulation data exists"; else echo "✗ No simulation data"; fi
	@if [ -f cpi_vs_line_size.png ]; then echo "✓ Graphs generated"; else echo "✗ No graphs"; fi
	@echo ""
	@echo "Available targets:"
	@echo "- make all          : Run complete workflow"
	@echo "- make simulate     : Run simulation only"
	@echo "- make analyze      : Generate graphs and analysis"
	@echo "- make test         : Run tests"
	@echo "- make install-deps : Install Python dependencies"
	@echo "- make clean        : Clean all generated files"

# Clean all generated files
clean:
	rm -f $(TARGET) test_cache
	rm -f simulation_results.csv
	rm -f *.png
	@echo "Cleaned all generated files"

# Help target
help:
	@echo "Cache Simulator Makefile"
	@echo "========================"
	@echo ""
	@echo "Targets:"
	@echo "  all          - Run complete workflow (compile, simulate, analyze)"
	@echo "  simulate     - Compile and run simulation"
	@echo "  analyze      - Generate graphs and analysis from existing data"
	@echo "  run-complete - Same as 'all'"
	@echo "  run          - Run simulation only"
	@echo "  graphs       - Generate graphs from existing data"
	@echo "  test         - Run cache simulator tests"
	@echo "  install-deps - Install required Python packages"
	@echo "  status       - Show project status"
	@echo "  clean        - Remove all generated files"
	@echo "  help         - Show this help message"
	@echo ""
	@echo "Examples:"
	@echo "  make all          # Run everything"
	@echo "  make simulate     # Just run simulation"
	@echo "  make clean        # Clean everything"

.PHONY: all simulate analyze run-complete run graphs test install-deps status clean help 