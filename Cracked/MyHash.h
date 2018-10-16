//
//  MyHash.h
//  Cracked
//
//  Created by Jack Li on 3/9/18.
//  Copyright © 2018 Jack Li. All rights reserved.
//

#ifndef MyHash_h
#define MyHash_h

#include <iostream>

const int DEFAULT_HASH_TABLE_SIZE = 100;

template<typename KeyType, typename ValueType>
class MyHash
{
public:
    MyHash(double maxLoadFactor = 0.5);
    ~MyHash();
    void reset();
    void associate(const KeyType& key, const ValueType& value);
    int getNumItems() const;
    double getLoadFactor() const;

      // for a map that can't be modified, return a pointer to const ValueType
    const ValueType* find(const KeyType& key) const;

      // for a modifiable map, return a pointer to modifiable ValueType
    ValueType* find(const KeyType& key)
    {
        return const_cast<ValueType*>(const_cast<const MyHash*>(this)->find(key));
    }

      // C++11 syntax for preventing copying and assignment
    MyHash(const MyHash&) = delete;
    MyHash& operator=(const MyHash&) = delete;
private:
    double m_maxLoadFactor;
    double m_currentLoadFactor;
    struct Node{ //Node for linked list in each bucket
        KeyType m_key;
        ValueType m_value;
        Node* m_next;
    };
    Node** m_hashTable; 
    int m_buckets;  //Not guanranteed to not grow past 100.
    int m_items;
    unsigned int getBucketNumber(const KeyType& key) const;
    void clear(); //destructor delegates work to this function
    void reHash();  //Rehash is triggered when current load factor exceeds maximum allowed load factor.
};

//MyHash Public Methods Implementation
template <class KeyType, class ValueType>
MyHash<KeyType, ValueType>::MyHash(double maxLoadFactor)
{
    if (maxLoadFactor <= 0.0) {
        maxLoadFactor = 0.5;
    }
    else if (maxLoadFactor > 2.0){
        maxLoadFactor = 2.0;
    }
    m_maxLoadFactor = maxLoadFactor;
    m_currentLoadFactor = 0;
    m_items = 0;
    m_buckets = DEFAULT_HASH_TABLE_SIZE;
    m_hashTable = new Node* [m_buckets];
    for (int i = 0; i < m_buckets; i++) {
        m_hashTable[i] = nullptr;
    }
}

template <class KeyType, class ValueType>
MyHash<KeyType, ValueType>::~MyHash()
{
    clear();
}

template <class KeyType, class ValueType>
void MyHash<KeyType, ValueType>::reset()
{
    clear();
    m_hashTable = new Node* [DEFAULT_HASH_TABLE_SIZE];
    m_currentLoadFactor = 0.0;
    m_items = 0;
}

template <class KeyType, class ValueType>
void MyHash<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
    unsigned int hash_key = getBucketNumber(key);
    Node* p = m_hashTable[hash_key];
    if (find(key) != nullptr) {
        while (p != nullptr && p->m_key != key) {
            p = p->m_next;
        }
        p->m_value = value; //update a value for an existing key. No insertions made.
    }
    else {
        m_items++;
        m_currentLoadFactor = double(m_items)/double(m_buckets);
        Node* newNode = new Node();
        newNode->m_key = key;
        newNode->m_value = value;
        newNode->m_next = nullptr;
        if (p == nullptr) {
            m_hashTable[hash_key] = newNode;
        }
        else{
            while (p->m_next != nullptr) {
                p = p->m_next;
            }
            p->m_next = newNode;
        }
        if (m_currentLoadFactor > m_maxLoadFactor) { //rehash triggered if inserting a new item results in the current load factor exceeding the max load factor.
            reHash();
        }
    }
}

template <class KeyType, class ValueType>
const ValueType* MyHash<KeyType, ValueType>::find(const KeyType& key) const
{
    unsigned int hash_key = getBucketNumber(key);
    Node* temp = m_hashTable[hash_key];
    while (temp != nullptr) {
        if (temp->m_key == key) {
            return &(temp->m_value);
        }
        temp = temp->m_next;
    }
    return nullptr;
}

template <class KeyType, class ValueType>
int MyHash<KeyType, ValueType>::getNumItems() const
{
    return m_items;
}

template <class KeyType, class ValueType>
double MyHash<KeyType, ValueType>::getLoadFactor() const
{
    return m_currentLoadFactor;
}

//Private Methods Implementation
template <class KeyType, class ValueType>
unsigned int MyHash<KeyType, ValueType>::getBucketNumber(const KeyType& key) const
{
    unsigned int hash(const KeyType& k);  //prototype
    unsigned int slot = hash(key);
    return slot % m_buckets;  //make provided hash values applicable
}

template <class KeyType, class ValueType>
void MyHash<KeyType, ValueType>::clear()
{
    for (int i = 0; i < m_buckets; i++) { //deallocate each bucket
        Node* begin = m_hashTable[i];
        while (begin != nullptr) {
            Node* killMe = begin;
            begin = begin->m_next;
            delete killMe;
        }
    }
    delete [] m_hashTable; //deallocate whole dynamic array
}

template <class KeyType, class ValueType>
void MyHash<KeyType, ValueType>::reHash()
{
    int previousSize = m_buckets; //save the size of the old array
    m_buckets *= 2;
    Node** newHashTable = new Node* [m_buckets];
    for (int i = 0; i < m_buckets; i++) {
        newHashTable[i] = nullptr;
    }
    for (int i = 0; i < previousSize; i++) { //copy over elements of old array into new array.
        Node* p = m_hashTable[i];
        while (p != nullptr) {
            unsigned int newSlot = getBucketNumber(p->m_key); //NOTE: Elements of old array will unlikely not be assigned to the same bucket in the new array since the new array is bigger. Also, elements belonging to the same bucket may not be grouped together in new array once again because the size of the array has changed.
            Node* newNode = new Node();
            newNode->m_key = p->m_key;
            newNode->m_value = p->m_value;
            newNode->m_next = nullptr;
            if (newHashTable[newSlot] == nullptr) {
                newHashTable[newSlot] = newNode;
            }
            else{
                Node* q = newHashTable[newSlot];
                while (q->m_next != nullptr) {
                    q = q->m_next;
                }
                q->m_next = newNode;
            }
            p = p->m_next;
        }
    }
    //clear(); //delete contents of old array
    //m_buckets *= 2; //update bucket size
    m_currentLoadFactor /= 2.0; //update load factor
    m_hashTable = newHashTable;
}

#endif /* MyHash_h */
