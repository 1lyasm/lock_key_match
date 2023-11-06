#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void printArr(char *msg, int *arr, int len) {
  int i;
  printf("%s: ", msg);
  for (i = 0; i < len; ++i) {
    printf("%d ", arr[i]);
  }
  printf("\n");
}

void swap(int *a, int *b) {
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

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

void printMatches(int *lockIds, int *keyIds, int n) {
    int i;
    for (i = 0; i < n; ++i) {
        printf("lock %d <-> key %d\n", lockIds[i], keyIds[i]);
    }
}

void printAll(int *lock, int *lockIds, int *key, int *keyIds, int *sorted, int n, int is_after) {
    if (is_after) {
        printf("After:\n");
    } else {
        printf("Before:\n");
    }
    printArr("lock array", lock, n);
    printArr("lock id array", lockIds, n);
    printArr("key array", key, n);
    printArr("key id array", keyIds, n);
    printArr("expected sorted array", sorted, n);
    if (is_after) {
        printf("\nMatches:\n");
        printMatches(lockIds, keyIds, n);
    }
    printf("\n");
}

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
