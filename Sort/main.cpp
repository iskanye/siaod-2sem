#include <iostream>
#include <bitset>
#include <vector>

using namespace std;

int main()
{
    const int n = 64;
    int numbers[] {0, 1, 6, 5, 7, 2, 3, 63, 30, 21, 25};
    unsigned long long sorted = 0;
    unsigned long long mask = 1;

    for (auto i : numbers) {
        sorted |= mask << (n - i - 1);
    }

    for (int i = 0; i < n; i++) {
        if ((sorted & (mask << (n - i - 1))) > 0) {
            cout << i << ", ";
        }
    }

    cout << endl;
    vector<unsigned char> sort_vec {0};

    for (auto i : numbers) {
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
                cout << 8 * i + j << ", ";
            }
        }
    }

    return 0;
}
