Strategy:

I generate a random valid puzzle as a starting point. From this starting point, 
I iterate though all the cells and create (rows * columns - 1) neighbors, all 
with the (i,j) cell's value set to a random number. I then calculate the 
heuristic for all permutations and change to the max of these permutations if its
heuristic is greater than the starting point. I repeat until no permutations 
result in a higher heuristic. However, there is a 1/10th chance that we will 
continue to hill climb regardless.

I do this for multiple starting points until I run out of time. 