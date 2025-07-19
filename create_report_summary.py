#!/usr/bin/env python3
"""
Generate a summary of key findings for the cache simulation report
"""

import pandas as pd

def generate_summary():
    """Generate a summary of the simulation results"""
    
    # Load data
    df = pd.read_csv('simulation_results.csv')
    
    print("=" * 60)
    print("TWO-LEVEL CACHE SIMULATION - KEY FINDINGS SUMMARY")
    print("=" * 60)
    print()
    
    # Best and worst performers
    best = df.loc[df['CPI'].idxmin()]
    worst = df.loc[df['CPI'].idxmax()]
    
    print("🏆 BEST PERFORMANCE:")
    print(f"   Line Size: {best['Line_Size']}B")
    print(f"   Pattern: {best['Memory_Generator']}")
    print(f"   CPI: {best['CPI']:.4f}")
    print()
    
    print("⚠️  WORST PERFORMANCE:")
    print(f"   Line Size: {worst['Line_Size']}B")
    print(f"   Pattern: {worst['Memory_Generator']}")
    print(f"   CPI: {worst['CPI']:.4f}")
    print()
    
    # Line size analysis
    print("📊 LINE SIZE IMPACT:")
    line_avg = df.groupby('Line_Size')['CPI'].mean().sort_values()
    for line_size, avg_cpi in line_avg.items():
        print(f"   {line_size:3d}B: {avg_cpi:.4f} CPI")
    print()
    
    # Pattern analysis
    print("🎯 MEMORY PATTERN ANALYSIS:")
    pattern_avg = df.groupby('Memory_Generator')['CPI'].mean().sort_values()
    for pattern, avg_cpi in pattern_avg.items():
        print(f"   {pattern:15s}: {avg_cpi:.4f} CPI")
    print()
    
    # Key insights
    print("💡 KEY INSIGHTS:")
    print("   1. Larger line sizes (128B) generally improve performance")
    print("   2. Spatial locality dramatically affects cache performance")
    print("   3. Sequential 4KB pattern shows near-ideal performance")
    print("   4. Random Full pattern suffers from cache thrashing")
    print("   5. Strided access benefits significantly from larger lines")
    print()
    
    print("📈 PERFORMANCE RECOMMENDATIONS:")
    print("   • Use 128B line size for optimal general performance")
    print("   • Optimize for spatial locality in memory access patterns")
    print("   • Keep working sets within cache capacity when possible")
    print("   • Consider cache behavior in algorithm design")
    print()
    
    print("=" * 60)
    print("Report generated successfully!")
    print("Complete data available in: simulation_results.csv")
    print("Graphs available in: cpi_vs_line_size.png, cpi_heatmap.png")
    print("=" * 60)

if __name__ == "__main__":
    generate_summary() 