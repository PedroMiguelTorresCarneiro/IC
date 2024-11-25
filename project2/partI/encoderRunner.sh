#!/bin/bash

inputFile=$1
outputFile=$2
iterations=100
totalTime=0.0

for ((i=1; i<=iterations; i++))
do
    # Run the encoder and capture the output
    output=$(./build/encoder1 "$inputFile" "$outputFile")
    #output=$(./build/encoder "$inputFile" "$outputFile")
    #output=$(./build/encoderMeaning "$inputFile" "$outputFile")
    #output=$(./build/encoderMeaning1 "$inputFile" "$outputFile")

    # Extract the timing value using awk
    timeTaken=$(echo "$output" | awk '/Encoding completed in/ {print $4}')

    # Add the extracted time to the totalTime
    totalTime=$(echo "$totalTime + $timeTaken" | bc -l)

    # Print the extracted time for this run
    printf "Run %d: %.6f ms\n" "$i" "$timeTaken"
done

# Calculate and print the mean time as a floating-point value
meanTime=$(echo "scale=6; $totalTime / $iterations" | bc -l)
printf "Mean encoding time over %d runs: %.6f ms\n" "$iterations" "$meanTime"
