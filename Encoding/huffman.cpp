#include <queue>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

using namespace std;

unordered_map<char, int> calculateFrequencies(const string& text);

struct HuffmanNode {
    char symbol;
    int frequency;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(char s, int f) : symbol(s), frequency(f), left(nullptr), right(nullptr) {}
    HuffmanNode(int f, HuffmanNode* l, HuffmanNode* r)
        : symbol('\0'), frequency(f), left(l), right(r) {}
};

struct CompareNodes {
    bool operator()(const HuffmanNode* a, const HuffmanNode* b) {
        return a->frequency > b->frequency;
    }
};

// Функция для построения кодов Хаффмана
void buildHuffmanCodes(HuffmanNode* node,
                       const string& code,
                       unordered_map<char, string>& codes) {
    if (!node)
        return;

    if (node->symbol != '\0') {
        codes[node->symbol] = code;
        return;
    }

    buildHuffmanCodes(node->left, code + "0", codes);
    buildHuffmanCodes(node->right, code + "1", codes);
}

string compressHuffman(const string& text, const unordered_map<char, string>& codes) {
    string compressed = "";
    for (char c : text) {
        compressed += codes.at(c);
    }
    return compressed;
}

string decompressHuffman(const string& compressed, HuffmanNode* root) {
    string decompressed = "";
    HuffmanNode* current = root;

    for (char bit : compressed) {
        if (bit == '0') {
            current = current->left;
        } else {
            current = current->right;
        }

        if (current->symbol != '\0') {
            decompressed += current->symbol;
            current = root;
        }
    }

    return decompressed;
}

void testHuffman() {
    cout << "\n=== Алгоритм Хаффмана ===" << endl;
    string fullName = "бикташев искандер ирикович";
    cout << "Исходная строка: " << fullName << endl;

    // Подсчет частот
    unordered_map<char, int> frequencies = calculateFrequencies(fullName);

    // Создание приоритетной очереди
    priority_queue<HuffmanNode*,
                   vector<HuffmanNode*>,
                   CompareNodes> pq;

    for (auto& pair : frequencies) {
        pq.push( new HuffmanNode(pair.first, pair.second));
    }

    // Построение дерева Хаффмана
    while (pq.size() > 1) {
        auto left = pq.top(); pq.pop();
        auto right = pq.top(); pq.pop();

        int combinedFreq = left->frequency + right->frequency;
        auto parent = new HuffmanNode(combinedFreq, left, right);
        pq.push(parent);
    }

    HuffmanNode* root = pq.top();

    // Построение кодов
    unordered_map<char, string> huffmanCodes;
    buildHuffmanCodes(root, "", huffmanCodes);

    cout << "\nТаблица кодов Хаффмана:" << endl;
    for (auto& pair : huffmanCodes) {
        cout << "'" << pair.first << "' : " << pair.second
             << " (частота: " << frequencies[pair.first] << ")" << endl;
    }

    string compressed = compressHuffman(fullName, huffmanCodes);
    cout << "\nЗакодированная строка: " << compressed << endl;

    string decompressed = decompressHuffman(compressed, root);
    cout << "Декодированная строка: " << decompressed << endl;

    // Расчет сжатия
    int originalBits = fullName.length() * 8;
    int compressedBits = compressed.length();
    double compressionRatio = (1.0 - (double)compressedBits / originalBits) * 100;

    cout << "\nЭффективность сжатия:" << endl;
    cout << "Исходный размер: " << originalBits << " бит" << endl;
    cout << "Сжатый размер: " << compressedBits << " бит" << endl;
    cout << "Коэффициент сжатия: " << compressionRatio << "%" << endl;
}
