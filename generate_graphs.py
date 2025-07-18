#!/usr/bin/env python3
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np

plt.style.use('seaborn-v0_8')
sns.set_palette("husl")

def load_data():
    """Load the simulation results from CSV"""
    df = pd.read_csv('simulation_results.csv')
    return df

def create_cpi_vs_line_size_graph(df):
    """Create the main CPI vs Line Size graph for different memory generators"""
    plt.figure(figsize=(12, 8))
    
    generators = df['Memory_Generator'].unique()
    line_sizes = df['Line_Size'].unique()
    
    for generator in generators:
        data = df[df['Memory_Generator'] == generator]
        plt.plot(data['Line_Size'], data['CPI'], marker='o', linewidth=2, 
                markersize=8, label=generator)
    
    plt.xlabel('L1 Cache Line Size (bytes)', fontsize=12)
    plt.ylabel('Cycles Per Instruction (CPI)', fontsize=12)
    plt.title('CPI vs L1 Cache Line Size for Different Memory Access Patterns', 
              fontsize=14, fontweight='bold')
    plt.legend(fontsize=10)
    plt.grid(True, alpha=0.3)
    plt.xticks(line_sizes)
    
    plt.yscale('log')
    plt.tight_layout()
    plt.savefig('cpi_vs_line_size.png', dpi=300, bbox_inches='tight')
    plt.show()

def create_heatmap(df):
    """Create a heatmap showing CPI values for all combinations"""
    pivot_df = df.pivot(index='Memory_Generator', columns='Line_Size', values='CPI')
    
    plt.figure(figsize=(10, 6))
    sns.heatmap(pivot_df, annot=True, fmt='.2f', cmap='YlOrRd', 
                cbar_kws={'label': 'CPI'})
    plt.title('CPI Heatmap: Memory Generators vs Line Sizes', 
              fontsize=14, fontweight='bold')
    plt.xlabel('L1 Cache Line Size (bytes)', fontsize=12)
    plt.ylabel('Memory Access Pattern', fontsize=12)
    plt.tight_layout()
    plt.savefig('cpi_heatmap.png', dpi=300, bbox_inches='tight')
    plt.show()

def create_individual_graphs(df):
    """Create individual graphs for each memory generator"""
    generators = df['Memory_Generator'].unique()
    
    fig, axes = plt.subplots(2, 3, figsize=(15, 10))
    axes = axes.flatten()
    
    for i, generator in enumerate(generators):
        data = df[df['Memory_Generator'] == generator]
        
        axes[i].plot(data['Line_Size'], data['CPI'], marker='o', linewidth=2, 
                    markersize=8, color='steelblue')
        axes[i].set_xlabel('Line Size (bytes)')
        axes[i].set_ylabel('CPI')
        axes[i].set_title(f'{generator}')
        axes[i].grid(True, alpha=0.3)
        axes[i].set_xticks(data['Line_Size'])
        
        if data['CPI'].max() > 10:
            axes[i].set_yscale('log')
    
    axes[-1].set_visible(False)
    
    plt.tight_layout()
    plt.savefig('individual_patterns.png', dpi=300, bbox_inches='tight')
    plt.show()

def print_statistics(df):
    """Print statistical analysis of the data"""
    print("=== CACHE SIMULATION STATISTICS ===\n")
    
    print(f"Total simulations run: {len(df)}")
    print(f"Line sizes tested: {sorted(df['Line_Size'].unique())}")
    print(f"Memory generators tested: {list(df['Memory_Generator'].unique())}\n")
    
    best_cpi = df.loc[df['CPI'].idxmin()]
    worst_cpi = df.loc[df['CPI'].idxmax()]
    
    print("=== BEST PERFORMANCE ===")
    print(f"Line Size: {best_cpi['Line_Size']}B")
    print(f"Generator: {best_cpi['Memory_Generator']}")
    print(f"CPI: {best_cpi['CPI']:.4f}\n")
    
    print("=== WORST PERFORMANCE ===")
    print(f"Line Size: {worst_cpi['Line_Size']}B")
    print(f"Generator: {worst_cpi['Memory_Generator']}")
    print(f"CPI: {worst_cpi['CPI']:.4f}\n")
    
    print("=== AVERAGE CPI BY LINE SIZE ===")
    line_size_avg = df.groupby('Line_Size')['CPI'].mean()
    for line_size, avg_cpi in line_size_avg.items():
        print(f"{line_size}B: {avg_cpi:.4f}")
    
    print("\n=== AVERAGE CPI BY MEMORY GENERATOR ===")
    generator_avg = df.groupby('Memory_Generator')['CPI'].mean()
    for generator, avg_cpi in generator_avg.items():
        print(f"{generator}: {avg_cpi:.4f}")

def main():
    """Main function to run the analysis"""
    print("Loading simulation data...")
    df = load_data()
    
    print("Generating graphs...")
    create_cpi_vs_line_size_graph(df)
    create_heatmap(df)
    create_individual_graphs(df)
    
    print("Printing statistics...")
    print_statistics(df)
    
    print("\nAnalysis complete! Generated files:")
    print("- cpi_vs_line_size.png")
    print("- cpi_heatmap.png") 
    print("- individual_patterns.png")

if __name__ == "__main__":
    main() 