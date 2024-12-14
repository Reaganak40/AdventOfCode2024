#include "pch.h"
#define FREE_SPACE (-1)

uint64_t DiskFragmenter(const std::string& sequence) {
    std::vector<int> disk;
    bool is_file = true;
    int id = 0;

    // parse the sequence
    for (char c : sequence) {
        if (c == '\n') {
            continue;
        }

        int num = c - '0';
        int val = FREE_SPACE;
        if (is_file) {
            val = id++;
        }

        for (int i = 0; i < num; i++) {
            disk.push_back(val);
        }

        is_file = !is_file;
    }

    int i = 0;
    int j = disk.size() - 1;

    // swap until contiguous
    while (i < j) {
        if (disk[i] != FREE_SPACE) {
            i++;
            continue;
        }

        if (disk[j] == FREE_SPACE) {
            j--;
            continue;
        }

        std::swap(disk.at(i), disk.at(j));
    }

    // calculate checksum
    uint64_t checksum = 0;

    for (int k = 0; k < i; k++) {
        checksum += k * disk[k];
    }
    
    return checksum;
}

uint64_t MoveWholeFiles(const std::string& sequence) {
    std::vector<int> disk;
    bool is_file = true;
    int id = 0;

    // parse the sequence
    for (char c : sequence) {
        if (c == '\n') {
            continue;
        }

        int num = c - '0';
        int val = FREE_SPACE;
        if (is_file) {
            val = id++;
        }

        for (int i = 0; i < num; i++) {
            disk.push_back(val);
        }

        is_file = !is_file;
    }

    int i = 0;
    int j = disk.size() - 1;

    int file = 0;
    int file_size = 0;

    // swap until contiguous
    while (i < j) {

        // find next file
        if (disk[j] == FREE_SPACE) {
            j--;
            continue;
        }

        // determine file size
        file = disk[j];
        file_size = 1;
        while (i < j && disk[j-1] == file) {
            j--;
            file_size++;
        }

        if (i >= j) {
            break;
        }

        // find next free spot
        find_next_free_space:
        while (i < j && disk[i] != FREE_SPACE) {
            i++;
        }
        if (i >= j) {
            // out of free spots, forget this file
            j--;
            i = 0;
            continue;
        }

        // check if free space is large enough
        int free_size = 1;
        while (i < j && disk[i+1] == FREE_SPACE) {
            i++;
            free_size++;
        }

        if (free_size < file_size) {
            i++;
            goto find_next_free_space;
        }

        // free space is large enough, move file
        i -= (free_size - 1);
        for (int k = 0; k < file_size; k++) {
            std::swap(disk.at(i + k), disk.at(j + k));
        }
        i = 0;
    }

    // calculate checksum
    uint64_t checksum = 0;

    for (int k = 0; k < disk.size(); k++) {
        if (disk[k] == FREE_SPACE) {
            continue;
        }
        checksum += k * disk[k];
    }
    return checksum;
}

void DoDay9() {
    std::cout << "Day 9: " << std::endl;
    std::ifstream file("inputs/day09/input.txt");
    if (!file.is_open()) {
        std::cerr << "Could not open file" << std::endl;
        return;
    }

    std::string line;
    std::string sequence;
    while (std::getline(file, line)) {
        sequence += line;
    }

    std::cout << "    Part 1: " << DiskFragmenter(sequence) << std::endl;
    std::cout << "    Part 2: " << MoveWholeFiles(sequence) << std::endl;
}