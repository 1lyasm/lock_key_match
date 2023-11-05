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

int partition(int *arr, int low, int high, int pivot) {
  int i = low;
  int j;
  for (j = low; j < high; ++j) {
    if (arr[j] == pivot) {
      swap(&arr[j], &arr[high]);
      --j;
    } else if (arr[j] < pivot) {
      swap(&arr[i], &arr[j]);
      ++i;
    }
  }
  swap(&arr[i], &arr[high]);
  return i;
}

void match(int *locks, int *keys, int low, int high) {
  int randKeyPos;
  int pivotPos;
  if (low >= high) {
    return;
  }
  randKeyPos = low + rand() % (high - low);
  pivotPos = partition(locks, low, high, keys[randKeyPos]);
  partition(keys, low, high, locks[pivotPos]);
  match(locks, keys, low, pivotPos - 1);
  match(locks, keys, pivotPos + 1, high);
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
    int locks[] = {2, 1, 4, 5, 3};
    int keys[] = {5, 2, 4, 3, 1};
    int n = sizeof(locks) / sizeof(locks[0]);
    int sorted[] = {1, 2, 3, 4, 5};
    printArr("keys before match", keys, n);
    printArr("locks before match", locks, n);
    match(locks, keys, 0, n - 1);
    printArr("keys after match", keys, n);
    printArr("locks after match", locks, n);
    assertArrEq(locks, sorted, n);
    assertArrEq(keys, sorted, n);
    printf("\n");
  }
  {
    int locks[] = {4, 3, 5, 1, 2};
    int keys[] = {2, 5, 4, 3, 1};
    int n = sizeof(locks) / sizeof(locks[0]);
    int sorted[] = {1, 2, 3, 4, 5};
    printArr("keys before match", keys, n);
    printArr("locks before match", locks, n);
    match(locks, keys, 0, n - 1);
    printArr("keys after match", keys, n);
    printArr("locks after match", locks, n);
    assertArrEq(locks, sorted, n);
    assertArrEq(keys, sorted, n);
    printf("\n");
  }
  return 0;
}
