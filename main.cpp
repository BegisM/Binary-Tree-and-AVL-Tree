#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <cmath>

struct TreeNode {
    int key;
    int height;
    TreeNode* left;
    TreeNode* right;

    TreeNode(int k) : key(k), height(1), left(nullptr), right(nullptr) {}
};

class BinaryTree {
private:
    TreeNode* root;

    TreeNode* insert(TreeNode* node, int key) {
        if (node == nullptr) {
            return new TreeNode(key);
        }

        if (key < node->key) {
            node->left = insert(node->left, key);
        } else if (key > node->key) {
            node->right = insert(node->right, key);
        } else {
            // Ignore duplicate keys
            return node;
        }

        return node;
    }

    void inOrderTraversal(TreeNode* node) {
        if (node != nullptr) {
            inOrderTraversal(node->left);
            std::cout << node->key << " ";
            inOrderTraversal(node->right);
        }
    }

public:
    BinaryTree() : root(nullptr) {}

    void insertKey(int key) {
        root = insert(root, key);
    }

    void traverseInOrder() {
        inOrderTraversal(root);
    }
};

class AVLTree {
private:
    TreeNode* root;

    int getHeight(TreeNode* node) {
        return (node != nullptr) ? node->height : 0;
    }

    int getBalanceFactor(TreeNode* node) {
        return (node != nullptr) ? getHeight(node->left) - getHeight(node->right) : 0;
    }

    void updateHeight(TreeNode* node){
        if (node != nullptr){
            node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
        }
    }

    TreeNode* rotateRight(TreeNode* y) {
        TreeNode* x = y->left;
        TreeNode* T2 = x->right;

        x->right = y;
        y->left = T2;

        updateHeight(y);
        updateHeight(x);

        return x;
    }

    TreeNode* rotateLeft(TreeNode* x) {
        TreeNode* y = x->right;
        TreeNode* T2 = y->left;

        y->left = x;
        x->right = T2;

        updateHeight(y);
        updateHeight(x);

        return y;
    }

    TreeNode* insert(TreeNode* node, int key) {
        if (node == nullptr) {
            return new TreeNode(key);
        }

        if (key < node->key) {
            node->left = insert(node->left, key);
        } else if (key > node->key) {
            node->right = insert(node->right, key);
        } else {
            // Ignore duplicate keys
            return node;
        }

        updateHeight(node);

        int balance = getBalanceFactor(node);

        // Left Heavy
        if (balance > 1) {
            if (key < node->left->key) {
                return rotateRight(node);
            } else {
                if (node->left->right == nullptr) {
                    return rotateRight(node);
                } else {
                    node->left = rotateLeft(node->left);
                    return rotateRight(node);
                }
            }
        }

        // Right Heavy
        if (balance < -1) {
            if (key > node->right->key) {
                return rotateLeft(node);
            } else {
                if (node->right->left == nullptr) {
                    return rotateLeft(node);
                } else {
                    node->right = rotateRight(node->right);
                    return rotateLeft(node);
                }
            }
        }

        return node;
    }

    void inOrderTraversal(TreeNode* node) {
        if (node != nullptr) {
            inOrderTraversal(node->left);
            std::cout << node->key << " ";
            inOrderTraversal(node->right);
        }
    }

public:
    AVLTree() : root(nullptr) {}

    void insertKey(int key) {
        root = insert(root, key);
    }

    void traverseInOrder() {
        inOrderTraversal(root);
    }
};

long runKeyInsertionBinaryTree(BinaryTree& binaryTree, const std::vector<int>& keys) {
    auto start = std::chrono::high_resolution_clock::now();
    for (int key : keys) {
        binaryTree.insertKey(key);
    }
    auto end = std::chrono::high_resolution_clock::now();
    long binaryTreeTime = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    return binaryTreeTime;
}

long runKeyInsertionAVLTree(AVLTree& avlTree, const std::vector<int>& keys) {
    auto start = std::chrono::high_resolution_clock::now();
    for (int key : keys) {
        avlTree.insertKey(key);
    }
    auto end = std::chrono::high_resolution_clock::now();
    long avlTreeTime = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    return avlTreeTime;
}

void getSortedBalancedVector(std::vector<int>& array, int start, int end, int& value) {
    if (start > end) {
        return;
    }

    int middle = (start + end) / 2;

    getSortedBalancedVector(array, start, middle - 1, value);
    array[middle] = value++;
    getSortedBalancedVector(array, middle + 1, end, value);
}

void runInsertionBESTCASEBinaryTree(BinaryTree& bn, const std::vector<int>& keys, int start, int end){
    if (start > end){
        return;
    }

    int middle = (start + end) / 2;
    bn.insertKey( keys[middle]);
    runInsertionBESTCASEBinaryTree(bn, keys, start, middle - 1);
    runInsertionBESTCASEBinaryTree(bn, keys, middle + 1, end);
}




int main() {
    std::random_device rd;
    std::mt19937 gen(rd());

    const int iterations = 100;
    std::cout << "size \t binary_tree \t avl_tree \tbestcase_avl \t std_set" << std::endl;



    for (int m = 10; m < 16; m++) {
        int dataSize = static_cast<int>(std::pow(2, m)) - 1;
        std::uniform_int_distribution<> dist(1, dataSize);
        long binaryTreeTime = 0;
        long avlTreeTime = 0;
        long stdSetTime = 0;
        long AVLBestCaseTimes = 0;

        BinaryTree binaryTree;
        AVLTree avlTree;
        AVLTree AVLBestCaseTree;
        std::set<int> stdSetContainer;

        std::vector<int> randomKeys(dataSize);
        for (int i = 0; i < dataSize; i++) {
            randomKeys[i] = dist(gen);
        }

        std::vector<int>bestCaseKeys(dataSize);
        int value = 1;

        getSortedBalancedVector(bestCaseKeys, 0, dataSize - 1, value);

        binaryTreeTime += runKeyInsertionBinaryTree(binaryTree, randomKeys);
        avlTreeTime += runKeyInsertionAVLTree(avlTree, randomKeys);

        auto best_CASE_time_start = std::chrono::high_resolution_clock::now();
        runKeyInsertionAVLTree(AVLBestCaseTree, bestCaseKeys);
        auto best_CASE_time_End = std::chrono::high_resolution_clock::now();
        AVLBestCaseTimes += std::chrono::duration_cast<std::chrono::nanoseconds>(best_CASE_time_End - best_CASE_time_start).count();

        auto stdSetStart = std::chrono::high_resolution_clock::now();
        for (int key : randomKeys) {
            stdSetContainer.insert(key);
        }
        auto stdSetEnd = std::chrono::high_resolution_clock::now();
        stdSetTime += std::chrono::duration_cast<std::chrono::nanoseconds>(stdSetEnd - stdSetStart).count();


        std::cout
                << dataSize << " \t\t "
                << binaryTreeTime / iterations << " \t\t\t\t"
                << avlTreeTime / iterations << " \t\t\t"
                << AVLBestCaseTimes / iterations << " \t\t\t "
                << stdSetTime / iterations << std::endl;
    }

    return 0;
}
