/**
 * BÀI THỰC NGHIỆM CÁC THUẬT TOÁN SẮP XẾP (WEEK 4)
 * Môn: Cấu trúc dữ liệu và Giải thuật (IT003.R17)
 * Họ và tên: Hoàng Vũ Minh Huy
 * MSSV: 21522142
 * 
 * Các thuật toán thử nghiệm:
 * 1. QuickSort (Randomized Pivot)
 * 2. HeapSort
 * 3. MergeSort
 * 4. C++ std::sort (Introsort)
 *
 * Bộ dữ liệu:
 * - 10 dãy số thực double (1.000.000 phần tử/dãy)
 * - Dãy 1: Tăng dần
 * - Dãy 2: Giảm dần
 * - Dãy 3 - 10: Ngẫu nhiên
 */

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <fstream>

using namespace std;
using namespace std::chrono;

// ============================================================================
// 1. CÁC HÀM SẮP XẾP (SORTING ALGORITHMS)
// ============================================================================

// ------------------- QUICK SORT (Randomized Pivot) -------------------
int partition(vector<double>& arr, int low, int high) {
    // Chọn pivot ngẫu nhiên để tránh trường hợp xấu O(N^2) khi mảng đã tăng/giảm dần
    int randomPivot = low + rand() % (high - low + 1);
    swap(arr[randomPivot], arr[high]);

    double pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSortHelper(vector<double>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSortHelper(arr, low, pi - 1);
        quickSortHelper(arr, pi + 1, high);
    }
}

void quickSort(vector<double>& arr) {
    if (arr.empty()) return;
    quickSortHelper(arr, 0, arr.size() - 1);
}

// ------------------- HEAP SORT -------------------
void heapify(vector<double>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;

    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(vector<double>& arr) {
    int n = arr.size();
    // Dựng Max-Heap
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    // Lần lượt trích xuất phần tử từ Heap
    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

// ------------------- MERGE SORT -------------------
void merge(vector<double>& arr, int left, int mid, int right, vector<double>& temp) {
    int i = left;
    int j = mid + 1;
    int k = left;

    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }

    while (i <= mid) {
        temp[k++] = arr[i++];
    }

    while (j <= right) {
        temp[k++] = arr[j++];
    }

    for (int idx = left; idx <= right; idx++) {
        arr[idx] = temp[idx];
    }
}

void mergeSortHelper(vector<double>& arr, int left, int right, vector<double>& temp) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortHelper(arr, left, mid, temp);
        mergeSortHelper(arr, mid + 1, right, temp);
        merge(arr, left, mid, right, temp);
    }
}

void mergeSort(vector<double>& arr) {
    if (arr.empty()) return;
    vector<double> temp(arr.size());
    mergeSortHelper(arr, 0, arr.size() - 1, temp);
}

// ------------------- C++ std::sort -------------------
void cppStdSort(vector<double>& arr) {
    std::sort(arr.begin(), arr.end());
}

// ============================================================================
// 2. TẠO BỘ DỮ LIỆU THỰC NGHIỆM
// ============================================================================

vector<vector<double>> generateDatasets(int numDatasets = 10, int size = 1000000) {
    vector<vector<double>> datasets(numDatasets, vector<double>(size));
    
    mt19937_64 rng(42); // Seed cố định đảm bảo tính tái lập (Reproducibility)
    uniform_real_distribution<double> dist(-1000000.0, 1000000.0);

    cout << "Dang tao " << numDatasets << " bo du lieu (" << size << " phan tu/day)..." << endl;

    for (int d = 0; d < numDatasets; d++) {
        for (int i = 0; i < size; i++) {
            datasets[d][i] = dist(rng);
        }

        if (d == 0) {
            // Dãy 1: Tăng dần
            std::sort(datasets[d].begin(), datasets[d].end());
            cout << " -> Day 1: Da sap xep TANG DAN" << endl;
        } else if (d == 1) {
            // Dãy 2: Giảm dần
            std::sort(datasets[d].begin(), datasets[d].end(), greater<double>());
            cout << " -> Day 2: Da sap xep GIAM DAN" << endl;
        } else {
            // Dãy 3-10: Ngẫu nhiên
            cout << " -> Day " << d + 1 << ": Trat tu NGAU NHIEN" << endl;
        }
    }
    return datasets;
}

// ============================================================================
// 3. ĐO THỜI GIAN VÀ CHẠY THỰC NGHIỆM
// ============================================================================

template <typename Func>
double measureExecutionTime(Func sortFunc, const vector<double>& originalData) {
    vector<double> dataCopy = originalData; // Tạo bản sao mảng để dữ liệu gốc không bị thay đổi
    
    auto start = high_resolution_clock::now();
    sortFunc(dataCopy);
    auto end = high_resolution_clock::now();

    duration<double, milli> elapsed = end - start;
    return elapsed.count();
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    const int NUM_DATASETS = 10;
    const int DATASET_SIZE = 1000000;

    vector<vector<double>> datasets = generateDatasets(NUM_DATASETS, DATASET_SIZE);

    cout << "\n=================================================================================\n";
    cout << "                KET QUA THUC NGHIEM CAC THUAT TOAN SAP XEP (ms)\n";
    cout << "=================================================================================\n";
    cout << left << setw(15) << "Bo du lieu" 
         << setw(18) << "QuickSort (ms)" 
         << setw(18) << "HeapSort (ms)" 
         << setw(18) << "MergeSort (ms)" 
         << setw(18) << "std::sort (ms)" << "\n";
    cout << "---------------------------------------------------------------------------------\n";

    double totalQuick = 0, totalHeap = 0, totalMerge = 0, totalStd = 0;

    ofstream csvFile("sorting_benchmark_results.csv");
    csvFile << "Dataset,QuickSort,HeapSort,MergeSort,std_sort\n";

    for (int i = 0; i < NUM_DATASETS; i++) {
        string name = "Day " + to_string(i + 1);
        if (i == 0) name += " (Tang)";
        else if (i == 1) name += " (Giam)";

        double tQuick = measureExecutionTime(quickSort, datasets[i]);
        double tHeap  = measureExecutionTime(heapSort, datasets[i]);
        double tMerge = measureExecutionTime(mergeSort, datasets[i]);
        double tStd   = measureExecutionTime(cppStdSort, datasets[i]);

        totalQuick += tQuick;
        totalHeap  += tHeap;
        totalMerge += tMerge;
        totalStd   += tStd;

        cout << left << setw(15) << name 
             << fixed << setprecision(2)
             << setw(18) << tQuick 
             << setw(18) << tHeap 
             << setw(18) << tMerge 
             << setw(18) << tStd << "\n";

        csvFile << "Day " << (i + 1) << "," << tQuick << "," << tHeap << "," << tMerge << "," << tStd << "\n";
    }

    double avgQuick = totalQuick / NUM_DATASETS;
    double avgHeap  = totalHeap / NUM_DATASETS;
    double avgMerge = totalMerge / NUM_DATASETS;
    double avgStd   = totalStd / NUM_DATASETS;

    cout << "---------------------------------------------------------------------------------\n";
    cout << left << setw(15) << "TRUNG BINH" 
         << fixed << setprecision(2)
         << setw(18) << avgQuick 
         << setw(18) << avgHeap 
         << setw(18) << avgMerge 
         << setw(18) << avgStd << "\n";
    cout << "=================================================================================\n";

    csvFile << "TRUNG BINH," << avgQuick << "," << avgHeap << "," << avgMerge << "," << avgStd << "\n";
    csvFile.close();

    cout << "\nDa xuat ket qua chi tiet ra file CSV: sorting_benchmark_results.csv" << endl;

    return 0;
}
