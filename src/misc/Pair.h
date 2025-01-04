//
// Created by Noah Schwenold on 1/3/2025.
//

#ifndef PAIR_H
#define PAIR_H

struct Pair {
    int row;
    int column;

    Pair(const int r, const int c) : row(r), column(c) {}

    // Define equality operator for Pair to use it as a key in unordered_map
    bool operator==(const Pair& other) const {
        return row == other.row && column == other.column;
    }
};

// Hash function for Pair to use in unordered_map
namespace std {
    template <>
    struct hash<Pair> {
        size_t operator()(const Pair& p) const {
            return hash<int>()(p.row) ^ hash<int>()(p.column); // Combine hashes of row and column
        }
    };
} // namespace std

#endif //PAIR_H
