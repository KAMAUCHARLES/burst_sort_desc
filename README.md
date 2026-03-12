# Burst Sort Implementation (Descending Order)

## Student Information
*   **Name:** CHARLES KAMAU MAINA
*   **Registration Number:** EB3/67291/23

## 1. Description of the Algorithm
This program implements **Burst Sort**, a high-performance, trie-based sorting algorithm. While traditionally used for strings, this implementation adapts it for integers by treating each integer as a sequence of 4 bytes (8-bit chunks). 

The algorithm starts by placing elements into a "root" container (a bucket). When a bucket grows larger than a specific limit (the **Burst Threshold**), it "bursts," transforming into an internal node of a trie and distributing its elements into 256 smaller sub-buckets based on the value of the current byte being processed.


## 2. Experiment Results (Step 4 & 9)
The following table shows the performance of the algorithm across different list sizes. The "Comparisons" count refers to logic used during the bucket sorting phase, and "Swaps" refers to the movement of data into buckets and internal redistribution.

## 3. How the Algorithm Works (Step 5)
1.  **Bucket Insertion:** Every number starts at the root. If the root is a "Leaf" (a bucket), the number is simply added to a list.
2.  **The Burst:** If the number of items in a bucket exceeds the threshold (set to 50 or 100), the bucket "bursts." We create an array of 256 pointers (one for every possible byte value 0-255).
3.  **Redistribution:** The items from the old bucket are moved into 256 new sub-buckets based on their most significant byte.
4.  **Recursion:** This process repeats for the next byte if sub-buckets also exceed the threshold.
5.  **Descending Collection:** To get the sorted result, we traverse the trie. Because we want **descending order**, we visit the children pointers from index 255 down to 0. Within each small bucket, we use a manual Selection Sort to finalize the order.

## 4. Step-by-Step Example (Step 6)
**Example List:** `[150, 10, 200]`, **Burst Threshold:** `2`

1.  **Insert 150:** Root is a leaf. Root Bucket = `{150}`.
2.  **Insert 10:** Root is a leaf. Root Bucket = `{150, 10}`.
3.  **Insert 200:** Root Bucket exceeds threshold (3 > 2). **Burst occurs!**
    *   Root becomes an internal node with 256 slots.
    *   `150` moves to slot `150`.
    *   `10` moves to slot `10`.
    *   `200` moves to slot `200`.
4.  **Collection (Descending):**
    *   Check slot 255... down to 200. Found `200`.
    *   Check slot 199... down to 150. Found `150`.
    *   Check slot 149... down to 10. Found `10`.
**Final Result:** `[200, 150, 10]`

## 5. Time Complexity Analysis (Step 7)
*   **Best-Case: $O(n)$**
    Occurs when the data is uniformly distributed. The trie remains shallow, and each bucket contains very few items, leading to near-linear performance.
*   **Average-Case: $O(n \cdot \frac{w}{k})$**
    Where $w$ is the word size (32-bit for integers) and $k$ is the chunk size (8-bit). Since $w/k$ is a constant (4), this simplifies to **$O(n)$**. It is significantly faster than comparison-based sorts for large $n$.
*   **Worst-Case: $O(n^2)$**
    Occurs if all $n$ elements are identical. They will all fall into the same bucket at the maximum depth of the trie. Since the program uses a manual Selection Sort for items within a single bucket, the complexity reverts to $O(n^2)$ for that specific bucket.

## 6. Space Complexity Analysis (Step 8)
**Space Complexity: $O(n + A \cdot m)$**
*   **$n$**: We must store all $n$ integers in buckets.
*   **$A$**: The alphabet size (256 pointers per internal node).
*   **$m$**: The number of internal nodes created by bursts.
Burst Sort is **not** in-place. It trades high memory usage for speed. Each "Burst" allocates an array of 256 pointers, which can consume significant RAM if the trie becomes very deep.
