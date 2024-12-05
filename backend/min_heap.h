#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include "song.h"
using namespace std;
#ifndef MBTIMELODY_MINHEAP_H
#define MBTIMELODY_MINHEAP_H
class min_heap {
private:
    vector<Song> heap;
    // helper functions for heap operations
    // location in vector in comparison to node i
    int static parent(int i) {
        return (i - 1) / 2;
    }
    int static left(int i) {
        return 2 * i + 1;
    }
    int static right(int i) {
        return 2 * i + 2;
    }

    // sift up the element at index i to maintain heap property
    void SiftUp(int i) {
        while (i > 0 && heap[parent(i)].similarity > heap[i].similarity) {
            swap(heap[i], heap[parent(i)]);
            i = parent(i);
        }
    }

    // sift down the element at index i to maintain heap property
    void SiftDown(int i) {
        int minIndex = i;
        int l = left(i);
        if (l < heap.size() && heap[l].similarity < heap[minIndex].similarity)
            minIndex = l;
        int r = right(i);
        if (r < heap.size() && heap[r].similarity < heap[minIndex].similarity)
            minIndex = r;
        if (i != minIndex) {
            swap(heap[i], heap[minIndex]);
            SiftDown(minIndex);
        }
    }

public:
    // insert a new element into the heap
    void Insert(Song& element) {
        heap.push_back(element);
        SiftUp((int) heap.size() - 1);
    }

    // remove and return the smallest element (root) of the heap
    Song Pop() {
        if (heap.empty()) {
            throw runtime_error("Heap is empty");
        }
        Song result = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        SiftDown(0);
        return result;
    }

    // get the smallest element without removing it
    Song Top() {
        if (heap.empty()) {
            throw runtime_error("Heap is empty");
        }
        return heap[0];
    }

    // if the heap is empty
    bool Empty() {
        return heap.empty();
    }

    // size of the heap
    int Size() {
        return (int) heap.size();
    }

    // function to find top-k songs based on a specific profile
    vector<Song> TopKSongs(int k) {
        // extract top k songs from the heap
        vector<Song> topKSongs;
        if (k >= Size()) {
            cout << "There is not enough data to traverse." << endl;
        }
        while (!this->Empty() && k > 0) {
            topKSongs.push_back(this->Pop());
            k--;
        }
        return topKSongs;
    }

    void printSongs(int k) {
        vector<Song> topKSongs = TopKSongs(k);
        for (int i = 0; i < topKSongs.size(); i++) {
            cout << i + 1 << ". ";
            topKSongs[i].displayShort();
            cout << endl;
        }
    }

};
#endif //MBTIMELODY_MINHEAP_H
