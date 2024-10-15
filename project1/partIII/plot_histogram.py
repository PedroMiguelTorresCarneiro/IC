"""
THIS MAKE A PLOT BUT NOT EXACTLY AN HISTOGRAM

import sys
import matplotlib.pyplot as plt
import numpy as np

def plot_histogram_grayscale(data):
    # Parse the grayscale data into a list of integers
    grayscale_values = list(map(int, data.split()))
    
    # Plot the grayscale histogram
    plt.figure()
    plt.title("Grayscale Histogram")
    plt.xlabel("Pixel Intensity")
    plt.ylabel("Frequency")
    plt.plot(grayscale_values, color='black')
    plt.xlim([0, 255])
    plt.show()

def plot_histogram_bgr(data):
    # Parse the BGR data (each channel is separated by a comma)
    channels = data.split(",")
    if len(channels) != 3:
        print("Error: Expected 3 channels for BGR, but received:", len(channels))
        return
    
    blue_values = list(map(int, channels[0].split()))
    green_values = list(map(int, channels[1].split()))
    red_values = list(map(int, channels[2].split()))

    # Plot BGR histograms on the same plot with different colors
    plt.figure()
    plt.title("BGR Histogram")
    plt.xlabel("Pixel Intensity")
    plt.ylabel("Frequency")
    
    plt.plot(blue_values, color='blue', label='Blue Channel')
    plt.plot(green_values, color='green', label='Green Channel')
    plt.plot(red_values, color='red', label='Red Channel')
    
    plt.xlim([0, 255])
    plt.legend()
    plt.show()


if __name__ == "__main__":
    mode = sys.argv[1]  # First argument is the mode (grayscale or bgr)
    data = sys.argv[2]  # Second argument is the histogram data

    if mode == "grayscale":
        plot_histogram_grayscale(data)
    elif mode == "bgr":
        plot_histogram_bgr(data)
    else:
        print("Error: Unsupported mode. Use 'grayscale' or 'bgr'.")
"""

import matplotlib
matplotlib.use('TkAgg')  # Force matplotlib to use the TkAgg backend for rendering

import sys
import matplotlib.pyplot as plt
import numpy as np

def plot_histogram_bgr(data):
    # Parse the BGR data (each channel is separated by a comma)
    channels = data.split(",")
    if len(channels) != 3:
        print("Error: Expected 3 channels for BGR, but received:", len(channels))
        return
    
    blue_values = list(map(int, channels[0].split()))
    green_values = list(map(int, channels[1].split()))
    red_values = list(map(int, channels[2].split()))

    # Create an array of intensity values (0-255)
    bins = np.arange(257) - 0.5  # For histograms with integer bins

    # Plot histograms with transparency (alpha)
    plt.figure()
    plt.title("BGR Histogram")
    plt.xlabel("Pixel Intensity")
    plt.ylabel("Frequency")
    
    # Plot each channel's histogram with transparency
    plt.hist(np.arange(256), bins=bins, weights=blue_values, color='blue', alpha=0.5, label='Blue Channel')
    plt.hist(np.arange(256), bins=bins, weights=green_values, color='green', alpha=0.5, label='Green Channel')
    plt.hist(np.arange(256), bins=bins, weights=red_values, color='red', alpha=0.5, label='Red Channel')

    # Set the limits for the x-axis
    plt.xlim([0, 255])
    
    # Add a legend
    plt.legend()

    # Show the plot
    plt.show()

def plot_histogram_grayscale(data):
    # Parse grayscale histogram data
    grayscale_values = list(map(int, data.split()))
    
    # Create an array of intensity values (0-255)
    bins = np.arange(257) - 0.5  # For histograms with integer bins

    # Plot the grayscale histogram
    plt.figure()
    plt.title("Grayscale Histogram")
    plt.xlabel("Pixel Intensity")
    plt.ylabel("Frequency")
    
    # Plot histogram
    plt.hist(np.arange(256), bins=bins, weights=grayscale_values, color='gray')

    # Set the limits for the x-axis
    plt.xlim([0, 255])
    
    # Show the plot
    plt.show()
    
    
if __name__ == "__main__":
    mode = sys.argv[1]  # First argument is the mode (grayscale or bgr)
    data = sys.argv[2]  # Second argument is the histogram data

    if mode == "grayscale":
        plot_histogram_grayscale(data)
    elif mode == "bgr":
        plot_histogram_bgr(data)
    else:
        print("Error: Unsupported mode. Use 'grayscale' or 'bgr'.")