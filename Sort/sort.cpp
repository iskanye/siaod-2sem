#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>

using namespace std;

int main1()
{
    setlocale(LC_ALL, "ru");
    unsigned int start = clock();
    ifstream in("in.txt");
    ofstream out("out.txt");

    string line;
    unsigned long long mask = 1;
    vector<unsigned char> sort_vec {0};

    while (getline(in, line)) {
        unsigned long long i = stoi(line);
        if (sort_vec.size() < i / 8 + 1) {
            int size = sort_vec.size();
            for (int j = 0; j < i / 8 + 1 - size; j++) {
                sort_vec.push_back(0);
            }
        }
        sort_vec[i / 8] |= mask << (7 - i % 8);
    }

    for (int i = 0; i < sort_vec.size(); i++) {
        for (int j = 0; j < 8; j++) {
            if ((sort_vec[i] & (mask << (7 - j))) > 0) {
                out << 8 * i + j << endl;
            }
        }
    }

    cout << (clock() - start) / 1000.0 << " секунд" << endl;
    cout << sizeof(unsigned char) * sort_vec.size() / 1024.0 / 1024.0 << " Мегабайт";

    return 0;
}
