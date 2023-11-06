#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 In this document, there is the solution to the second problem in homework 2.
 It solves classic nuts and bolts (in this case, locks and keys) matching
 problem using divide and conquer approach in O(n * logn) time. Solution
 progressively sorts both of the locks and keys arrays by partitioning them
 using other array's element as pivot and continues to do so by recursively
 calling itself on the ranges that are to the left and right of the newly found
 pivot position.
*/

/*
 @brief Prints a message and an array

 @param msg Message to print before printing array
 @param arr Pointer to the first element of an array to be printed
 @param len Number of elements in array

 @return
*/
void printArr(char *msg, int *arr, int len) {
  int i;               // Counter that will be used to iterate over the array
  printf("%s: ", msg); // Print the message by using formatting
  for (i = 0; i < len; ++i) { // Iterate over the array
    printf("%d ", arr[i]);    // Print i'th element of the array
  }
  printf("\n"); // Print newline
}

/*
 @brief Swaps values pointed by a and b

 @param a Pointer to first value to be swapped
 @param b Pointer to second value to be swapped

 @return
*/
void swap(int *a, int *b) {
  int tmp = *a; // Put value pointed by a to a temporary variable
  *a = *b; // Update value of memory location pointed by a to value pointed by b
  *b = tmp; // Update value of b
}

/*
 @brief partitions an array using pivot argument and
        when swapping elements of the array, also swaps elements of ids

 @param arr Array to be partitioned
 @param ids Array that holds ids of elements in the array
 @param pivot Pivot value that is used to partition the array
 @param start Inclusive starting index of partition range
 @param end Inclusive ending index of partition range

 @return Index that pivot is placed
*/
int partition(int *arr, int *ids, int pivot, int start, int end) {
  int i = start; // Index on the left side that will be used to swap arr[i] and
                 // arr[j]
  int j = start; // Index on the right side that looks for a
                 // value that is lower than or equal to the pivot value
  while (j < end) { // Last element will be the pivot value during the loop, so
                    // iterate until j is equal to end
    int isPivot = 0;       // Value that is 1 if arr[j] is pivot value itself
    if (arr[j] == pivot) { // Check if it is pivot value
      isPivot = 1;         // Set isPivot to 1 to be later used
      swap(&arr[j],
           &arr[end]); // Swap current index (that has pivot value) with end of
                       // array, this is like changing the array so that it
                       // looks more like other partition schemes where pivot is
                       // the last element
      swap(&ids[j], &ids[end]); // Whenever something is swapped in the array,
                                // corresponding indices must also be swapped in
                                // id array to not lose id information
    } else if (arr[j] <
               pivot) { // Check if right iterator has value less than pivot
      swap(&arr[i],
           &arr[j]); // Swap indices i and j, i is wanted to be brought
                     // to over the element that is more than pivot,
                     // and j is brought to over element that has is less
      swap(&ids[i], &ids[j]);
      ++i; // Update i to point to the next item so that it may point to larger
           // value than pivot
    }
    if (!isPivot) { // Check if value looked at in this iteration was pivot
      ++j; // Update j to the next item, do not update if it was pivot because,
           // it is not wanted to go over the newly brought value from end of
           // array
    }
  }
  swap(&arr[i], &arr[end]); // Insert the pivot from end of array to its correct
                            // position i
  swap(&ids[i], &ids[end]);
  return i; // Return correct pivot position of pivot value
}

/*
 @brief sorts keys and locks using quicksort with random pivot selection,
        and arranges id arrays so that {keyIds[i], lockIds[i]} is a matching
        key-lock pair

 @param key Pointer to first element of keys array
 @param keyIds Pointer to first element of array that holds key ids
 @param lock Pointer to first element of locks array
 @param lockIds Pointer to first element of array that holds lock ids
 @param start Inclusive start of range that is sorted
 @param end Inclusive end of range that is sorted

 @return
*/
void sort(int *key, int *keyIds, int *lock, int *lockIds, int start, int end) {
  int randKeyPos;     // Random index to choose a key
  int keyPivot;       // Value of the random pivot from keys
  int lockPivotPos;   // Index of the pivot from locks
  int lockPivot;      // Value of the pivot from locks
  if (start >= end) { // Range has no elements, return
    return;
  }
  randKeyPos = start + rand() % (end - start); // Choose a random index
                                               // from the range [start, end]
  keyPivot = key[randKeyPos]; // Take value of random pivot from keys
  lockPivotPos = partition(lock, lockIds, keyPivot, start,
                           end);  // Partition the locks array
                                  // using the pivot value from keys,
                                  // and take correct position of pivot
  lockPivot = lock[lockPivotPos]; // Using the newly found position of pivot in
                                  // locks, take its value
  partition(key, keyIds, lockPivot, start,
            end); // Partition the keys array
                  // using the pivot value from locks array,
                  // return value is no longer needed
  sort(key, keyIds, lock, lockIds, start,
       lockPivotPos - 1); // Call this function on left side of the pivot,
                          // index of the pivot is same in both keys and locks,
                          // because, all values are unique
  sort(key, keyIds, lock, lockIds, lockPivotPos + 1,
       end); // Call this function on right side of the pivot
}

/*
 @brief Allocates an array and fills it with values starting
        with 0 and ending with n - 1 in increasing order

 @param n Number of elements that array should hold

 @return Pointer to the newly allocated array
*/
int *makeIds(int n) {
  int i;                              // Iterator over the ids array
  int *ids = malloc(n * sizeof(int)); // Allocate n ints
  if (!ids) {                         // Exit if malloc fails
    fprintf(stderr, "malloc failed");
    exit(EXIT_FAILURE);
  }
  for (i = 0; i < n; ++i) {
    ids[i] = i; // Set each element of ids to its index
  }
  return ids; // Return pointer to first element of ids array
}

/*
 @brief Prints key-lock matchings

 @param lockIds Pointer to first element of array that holds lock ids
 @param keyIds Pointer to first element of array that holds key ids
 @param n Number of elements in lockIds and keyIds

 @return
*/
void printMatches(int *lockIds, int *keyIds, int n) {
  int i;
  for (i = 0; i < n; ++i) {
    printf("key %d <-> lock %d\n", keyIds[i],
           lockIds[i]); // keyIds[i] and lockIds[i]
                        // are ids of the key and lock that match
  }
}

/*
 @brief Prints all inputs to the matching sort function and resulting matches

 @param lock Locks array
 @param lockIds Lock ids array
 @param key Keys array
 @param keyIds Key ids array
 @param sorted Array that holds sorted version of locks and keys arrays
 @param n Number of elements in lock, lockIds, key, keyIds, and sorted
 @param isAfter Value that holds 1 if function is called after sort function is
 run, 0 if it is called before

 @return
*/
void printAll(int *lock, int *lockIds, int *key, int *keyIds, int *sorted,
              int n, int isAfter) {
  if (isAfter) { // Print corresponding message depending on whether
                 // function is called before or after
    printf("After:\n");
  } else {
    printf("Before:\n");
  }
  printArr("lock array", lock, n);
  printArr("lock id array", lockIds, n);
  printArr("key array", key, n);
  printArr("key id array", keyIds, n);
  printArr("expected sorted array", sorted, n);
  if (isAfter) { // If function is called after matching is done, print matches
    printf("\nMatches:\n");
    printMatches(lockIds, keyIds, n);
  }
  printf("\n");
}

/*
 @brief Main function that calls matching sort function with different inputs
 and prints relevant information about inputs and outputs
 @return Returns 0, indicating that process exited succesfully, if everytihg
 went ok
*/
int main() {
  srand((unsigned)time(0)); // Seed random number generator using time function
  {
    int lock[] = {2, 1, 4, 5,
                  3}; // Example lock values that represent radius of the locks
    int key[] = {5, 2, 4, 3, 1};
    int n =
        sizeof(lock) / sizeof(lock[0]); // Number of elements in locks and keys
    int sorted[] = {1, 2, 3, 4, 5};     // Sorted expected output
    int *lockIds =
        makeIds(n); // Initialize ids with increasing numbers in [0, n - 1]
    int *keyIds = makeIds(n);
    printAll(lock, lockIds, key, keyIds, sorted, n, 0); // Print all inputs
    sort(key, keyIds, lock, lockIds, 0, n - 1); // Call the matcher function
    printAll(lock, lockIds, key, keyIds, sorted, n,
             1); // Print resulting state of inputs
    free(lockIds);
    free(keyIds);
  }
  {
    int lock[] = {4, 3, 5, 1, 2};
    int key[] = {2, 5, 4, 3, 1};
    int n = sizeof(lock) / sizeof(lock[0]);
    int sorted[] = {1, 2, 3, 4, 5};
    int *lockIds = makeIds(n);
    int *keyIds = makeIds(n);
    printAll(lock, lockIds, key, keyIds, sorted, n, 0);
    sort(key, keyIds, lock, lockIds, 0, n - 1);
    printAll(lock, lockIds, key, keyIds, sorted, n, 1);
    free(lockIds);
    free(keyIds);
  }
  return 0;
}
