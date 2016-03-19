# Multithreaded Sorting Application
This is a multithreaded sorting program that works as follows:
* A list of integers is divided into two smaller lists of equal size.
* Two seperate threads (which we will term sorting threads) sort each
  sublist using a sorting algorithm. In this case, I chose to use a
  heap sort.
* The two sublists are then merged by a third thread - a merging thread
  - which merges the two sublists into a single sorted list.  

The list to be sorted comes from a seperate file referenced in the 
calling argument.

		./threadSort unsortedlist.txt