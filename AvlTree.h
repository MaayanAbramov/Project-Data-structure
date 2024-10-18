//
// Created by maaya on 20/03/2024.
//

#ifndef WINTERRANK_AVLTREE_H
#define WINTERRANK_AVLTREE_H
//
// Created by maaya on 03/01/2024.
//


#include <iostream>
#include <cassert>
#include <cmath>
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


template <class Key, class Value>
class AvlTree{


public:
    class Node {
    public:
        Node* father;
        Node* right;
        Node* left;
        Key key;
        Value value;
        int heightFromRoot;
        int BF; //balance factor
    public:
        explicit Node(const Key& newkey, const Value& newvalue, Node* newLeft, Node* newRight, int newHeight) :
                father(nullptr), right(newRight), left(newLeft), key(newkey), value(newvalue), heightFromRoot(newHeight), BF
                (ZERO_INIT) {}
        explicit Node(const Key& newKey) : father(nullptr), right(nullptr), left(nullptr), key(newKey), value
                (DIFF_ZERO),
                                           heightFromRoot(ZERO_INIT), BF(ZERO_INIT){}
        explicit Node(const Key& newKey, const Value& newValue) : father(nullptr), right(nullptr), left(nullptr), key
                (newKey),
                                                                  value(newValue), heightFromRoot(ZERO_INIT), BF(ZERO_INIT){}

        //to access value and key objects without copying it by value, we will write getters:
        Key* getKey() {return &key;}
        Value* getValue() {return &value;}
    };
    AvlTree() : root(nullptr), numOfNodes(ZERO_INIT) {} //big three-------------------------------------

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
    Node* swapNodes(Node* toDelete, Node* NextInorder);
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
        if (node->father == nullptr) { //this is a post order deletion
            deleteTrivialTree();
        }
        else {
            delete node;
        }

    }

    ~AvlTree() {
        DeleteWholeTree(root);
    }

    AvlTree& operator=(const AvlTree& other) = delete;


    Node* find(const Key& key, Node* curr_node) const;
    Node* findMax(Node* curr_node);
    Node* findMin(Node* curr_node);
    static void destroy_the_tree_and_values(Node* root);
    static int heightDetermination(Node* node);
    static int balanceFactorDetermination(Node* node);
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
    static void inorderFromArrayToTree(AvlTree<Key, Value>::Node* currNode, Key* arrayOfKeys, Value* arrayOfValues,
                                       int* index);
    //testing funcitons
    bool is_tree_valid(Node* root);
    int getHeight(Node* root);
    int BalanceFactor(Node* node);
    //int getWeight(Node* root);
    void print2DHelper(Node* root, int space);
    void print2D(Node* root);
public: //TO DO : change it to private
    Node* root;
    int numOfNodes;
};

template <class Key, class Value>
void AvlTree<Key, Value>::deleteTrivialTree() {
    assert(numOfNodes > 0);
    numOfNodes--;
    delete root;
    root = nullptr;
    return;
}
template <class Key, class Value>
void AvlTree<Key,Value>::destroy_the_tree_and_values(Node* root){
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
    delete(value);
    root->value=nullptr;

}
template <class Key, class Value>
typename AvlTree<Key, Value>::Node* AvlTree<Key, Value>::deleteParentOfOneBrotherOnly() {
    assert((root->right == nullptr && root->left != nullptr) || (root->left == nullptr && root->right != nullptr));
    Node* copyOfSon = (root->right == nullptr) ? root->left : root->right;
    copyOfSon->father = nullptr; // we don't want to point to a deleted node (valgrind)
    numOfNodes--;
    delete root;
    root = copyOfSon;
    updateFieldsAfterChangeInTree(root);
    return nullptr;
}

template <class Key, class Value>
void AvlTree<Key, Value>::deleteLeaf(Node* leafToRemove) {
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
void AvlTree<Key, Value>::updateNodesTillRoot(Node* curr_node) {
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
typename AvlTree<Key, Value>::Node* AvlTree<Key, Value>::find(const Key& key, Node* curr_node) const {
    if (curr_node == nullptr || *(curr_node->getKey()) == key) {
        return curr_node;
    }
    return (key < *(curr_node->getKey())) ? find(key, curr_node->left) : find(key, curr_node->right);
}

template <class Key, class Value>
int AvlTree<Key, Value>::heightDetermination(Node *node) {
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
int AvlTree<Key, Value>::balanceFactorDetermination(Node* node) {
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
void AvlTree<Key, Value>::updateFieldsAfterChangeInTree(Node *node) {
    if (node == nullptr) {
        return;
    }
    node->heightFromRoot = heightDetermination(node);
    node->BF = balanceFactorDetermination(node);
}

template <class Key, class Value>
void AvlTree<Key, Value>::rotationLL(Node* node) {
    assert(node->left != nullptr);
    Node* tempA = node->left;
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
        }
        else {
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
    return;
}
template <class Key, class Value>
void AvlTree<Key, Value>::printTreeWithInfo(typename AvlTree<Key, Value>::Node* node, int depth , char branch) {
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
void AvlTree<Key, Value>::rotationRR(Node* node) {
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
        }
        else {
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
}

template <class Key, class Value>
void AvlTree<Key, Value>::rotationLR(Node* node){ //same as beneath
    Node* tempC = node;
    assert(tempC->left != nullptr && tempC->left->right != nullptr);
    Node* tempA = tempC->left;
    rotationRR(tempA); //notice that this line was tempC->left = rotationRR(tempA);
    tempC->left->father = tempC;
    rotationLL(tempC); //notice to the difference with rotation RL
    return;
}

template <class Key, class Value>
void AvlTree<Key, Value>::rotationRL(Node* node){ // to understand this you should check
    //Maayan's notebook, she has a scatch
    Node* tempC = node;
    assert(tempC->right != nullptr && tempC->right->left != nullptr);
    Node* tempA = tempC->right;
    rotationLL(tempA); //tempC->right = rotationLL(tempA);
    tempC->right->father = tempC;
    rotationRR(tempC); //this value should be returned, I changed it
    return;
}


template <class Key, class Value>
typename AvlTree<Key, Value>::Node* AvlTree<Key, Value>::rotate(Node *node) {
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
void AvlTree<Key, Value>::insertToAnEmptyTree(const Key& key, const Value& value) {
    numOfNodes++;
    root = new Node(key, value);
    updateFieldsAfterChangeInTree(root);
    return;
}

template <class Key, class Value>
void AvlTree<Key, Value>::insertAux(const Key& key, const Value& value) {
    if (root == nullptr) {
        insertToAnEmptyTree(key, value);
        return;
    }//else if there the tree is not empty
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
void AvlTree<Key, Value>::recursive_insert(Node* curr_node, const Key& key, const
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
    }
    if (key > *(curr_node->getKey()) && curr_node->right == nullptr) {
        numOfNodes++;
        curr_node->right = new Node(key, value);
        curr_node->right->father = curr_node;
        updateFieldsAfterChangeInTree(curr_node->right);
        *createdAlready = true;
    }
    if (key > *(curr_node->getKey()) && !(*createdAlready)) {
        recursive_insert(curr_node->right, key, value, createdAlready);
    } else if (key < *(curr_node->getKey()) && !(*createdAlready)) {
        recursive_insert(curr_node->left, key, value, createdAlready);
    }
    updateFieldsAfterChangeInTree(curr_node);
    rotate(curr_node);
}



template <class Key, class Value>
void AvlTree<Key, Value>::remove(const Key& keyToRemove) { //help functions should be recursive, to
    // update the route
    // till the root
    Key copy = keyToRemove;
    if (root == nullptr) {
        throw invalid_argument("The key is empty");
    }
    Node* toRemove = find(keyToRemove, root);
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
void AvlTree<Key, Value>::deleteNodeIfHasOneSon(const Key& keyToDelete, Node* curr_node) { //may be need update till
    // the root, even thought it looks like not because height of subtree of root changes only in 1
    Node* toRemove = find(keyToDelete, root); //the callee already deals with not found case
    Node* fatherInLaw = toRemove->father;
    if (toRemove == root) {
        deleteParentOfOneBrotherOnly();
        return;
    }
    if (toRemove->right == nullptr) {
        //toRemove->left->father = fatherInLaw;
        if (fatherInLaw->right == toRemove) {
            fatherInLaw->right = toRemove->left;
            fatherInLaw->right->father = fatherInLaw;
            updateFieldsAfterChangeInTree(fatherInLaw->right);
        }
        else if (fatherInLaw->left == toRemove) {
            fatherInLaw->left = toRemove->left;
            fatherInLaw->left->father = fatherInLaw;
            updateFieldsAfterChangeInTree(fatherInLaw->left);
        }
        toRemove->left = nullptr; //obviously
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
        }
        else if (fatherInLaw->right == toRemove) {
            fatherInLaw->right = toRemove->right;
            fatherInLaw->right->father = fatherInLaw;
            updateFieldsAfterChangeInTree(fatherInLaw->right);
        }
        toRemove->right = nullptr;
        toRemove->father = nullptr;
        numOfNodes--;
        delete toRemove;
        updateFieldsAfterChangeInTree(fatherInLaw);
    }
    updateNodesTillRoot(fatherInLaw);
}

template <class Key, class Value>
typename AvlTree<Key, Value>::Node* AvlTree<Key, Value>::findNextNodeInOrder(const Key& key) {
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
void AvlTree<Key, Value>::deleteNodeIfHasTwoSons(Key* keyToDelete, Node* curr_node) {
    if (*(curr_node->getKey()) == *keyToDelete) {
        Node* successor = findNextNodeInOrder(*keyToDelete); //successor has to be a leaf or to have one son only
        //cout << "the successor of 3 is --------------------" << successor->key << endl;
        //cout << "before swap" << endl;
        //printTreeWithInfo(root, 0, '-');
        curr_node = swapNodes(curr_node, successor);
        //cout << "after swap" << endl;
        //printTreeWithInfo(root, 0, '-');
        updateFieldsAfterChangeInTree(successor);
        updateFieldsAfterChangeInTree(curr_node);
        //rotate(successor);
        //rotate(curr_node);
        *keyToDelete = *(successor->getKey());
        if (successor->left == nullptr && successor->right == nullptr) {
            deleteLeaf(successor);
            return;
        }
        if (successor->left == nullptr) {
            if (successor->father->right == successor) { // pleasenotice that if the successor is a right son then his
                // father is curr_node
                curr_node->right = successor->right;
                if(successor->right != nullptr) {
                    successor->right->father = curr_node;
                }
                successor->father = nullptr;
                successor->right = nullptr;
                delete successor;
                numOfNodes--;
                updateNodesTillRoot(curr_node->right);
                return;
            }
            if (successor->father->left == successor) { //please notice that if the successor is a left son it means
                // that the father is not curr_node
                Node* successorsFather = successor->father;
                successorsFather->left = successor->right;
                if(successor->right != nullptr) {
                    successor->right->father = successorsFather;
                }
                successor->right = nullptr;
                successor->father = nullptr;
                delete successor;
                numOfNodes--;
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
typename AvlTree<Key, Value>::Node* AvlTree<Key, Value>::swapNodes(Node* curr_node, Node* successor) { //curr_node,
    // successor
    Key tempK = *(curr_node->getKey());
    Value tempV = *(curr_node->getValue());
    curr_node->key = successor->key;
    curr_node->value = successor->value;
    successor->key = tempK;
    successor->value = tempV;
    return curr_node;
}

template <class Key, class Value>
typename AvlTree<Key,Value>::Node* AvlTree<Key, Value>::recursive_skeleton_build(int size, int height) {
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
    return father_of_sub_trees;
}



template <class Key, class Value>
void AvlTree<Key,Value>::CutLeafs(Node* curr_node, int* amountOfLeafsExpectedToCut) {
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
    }



}

template <class Key, class Value>
typename AvlTree<Key, Value>::Node* AvlTree<Key, Value>::createCompleteTree(int numOfElements) { //the caller
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
void AvlTree<Key, Value>::inorderFromArrayToTree(AvlTree::Node *currNode, tuples<Key, Value> *array, int *index) {

}*/

template <class Key, class Value>
void AvlTree<Key, Value>::inorderFromArrayToTree(AvlTree<Key, Value>::Node* currNode, Key* arrayOfKeys, Value*
arrayOfValues, int* index) {
    assert(arrayOfKeys != nullptr && arrayOfValues != nullptr);
    if (currNode == nullptr) {
        return;
    }
    inorderFromArrayToTree(currNode->left, arrayOfKeys, arrayOfValues, index);
    currNode->key = arrayOfKeys[*index];
    currNode->value = arrayOfValues[*index];
    (*index)++;
    inorderFromArrayToTree(currNode->right, arrayOfKeys, arrayOfValues, index);
}

template <class Key, class Value>
typename AvlTree<Key, Value>::Node* AvlTree<Key, Value>::findMax(Node* curr_node) {
    if (curr_node->right == nullptr) {
        return curr_node;
    }
    return findMax(curr_node->right);
}

template <class Key, class Value>
typename AvlTree<Key, Value>::Node* AvlTree<Key, Value>::findMin(Node* curr_node) {
    if (curr_node->left == nullptr) {
        return curr_node;
    }
    return findMin(curr_node->left);
}



#endif //WINTERRANK_AVLTREE_H
