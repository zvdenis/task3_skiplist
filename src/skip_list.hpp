////////////////////////////////////////////////////////////////////////////////
// Module Name:  skip_list.h/hpp
// Authors:      Leonid Dworzanski, Sergey Shershakov
// Version:      2.0.0
// Date:         28.10.2018
//
// This is a part of the course "Algorithms and Data Structures"
// provided by  the School of Software Engineering of the Faculty
// of Computer Science at the Higher School of Economics.
////////////////////////////////////////////////////////////////////////////////

// !!! DO NOT include skip_list.h here, 'cause it leads to circular refs. !!!

#include <cstdlib>
#include "skip_list.h"


//==============================================================================
// class NodeSkipList
//==============================================================================

template<class Value, class Key, int numLevels>
void NodeSkipList<Value, Key, numLevels>::clear(void) {
    for (int i = 0; i < numLevels; ++i)
        Base::nextJump[i] = 0;

    Base::levelHighest = -1;
}

//------------------------------------------------------------------------------

template<class Value, class Key, int numLevels>
NodeSkipList<Value, Key, numLevels>::NodeSkipList(void) {
    clear();
}

//------------------------------------------------------------------------------

template<class Value, class Key, int numLevels>
NodeSkipList<Value, Key, numLevels>::NodeSkipList(const Key &tkey) {
    clear();

    Base::Base::key = tkey;
}

//------------------------------------------------------------------------------

template<class Value, class Key, int numLevels>
NodeSkipList<Value, Key, numLevels>::NodeSkipList(const Key &tkey, const Value &val) {
    clear();

    Base::Base::key = tkey;
    Base::Base::value = val;
}


//==============================================================================
// class SkipList
//==============================================================================

template<class Value, class Key, int numLevels>
SkipList<Value, Key, numLevels>::SkipList(double probability) {
    _probability = probability;

    // Lets use m_pPreHead as a final sentinel element
    for (int i = 0; i < numLevels; ++i)
        Base::_preHead->nextJump[i] = Base::_preHead;

    Base::_preHead->levelHighest = numLevels - 1;
}

template<class Value, class Key, int numLevels>
void SkipList<Value, Key, numLevels>::insert(const Value &val, const Key &key) {

    int level = genLevel();
    Node *insertedElement = new NodeSkipList<Value, Key, numLevels>(key, val);
    insertedElement->levelHighest = level;

    NodeSkipList<Value, Key, numLevels> *cur = this->_preHead;

    for (int i = numLevels - 1; i >= 0; i--) {
        while (cur->nextJump[i] != this->_preHead && key >= cur->nextJump[i]->key) {
            cur = cur->nextJump[i];
        }
        if (i <= level) {
            insertedElement->nextJump[i] = cur->nextJump[i];
            cur->nextJump[i] = insertedElement;

            if(i == 0){
                insertedElement->next = cur->next;
                cur->next = insertedElement;
            }
        }
    }

}

template<class Value, class Key, int numLevels>
int SkipList<Value, Key, numLevels>::genLevel() {

    int level = 0;
    const double range = 1000;

    while (rand() % (int) range / range < _probability && level + 1 < numLevels) {
        level++;
    }
    return level;
}


template<class Value, class Key, int numLevels>
void SkipList<Value, Key, numLevels>::removeNext(SkipList::Node *nodeBefore) {
    if (nodeBefore->next == this->_preHead)throw std::invalid_argument("");
    if (nodeBefore == nullptr) throw std::invalid_argument("");
    NodeSkipList<Value, Key, numLevels> *deleted = nodeBefore->next;
    NodeSkipList<Value, Key, numLevels> *cur = this->_preHead;

    for (int i = numLevels - 1; i >= 0; i--) {
        while (cur->nextJump[i] != this->_preHead && deleted->key > cur->nextJump[i]->key) {
            if (cur->nextJump[i] == deleted) {
                cur->nextJump[i] = cur->nextJump[i]->nextJump[i];
                continue;
            }
            cur = cur->nextJump[i];
        }
        if (cur->nextJump[i] == deleted) {
            cur->nextJump[i] = cur->nextJump[i]->nextJump[i];
            continue;
        }
    }
    NodeSkipList<Value, Key, numLevels> *afterDeleted = nodeBefore->next->next;
    nodeBefore->next->clear();
    delete nodeBefore->next;
    nodeBefore->next = afterDeleted;
}

template<class Value, class Key, int numLevels>
NodeSkipList<Value, Key, numLevels> *SkipList<Value, Key, numLevels>::findLastLessThan(const Key &key) const {

    NodeSkipList<Value, Key, numLevels> *cur = this->_preHead;

    for (int i = numLevels - 1; i >= 0; i--) {
        while (cur->nextJump[i] != this->_preHead && key > cur->nextJump[i]->key) {
            cur = cur->nextJump[i];
        }
    }
    return cur;
}

template<class Value, class Key, int numLevels>
NodeSkipList<Value, Key, numLevels> *SkipList<Value, Key, numLevels>::findFirst(const Key &key) const {

    NodeSkipList<Value, Key, numLevels> *cur = this->_preHead;

    for (int i = numLevels - 1; i >= 0; i--) {
        while (cur->nextJump[i] != this->_preHead && key >= cur->nextJump[i]->key) {
            cur = cur->nextJump[i];
            if (cur->key == key) return cur;
        }
    }

    while (cur->next != this->_preHead && key >= cur->next->key){
        cur = cur->next;
        if (cur->key == key) return cur;
    }

    return nullptr;
}

template<class Value, class Key, int numLevels>
SkipList<Value, Key, numLevels>::~SkipList() {

}

// TODO: !!! One need to implement all declared methods !!!
