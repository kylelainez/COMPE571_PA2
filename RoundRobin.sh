#!/bin/bash

sum=0
for i in {1..10}
do
    output=$(./RR.o | tail -n1)
    sum=$(echo "$sum + $output" | bc)
done

average=$(echo "scale=4; $sum / 10" | bc)

echo "10 run Average: $average"

