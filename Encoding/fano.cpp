#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <fstream>
#include <bitset>
#include <sstream>
#include <cmath>

using namespace std;

// Структура для узла дерева Шеннона-Фано
struct ShannonFanoNode {
    char symbol;
    int frequency;
    string code;

    ShannonFanoNode(char s, int f) : symbol(s), frequency(f), code("") {}
};

unordered_map<char, int> calculateFrequencies(const string& text) {
    unordered_map<char, int> frequencies;
    for (char c : text) {
        frequencies[c]++;
    }
    return frequencies;
}

void buildShannonFanoCodes(vector<ShannonFanoNode>& nodes, int start, int end) {
    if (start >= end || start == end - 1)
        return;

    // Находим точку разделения
    int total = 0;
    for (int i = start; i < end; i++) {
        total += nodes[i].frequency;
    }

    int sum = 0;
    int splitIndex = start;
    int minDiff = INT_MAX;

    for (int i = start; i < end; i++) {
        sum += nodes[i].frequency;
        int diff = abs(total - 2 * sum);
        if (diff < minDiff) {
            minDiff = diff;
            splitIndex = i + 1;
        }
    }

    for (int i = start; i < splitIndex; i++) {
        nodes[i].code += "0";
    }
    for (int i = splitIndex; i < end; i++) {
        nodes[i].code += "1";
    }

    buildShannonFanoCodes(nodes, start, splitIndex);
    buildShannonFanoCodes(nodes, splitIndex, end);
}

string compressShannonFano(const string& text, unordered_map<char, string>& codes) {
    string compressed = "";
    for (char c : text) {
        compressed += codes[c];
    }
    return compressed;
}

string decompressShannonFano(const string& compressed, unordered_map<string, char>& codeToChar) {
    string decompressed, sub;

    int i = 0, j = 1;
    for (; j < compressed.length(); j++) {
        sub = compressed.substr(i, j - i);

        if (codeToChar.count(sub) == 1) {
            decompressed += codeToChar.at(sub);
            i = j;
        }
    }

    decompressed += codeToChar.at(compressed.substr(i));

    return decompressed;
}

// Главная функция для тестирования Шеннона-Фано
void testShannonFano(string text) {
    cout << "=== Алгоритм Шеннона-Фано ===" << endl;

    cout << "Исходный текст: " << text << endl;
    cout << "Длина исходного текста: " << text.length() << " символов" << endl;

    // Подсчет частот
    unordered_map<char, int> frequencies = calculateFrequencies(text);

    // Создание узлов
    vector<ShannonFanoNode> nodes;
    for (auto& pair : frequencies) {
        nodes.push_back(ShannonFanoNode(pair.first, pair.second));
    }

    sort(nodes.begin(), nodes.end(),
         [](const ShannonFanoNode& a, const ShannonFanoNode& b) {
             return a.frequency > b.frequency;
         });

    // Построение кодов
    buildShannonFanoCodes(nodes, 0, nodes.size());

    // Создание таблицы кодирования
    unordered_map<char, string> charToCode;
    unordered_map<string, char> codeToChar;

    cout << "\nТаблица кодов Шеннона-Фано:" << endl;
    for (auto& node : nodes) {
        charToCode[node.symbol] = node.code;
        codeToChar[node.code] = node.symbol;
        cout << "'" << node.symbol << "' : " << node.code
             << " (частота: " << node.frequency << ")" << endl;
    }

    string compressed = compressShannonFano(text, charToCode);
    string decompressed = decompressShannonFano(compressed, codeToChar);

    // Вывод результатов
    cout << "\nРезультаты сжатия:" << endl;
    cout << "Зашифрованный текст:" << compressed << endl;
    cout << "Длина исходного текста (байт): " << text.length() << endl;
    cout << "Длина сжатых данных (байт): " << (double)compressed.length() / 8.0 << endl;

    double compressionRatio = (1.0 - ((double)compressed.length() / 8.0) / text.length()) * 100;
    cout << "Процент сжатия: " << compressionRatio << "%" << endl;
}
