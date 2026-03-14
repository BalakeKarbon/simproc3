#!/bin/bash
gcc mainProject.c -o a.out
echo "1532 9 80 25 10 2" | ./a.out > run1_seed1532.txt
echo "223576 2 30 10 4 2" | ./a.out > run2_seed223576.txt
echo "404 3 35 35 30 5" | ./a.out > run3_seed404.txt
# Extra Credit
echo "404 3 35 35 30 5" | ./a.out > run4_edgecase.txt
