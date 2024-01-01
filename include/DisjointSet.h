#pragma once

#include <unordered_map>
#include <vector>

template <typename T>
class DisjointSet {
public:
    // Constructor initializes each element in its own set
    DisjointSet(const std::vector<T>& elements) {
        for (const auto& element : elements) {
            parent[element] = element;
            rank[element] = 0;
        }
    }

    // Find the representative (root) of the set to which an element belongs
    T Find(const T& element) {
        if (parent[element] != element) {
            // Path compression: Set the parent of the element to its root
            parent[element] = Find(parent[element]);
        }
        return parent[element];
    }

    // Merge two sets by rank to optimize union operation
    void Union(const T& set1, const T& set2) {
        T root1 = Find(set1);
        T root2 = Find(set2);

        if (root1 != root2) {
            // Union by rank: Attach the shorter tree to the root of the taller tree
            if (rank[root1] < rank[root2]) {
                parent[root1] = root2;
            } else if (rank[root1] > rank[root2]) {
                parent[root2] = root1;
            } else {
                parent[root1] = root2;
                rank[root2]++;
            }
        }
    }

private:
    std::unordered_map<T, T> parent; // Maps each element to its parent (representative)
    std::unordered_map<T, int> rank;  // Maps each element to its rank (approximate depth of the tree)
};
