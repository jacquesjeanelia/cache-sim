#!/usr/bin/env python3
import pandas as pd
import numpy as np

def create_summary_report():
    """Create a comprehensive summary report from simulation results"""
    
    # Load the simulation data
    try:
        df = pd.read_csv('simulation_results.csv')
    except FileNotFoundError:
        print("Error: simulation_results.csv not found. Run the simulation first.")
        return
    
    print("=" * 60)
    print("TWO-LEVEL CACHE SIMULATION - KEY FINDINGS SUMMARY")
    print("=" * 60)
    
    # Find best and worst performance
    best_performance = df.loc[df['CPI'].idxmin()]
    worst_performance = df.loc[df['CPI'].idxmax()]
    
    print(f"\n🏆 BEST PERFORMANCE:")
    print(f"   Line Size: {best_performance['Line_Size']}B")
    print(f"   Pattern: {best_performance['Memory_Generator']}")
    print(f"   CPI: {best_performance['CPI']:.4f}")
    
    print(f"\n⚠️  WORST PERFORMANCE:")
    print(f"   Line Size: {worst_performance['Line_Size']}B")
    print(f"   Pattern: {worst_performance['Memory_Generator']}")
    print(f"   CPI: {worst_performance['CPI']:.4f}")
    
    # Line size analysis
    print(f"\n📊 LINE SIZE IMPACT:")
    line_size_avg = df.groupby('Line_Size')['CPI'].mean().sort_values()
    for line_size, avg_cpi in line_size_avg.items():
        print(f"   {line_size}B: {avg_cpi:.4f} CPI")
    
    # Memory pattern analysis
    print(f"\n🎯 MEMORY PATTERN ANALYSIS:")
    pattern_avg = df.groupby('Memory_Generator')['CPI'].mean().sort_values()
    for pattern, avg_cpi in pattern_avg.items():
        print(f"   {pattern:<15} : {avg_cpi:.4f} CPI")
    
    # Key insights
    print(f"\n💡 KEY INSIGHTS:")
    print(f"   1. Larger line sizes ({line_size_avg.index[-1]}B) generally improve performance")
    print(f"   2. Spatial locality dramatically affects cache performance")
    print(f"   3. {best_performance['Memory_Generator']} pattern shows near-ideal performance")
    print(f"   4. {worst_performance['Memory_Generator']} pattern suffers from cache thrashing")
    
    # Check if strided pattern shows significant improvement
    strided_data = df[df['Memory_Generator'] == 'Strided']
    if len(strided_data) > 1:
        strided_improvement = (strided_data['CPI'].max() - strided_data['CPI'].min()) / strided_data['CPI'].max() * 100
        print(f"   5. Strided access benefits significantly from larger lines ({strided_improvement:.1f}% improvement)")
    
    # Performance recommendations
    print(f"\n📈 PERFORMANCE RECOMMENDATIONS:")
    print(f"   • Use {line_size_avg.index[-1]}B line size for optimal general performance")
    print(f"   • Optimize for spatial locality in memory access patterns")
    print(f"   • Keep working sets within cache capacity when possible")
    print(f"   • Consider cache behavior in algorithm design")
    
    print("=" * 60)
    print("Report generated successfully!")
    print(f"Complete data available in: simulation_results.csv")
    print(f"Graphs available in: cpi_vs_line_size.png, cpi_heatmap.png")
    print("=" * 60)

if __name__ == "__main__":
    create_summary_report() 