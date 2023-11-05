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

int partition(int *arr, int pivot, int start, int end) {
  int i = start;
  int j = start;
  while (j < end) {
    int isPivot = 0;
    if (arr[j] == pivot) {
      isPivot = 1;
      swap(&arr[j], &arr[end]);
    } else if (arr[j] < pivot) {
      swap(&arr[i], &arr[j]);
      ++i;
    }
    if (!isPivot) {
      ++j;
    }
  }
  swap(&arr[i], &arr[end]);
  return i;
}

void sort(int *key, int *lock, int start, int end) {
  int randKeyPos;
  int keyPivot;
  int lockPivotPos;
  int lockPivot;
  if (start >= end) {
    return;
  }
  randKeyPos = start + rand() % (end - start);
  keyPivot = key[randKeyPos];
  lockPivotPos = partition(lock, keyPivot, start, end);
  lockPivot = lock[lockPivotPos];
  partition(key, lockPivot, start, end);
  sort(key, lock, start, lockPivotPos - 1);
  sort(key, lock, lockPivotPos + 1, end);
}

void assertArrEq(int *arr1, int *arr2, int len) {
  int i;
  for (i = 0; i < len; ++i) {
    assert(arr1[i] == arr2[i]);
  }
}

int main() {
  srand((unsigned)time(0));
  {
    int lock[] = {2, 1, 4, 5, 3};
    int key[] = {5, 2, 4, 3, 1};
    int n = sizeof(lock) / sizeof(lock[0]);
    int sorted[] = {1, 2, 3, 4, 5};
    printArr("key array before match", key, n);
    printArr("lock array before match", lock, n);
    sort(key, lock, 0, n - 1);
    printArr("key array after match", key, n);
    printArr("lock array after match", lock, n);
    assertArrEq(lock, sorted, n);
    assertArrEq(key, sorted, n);
    printf("\n");
  }
  {
    int lock[] = {4, 3, 5, 1, 2};
    int key[] = {2, 5, 4, 3, 1};
    int n = sizeof(lock) / sizeof(lock[0]);
    int sorted[] = {1, 2, 3, 4, 5};
    printArr("key array before match", key, n);
    printArr("lock array before match", lock, n);
    sort(key, lock, 0, n - 1);
    printArr("key array after match", key, n);
    printArr("lock array after match", lock, n);
    assertArrEq(lock, sorted, n);
    assertArrEq(key, sorted, n);
    printf("\n");
  }
  return 0;
}
