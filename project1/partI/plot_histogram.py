import matplotlib
matplotlib.use('TkAgg')  # Force matplotlib to use the TkAgg backend for rendering

import sys
import matplotlib.pyplot as plt

def plot_histogram(data, mode):
    # Convert the string data to a dictionary
    pairs = data.split(',')
    frequencies = {}
    for pair in pairs:
        key, value = pair.split(':')
        frequencies[key] = int(value)
    
    # Apply filtering based on mode
    if mode == 'word':
        frequencies = {k: v for k, v in frequencies.items() if v > 5}  # Words with frequency > 5
    elif mode == 'ngrams':
        frequencies = {k: v for k, v in frequencies.items() if v > 2}  # N-grams with frequency > 2
    
    # Prepare the data for the plot
    labels = list(frequencies.keys())
    values = list(frequencies.values())
    
    # Create the plot
    plt.figure()
    plt.bar(labels, values)
    plt.title(f"{mode.capitalize()} Frequency Histogram")
    plt.xlabel("Characters/Words/N-Grams")
    plt.ylabel("Frequency")
    
    # Set x-axis label rotation for words and N-grams
    if mode in ['word']:
        plt.xticks(rotation=45)  # Rotate labels by 45 degrees for readability
    elif mode in ['ngrams']:
        plt.xticks(rotation=90)
    else:
        plt.xticks(rotation=0)  # Keep horizontal for characters
    
    # Ensure proper layout
    plt.tight_layout()  
    plt.show()

if __name__ == "__main__":
    mode = sys.argv[1]  # First argument is the mode (char, word, or ngrams)
    data = sys.argv[2]  # Second argument is the frequency data

    plot_histogram(data, mode)
