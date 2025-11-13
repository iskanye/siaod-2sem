#include <iostream>
#include <clocale>
#include <string>

using namespace std;

void testShannonFano(string text);

int main() {
    setlocale(LC_ALL, "");
    string text;
    getline(cin, text);

    testShannonFano(text);
}
