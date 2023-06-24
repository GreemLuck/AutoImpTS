#!/bin/bash

# Base directory
base_dir="post_processing"

# Dataset name
dataset_name="chlorine"

# For each algorithm
for algorithm in "$base_dir"/*; do
    # If it's not a directory, skip
    [ -d "$algorithm" ] || continue

    # Get just the name of the algorithm
    algorithm_name=$(basename "$algorithm")

    # Source file
    src_file="${algorithm}/${dataset_name}/results.txt"

    # Destination file
    dest_file="${algorithm_name}_${dataset_name}.txt"

    # If the source file exists, copy it to the destination
    if [ -f "$src_file" ]; then
        cp "$src_file" "$dest_file"
    else
        echo "No results.txt found in $src_file"
    fi
done

echo "Copying completed."