#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


/*
 In this document, there is the solution to the second problem in homework 2.
 It solves classic nuts and bolts (in this case, locks and keys) matching problem
    using divide and conquer approach in O(n * logn) time. Solution progressively
    sorts both of the locks and keys arrays by partitioning them using other
    array's element as pivot and continues to do so by recursively calling itself
    on the ranges that are to the left and right of the newly found pivot position.
*/

/*
 @brief Prints a message and an array

 @param msg Message to print before printing array
 @param arr Pointer to the first element of an array to be printed
 @param len Number of elements in array

 @return
*/
void printArr(char *msg, int *arr, int len) {
  int i;
  printf("%s: ", msg);
  for (i = 0; i < len; ++i) {
    printf("%d ", arr[i]);
  }
  printf("\n");
}

/*
 @brief Swaps values pointed by a and b

 @param a Pointer to first value to be swapped
 @param b Pointer to second value to be swapped

 @return
*/
void swap(int *a, int *b) {
  int tmp = *a;
  *a = *b;
  *b = tmp;
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
  int i = start;
  int j = start;
  while (j < end) {
    int isPivot = 0;
    if (arr[j] == pivot) {
      isPivot = 1;
      swap(&arr[j], &arr[end]);
      swap(&ids[j], &ids[end]);
    } else if (arr[j] < pivot) {
      swap(&arr[i], &arr[j]);
      swap(&ids[i], &ids[j]);
      ++i;
    }
    if (!isPivot) {
      ++j;
    }
  }
  swap(&arr[i], &arr[end]);
  swap(&ids[i], &ids[end]);
  return i;
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
  int randKeyPos;
  int keyPivot;
  int lockPivotPos;
  int lockPivot;
  if (start >= end) {
    return;
  }
  randKeyPos = start + rand() % (end - start);
  keyPivot = key[randKeyPos];
  lockPivotPos = partition(lock, lockIds, keyPivot, start, end);
  lockPivot = lock[lockPivotPos];
  partition(key, keyIds, lockPivot, start, end);
  sort(key, keyIds, lock, lockIds, start, lockPivotPos - 1);
  sort(key, keyIds, lock, lockIds, lockPivotPos + 1, end);
}

/*
 @brief Allocates an array and fills it with values starting
        with 0 and ending with n - 1 in increasing order

 @param n Number of elements that array should hold

 @return Pointer to the newly allocated array
*/
int *makeIds(int n) {
  int i;
  int *ids = malloc(n * sizeof(int));
  if (!ids) {
    fprintf(stderr, "malloc failed");
    exit(EXIT_FAILURE);
  }
  for (i = 0; i < n; ++i) {
    ids[i] = i;
  }
  return ids;
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
    printf("key %d <-> lock %d\n", keyIds[i], lockIds[i]);
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
  if (isAfter) {
    printf("After:\n");
  } else {
    printf("Before:\n");
  }
  printArr("lock array", lock, n);
  printArr("lock id array", lockIds, n);
  printArr("key array", key, n);
  printArr("key id array", keyIds, n);
  printArr("expected sorted array", sorted, n);
  if (isAfter) {
    printf("\nMatches:\n");
    printMatches(lockIds, keyIds, n);
  }
  printf("\n");
}

/*
 @brief Main function that calls matching sort function with different inputs and
        prints relevant information about inputs and outputs
 @return Returns 0, indicating that process exited succesfully, if everytihg went ok
*/
int main() {
  srand((unsigned)time(0));
  {
    int lock[] = {2, 1, 4, 5, 3};
    int key[] = {5, 2, 4, 3, 1};
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
