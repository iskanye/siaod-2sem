#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <unordered_set>

using namespace std;

string removeRepeatedWords(string sentence) {
    unordered_map<string, int> wordCount;
    istringstream in(sentence);
    string word;
    vector<string> words;

    while (in >> word) {
        wordCount[word]++;
        words.push_back(word);
    }

    string result;
    for (auto w : words) {
        if (wordCount[w] == 1) {
            if (!result.empty())
                result += " ";
            result += w;
        }
    }
    return result;
}

vector<int> prefixFunc(string pattern) {
    int N = pattern.length();
    vector<int> pi(N, 0);
    int j = 0;
    int i = 1;

    while (i < N) {
        if (pattern[i] == pattern[j]) {
            pi[i] = j + 1;
            i++;
            j++;
        } else if (j == 0) {
            pi[i] = 0;
            i++;
        } else {
            j = pi[j - 1];
        }
    }

    return pi;
}

unordered_set<int> KMP(string str, string sub) {
    unordered_set<int> solution;
    vector<int> pi =  prefixFunc(sub);
    int i = 0, j = 0;

    while (i < str.length()) {
        if (str[i] == sub[j]) {
            i++;
            j++;
            if (j == sub.size()) {
                solution.insert(i - j);
                j = pi[j - 1];
            }
        } else if (j == 0) {
            i++;
        } else {
            j = pi[j - 1];
        }
    }

    return solution;
}

string removeWordOccurrences(string text, string pattern) {
    unordered_set<int> toDelete = KMP(text, pattern);
    string res;

    for (int i = 0; i < text.size(); i++) {
        if (toDelete.count(i) == 1) {
            i += pattern.size();
        }
        res += text[i];
    }

    // Удалить все пробелы
    istringstream in(res);
    string result, word;

    while (in >> word) {
        result += word + " ";
    }

    return result;
}

int main() {
    setlocale(LC_ALL, "Russian");
    string sentence;
    getline(cin, sentence);
    cout << removeRepeatedWords(sentence) << endl;

    string word;
    cout << "Введите предложение: ";
    getline(cin, sentence);
    cout << "Введите удаляемое слово: ";
    cin >> word;

    string result = removeWordOccurrences(sentence, word);
    cout << "Результат: " << result << endl;
    return 0;
}
