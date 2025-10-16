#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std;

const string DISEASES[] = {"A01", "B14", "C08", "A02", "A21", "A14", "B01", "B32",
    "C11", "B21", "B45", "C06", "A12", "B20", "B09", "C06", "C26"};
const string DOCTORS[] = {"Лубеников", "Горбунов", "Бандурина", "Шатрова",
    "Ардашева", "Гурчиани", "Рябов", "Зыкова", "Калмыков", "Петрунина",
    "Ильина", "Максимов"};
const int N = 600000;

// Размеры полей структуры Patient
const size_t cardNumSize = sizeof(unsigned long);
const size_t diseaseNumSize = 4;
const size_t doctorNameSize = 20;

// Данные пациента
struct Patient {
    unsigned long cardNum;
    char* diseaseNum;
    char* doctorName;

    // Переопределяем оператор для сортировки
    bool operator<(const Patient& other) const {
        return cardNum < other.cardNum;
    }
};

// Для быстрого конвертирования
union LongByte {
    unsigned long asLong;
    char asBytes[4];
};

// Функция поиска
typedef int (*searchFunc)(Patient*, unsigned long);

// Записывает данные в текстовый файл
void writeTxtFile() {
    ofstream out("patients.txt");
    random_device rd;
    mt19937 gen(rd()); // Генератор случайных чисел

    // Создание диапазона чисел
    vector<unsigned long> cards;
    for (unsigned long i = 0; i < N; i++) {
        cards.push_back(i);
    }

    shuffle(cards.begin(), cards.end(), gen);

    for (unsigned long i = 0; i < N; i++) {
        string disease = DISEASES[gen() % 17];
        string doctor = DOCTORS[gen() % 12];

        out << cards[i] << "|" << disease << "|" << doctor << endl;
    }

    out.close();
}

// Читает данные из текстого файла и конвертирует их в бинарный файл
void convertToBin() {
    ifstream in("patients.txt");
    ofstream out("patients.bin", ios::binary);

    // Буфер
    string line;
    size_t prev;
    size_t next;
    while (getline(in, line)) {
        // Находит первую подстроку до разделителя
        next = line.find("|");
        auto cardNumStr = line.substr(0, next);
        LongByte cardNum;
        cardNum.asLong = stoul(cardNumStr);
        prev = next + 1;

        next = line.find("|", prev);
        auto diseaseNum = line.substr(prev, next - prev);
        prev = next + 1;

        auto doctorName = line.substr(prev);

        // Записывает в файл блок данных заданного размером
        out.write(cardNum.asBytes, cardNumSize);
        out.write(diseaseNum.c_str(), sizeof(char) * diseaseNumSize);
        out.write(doctorName.c_str(), sizeof(char) * doctorNameSize);
    }

    in.close();
    out.close();
}

// Читает данные из бинарника
Patient* readFromBin() {
    ifstream in("patients.bin", ios::binary);
    Patient* result = new Patient[N];

    for (int i = 0; i < N; i++) {
        // Создаем указатель на данные пациента
        Patient patient;
        // Инициализирую байты каждого свойства структуры
        LongByte cardNum;
        patient.diseaseNum = new char[diseaseNumSize];
        patient.doctorName = new char[doctorNameSize];
        // Читаем каждое поле побайтово
        in.read(cardNum.asBytes, cardNumSize);
        patient.cardNum = cardNum.asLong;
        in.read(patient.diseaseNum, diseaseNumSize);
        in.read(patient.doctorName, doctorNameSize);
        // Записывает в массив указатель на данные пациента
        result[i] = patient;
    }

    in.close();
    return result;
}

// Линейный поиск
int linearSearch(Patient* array, unsigned long toFind) {
    for (int i = 0; i < N; i++) {
        if (array[i].cardNum == toFind) {
            return i;
        }
    }

    return -1; // Not found
}

// Интерполяционный поиск
int interpolationSearch(Patient* array, unsigned long toFind) {
    int mid;
    // Возвращает индекс элемента со значением toFind или -1, если такого элемента не существует
    int low = 0;
    int high = N - 1;

    while (array[low].cardNum < toFind && array[high].cardNum > toFind) {
        if (array[high].cardNum == array[low].cardNum) // Защита от деления на 0
            break;
        mid = low + ((toFind - array[low].cardNum) * (high - low)) / (array[high].cardNum - array[low].cardNum);

        if (array[mid].cardNum < toFind)
            low = mid + 1;
        else if (array[mid].cardNum > toFind)
            high = mid - 1;
        else
            return mid;
    }

    if (array[low].cardNum == toFind)
        return low;
    if (array[high].cardNum == toFind)
        return high;

    return -1; // Not found
}

// Поиск значения по ключу
void searchNum(Patient* patients, searchFunc search) {
    int cardNum;
    cout << "Введите нужный номер карты: ";
    cin >> cardNum;
    auto begin = chrono::steady_clock::now();
    int found = search(patients, cardNum);
    auto end = chrono::steady_clock::now();

    if (found != -1) {
        Patient patient = patients[found];
        cout << patient.cardNum << " " <<
                patient.diseaseNum << " " <<
                patient.doctorName << endl;
    } else {
        cout << "Карта не найдена";
    }

    cout << "Прошло: " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << " мс" << endl;
}

// Вывод на экран информации о пациентах
void printPatiens(Patient* patients) {
    for (int i = 0; i < N; i++) {
        cout << patients[i].cardNum << " " <<
            patients[i].diseaseNum << " " <<
            patients[i].doctorName << endl;
    }
}

int main()
{
    setlocale(0, "ru");
    srand(time(0));
    writeTxtFile();
    convertToBin();
    auto patients = readFromBin();

    // printPatiens(patients);

    // Линейный поиск
    searchNum(patients, &linearSearch);

    // Сортирую массив Patient (необходимо для интерполяционного поиска)
    sort(patients, patients + N);

    // Интерполяционный поиск
    searchNum(patients, &interpolationSearch);

    return 0;
}
