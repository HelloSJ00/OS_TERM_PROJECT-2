#ifndef LRU_CACHE_H
#define LRU_CACHE_H

#include <unordered_map>
#include <iostream>
using namespace std;

// 노드 구조체 정의
struct Node {
    int physicalAddress;
    Node* prev;
    Node* next;
    Node(int physicalAddress) : physicalAddress(physicalAddress), prev(nullptr), next(nullptr) {}
};

class LRUCache {
    unordered_map<int, Node*> cache; // 키와 노드 포인터를 매핑
    int capacity;                   // 캐시 크기
    Node* head;                     // 더블 링크드 리스트의 헤드
    Node* tail;                     // 더블 링크드 리스트의 테일

    // 노드를 리스트에서 제거
    void removeNode(Node* node) {
        if (node->prev) node->prev->next = node->next;
        if (node->next) node->next->prev = node->prev;
        if (node == head) head = node->next;
        if (node == tail) tail = node->prev;
    }

    // 노드를 헤드로 이동
    void moveToHead(Node* node) {
        removeNode(node);
        node->next = head;
        node->prev = nullptr;
        if (head) head->prev = node;
        head = node;
        if (!tail) tail = head; // 리스트가 비어있던 경우
    }

    // 노드를 헤드에 추가
    void addNodeToHead(Node* node) {
        node->next = head;
        node->prev = nullptr;
        if (head) head->prev = node;
        head = node;
        if (!tail) tail = head; // 리스트가 비어있던 경우
    }

    // 테일 노드 삭제
    void removeTail() {
        if (!tail) return;
        cache.erase(tail->physicalAddress);
        if (tail->prev) {
            tail->prev->next = nullptr;
        } else {
            head = nullptr; // 리스트가 비어있는 경우
        }
        Node* oldTail = tail;
        tail = tail->prev;
        delete oldTail;
    }

public:
    // 생성자
    LRUCache(int cap=8) : capacity(cap), head(nullptr), tail(nullptr) {}

    // 캐시에 물리주소를 추가하거나 갱신
    void add(int physicalAddress) {
        if (cache.find(physicalAddress) != cache.end()) {
            // 이미 존재하면 헤드로 이동
            moveToHead(cache[physicalAddress]);
        } else {
            // 새로 추가
            Node* newNode = new Node(physicalAddress);
            cache[physicalAddress] = newNode;
            addNodeToHead(newNode);
            if (cache.size() > capacity) {
                removeTail(); // 용량 초과 시 가장 오래된 노드 제거
            }
        }
    }

    // 가장 오래된 물리주소 반환
    int getLeastRecentlyUsed() {
        return tail ? tail->physicalAddress : -1; // -1은 비어있음을 의미
    }
};

#endif
