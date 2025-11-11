#include <iostream>

using namespace std;

int main()
{
    unsigned char x = 0b11111111; //8-разрядное двоичное число 11111111 (255)
    unsigned char mask = 1; //1=00000001 – 8-разрядная маска
    x &= (~(mask << 4)); // 5 бит в 0
    cout << (int)x << endl;
    x = 0b10111111;
    x |= mask << 6; // 7 бит в 1
    cout << (int)x << endl;
    return 0;
}
