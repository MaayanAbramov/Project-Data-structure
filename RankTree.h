//
// Created by maaya on 16/03/2024.
//

#ifndef WINTERRANK_RANKTREE_H
#define WINTERRANK_RANKTREE_H


#include <iostream>
#include <cassert>
#include <cmath>
#include <limits.h>
#define ZERO_INIT 0
#define ONE_STEP 1
#define MINUS_ONE_INIT -1
#define MAX_BF 2
#define MIN_BF -2
#define DIFF_ZERO 0
#define DIFF_ONE 1
#define DIFF_MINUS_ONE -1
#define FLAG_KEY -1
using namespace std;
#include <stdexcept>


#include "to_bubble_info.h"

template <class Key, class Value>
class RankTree{


public:
    class Node {
    public:
        Node* father;
        Node* right;
        Node* left;
        Key key;
        Value value;
        int change;

        int nodes_in_sub_tree;
        to_bubble_info to_bubble;
        int heightFromRoot;
        int BF; //balance factor
    public:
        explicit Node(const Key& newkey, const Value& newvalue, Node* newLeft, Node* newRight, int newHeight) :
                father(nullptr), right(newRight), left(newLeft), key(newkey), value(newvalue),change(ZERO_INIT),
                nodes_in_sub_tree(1), to_bubble(ZERO_INIT),
                heightFromRoot
                (newHeight), BF
                (ZERO_INIT) {}
        explicit Node(const Key& newKey) : father(nullptr), right(nullptr), left(nullptr), key(newKey), value
                (DIFF_ZERO), change(ZERO_INIT),nodes_in_sub_tree(1), to_bubble(ZERO_INIT),
                heightFromRoot(ZERO_INIT), BF(ZERO_INIT){}
        explicit Node(const Key& newKey, const Value& newValue) : father(nullptr), right(nullptr), left(nullptr),
        key(newKey),
                                                                  value(newValue), change(ZERO_INIT)
                                                                  ,nodes_in_sub_tree(1), to_bubble
                                                                  (ZERO_INIT),heightFromRoot(ZERO_INIT), BF(ZERO_INIT){}

        //to access value and key objects without copying it by value, we will write getters:
        Key* getKey() {return &key;}
        Value* getValue() {return &value;}
        void generate_new_to_bubble_info(int to_bubble , Node* node);
    };
    RankTree() : root(nullptr), numOfNodes(ZERO_INIT) {} //big three-------------------------------------

    void deleteTrivialTree(); //deletes the tree with numOfNodes = 1
    Node* deleteParentOfOneBrotherOnly(); //deleting the root and initializing it to be the son. numOfNodes = 2
    void deleteLeaf(Node* leafToRemove);
    void updateNodesTillRoot(Node* curr_node);

    void deleteNodeIfHasOneSon(const Key& keyToDelete, Node* curr_node); //connects between the grandpa and the son
    void deleteNodeIfHasTwoSons(Key* keyToDelete, Node* curr_node);//check that you actually update the BF and
    // height //
    // from
    // the swapped till the root
    Node* findNextNodeInOrder(const Key& key);
    Node* swapNodes(Node* toDelete, Node* NextInorder,int accumulated_change_from_node_to_successor);
    void update_bubble_all_tree(Node* n);
    void remove(const Key& keyToRemove); //should check before calling if numOfNodes == 0 then return failure, there
    // shouldnt be a call if numOfNodes == 0
    //returns null in the case of not finding the key
    //returns the root in the success case

    void DeleteWholeTree(Node* node) {
        if (node == nullptr) { //if we reached a step under the leaves it means we reached the end
            return;
        }
        DeleteWholeTree(node->right);
        DeleteWholeTree(node->left);
        node->right=nullptr;
        node->left= nullptr;
        if (node->father == nullptr) { //this is a post order deletion
            deleteTrivialTree();
        }
        else {
            delete node;
        }

    }

    ~RankTree() {
        DeleteWholeTree(root);
    }

    RankTree& operator=(const RankTree& other) = delete;

    Node* find_algorithm(const Key& key);
    Node* find(const Key& key, Node* curr_node) const;
    Node* findMax();
    Node* findMax_aux(Node* curr_node);
    Node* findMin();
    Node* findMin_aux(Node* curr_node);
    static void destroy_the_tree_and_values(Node* root);
    static int heightDetermination(Node* node);
    static int balanceFactorDetermination(Node* node);
    static int num_nodes_sub_tree_determination(Node* node);
    void updateFieldsAfterChangeInTree(Node* node);
    void rotationLL(Node* node);
    void printTreeWithInfo(Node* node, int depth, char branch);
    void rotationRR(Node* node);
    void rotationLR(Node* node);
    void rotationRL(Node* node);
    Node* rotate(Node* node);
    void insertToAnEmptyTree(const Key& key, const Value& value);
    void insertAux(const Key& key, const Value& value);
    void recursive_insert(Node* curr_node, const Key& key, const Value& value, bool* createdAlready);
    static Node* recursive_skeleton_build(int size, int height);
    //please notice the caller should update root to be the result and numOfElements
    static Node* createCompleteTree(int numOfElements);
    static void CutLeafs(Node* curr_node, int* amountOfLeafsExpectedToCut);
    static void inorderFromArrayToTree(RankTree<Key, Value>::Node* currNode, Key* arrayOfKeys, Value* arrayOfValues,
                                       int* index);
    int acumulated_sum(Node* curr);
    void add_amount(const Key& upper_bound, int amount);
    void add(const Key& upper_bound,const Key& low_bound, int amount);
//assuming that i and j are the closest to the given parameters low and high
    void add_amount_aux(Node* curr_node, bool is_last_turn_right, const Key& upper_bound, int amount);
    void bubble_up(Node* curr_node, const Key& key);
    to_bubble_info bubble_field(Node* curr);
    to_bubble_info max_of_three(to_bubble_info&,to_bubble_info&,to_bubble_info&);

    Node *findprevNodeInOrder(const Key &key);
    int algorithm_rank(const Key& key) const;
    int algorithm_rank_aux(const Key& key, Node* curr_node, int r=-1) const;
    Node* select_aux(int index, Node* curr_node) const;
    Node* select(int index) const;
    int w(Node* node) const;
    Node* next_to_up_bound_from_down(const Key& upper_bound);
    Node* next_to_up_bound_from_down_aux(Node* curr_node, const Key& upper_bound);
    Node* next_to_low_from_above(const Key& low);
    Node* next_to_low_from_above_aux(Node* curr_node, const Key& low);
    //testing funcitons
    /*bool is_tree_valid(Node* root);
    int getHeight(Node* root);
    int BalanceFactor(Node* node);*/
    //int getWeight(Node* root);
    /*void print2DHelper(Node* root, int space);
    void print2D(Node* root);*/
    void inorderFromTreeToArray_aux(Node *currNode, Key *arrayOfKeys, Value *arrayOfValues, int *index);
    void inorderFromTreeToArray(Key *arrayOfKeys, Value *arrayOfValues, int *index);
public:
    Node* root;
    int numOfNodes;



};

template<class Key, class Value>
void RankTree<Key, Value>::Node::generate_new_to_bubble_info(int to_bubble, Node* node) {
    int node_value_id = node->value->getId();
    int num_participants = node->value->getParticipantCount();
    node->to_bubble = to_bubble_info(node_value_id, num_participants, to_bubble);
    return;
}

template <class Key, class Value>
void RankTree<Key, Value>::deleteTrivialTree() {
    assert(numOfNodes > 0);
    numOfNodes--;
    delete root;
    root = nullptr;
    return;
}
template <class Key, class Value>
void RankTree<Key,Value>::destroy_the_tree_and_values(Node* root){
    if(root==nullptr){
        return;
    }
    if(root->left!=nullptr) {
        destroy_the_tree_and_values(root->left);
    }
    if(root->right!=nullptr) {
        destroy_the_tree_and_values(root->right);
    }
    Value value = root->value;
    root->value=nullptr;
    delete(value);


}
template <class Key, class Value>
typename RankTree<Key, Value>::Node* RankTree<Key, Value>::deleteParentOfOneBrotherOnly() {
    assert((root->right == nullptr && root->left != nullptr) || (root->left == nullptr && root->right != nullptr));
    Node* copyOfSon = (root->right == nullptr) ? root->left : root->right;
    copyOfSon->change += root->change;
    copyOfSon->father = nullptr; // we don't want to point to a deleted node (valgrind)
    numOfNodes--;
    delete root;
    root = copyOfSon;
    updateFieldsAfterChangeInTree(root);
    bubble_up(copyOfSon,copyOfSon->key);
    return nullptr;
}

template <class Key, class Value>
void RankTree<Key, Value>::deleteLeaf(Node* leafToRemove) {
    Node* leafsFather = leafToRemove->father;
    if (leafsFather->right == leafToRemove) {
        leafsFather->right = nullptr;
    }
    else {
        leafsFather->left = nullptr;
    }
    leafToRemove->father = nullptr;
    delete leafToRemove;
    numOfNodes--;
    updateFieldsAfterChangeInTree(leafsFather);
    updateNodesTillRoot(leafsFather);
    return;
}
template <class Key, class Value>
void RankTree<Key, Value>::updateNodesTillRoot(Node* curr_node) {
    if (curr_node->father == nullptr) {
        updateFieldsAfterChangeInTree(curr_node);
        rotate(curr_node);
    }
    if (curr_node->father != nullptr) {
        updateFieldsAfterChangeInTree(curr_node);
        rotate(curr_node);
        updateNodesTillRoot(curr_node->father);
    }
}
template <class Key, class Value>
typename RankTree<Key, Value>::Node* RankTree<Key, Value>::find_algorithm(const Key& key) {
    return this->find(key, root);
}

template <class Key, class Value>
typename RankTree<Key, Value>::Node* RankTree<Key, Value>::find(const Key& key, Node* curr_node) const {
    if (curr_node == nullptr || *(curr_node->getKey()) == key) {
        return curr_node;
    }
    return (key < *(curr_node->getKey())) ? find(key, curr_node->left) : find(key, curr_node->right);
}

template <class Key, class Value>
int RankTree<Key, Value>::heightDetermination(Node *node) {
    if (node == nullptr) {
        return MINUS_ONE_INIT;
    }
    if (node->right == nullptr && node->left == nullptr) {
        return ZERO_INIT;
    }
    assert(node->right != nullptr || node->left != nullptr);
    int right_height = (node->right == nullptr)? ZERO_INIT : ONE_STEP + node->right->heightFromRoot;
    int left_height = (node->left == nullptr) ? ZERO_INIT : ONE_STEP + node->left->heightFromRoot;
    return std::max(right_height, left_height);
}

template <class Key, class Value>
int RankTree<Key, Value>::balanceFactorDetermination(Node* node) {
    if (node->right == nullptr && node->left == nullptr) {
        return ZERO_INIT;
    }
    else if (node->right == nullptr) {
        return node->left->heightFromRoot - MINUS_ONE_INIT;
    }
    else if (node->left == nullptr) {
        return MINUS_ONE_INIT - node->right->heightFromRoot;
    }
    else {
        return node->left->heightFromRoot - node->right->heightFromRoot;
    }
}
template <class Key, class Value>
int RankTree<Key, Value>::num_nodes_sub_tree_determination(Node* node) {
    if (node->right == nullptr && node->left == nullptr) {
        return 1; //only me
    }
    else if (node->right == nullptr) {
        return node->left->nodes_in_sub_tree + 1;
    }
    else if (node->left == nullptr) {
        return node->right->nodes_in_sub_tree + 1;
    }
    else {
        return node->left->nodes_in_sub_tree + node->right->nodes_in_sub_tree + 1;
    }
}

template <class Key, class Value>
void RankTree<Key, Value>::updateFieldsAfterChangeInTree(Node *node) {
    if (node == nullptr) {
        return;
    }
    node->heightFromRoot = heightDetermination(node);
    node->BF = balanceFactorDetermination(node);
    node->nodes_in_sub_tree = num_nodes_sub_tree_determination(node);
}

template <class Key, class Value>
void RankTree<Key, Value>::rotationLL(Node* node) {
    assert(node->left != nullptr);
    Node* tempA = node->left;

    if(tempA != nullptr) {
        if (tempA != nullptr) {
            assert(node->left != nullptr);
            node->left = tempA->right;
            tempA->father = node->father;
            if (tempA->father != nullptr) {
                assert(tempA->father != nullptr);
                if (node->father->right == node) {
                    tempA->father->right = tempA;
                }
                if (node->father->left == node) {
                    tempA->father->left = tempA;
                }
            } else {
                root = tempA;
            }
            node->father = tempA;
            tempA->right = node;
            updateFieldsAfterChangeInTree(node);
            updateFieldsAfterChangeInTree(tempA);
            if (node->left != nullptr) {
                node->left->father = node;
            }
        }
        //B = node, A= tempA
        int old_change_a = tempA->change;
        int old_change_b = node->change;
        tempA->change = old_change_a +old_change_b;
        node->change = -1*old_change_a;
        if(node->left!=nullptr){
            node->left->change += old_change_a;
        }

        /*-----------------------now updating to bubble------------------*/
        if (node != nullptr && node->right != nullptr) {
            bubble_up(node->right,node->right->key);
        }
        if (node != nullptr && node->left != nullptr) {
            bubble_up(node->left, node->left->key);
        }
        if (tempA != nullptr && tempA->left != nullptr) {
            bubble_up(tempA->left, tempA->left->key);
        }
        if (node != nullptr) {
            bubble_up(node, node->key);
        }
        if (tempA!= nullptr) {
            bubble_up(tempA, tempA->key);
        }


    }

    return;
}


template <class Key, class Value>
void RankTree<Key, Value>::printTreeWithInfo(typename RankTree<Key, Value>::Node* node, int depth , char branch) {
    const int spaces = 4;

    if (node == nullptr) {
        std::cout << std::string(depth * spaces, ' ') << "NULL" << std::endl;
        return;
    }

    std::cout << std::string(depth * spaces, ' ') << branch << " Key: " << node->key << ", Height: " << node->heightFromRoot << ", BF: " << node->BF << std::endl;

    printTreeWithInfo(node->right, depth + 1, '/');
    printTreeWithInfo(node->left, depth + 1, '\\');
}


template <class Key, class Value>
void RankTree<Key, Value>::rotationRR(Node* node) {

    assert(node->right != nullptr);
    Node* tempA = node->right;


    if (tempA != nullptr) {
        node->right = tempA->left;
        tempA->father = node->father;
        if (tempA->father != nullptr) {
            assert(tempA->father != nullptr);
            if (node->father->right == node) {
                tempA->father->right = tempA;
            }
            if (node->father->left == node) {
                tempA->father->left = tempA;
            }
        } else {
            root = tempA;
        }
        node->father = tempA;
        tempA->left = node;
        updateFieldsAfterChangeInTree(node);
        updateFieldsAfterChangeInTree(tempA);
        if (node->right != nullptr) {
            node->right->father = node;
        }
    }

        //node = B, tempA= A
        int old_change_a = tempA->change;
        int old_change_b = node->change;
        node->change = -1*old_change_a;
        tempA->change = old_change_a+old_change_b;
        if(node->right !=nullptr)
        {
            node->right->change += old_change_a;
        }
        /*-----------------------now updating to bubble------------------*/
        if (node->left != nullptr) {
            bubble_up(node->left, node->left->key);
        }
        if(node->right != nullptr) {
            bubble_up(node->right, node->right->key);
        }
        if (tempA->right != nullptr) {
            bubble_up(tempA->right, tempA->right->key);
        }
        if (node != nullptr) {
            bubble_up(node, node->key);
        }
        if(tempA != nullptr) {
            bubble_up(tempA, tempA->key);
        }



    return;
}

template <class Key, class Value>
void RankTree<Key, Value>::rotationLR(Node* node){ //same as beneath
    Node* tempC = node;
    assert(tempC->left != nullptr && tempC->left->right != nullptr);
    Node* tempA = tempC->left;
    rotationRR(tempA);
    tempC->left->father = tempC;
    rotationLL(tempC);
    return;
}

template <class Key, class Value>
void RankTree<Key, Value>::rotationRL(Node* node){ // to understand this you should check
    //Maayan's notebook
    Node* tempC = node;
    assert(tempC->right != nullptr && tempC->right->left != nullptr);
    Node* tempA = tempC->right;
    rotationLL(tempA);
    tempC->right->father = tempC;
    rotationRR(tempC);
    return;
}


template <class Key, class Value>
typename RankTree<Key, Value>::Node* RankTree<Key, Value>::rotate(Node *node) {
    if (node->BF == MAX_BF) {
        Node* leftSon = node->left;
        if (balanceFactorDetermination(leftSon) >= DIFF_ZERO) {
            rotationLL(node);
        }
        else if (balanceFactorDetermination(leftSon) == DIFF_MINUS_ONE) {
            rotationLR(node);
        }
    }
    else if (node->BF == MIN_BF) {
        Node* rightSon = node->right;
        if(balanceFactorDetermination(rightSon) <= DIFF_ZERO) {
            rotationRR(node);
        }
        else if (balanceFactorDetermination(rightSon) == DIFF_ONE) {
            rotationRL(node);
        }
    }
    return node;
}

template <class Key, class Value>
void RankTree<Key, Value>::insertToAnEmptyTree(const Key& key, const Value& value) {
    numOfNodes++;
    root = new Node(key, value);
    updateFieldsAfterChangeInTree(root);
    bubble_up(root, root->key);
    return;
}


template <class Key, class Value>
void RankTree<Key, Value>::insertAux(const Key& key, const Value& value) {
    if (root == nullptr) {
        insertToAnEmptyTree(key, value);
        return;
    }//else if the tree is not empty
    try {
        bool createdAlready = false;
        recursive_insert(root, key, value, &createdAlready);
    }
    catch (...) {
        cout << "key already exists or allocation problem" << endl;
    }

    return;
}

template <class Key, class Value>
void RankTree<Key, Value>::recursive_insert(Node* curr_node, const Key& key, const
Value& value, bool* createdAlready) {
    if (*(curr_node->getKey()) == key && !*createdAlready) {
        cout << "check" << endl;
        throw invalid_argument("the key already exists");

    }
    if (key < *(curr_node->getKey()) && curr_node->left == nullptr) {
        numOfNodes++;
        //Value* valueToInsert = new Value(value)
        curr_node->left = new Node(key, value);
        curr_node->left->father = curr_node;
        updateFieldsAfterChangeInTree(curr_node->left);
        *createdAlready = true;
        curr_node->left->change = (-1)* acumulated_sum(curr_node->left);
        int node_to_bubble = curr_node->left->key.strength+curr_node->left->change;
        curr_node->left->generate_new_to_bubble_info(node_to_bubble,curr_node->left);
        bubble_up(curr_node->left,curr_node->left->key);//test
        //curr_node->left->to_bubble = curr_node->left->key.strength+curr_node->left->change;
    }
    if (key > *(curr_node->getKey()) && curr_node->right == nullptr) {
        numOfNodes++;
        curr_node->right = new Node(key, value);
        curr_node->right->father = curr_node;
        updateFieldsAfterChangeInTree(curr_node->right);
        *createdAlready = true;
        curr_node->right->change = (-1)* acumulated_sum(curr_node->right);
        int node_to_bubble = curr_node->right->key.strength+curr_node->right->change;
        curr_node->right->generate_new_to_bubble_info(node_to_bubble,curr_node->right);
        bubble_up(curr_node->right,curr_node->right->key);//test
    }
    if (key > *(curr_node->getKey()) && !(*createdAlready)) {
        recursive_insert(curr_node->right, key, value, createdAlready);
    } else if (key < *(curr_node->getKey()) && !(*createdAlready)) {
        recursive_insert(curr_node->left, key, value, createdAlready);
    }
    updateFieldsAfterChangeInTree(curr_node);
    bubble_up(curr_node, curr_node->key);
    rotate(curr_node);
}



template <class Key, class Value>
void RankTree<Key, Value>::remove(const Key& keyToRemove) { //help functions should be recursive, to
    // update the route
    // till the root
    Key copy = keyToRemove;
    if (root == nullptr) {
        throw invalid_argument("The root is empty");
    }
    Node* toRemove = find(keyToRemove, root);
    int medals = acumulated_sum(toRemove);
    int rank = medals + toRemove->value->getStrength();
    auto prev = this->findprevNodeInOrder(toRemove->key);
    //assert(prev!=nullptr);
    if (prev != nullptr) { //otherwise this is the smallest elem in tree, and we dont need to update his left son
        // which does not exist
        this->add_amount( toRemove->key,-1*rank);
        this->add_amount( prev->key,rank);
    }
    if (prev == nullptr) {
        this->add_amount( toRemove->key,-1*rank);
    }

    if (toRemove == nullptr) {
        throw invalid_argument("The key does not exist");
    }
    if (toRemove->left == nullptr && toRemove->right == nullptr && toRemove != root) {
        deleteLeaf(toRemove);

    }
    else if (toRemove->left == nullptr && toRemove->right == nullptr && toRemove == root) {
        deleteTrivialTree();
    }
    else if (toRemove->left == nullptr || toRemove->right == nullptr) {
        deleteNodeIfHasOneSon(*(toRemove->getKey()), root);
    }
    else {
        deleteNodeIfHasTwoSons(&copy, root);
    }
}
template <class Key, class Value>
void RankTree<Key, Value>::deleteNodeIfHasOneSon(const Key& keyToDelete, Node* curr_node) {
    Node* toRemove = find(keyToDelete, root); //the callee already deals with not found case
    Node* fatherInLaw = toRemove->father;
    if (toRemove == root) {
        deleteParentOfOneBrotherOnly();
        return;
    }
    if (toRemove->right == nullptr) {

        if (fatherInLaw->right == toRemove) {
            fatherInLaw->right = toRemove->left;
            fatherInLaw->right->father = fatherInLaw;
            updateFieldsAfterChangeInTree(fatherInLaw->right);
            fatherInLaw->right->change += toRemove->change;
            bubble_up(fatherInLaw->right, fatherInLaw->right->key);
        }
        else if (fatherInLaw->left == toRemove) {
            fatherInLaw->left = toRemove->left;
            fatherInLaw->left->father = fatherInLaw;
            updateFieldsAfterChangeInTree(fatherInLaw->left);
            fatherInLaw->left->change += toRemove->change;
            bubble_up(fatherInLaw->left,fatherInLaw->left->key);
        }
        toRemove->left = nullptr; //valgrind
        toRemove->father = nullptr;
        numOfNodes--;
        delete toRemove;
        updateFieldsAfterChangeInTree(fatherInLaw);
    }
    else if(toRemove->left == nullptr) {

        if (fatherInLaw->left == toRemove) {
            fatherInLaw->left = toRemove->right;
            fatherInLaw->left->father = fatherInLaw;
            updateFieldsAfterChangeInTree(fatherInLaw->left);
            fatherInLaw->left->change += toRemove->change;
            bubble_up(fatherInLaw->left, fatherInLaw->left->key);
        }
        else if (fatherInLaw->right == toRemove) {
            fatherInLaw->right = toRemove->right;
            fatherInLaw->right->father = fatherInLaw;
            updateFieldsAfterChangeInTree(fatherInLaw->right);
            fatherInLaw->right->change += toRemove->change;
            bubble_up(fatherInLaw->right, fatherInLaw->right->key);
        }
        toRemove->right = nullptr;
        toRemove->father = nullptr;
        numOfNodes--;
        delete toRemove;
        updateFieldsAfterChangeInTree(fatherInLaw);
        updateFieldsAfterChangeInTree(fatherInLaw->left);
    }
    updateNodesTillRoot(fatherInLaw);
}

template <class Key, class Value>
typename RankTree<Key, Value>::Node* RankTree<Key, Value>::findNextNodeInOrder(const Key& key) {
    Node* successor = nullptr;
    Node* temp = root;
    while (temp) {
        if (!(key < *(temp->getKey()))) {
            temp = temp->right;
        }
        else {
            successor = temp;
            temp = temp->left;
        }
    }
    return successor;
}
template <class Key, class Value>
typename RankTree<Key, Value>::Node* RankTree<Key, Value>::findprevNodeInOrder(const Key& key) {
    Node* prev = nullptr;
    Node* temp = root;
    while (temp) {
        if (!(key > *(temp->getKey()))) {
            temp = temp->left;
        }
        else {
            prev = temp;
            temp = temp->right;
        }
    }
    return prev;
}
template <class Key, class Value>
void RankTree<Key, Value>::deleteNodeIfHasTwoSons(Key* keyToDelete, Node* curr_node) {
    if (*(curr_node->getKey()) == *keyToDelete) {
        Node* successor = findNextNodeInOrder(*keyToDelete); //successor has to be a leaf or to have one son only
        //Node* nodeToDelete = find(*keyToDelete, root);
        //cout << "the successor of 3 is --------------------" << successor->key << endl;
        //cout << "before swap" << endl;
        //printTreeWithInfo(root, 0, '-');
        int accumulated_sum = acumulated_sum(successor)- acumulated_sum(curr_node);
        curr_node = swapNodes(curr_node, successor,accumulated_sum );
        //cout << "after swap" << endl;
        //printTreeWithInfo(root, 0, '-');
        updateFieldsAfterChangeInTree(successor);
        updateFieldsAfterChangeInTree(curr_node);
        //rotate(successor);
        //rotate(curr_node);
        *keyToDelete = *(successor->getKey());


        if (successor->left == nullptr && successor->right == nullptr) {
            deleteLeaf(successor);
            //int num_of_medal_supposed_for_left_curr_node = acumulated_sum(curr_node->left);
            bubble_up(curr_node, curr_node->key);
            if (curr_node->left != nullptr) {
                bubble_up(curr_node->left, curr_node->left->key);
            }

            return;
        }
        if (successor->left == nullptr) {
            /*int medals = acumulated_sum(successor);
            int rank = medals + successor->key.strength;
            successor->change = (-1)*rank;
            bubble_up(successor, successor->key);*/
            if (successor->father->right == successor) { // pleasenotice that if the successor is a right son then his
                // father is curr_node
                curr_node->right = successor->right;
                successor->right->change += successor->change;
                if(successor->right != nullptr) {
                    successor->right->father = curr_node;
                }
                successor->father = nullptr;
                successor->right = nullptr;
                delete successor;
                numOfNodes--;
                bubble_up(curr_node->right,curr_node->right->key);
                updateNodesTillRoot(curr_node->right);
                return;
            }
            if (successor->father->left == successor) { //please notice that if the successor is a left son it means
                // that the father is not curr_node
                //todo think about this scenario
                Node* successorsFather = successor->father;
                successorsFather->left = successor->right;
                if(successor->right != nullptr) {
                    successor->right->father = successorsFather;
                }
                successor->right->change += successor->change;
                successor->right = nullptr;
                successor->father = nullptr;
                delete successor;
                numOfNodes--;
                bubble_up(successorsFather->left,successorsFather->left->key);
                updateNodesTillRoot(successorsFather->left);
            }
        }
        // don't think can be an option for (successor->right == nullptr)
        return;
    }
    if (*(curr_node->getKey()) < *keyToDelete) {
        deleteNodeIfHasTwoSons(keyToDelete, curr_node->right);
    }
    if(*(curr_node->getKey()) > *keyToDelete) {
        deleteNodeIfHasTwoSons(keyToDelete, curr_node->left);
    }
    updateFieldsAfterChangeInTree(curr_node);
    rotate(curr_node);

}

template <class Key, class Value>
typename RankTree<Key, Value>::Node* RankTree<Key, Value>::swapNodes(Node* curr_node, Node* successor, int
accumulated_change_from_node_to_successor) { //curr_node,
    // successor
    //before swap -> curr_Node = B , successor = A
    Key tempK = *(curr_node->getKey());
    Value tempV = *(curr_node->getValue());
    //int tempChange = curr_node->change;
    curr_node->key = successor->key;
    curr_node->value = successor->value;
   // curr_node->change = successor->change;
    successor->key = tempK;
    successor->value = tempV;
    // now , we update the change attribute
    // after swap -> curr_node = A , successor = B
    assert(curr_node!=nullptr);
    assert(successor!=nullptr);
    curr_node->change += accumulated_change_from_node_to_successor;
    assert(curr_node->left!=nullptr);
    curr_node->left->change -= accumulated_change_from_node_to_successor;
    assert(curr_node->right!=nullptr);
    curr_node->right->change -= accumulated_change_from_node_to_successor;
    successor->change -= accumulated_change_from_node_to_successor;
//    if(successor->left!=nullptr){
//        successor->left->change += accumulated_change_from_node_to_successor;
//    }
    if(successor->right!=nullptr){
        successor->right->change+= accumulated_change_from_node_to_successor;
    }
    if(successor->right!=nullptr){
        bubble_up(successor->right, successor->right->key);
    }
//    if(successor->left!=nullptr){
//        bubble_up(successor->left, successor->left->key);
//    }
    bubble_up(successor,successor->key);//test tes
    bubble_up(curr_node->right,curr_node->right->key);//test
    bubble_up(curr_node->left,curr_node->left->key);//test
    bubble_up(curr_node,curr_node->key);//test


    //successor->change = tempChange;
    return curr_node;
}
template<class Key,class Value>
void RankTree<Key,Value>::update_bubble_all_tree(Node* n){
    return;
    if(n == nullptr){
        return;
    }
    update_bubble_all_tree(n->left);
    update_bubble_all_tree(n->right);
    bubble_up(this->root,n->key);
}
template <class Key, class Value>
typename RankTree<Key,Value>::Node* RankTree<Key, Value>::recursive_skeleton_build(int size, int height) {
    if (size <= 1) {
        Node* leaf_of_skeleton = new Node(FLAG_KEY);
        return leaf_of_skeleton;
    }
    int curr_size = (size - 1)/2;
    Node* left_sub_tree = recursive_skeleton_build(curr_size, height - 1);
    Node* right_sub_tree = recursive_skeleton_build(curr_size, height - 1);
    Value blank = Value();
    Node* father_of_sub_trees = new Node(0, blank, left_sub_tree, right_sub_tree, height);
    left_sub_tree->father = father_of_sub_trees;
    right_sub_tree->father = father_of_sub_trees;
    father_of_sub_trees->heightFromRoot = heightDetermination(father_of_sub_trees);
    father_of_sub_trees->BF = balanceFactorDetermination(father_of_sub_trees);
    father_of_sub_trees->nodes_in_sub_tree = num_nodes_sub_tree_determination(father_of_sub_trees);
    return father_of_sub_trees;
}



template <class Key, class Value>
void RankTree<Key,Value>::CutLeafs(Node* curr_node, int* amountOfLeafsExpectedToCut) {
    if (curr_node == nullptr || *amountOfLeafsExpectedToCut == 0) {
        return;
    }
    CutLeafs(curr_node->right, amountOfLeafsExpectedToCut);
    CutLeafs(curr_node->left, amountOfLeafsExpectedToCut);

    if (curr_node->key == FLAG_KEY) {

        Node* parent = curr_node->father;
        if (parent != nullptr) { //the trivial case(one node only)
            if (parent->right == curr_node) {  //only for valgrind. We don't want to point to a place that was deleted
                parent->right = nullptr;
            }
            else {
                parent->left = nullptr;
            }
        }
        (*amountOfLeafsExpectedToCut)--;
        delete curr_node;
    }
    else {
        curr_node->heightFromRoot= heightDetermination(curr_node);
        curr_node->BF = balanceFactorDetermination(curr_node);
        curr_node->nodes_in_sub_tree= num_nodes_sub_tree_determination(curr_node);
    }



}

template <class Key, class Value>
typename RankTree<Key, Value>::Node* RankTree<Key, Value>::createCompleteTree(int numOfElements) { //the caller
    // will check size < 0 validation //throw invalid argument exception
    //please notice the caller should update root to be the result and numOfElements
    if (numOfElements == 0) { // If requested an empty binary tree then return null
        return nullptr;
    }
    int exponentOfUpperBound = int(floor(log2(double(numOfElements)))) + 1;
    int numOfElementExpectedToDelete = pow(2, exponentOfUpperBound) - 1 - numOfElements;
    int* ptrNumOfElementsExpectedToDelete = new int(numOfElementExpectedToDelete);
    Node* rootOfSkeletonTree = recursive_skeleton_build(pow(2, exponentOfUpperBound)-1,exponentOfUpperBound-1);
    CutLeafs(rootOfSkeletonTree, ptrNumOfElementsExpectedToDelete);
    delete ptrNumOfElementsExpectedToDelete;
    return rootOfSkeletonTree;


}
/*
template<class Key, class Value>
void RankTree<Key, Value>::inorderFromArrayToTree(RankTree::Node *currNode, tuples<Key, Value> *array, int *index) {

}*/

template <class Key, class Value>
void RankTree<Key, Value>::inorderFromArrayToTree(RankTree<Key, Value>::Node* currNode, Key* arrayOfKeys, Value*
arrayOfValues, int* index) {
    if (currNode == nullptr) {
        return;
    }
    assert(arrayOfKeys != nullptr && arrayOfValues != nullptr);
    inorderFromArrayToTree(currNode->left, arrayOfKeys, arrayOfValues, index);
    currNode->key = arrayOfKeys[*index];
    currNode->value = arrayOfValues[*index];
    (*index)++;
    inorderFromArrayToTree(currNode->right, arrayOfKeys, arrayOfValues, index);
}
template <class Key, class Value>
void RankTree<Key, Value>::inorderFromTreeToArray(Key* arrayOfKeys, Value* arrayOfValues, int* index) {
    inorderFromTreeToArray_aux(this->root, arrayOfKeys, arrayOfValues, index);
    return;
}
template <class Key, class Value>
void RankTree<Key, Value>::inorderFromTreeToArray_aux(RankTree<Key, Value>::Node* currNode, Key* arrayOfKeys, Value*
arrayOfValues, int* index) {
    assert(arrayOfKeys != nullptr && arrayOfValues != nullptr);
    if (currNode == nullptr) {
        return;
    }
    inorderFromTreeToArray_aux(currNode->left, arrayOfKeys, arrayOfValues, index);
    arrayOfKeys[*index] = currNode->key;
    arrayOfValues[*index]= currNode->value;
    currNode->value = nullptr;
    (*index)++;
    inorderFromTreeToArray_aux(currNode->right, arrayOfKeys, arrayOfValues, index);
}
template <class Key, class Value>
typename RankTree<Key, Value>::Node* RankTree<Key, Value>::findMax() {
    if(this->root == nullptr){
        return nullptr;
    }
    return this->findMax_aux(this->root);
}

template <class Key, class Value>
typename RankTree<Key, Value>::Node* RankTree<Key, Value>::findMax_aux(Node* curr_node) {
    if (curr_node->right == nullptr) {
        return curr_node;
    }
    return findMax_aux(curr_node->right);
}
template <class Key, class Value>
typename RankTree<Key, Value>::Node* RankTree<Key, Value>::findMin() {
    if(this->root == nullptr){
        return nullptr;
    }
    return this->findMin_aux(this->root);
}
template <class Key, class Value>
typename RankTree<Key, Value>::Node* RankTree<Key, Value>::findMin_aux(Node* curr_node) {
    if (curr_node->left == nullptr) {
        return curr_node;
    }
    return findMin_aux(curr_node->left);
}

template <class Key, class Value>
int RankTree<Key, Value>::acumulated_sum(Node *curr) {
    if (curr->father == nullptr) {
        return curr->change;
    }
    int sum = acumulated_sum(curr->father);
    sum += curr->change;
    return sum;
}
//assuming that i and j are the closest to the given parameters low and high
template <class Key, class Value>
void RankTree<Key, Value>::add_amount(const Key& upper_bound, int amount) {
    this->add_amount_aux(this->root, false, upper_bound, amount);
}
//assuming that i and j are the closest to the given parameters low and high
template <class Key, class Value>
void RankTree<Key, Value>::add_amount_aux(typename RankTree<Key, Value>::Node* curr_node, bool is_last_turn_right, const
Key& upper_bound, int amount) {
    if (curr_node == nullptr) {
        return;
    }

    else if (curr_node->key == upper_bound) {
        if (!is_last_turn_right) {
            curr_node->change += amount;
            bubble_up(curr_node, curr_node->key);
        }
        if (curr_node->right != nullptr) {
            curr_node->right->change -= amount;
            bubble_up(curr_node->right, curr_node->right->key);
        }
    }

    else if (curr_node->key > upper_bound) {
        if (is_last_turn_right) {
            curr_node->change -= amount;
            bubble_up(curr_node, curr_node->key);
        }
        add_amount_aux(curr_node->left, false, upper_bound, amount);
    }

    else {
        if (!is_last_turn_right) {
            curr_node->change += amount;
            bubble_up(curr_node, curr_node->key);
        }
        add_amount_aux(curr_node->right, true, upper_bound, amount);
    }
}
template <class Key, class Value>
void RankTree<Key, Value>::bubble_up(typename RankTree<Key, Value>::Node* curr_node, const Key& key) {
    if (curr_node == nullptr) {
        return;
    }
//    if (curr_node->key == key) {
//        curr_node->to_bubble = bubble_field(curr_node);
//        return;
//    }
//    else if (curr_node->key < key) {
//        bubble_up(curr_node->right, key);
//    }
//    else {
//        bubble_up(curr_node->left, key);
//    }

    curr_node->to_bubble = bubble_field(curr_node);
    return bubble_up(curr_node->father,key);

}
template <class Key, class Value>
to_bubble_info RankTree<Key, Value>::bubble_field(typename RankTree<Key, Value>::Node* curr) {
    to_bubble_info right_son_bubble_field = to_bubble_info(INT_MIN);
    to_bubble_info left_son_bubble_field = to_bubble_info(INT_MIN);
    if (curr->right != nullptr) {
        right_son_bubble_field = curr->right->to_bubble;
    }
    if (curr->left != nullptr) {
        left_son_bubble_field = curr->left->to_bubble;
    }
    curr->generate_new_to_bubble_info(curr->key.strength,curr);
    to_bubble_info res = max_of_three(left_son_bubble_field, right_son_bubble_field, curr->to_bubble);
    res.add_change(curr->change);
    return res;

}

template <class Key, class Value>
to_bubble_info RankTree<Key, Value>::max_of_three( to_bubble_info& num1, to_bubble_info& num2, to_bubble_info&
num3) {
    to_bubble_info big_of_two = max(num1, num2);
    return max(big_of_two, num3);
}

template <class Key, class Value>
int RankTree<Key,Value>::algorithm_rank(const Key& key) const{
    return this->algorithm_rank_aux(key,this->root);
}
template <class Key, class Value>
int RankTree<Key, Value>::algorithm_rank_aux(const Key& key, Node* curr_node, int r) const{
    if (key == *(curr_node->getKey())) {
        if (curr_node->left != nullptr) {
            r = r + curr_node->left->nodes_in_sub_tree + 1;
        }
        else {
            r = r + 1; //with me
        }
        return r;
    }
    else if (key < *(curr_node->getKey())) {
        return algorithm_rank_aux(key, curr_node->left, r);
    }
    else {
        if (curr_node->left != nullptr) {
            r = r + curr_node->left->nodes_in_sub_tree + 1;
        }
        else {
            r = r + 1;
        }
        return algorithm_rank_aux(key, curr_node->right, r);
    }
}
template <class Key, class Value>
typename RankTree<Key, Value>::Node* RankTree<Key, Value>::select(int index) const {
    return this->select_aux(index+1, root);
}

template <class Key, class Value>
typename RankTree<Key, Value>::Node* RankTree<Key, Value>::select_aux(int index, Node* curr_node) const {
    if(curr_node == nullptr){
        return nullptr;
    }
    if (w(curr_node->left) == index - 1) {
        return curr_node;
    }
    else if (w(curr_node->left) > index -1) {
        return select_aux(index, curr_node->left);
    }
    else  {
        return select_aux(index - w(curr_node->left) - 1, curr_node->right);
    }
}

template <class Key, class Value>
int RankTree<Key, Value>::w(Node* node) const{
    return node == nullptr? 0: node->nodes_in_sub_tree;
}

template <class Key, class Value>
typename RankTree<Key, Value>::Node* RankTree<Key, Value>::next_to_low_from_above(const Key& low) {
    return this->next_to_low_from_above_aux(root, low);
}

template <class Key, class Value>
typename RankTree<Key, Value>::Node* RankTree<Key, Value>::next_to_low_from_above_aux(Node* curr_node, const Key& low) {
    assert(curr_node != nullptr);
    if (low == *(curr_node->getKey())) {
        return curr_node;
    }
    else if (low < *(curr_node->getKey())) {
        if (curr_node->left != nullptr ) {
            return next_to_low_from_above_aux(curr_node->left, low);
        }
        return curr_node;
    }
    else {
        if (curr_node->right != nullptr ) {
            return next_to_low_from_above_aux(curr_node->right, low);
        }
        if (curr_node->key < low) {
            return findNextNodeInOrder(curr_node->key);
        }
        return curr_node;
    }
}

template <class Key, class Value>
typename RankTree<Key, Value>::Node* RankTree<Key, Value>::next_to_up_bound_from_down(const Key& upper_bound) {
    return this->next_to_up_bound_from_down_aux(root, upper_bound);
}

template <class Key, class Value>
typename RankTree<Key, Value>::Node* RankTree<Key, Value>::next_to_up_bound_from_down_aux(Node* curr_node, const Key&
upper_bound) {
    assert(curr_node != nullptr);
    if (upper_bound == *(curr_node->getKey())) {
        return curr_node;
    }
    else if (upper_bound < *(curr_node->getKey())) {
        if (curr_node->left != nullptr) {
            return next_to_up_bound_from_down_aux(curr_node->left, upper_bound);
        }
        if (*(curr_node->getKey()) > upper_bound) {
            return findprevNodeInOrder(curr_node->key);
        }
        return curr_node;
    }
    else {
        if (curr_node->right != nullptr) {
            return next_to_up_bound_from_down_aux(curr_node->right, upper_bound);
        }
        return curr_node;
    }
}

template<class Key, class Value>
void RankTree<Key, Value>::add(const Key& upper_bound,const Key& low_bound, int amount) {
    this->add_amount(upper_bound, amount);
    auto prev = this->findprevNodeInOrder(low_bound);
    if (prev != nullptr) { //otherwise this is the smallest elem in tree, and we dont need to update his left son
        // which does not exist
        this->add_amount( prev->key,-1*amount);
    }
}


#endif //WINTERRANK_RANKTREE_H
