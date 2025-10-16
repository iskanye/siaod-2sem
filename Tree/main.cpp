#include <iostream>
#include <queue>
#include <stack>
#include <algorithm>

using namespace std;

// Звено дерева
struct Node {
    char key;
    Node* left;
    Node* right;
    int height;
    Node(char k) : key(k), left(nullptr), right(nullptr), height(1) {}
};

// Поиск высоты поддерева
int height(Node* N) {
    return N ? N->height : 0;
}

// Правый поворот
Node* rightRotate(Node* y) {
    Node* x = y->left;
    Node* temp = x->right;

    x->right = y;
    y->left = temp;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

// Левый поворот
Node* leftRotate(Node* x) {
    Node* y = x->right;
    Node* temp = y->left;

    y->left = x;
    x->right = temp;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

// Поиск разницы высот поддеревьев
int getBalance(Node* node) {
    if (!node) return 0;
    return height(node->left) - height(node->right);
}

// Балансировка
Node* balance(Node* node){
    if (!node)
        return nullptr;

    node->height = 1 + max(height(node->left), height(node->right));
    int balance = getBalance(node);

    if (balance > 1 && getBalance(node->left) >= 0)
        return rightRotate(node);

    if (balance < -1 && getBalance(node->right) <= 0)
        return leftRotate(node);

    if (balance > 1 && getBalance(node->left) < 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && getBalance(node->right) > 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

// Вставка звена в поддерево
Node* insert(Node* node, char key) {
    if (!node)
        return new Node(key);

    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    else // равные ключи не допускаются
        return node;

    return balance(node);
}

// Поиск наименьшего звена в поддереве
Node* findMinNode(Node* node) {
    Node* current = node;
    while (current && current->left) {
        current = current->left;
    }
    return current;
}

// Удалить ноду из поддерева
Node* deleteNode(Node* node, char value) {
    if (!node) {
        return nullptr;
    }

    // Поиск удаляемого узла
    if (value < node->key) {
        node->left = deleteNode(node->left, value);
    } else if (value > node->key) {
        node->right = deleteNode(node->right, value);
    } else {
        // Узел найден - выполняем удаление

        // Узел с одним потомком или без потомков
        if (!node->left || !node->right) {
            Node* temp = node->left ? node->left : node->right;

            // Нет потомков
            if (!temp) {
                node = nullptr;
            } else {
                // Один потомок
                *node = *temp; // Копируем содержимое потомка
            }
            delete temp;
        } else {
            // Узел с двумя потомками
            Node* temp = findMinNode(node->right);
            node->key = temp->key;
            node->right = deleteNode(node->right, temp->key);
        }
    }

    // Балансировка дерева
    return balance(node);
}

// Симметричный обход
void inOrder(Node* root) {
    if (root) {
        inOrder(root->left);
        cout << root->key << ' ';
        inOrder(root->right);
    }
}

// DFS
void DFS(Node* root) {
    if (!root) return;
    stack<Node*> q;
    q.push(root);
    while (!q.empty()) {
        Node* node = q.top();
        q.pop();
        cout << node->key << ' ';
        if (node->right) q.push(node->right);
        if (node->left) q.push(node->left);
    }
}

// Обход в ширину
void breadthFirst(Node* root) {
    if (!root) return;
    queue<Node*> q;
    q.push(root);
    while (!q.empty()) {
        Node* node = q.front();
        q.pop();
        cout << node->key << ' ';
        if (node->left) q.push(node->left);
        if (node->right) q.push(node->right);
    }
}

// Сумма звеньев
int sumLeafValues(Node* root) {
    if (!root) return 0;
    if (!root->left && !root->right)
        return (int)root->key;
    return sumLeafValues(root->left) + sumLeafValues(root->right);
}

void info(Node* root) {
    cout << "Симметричный обход: ";
    inOrder(root);
    cout << '\n';
    DFS(root);
    cout << '\n';

    cout << "Обход в ширину: ";
    breadthFirst(root);
    cout << '\n';

    cout << "Сумма значений листьев: " << sumLeafValues(root) << '\n';
    cout << "Высота дерева: " << height(root) << '\n';
}

int main() {
    setlocale(LC_ALL, "");
    Node* root = nullptr;
    root = insert(root, 'F');
    root = insert(root, 'B');
    root = insert(root, 'A');
    root = insert(root, 'D');
    root = insert(root, 'C');
    root = insert(root, 'E');
    root = insert(root, 'G');

    info(root);

    root = deleteNode(root, 'D');

    info(root);

    return 0;
}
