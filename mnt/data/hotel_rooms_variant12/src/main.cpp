#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <locale>

using namespace std;

const int SIZE = 20;
const int NAME_LEN = 60;
const int TEXT_LEN = 120;

enum ComfortLevel {
    LUX,
    SEMILUX,
    STANDARD,
    ECONOM
};

struct Amenities {
    bool wifi;
    bool tv;
    bool bath;
    bool balcony;
};

struct HotelRoom {
    char hotelName[NAME_LEN];
    int roomNumber;
    ComfortLevel comfort;
    int capacity;
    double price;
    Amenities amenities;
    bool hasAirConditioner; // поле будет заполнено из текстового файла
};

const char* comfortToString(ComfortLevel comfort) {
    switch (comfort) {
    case LUX: return "люкс";
    case SEMILUX: return "полулюкс";
    case STANDARD: return "стандарт";
    case ECONOM: return "эконом";
    default: return "неизвестно";
    }
}

void printAmenities(const Amenities& amenities) {
    cout << "Wi-Fi: " << (amenities.wifi ? "есть" : "нет") << ", ";
    cout << "TV: " << (amenities.tv ? "есть" : "нет") << ", ";
    cout << "ванна: " << (amenities.bath ? "есть" : "нет") << ", ";
    cout << "балкон: " << (amenities.balcony ? "есть" : "нет");
}

void printRoom(const HotelRoom& room) {
    cout << left << setw(18) << "Гостиница:" << room.hotelName << '\n';
    cout << left << setw(18) << "Номер:" << room.roomNumber << '\n';
    cout << left << setw(18) << "Комфортность:" << comfortToString(room.comfort) << '\n';
    cout << left << setw(18) << "Человек:" << room.capacity << '\n';
    cout << left << setw(18) << "Стоимость:" << fixed << setprecision(2) << room.price << " руб." << '\n';
    cout << left << setw(18) << "Кондиционер:" << (room.hasAirConditioner ? "есть" : "нет") << '\n';
    cout << left << setw(18) << "Опции:";
    printAmenities(room.amenities);
    cout << "\n----------------------------------------\n";
}

void printArray(const HotelRoom rooms[], int count, const char title[]) {
    cout << "\n========== " << title << " ==========\n";
    if (count == 0) {
        cout << "Нет данных.\n";
        return;
    }
    for (int i = 0; i < count; ++i) {
        printRoom(rooms[i]);
    }
}

// Оберточная функция для вывода отфильтрованных данных.
void printFiltered(const HotelRoom rooms[], int count, const char title[]) {
    cout << "\n>>> Результат фильтрации: " << title << "\n";
    printArray(rooms, count, title);
}

bool endsWithHouse(const char hotelName[]) {
    const char ending[] = "house";
    size_t len = strlen(hotelName);
    size_t endingLen = strlen(ending);
    if (len < endingLen) {
        return false;
    }
    return strcmp(hotelName + len - endingLen, ending) == 0;
}

void copyRoomsEndingWithHouse(const HotelRoom source[], int sourceCount, HotelRoom result[], int& resultCount) {
    resultCount = 0;
    for (int i = 0; i < sourceCount; ++i) {
        if (endsWithHouse(source[i].hotelName)) {
            result[resultCount] = source[i];
            ++resultCount;
        }
    }
}

void bubbleSortByCapacity(HotelRoom rooms[], int count) {
    for (int i = 0; i < count - 1; ++i) {
        for (int j = 0; j < count - i - 1; ++j) {
            if (rooms[j].capacity > rooms[j + 1].capacity) {
                HotelRoom temp = rooms[j];
                rooms[j] = rooms[j + 1];
                rooms[j + 1] = temp;
            }
        }
    }
}

int findRoomIndexByNumber(const HotelRoom rooms[], int count, int roomNumber) {
    for (int i = 0; i < count; ++i) {
        if (rooms[i].roomNumber == roomNumber) {
            return i;
        }
    }
    return -1;
}

void printRoomByNumber(const HotelRoom rooms[], int count, int roomNumber) {
    cout << "\n========== Данные по конкретному номеру " << roomNumber << " ==========\n";
    int index = findRoomIndexByNumber(rooms, count, roomNumber);
    if (index == -1) {
        cout << "Номер не найден.\n";
    } else {
        printRoom(rooms[index]);
    }
}

void printTop3MostExpensiveLux(const HotelRoom rooms[], int count) {
    HotelRoom luxRooms[SIZE];
    int luxCount = 0;

    for (int i = 0; i < count; ++i) {
        if (rooms[i].comfort == LUX) {
            luxRooms[luxCount] = rooms[i];
            ++luxCount;
        }
    }

    for (int i = 0; i < luxCount - 1; ++i) {
        for (int j = 0; j < luxCount - i - 1; ++j) {
            if (luxRooms[j].price < luxRooms[j + 1].price) {
                HotelRoom temp = luxRooms[j];
                luxRooms[j] = luxRooms[j + 1];
                luxRooms[j + 1] = temp;
            }
        }
    }

    cout << "\n========== 3 самых дорогих люкса и их бытовые опции ==========\n";
    int limit = luxCount < 3 ? luxCount : 3;
    for (int i = 0; i < limit; ++i) {
        cout << i + 1 << ") " << luxRooms[i].hotelName << ", номер " << luxRooms[i].roomNumber;
        cout << ", цена: " << fixed << setprecision(2) << luxRooms[i].price << " руб.\n";
        cout << "   Опции: ";
        printAmenities(luxRooms[i].amenities);
        cout << "\n";
    }
}

void changeRoomData(HotelRoom rooms[], int count, int roomNumber, const HotelRoom& newData) {
    int index = findRoomIndexByNumber(rooms, count, roomNumber);
    if (index == -1) {
        cout << "\nИзменение невозможно: номер " << roomNumber << " не найден.\n";
        return;
    }
    rooms[index] = newData;
    cout << "\nДанные номера " << roomNumber << " изменены.\n";
}

void filterRoomsWithBath(const HotelRoom source[], int sourceCount, HotelRoom result[], int& resultCount) {
    resultCount = 0;
    for (int i = 0; i < sourceCount; ++i) {
        if (source[i].amenities.bath) {
            result[resultCount] = source[i];
            ++resultCount;
        }
    }
}

void createTextFileForAirConditioner(const HotelRoom rooms[], int count, const char filename[]) {
    ofstream fout(filename);
    if (!fout.is_open()) {
        cout << "Ошибка создания текстового файла.\n";
        return;
    }

    for (int i = 0; i < count; ++i) {
        int airConditioner = (i % 3 == 0 || rooms[i].comfort == LUX) ? 1 : 0;
        fout << rooms[i].roomNumber << ' ' << airConditioner << '\n';
    }
    fout.close();
}

void readAirConditionerFromTextFile(HotelRoom rooms[], int count, const char filename[]) {
    ifstream fin(filename);
    if (!fin.is_open()) {
        cout << "Ошибка открытия текстового файла.\n";
        return;
    }

    int roomNumber;
    int airConditioner;
    while (fin >> roomNumber >> airConditioner) {
        int index = findRoomIndexByNumber(rooms, count, roomNumber);
        if (index != -1) {
            rooms[index].hasAirConditioner = airConditioner == 1;
        }
    }
    fin.close();
}

void writeRoomsToBinaryFile(const HotelRoom rooms[], int count, const char filename[]) {
    ofstream fout(filename, ios::binary);
    if (!fout.is_open()) {
        cout << "Ошибка открытия бинарного файла для записи.\n";
        return;
    }
    fout.write(reinterpret_cast<const char*>(rooms), count * sizeof(HotelRoom));
    fout.close();
}

int readRoomsFromBinaryFile(HotelRoom rooms[], int maxCount, const char filename[]) {
    ifstream fin(filename, ios::binary);
    if (!fin.is_open()) {
        cout << "Ошибка открытия бинарного файла для чтения.\n";
        return 0;
    }

    fin.read(reinterpret_cast<char*>(rooms), maxCount * sizeof(HotelRoom));
    int readCount = static_cast<int>(fin.gcount() / sizeof(HotelRoom));
    fin.close();
    return readCount;
}

int main() {
    setlocale(LC_ALL, "");

    HotelRoom rooms[SIZE] = {
        {"forest house", 101, LUX, 4, 19500, {true, true, true, true}, false},
        {"river house", 102, SEMILUX, 3, 11200, {true, true, true, false}, false},
        {"city hotel", 103, STANDARD, 2, 6900, {true, true, false, false}, false},
        {"green house", 104, ECONOM, 1, 3900, {true, false, false, false}, false},
        {"royal house", 105, LUX, 5, 27000, {true, true, true, true}, false},
        {"north inn", 106, STANDARD, 2, 6100, {false, true, true, false}, false},
        {"lake house", 107, SEMILUX, 3, 12600, {true, true, true, true}, false},
        {"sun house", 108, ECONOM, 1, 4200, {true, false, false, true}, false},
        {"plaza hotel", 109, LUX, 4, 24500, {true, true, true, false}, false},
        {"white house", 110, STANDARD, 2, 7600, {true, true, true, false}, false},
        {"sea resort", 111, SEMILUX, 3, 13000, {true, true, false, true}, false},
        {"dream house", 112, LUX, 2, 31000, {true, true, true, true}, false},
        {"park inn", 113, ECONOM, 1, 3500, {false, false, false, false}, false},
        {"stone house", 114, STANDARD, 2, 8200, {true, true, true, true}, false},
        {"grand hotel", 115, LUX, 3, 28900, {true, true, true, true}, false},
        {"quiet house", 116, ECONOM, 1, 4600, {true, false, true, false}, false},
        {"amber house", 117, SEMILUX, 4, 15800, {true, true, true, false}, false},
        {"central hotel", 118, STANDARD, 2, 7200, {true, true, false, false}, false},
        {"gold house", 119, LUX, 6, 34000, {true, true, true, true}, false},
        {"metro inn", 120, ECONOM, 1, 3300, {false, false, false, false}, false}
    };

    printArray(rooms, SIZE, "Исходный массив гостиничных номеров");

    createTextFileForAirConditioner(rooms, SIZE, "data/air_conditioner.txt");
    readAirConditionerFromTextFile(rooms, SIZE, "data/air_conditioner.txt");
    cout << "\nДанные числового поля 'кондиционер' считаны из текстового файла.\n";

    writeRoomsToBinaryFile(rooms, SIZE, "data/hotel_rooms.bin");
    HotelRoom loadedRooms[SIZE];
    int loadedCount = readRoomsFromBinaryFile(loadedRooms, SIZE, "data/hotel_rooms.bin");
    cout << "Из бинарного файла прочитано записей: " << loadedCount << "\n";

    HotelRoom houseRooms[SIZE];
    int houseCount = 0;
    copyRoomsEndingWithHouse(rooms, SIZE, houseRooms, houseCount);
    printFiltered(houseRooms, houseCount, "Гостиницы, название которых оканчивается на 'house'");

    bubbleSortByCapacity(houseRooms, houseCount);
    printFiltered(houseRooms, houseCount, "Те же номера после сортировки пузырьком по количеству человек");

    printRoomByNumber(rooms, SIZE, 105);
    printTop3MostExpensiveLux(rooms, SIZE);

    HotelRoom changedRoom = {"royal house", 105, LUX, 6, 30000, {true, true, true, true}, true};
    changeRoomData(rooms, SIZE, 105, changedRoom);
    printRoomByNumber(rooms, SIZE, 105);

    HotelRoom bathRooms[SIZE];
    int bathCount = 0;
    filterRoomsWithBath(rooms, SIZE, bathRooms, bathCount);
    printFiltered(bathRooms, bathCount, "Номера, имеющие ванну");

    cout << "\nПрограмма завершена.\n";
    return 0;
}
