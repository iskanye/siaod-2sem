#include <ctime>
#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

struct Product {
    unsigned short code;
    string name;
    unsigned short price;

    bool operator ==(const Product& other) const {
        return other.code == code;
    }
};

class HashTable {
    struct Node {
        Product value;
        bool state; // если значение флага state = false, значит элемент массива был удален
        Node(const Product& value_) : value(value_), state(true) {}
    };

    const int default_size = 8; // начальный размер нашей таблицы
    const double rehash_size = 0.75; // коэффициент, при котором произойдет увеличение таблицы

    Node** arr; // соответственно в массиве будут хранится структуры Node*
    int size; // сколько элементов у нас сейчас в массиве (без учета deleted)
    int buffer_size; // размер самого массива, сколько памяти выделено под хранение нашей таблицы
    int size_all_non_nullptr; // сколько элементов у нас сейчас в массиве (с учетом deleted)

    int hash1(unsigned short code) {
        return code % buffer_size;
    }
    int hash2(unsigned short code) {
        return code % (buffer_size - 1) + 1;
    }
public:
    HashTable() {
        buffer_size = default_size;
        size = 0;
        size_all_non_nullptr = 0;
        arr = new Node*[buffer_size];
        for (int i = 0; i < buffer_size; ++i)
            arr[i] = nullptr; // заполняем nullptr - то есть если значение отсутствует, и никто раньше по этому адресу не обращался
    }
    ~HashTable() {
        for (int i = 0; i < buffer_size; ++i)
            if (arr[i])
                delete arr[i];
        delete[] arr;
    }
    void Resize() {
        int past_buffer_size = buffer_size;
        buffer_size *= 2;
        size_all_non_nullptr = 0;
        size = 0;
        Node** arr2 = new Node*[buffer_size];

        for (int i = 0; i < buffer_size; ++i)
            arr2[i] = nullptr;

        swap(arr, arr2);
        for (int i = 0; i < past_buffer_size; ++i)
            if (arr2[i] && arr2[i]->state)
                Add(arr2[i]->value); // добавляем элементы в новый массив
        // удаление предыдущего массива
        for (int i = 0; i < past_buffer_size; ++i)
            if (arr2[i])
                delete arr2[i];
        delete[] arr2;
    }
    void Rehash() {
        size_all_non_nullptr = 0;
        size = 0;
        Node** arr2 = new Node*[buffer_size];

        for (int i = 0; i < buffer_size; ++i)
            arr2[i] = nullptr;

        swap(arr, arr2);
        for (int i = 0; i < buffer_size; ++i)
            if (arr2[i] && arr2[i]->state)
                Add(arr2[i]->value);
        // удаление предыдущего массива
        for (int i = 0; i < buffer_size; ++i)
            if (arr2[i])
                delete arr2[i];
        delete[] arr2;
    }
    bool Find(unsigned short code) {
        int h1 = hash1(code); // значение, отвечающее за начальную позицию
        int h2 = hash2(code); // значение, ответственное за "шаг" по таблице
        int i = 0;

        while (arr[h1] != nullptr && i < buffer_size) {
            if (arr[h1]->value.code == code && arr[h1]->state)
                return true; // такой элемент есть

            h1 = (h1 + h2) % buffer_size;
            i++; // если у нас i >=  buffer_size, значит мы уже обошли абсолютно все ячейки, именно для этого мы считаем i, иначе мы могли бы зациклиться.
        }

        return false;
    }
    bool Remove(unsigned short code) {
        int h1 = hash1(code);
        int h2 = hash2(code);
        int i = 0;

        while (arr[h1] != nullptr && i < buffer_size) {
            if (arr[h1]->value.code == code && arr[h1]->state) {
                arr[h1]->state = false;
                size_all_non_nullptr--;
                return true;
            }
            h1 = (h1 + h2) % buffer_size;
            i++;
        }

        return false;
    }
    bool Add(const Product& value) {
        if (size + 1 > int(rehash_size * buffer_size))
            Resize();
        else if (size_all_non_nullptr > 2 * size)
            Rehash(); // происходит рехеш, так как слишком много deleted-элементов

        int h1 = hash1(value.code);
        int h2 = hash2(value.code);
        int i = 0;
        int first_deleted = -1; // запоминаем первый подходящий (удаленный) элемент

        while (arr[h1] != nullptr && i < buffer_size) {
            if (arr[h1]->value == value && arr[h1]->state)
                return false; // такой элемент уже есть, а значит его нельзя вставлять повторно
            if (!arr[h1]->state && first_deleted == -1) // находим место для нового элемента
                first_deleted = h1;
            h1 = (h1 + h2) % buffer_size;
            i++;
        }

        if (first_deleted == -1) { // если не нашлось подходящего места, создаем новый Node
            arr[h1] = new Node(value);
            size_all_non_nullptr++; // так как мы заполнили один пробел, не забываем записать, что это место теперь занято
        } else {
            arr[first_deleted]->value = value;
            arr[first_deleted]->state = true;
        }

        size++; // и в любом случае мы увеличили количество элементов
        return true;
    }
    void Print() {
        for (int i = 0; i < buffer_size; i++) {
            cout << i << " ";
            if (!arr[i]) {
                cout << "empty" << endl;
            } else if (!arr[i]->state) {
                cout << "deleted" << endl;
            } else {
                cout << arr[i]->value.code << " " << arr[i]->value.name
                    << " " << arr[i]->value.price << endl;
            }
        }
        cout << "Size: " << size_all_non_nullptr << endl;
        cout << "Buffer size: " << buffer_size << endl;
    }
};

void fillHashTable(HashTable* table, const int n) {
    for (int i = 0; i < n; i++) {
        Product product;
        product.code = rand() % 999;
        product.name = "Product_" + to_string(rand());
        product.price = rand() % 999;
        if (!table->Add(product)) {
            i--;
        }
    }
}

bool add(HashTable* table, unsigned short code) {
    Product product;
    product.code = code;
    product.name = "Product_" + to_string(rand());
    product.price = rand() % 999;
    return table->Add(product);
}

int programCycle(HashTable* table) {
    string command;
    unsigned short code;

    while (true) {
        cout << "--------> ";
        cin >> command;
        if (command == "exit") {
            return 0;
        } else if (command == "add") {
            cin >> code;
            cout << add(table, code) << endl;
        } else if (command == "find") {
            cin >> code;
            cout << table->Find(code) << endl;
        } else if (command == "remove") {
            cin >> code;
            cout << table->Remove(code) << endl;
        } else if (command == "print") {
            table->Print();
        } else if (command == "help") {
            cout << "Commands: add find remove print exit" << endl;
        } else {
            cout << "Unknown command" << endl;
        }
    }
}

int main() {
    srand(time(0));
    HashTable table;
    fillHashTable(&table, 8);
    table.Print();
    return programCycle(&table);
}
