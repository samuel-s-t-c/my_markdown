#include <vector>
using namespace::std;
void bubble_sort(vector<int> &arr)
{
  bool is_sorted;
  for (int i = arr.size() - 1; i > 0; --i) {
    is_sorted = true;
    for (int j = 0; j < i; ++i) {
      if (arr[j] > arr[j+1]) {
        swap(arr[j], arr[j+1]);
        is_sorted = false;
      }
    }
    if (is_sorted) break;
  }
}
void quick_sort(vector<int> &arr, int l, int r)
{
  if (l < r) {
    int i = l, j = r, x = arr[i];
    while (i < j) {
      while (i < j && arr[j] > x) {j--;}
      if (i < j) arr[i++] = arr[j];
      while (i < j && arr[i] < x) {i++;}
      if (i < j) arr[j--] = arr[i];
    }
    arr[i] = x;
    quick_sort(arr, l, i-1);
    quick_sort(arr, i+1, r);
  }
}

void insert_sort(vector<int> &arr)
{
  for (int i = 1; i < arr.size(); ++i) {
    int j = i - 1;
    int tmp = arr[i];
    while (j >= 0 && arr[j] > tmp) {
      arr[j+1] = arr[j];
      --j;
    }
    arr[j+1] = tmp;
  }
}

void shell_sort(vector<int> &arr)
{
  int size = arr.size();
  for (int gap = size / 2; gap > 0; gap /= 2) {
    for (int i = 0; i < gap; ++i) {
      for (int j = i + gap; j < size; j += gap) {
        int k = j-gap;
        int tmp = arr[j];
        while (k >= 0 && arr[k] > tmp) {
          arr[k+gap] = arr[k];
          k -= gap;
        }
        arr[k+gap] = tmp;
      }
    }
  }
}

void radix_sort(vector<int> &arr)
{
  int max_val = 0;
  vector<int> bucket(10);
  vector<int> after(arr.size());
  for (vector<int>::size_type i = 0; i < arr.size(); ++i) {
    max_val = max(max_val, arr[i]);
  }
  for (int exp = 1; (max_val/exp) > 0; exp *= 10) {
    for (int i = 0; i < 10; ++i) bucket[i] = 0;
    for (vector<int>::size_type i = 0; i < arr.size(); ++i) {
      bucket[(arr[i]/exp)%10] += 1;
    }
    for (int i = 1; i < 10; ++i) {
      bucket[i] += bucket[i-1];
    }
    for (int i = (int)arr.size() - 1; i >= 0; --i) {
      after[--bucket[(arr[i]/exp)%10]] = arr[i];
    }
    arr.swap(after);
  }
}

void maxheap_down(vector<int> &arr, int beg, int end)
{
  int curr = beg, child = 2 * curr + 1, curr_val = arr[curr];
  while (child < end) {
    if (child + 1 < end && arr[child] < arr[child+1]) {
      child++;
    }
    if (curr_val >= arr[child]) break;
    else {
      swap(arr[curr], arr[child]);
    }
    curr = child;
    child = 2 * child + 1;
  }
}

void heap_sort(vector<int> &arr)
{
  int size = arr.size();
  for (int i = size / 2 - 1; i >= 0; --i) {
    maxheap_down(arr, i, size);
  }
  for (int i = size - 1; i > 0; --i) {
    swap(arr[i], arr[0]);
    maxheap_down(arr, 0, i);
  }
}
