#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>


#define SIZE 100000

typedef struct {
    double time_l;
    double time_h;
    double time_a;
} Time;


void swap(int* x, int* y) {
	int tmp = (tmp = *x, *x=*y, *y = tmp);
}


void print(int* arr, int len) {
	for (int i = 0; i < len; i++) {
		printf("%d ", arr[i]);
	}
	putchar(10);
}

void l_partition(int* arr, int left, int right, int* ret_l, int* ret_r) {
	//Lomuto partition
    *ret_l = left;
    *ret_r = right;
    if (left + 1 == right) {
        if (arr[left] > arr[right]) {
            swap(arr + left, arr + right);
        }
        return;
    }
    if (left == right) {
        return;
    }
    int r = rand();
    int x = left + rand() % (right - left + 1);
    int pivot = arr[x];
    swap(arr + left, arr + x);
    int l = left, h = left, c = left + 1;
    while (c <= right) {
        if (arr[c] < pivot) {
            int tmp = arr[c];
            arr[c++] = arr[++h];
            arr[h] = arr[l];
            arr[l++] = tmp;
        } else if (arr[c] == pivot) {
            swap(arr + h++ + 1, arr + c++);
        } else {
            c++;
        }
    }
    *ret_l = l - 1;
    *ret_r = h + 1;
}


void l_quicksort(int* nums, int left, int right) {
	//quicksort with Lomuto partition
    int new_left, new_right;
    l_partition(nums, left, right, &new_left, &new_right);
    if (new_left > left) {
        l_quicksort(nums, left, new_left);
    }
    if (new_right < right) {
        l_quicksort(nums, new_right, right);
    }
}

void h_partition(int* arr, int left, int right, int* ret_l, int* ret_r) {
	//Hoare partition
    int x = left + rand() % (right - left + 1);
    int pivot = arr[x];
    int i = left, j = right;
    while (i <= j) {
        while (arr[i] < pivot) {
            i++;
        }
        while (arr[j] > pivot) {
            j--;
        }
        if (i >= j) {
            break;
        }
        swap(arr + i++, arr + j--);
    }
    *ret_l = j;
    while (arr[i] == pivot && i < right) {
        i++;
    }
    *ret_r = i;
}


void h_quicksort(int* nums, int left, int right) {
	//quicksort with Hoare partition
    int new_left, new_right;
    h_partition(nums, left, right, &new_left, &new_right);
    if (new_left > left) {
        h_quicksort(nums, left, new_left);
    }
    if (new_right < right) {
        h_quicksort(nums, new_right, right);
    }
}

int* a_partition(int* first, int* last) {
	//Alexandrescu partition
    assert(first <= last);
    if (last - first < 2) {
        return first;
    }
    last--;
    if (*first > *last) {
       swap(first, last);
    }
    int* pivot_pos = first;
    int pivot = *first;
    do {
        first++;
    assert(first <= last);
    } while(*first < pivot);
    for (int* i = first + 1; i < last; i++) {
            int x = *i;
            int smaller = -(int)(x < pivot);
            int delta = smaller & (i - first);
            first[delta] = *first;
            i[-delta] = x;
            first -= smaller;
        }
    assert(*first >= pivot);
    first--;
    *pivot_pos = *first;
    *first = pivot;
    return first;
}

void a_quicksort(int* left, int* right) {
	//quicksort with Alexandrescu partition
   if (right - left > 1)  {
        int* pivot = a_partition(left, right);
        a_quicksort(left, pivot);
        a_quicksort(pivot + 1, right);
    }
}

bool correctness_check(int* ptr1, int* ptr2, int* ptr3, int len) {
	for (int i = 0; i < len; i++) {
		assert(ptr1[i] == ptr2[i] && ptr2[i] == ptr3[i]);
	}
	return true;
}

double measure_lq(int* arr, int len) {
	clock_t start = clock();
	l_quicksort(arr, 0, len - 1);
	clock_t end = clock();
	return (end - start) / CLOCKS_PER_SEC;
}


double measure_hq(int* arr, int len) {
	clock_t start = clock();
	h_quicksort(arr, 0, len - 1);
	clock_t end = clock();
	return (end - start) / CLOCKS_PER_SEC;
}


double measure_aq(int* arr, int len) {
	clock_t start = clock();
	a_quicksort(arr, arr + len);
	clock_t end = clock();
	return (end - start) / CLOCKS_PER_SEC;
}

Time test1() {
	int len = SIZE;
	int* arr = malloc(len * sizeof(int));
	for (int i = 0; i < len; i++) {
		arr[i] = i;
	}
	int* ptr1 = malloc(len * sizeof(int));
	memcpy(ptr1, arr, len * sizeof(int));
	int* ptr2 = malloc(len * sizeof(int));
	memcpy(ptr2, arr, len * sizeof(int));
	int* ptr3 = malloc(len * sizeof(int));
	memcpy(ptr3, arr, len * sizeof(int));
	Time res;
	res.time_l = measure_lq(ptr1, len);
	res.time_h = measure_hq(ptr2, len);
	res.time_a = measure_aq(ptr3, len);
	correctness_check(ptr1, ptr2, ptr3, len);
	free(ptr1); free(ptr2); free(ptr3); free(arr);
	return res;
}


Time test2() {
	int len = SIZE;
	int* arr = malloc(len * sizeof(int));
	for (int i = 0; i < len; i++) {
		arr[i] = len - i;
	}
	int* ptr1 = malloc(len * sizeof(int));
	memcpy(ptr1, arr, len * sizeof(int));
	int* ptr2 = malloc(len * sizeof(int));
	memcpy(ptr2, arr, len * sizeof(int));
	int* ptr3 = malloc(len * sizeof(int));
	memcpy(ptr3, arr, len * sizeof(int));
	Time res;
	res.time_l = measure_lq(ptr1, len);
	res.time_h = measure_hq(ptr2, len);
	res.time_a = measure_aq(ptr3, len);
	correctness_check(ptr1, ptr2, ptr3, len);
	free(ptr1); free(ptr2); free(ptr3); free(arr);
	return res;
}


Time test3() {
	int len = SIZE;
	int* arr = malloc(len * sizeof(int));
	for (int i = 0; i < len; i++) {
		arr[i] = rand() % 256;
	}
	int* ptr1 = malloc(len * sizeof(int));
	memcpy(ptr1, arr, len * sizeof(int));
	int* ptr2 = malloc(len * sizeof(int));
	memcpy(ptr2, arr, len * sizeof(int));
	int* ptr3 = malloc(len * sizeof(int));
	memcpy(ptr3, arr, len * sizeof(int));
	Time res;
	res.time_l = measure_lq(ptr1, len);
	res.time_h = measure_hq(ptr2, len);
	res.time_a = measure_aq(ptr3, len);
	correctness_check(ptr1, ptr2, ptr3, len);
	free(ptr1); free(ptr2); free(ptr3); free(arr);
	return res;
}


int main() {
	srand(time(NULL));
	Time first = test1();
	printf("%f %f %f\n", first.time_l, first.time_h, first.time_a);
	Time second = test2();
	printf("%f %f %f\n", second.time_l, second.time_h, second.time_a);
	Time third = test3();
	printf("%f %f %f\n", third.time_l, third.time_h, third.time_a);
	return 0;
}
