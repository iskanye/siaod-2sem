#include <iostream>
#include <clocale>
#include <string>
#include <unordered_map>

using namespace std;

void testShannonFano(string text);
void testHuffman();

unordered_map<char, int> calculateFrequencies(const string& text) {
    unordered_map<char, int> frequencies;
    for (char c : text) {
        frequencies[c]++;
    }
    return frequencies;
}

int main() {
    setlocale(LC_ALL, "");
    string text;
    getline(cin, text);

    cout << endl;

    testShannonFano(text);

    cout << endl;

    testHuffman();
}
