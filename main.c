#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif

double nowMs() {
#ifdef _WIN32
    LARGE_INTEGER freq, cnt;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&cnt);
    return cnt.QuadPart * 1000.0 / freq.QuadPart;
#else
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return ts.tv_sec * 1000.0 + ts.tv_nsec / 1000000.0;
#endif
}

int comp = 0;
int max_mem = 0;

int orig[20000];
int arr[20000];
int temp[20000];

void heapify(int n, int i, int depth) {
    int mem = 12 + depth * 28;
    if (mem > max_mem) max_mem = mem;

    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n) {
        comp++;
        if (arr[left] > arr[largest]) {
            largest = left;
        }
    }

    if (right < n) {
        comp++;
        if (arr[right] > arr[largest]) {
            largest = right;
        }
    }

    if (largest != i) {
        int t = arr[i];
        arr[i] = arr[largest];
        arr[largest] = t;

        heapify(n, largest, depth + 1);
    }
}

void heapSort(int n) {
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(n, i, 1);
    }

    for (int i = n - 1; i > 0; i--) {
        int t = arr[0];
        arr[0] = arr[i];
        arr[i] = t;

        heapify(i, 0, 1);
    }
}

void merge(int left, int mid, int right) {
    int i = left;
    int j = mid + 1;
    int k = left;

    while (i <= mid && j <= right) {
        comp++;
        if (arr[i] <= arr[j]) {
            temp[k] = arr[i];
            i++;
        }
        else {
            temp[k] = arr[j];
            j++;
        }
        k++;
    }

    while (i <= mid) {
        temp[k] = arr[i];
        i++;
        k++;
    }

    while (j <= right) {
        temp[k] = arr[j];
        j++;
        k++;
    }

    for (i = left; i <= right; i++) {
        arr[i] = temp[i];
    }
}

void mergeSort(int left, int right, int n, int depth) {
    int mem = n * 4 + depth * 20 + 24;
    if (mem > max_mem) max_mem = mem;

    if (left < right) {
        int mid = (left + right) / 2;
        mergeSort(left, mid, n, depth + 1);
        mergeSort(mid + 1, right, n, depth + 1);
        merge(left, mid, right);
    }
}

int getPivot(int low, int high, int type) {
    if (type == 1) {
        return low;
    }
    else if (type == 2) {
        return low + rand() % (high - low + 1);
    }
    else {
        int mid = (low + high) / 2;
        int a = arr[low];
        int b = arr[mid];
        int c = arr[high];

        comp++;
        if (a < b) {
            comp++;
            if (b < c) return mid;
            comp++;
            if (a < c) return high;
            else return low;
        }
        else {
            comp++;
            if (a < c) return low;
            comp++;
            if (b < c) return high;
            else return mid;
        }
    }
}

int partition(int low, int high, int type) {
    int pIndex = getPivot(low, high, type);

    int t = arr[low];
    arr[low] = arr[pIndex];
    arr[pIndex] = t;

    int pivot = arr[low];
    int i = low + 1;

    for (int j = low + 1; j <= high; j++) {
        comp++;
        if (arr[j] < pivot) {
            t = arr[i];
            arr[i] = arr[j];
            arr[j] = t;
            i++;
        }
    }

    t = arr[low];
    arr[low] = arr[i - 1];
    arr[i - 1] = t;

    return i - 1;
}

void quickSort(int low, int high, int type, int depth) {
    int mem = depth * 20 + 32 + 28;
    if (mem > max_mem) max_mem = mem;

    if (low < high) {
        int pi = partition(low, high, type);
        quickSort(low, pi - 1, type, depth + 1);
        quickSort(pi + 1, high, type, depth + 1);
    }
}

void copyArr(int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = orig[i];
    }
}

int isSorted(int n) {
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) return 0;
    }
    return 1;
}

int main(void) {
    srand(12345);
    int sizes[6] = { 1000, 2000, 4000, 8000, 16000, 20000 };
    double start, end;
    double time_ms;

    for (int s = 0; s < 6; s++) {
        int n = sizes[s];

        for (int i = 0; i < n; i++) {
            orig[i] = rand() % 100000;
        }

        printf("Data Size : %d\n\n", n);
        printf("Algorithm       Comparisons    Time(ms)    Memory(Bytes)\n");
        printf("--------------------------------------------------------\n");

        copyArr(n);
        comp = 0;
        max_mem = 0;
        start = nowMs();
        heapSort(n);
        end = nowMs();
        time_ms = end - start;
        if (isSorted(n) == 0) printf("Error!\n");
        printf("Heap Sort       %-14d %-11.3f %d\n", comp, time_ms, max_mem);

        copyArr(n);
        comp = 0;
        max_mem = 0;
        start = nowMs();
        mergeSort(0, n - 1, n, 1);
        end = nowMs();
        time_ms = end - start;
        if (isSorted(n) == 0) printf("Error!\n");
        printf("Merge Sort      %-14d %-11.3f %d\n", comp, time_ms, max_mem);

        copyArr(n);
        comp = 0;
        max_mem = 0;
        start = nowMs();
        quickSort(0, n - 1, 1, 1);
        end = nowMs();
        time_ms = end - start;
        if (isSorted(n) == 0) printf("Error!\n");
        printf("Quick (First)   %-14d %-11.3f %d\n", comp, time_ms, max_mem);

        copyArr(n);
        comp = 0;
        max_mem = 0;
        start = nowMs();
        quickSort(0, n - 1, 2, 1);
        end = nowMs();
        time_ms = end - start;
        if (isSorted(n) == 0) printf("Error!\n");
        printf("Quick (Random)  %-14d %-11.3f %d\n", comp, time_ms, max_mem);

        copyArr(n);
        comp = 0;
        max_mem = 0;
        start = nowMs();
        quickSort(0, n - 1, 3, 1);
        end = nowMs();
        time_ms = end - start;
        if (isSorted(n) == 0) printf("Error!\n");
        printf("Quick (Median3) %-14d %-11.3f %d\n\n\n", comp, time_ms, max_mem);
    }

    return 0;
}