#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <random>

using namespace std;

// Performance Tracking
long long total_comparisons = 0;
long long total_swaps = 0;

const int BURST_THRESHOLD = 50; // Max items in a bucket before it "bursts"
const int ALPHABET_SIZE = 256;  // 8-bit chunks for integer bytes

// Structure for the Burst Sort Trie Node
struct Node {
    bool is_leaf;
    vector<int> bucket;             
    Node* children[ALPHABET_SIZE]; 

    Node() {
        is_leaf = true;
        for (int i = 0; i < ALPHABET_SIZE; i++) children[i] = nullptr;
    }
};

// Logic to extract a specific byte from an integer (0 = MSB, 3 = LSB)
int getByte(int value, int level) {
    int shift = (3 - level) * 8;
    return (value >> shift) & 0xFF;
}

// Manual Selection Sort for buckets (Descending) - No built-in functions
void manualSortBucket(vector<int>& bucket) {
    int n = bucket.size();
    for (int i = 0; i < n - 1; i++) {
        int max_idx = i;
        for (int j = i + 1; j < n; j++) {
            total_comparisons++;
            if (bucket[j] > bucket[max_idx]) { // Descending
                max_idx = j;
            }
        }
        if (max_idx != i) {
            total_swaps++;
            int temp = bucket[i];
            bucket[i] = bucket[max_idx];
            bucket[max_idx] = temp;
        }
    }
}

// Insert values into the Burst Sort structure
void insert(Node* root, int value, int level) {
    if (root->is_leaf) {
        root->bucket.push_back(value);
        total_swaps++; // Counting data move as a "swap" equivalent

        if (root->bucket.size() > BURST_THRESHOLD && level < 3) {
            root->is_leaf = false;
            vector<int> temp = root->bucket;
            root->bucket.clear();
            for (int val : temp) {
                int b = getByte(val, level);
                if (root->children[b] == nullptr) root->children[b] = new Node();
                insert(root->children[b], val, level + 1);
            }
        }
    } else {
        int b = getByte(value, level);
        if (root->children[b] == nullptr) root->children[b] = new Node();
        insert(root->children[b], value, level + 1);
    }
}

// Traverse the trie in reverse (255 down to 0) for Descending order
void collect(Node* node, vector<int>& sortedList) {
    if (node == nullptr) return;

    if (node->is_leaf) {
        manualSortBucket(node->bucket);
        for (int val : node->bucket) sortedList.push_back(val);
    } else {
        for (int i = ALPHABET_SIZE - 1; i >= 0; i--) { // Reverse order
            collect(node->children[i], sortedList);
        }
    }
}

void burstSort(vector<int>& arr) {
    if (arr.empty()) return;
    Node* root = new Node();
    for (int x : arr) insert(root, x, 0);
    arr.clear();
    collect(root, arr);
}

void runTest(vector<int> data, string label) {
    total_comparisons = 0;
    total_swaps = 0;
    auto start = chrono::high_resolution_clock::now();
    burstSort(data);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;

    cout << left << setw(12) << label 
         << setw(15) << total_comparisons 
         << setw(15) << total_swaps 
         << fixed << setprecision(5) << elapsed.count() << "s" << endl;
}

int main() {
    int choice;
    cout << "--- Burst Sort (Descending Order) ---\n";
    cout << "1. Enter numbers manually\n";
    cout << "2. Run Experiment (Required List Sizes)\n";
    cout << "Choice: ";
    cin >> choice;

    if (choice == 1) {
        int n, val;
        cout << "How many numbers? "; cin >> n;
        vector<int> userList;
        for (int i = 0; i < n; i++) {
            cout << "Enter element " << i+1 << ": ";
            cin >> val;
            userList.push_back(val);
        }
        burstSort(userList);
        cout << "\nSorted List (Descending): ";
        for (int x : userList) cout << x << " ";
        cout << "\nComparisons: " << total_comparisons << " | Swaps: " << total_swaps << endl;

    } else {
        int sizes[] = {1, 2, 3, 4, 5, 10, 250, 999, 9999, 89786, 789300, 1780000};
        cout << "\n" << setw(12) << "Size" << setw(15) << "Comparisons" << setw(15) << "Swaps" << "Time" << endl;
        for (int s : sizes) {
            vector<int> testData;
            for (int i = 0; i < s; i++) testData.push_back(rand() % 1000000);
            runTest(testData, to_string(s));
        }
    }
    return 0;
}