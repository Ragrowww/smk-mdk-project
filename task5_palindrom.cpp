#include <iostream>
#include <string>
#include <stdexcept> 

using namespace std;

template <typename T>
class MyList {
private:
	T* data;       
	int count;    
	int capacity;  


	void Resize() {
		capacity = (capacity == 0) ? 2 : capacity * 2; 
		T* newData = new T[capacity];
		for (int i = 0; i < count; i++) {
			newData[i] = data[i];
		}
		delete[] data; 
		data = newData;
	}

public:
	
	MyList() : data(nullptr), count(0), capacity(0) {}


	~MyList() {
		delete[] data;
	}

	
	void Add(T item) {
		if (count >= capacity) {
			Resize();
		}
		data[count] = item;
		count++;
	}

	
	T& operator[](int index) {
		if (index < 0 || index >= count) {
			throw out_of_range("ОШИБКА: Индекс выходит за пределы списка!");
		}
		return data[index];
	}

	
	int Count() const {
		return count;
	}
};

int main() {
	
	setlocale(LC_ALL, "Russian");

	cout << "======================================================================\n";
	cout << "            СТРАХОВАЯ МЕДИЦИНСКАЯ КОМПАНИЯ (СМК)\n";
	cout << "            Тестирование обобщенного списка MyList<T>\n";
	cout << "======================================================================\n\n";

	
	cout << "1. ТЕСТИРОВАНИЕ СПИСКА ЦЕЛЫХ ЧИСЕЛ (ID клиентов СМК)\n";
	MyList<int> clientIDs;

	clientIDs.Add(10045);
	clientIDs.Add(10046);
	clientIDs.Add(10047);

	cout << "Добавлено клиентов: " << clientIDs.Count() << "\n";
	cout << "ID клиента под индексом 0: " << clientIDs[0] << "\n";
	cout << "ID клиента под индексом 1: " << clientIDs[1] << "\n";
	cout << "ID клиента под индексом 2: " << clientIDs[2] << "\n\n";

	
	cout << "2. ТЕСТИРОВАНИЕ СПИСКА СТРОК (ФИО застрахованных)\n";
	MyList<string> clientNames;

	clientNames.Add("Иванов И.И.");
	clientNames.Add("Петрова А.С.");
	clientNames.Add("Сидоров В.В.");
	clientNames.Add("Кузнецова Е.А.");

	cout << "Всего застрахованных: " << clientNames.Count() << "\n";
	for (int i = 0; i < clientNames.Count(); i++) {
		cout << "Клиент [" << i << "]: " << clientNames[i] << "\n";
	}

	cout << "\n";

	
	cout << "3. ПРОВЕРКА ОБРАБОТКИ ОШИБОК (выход за границы списка)\n";
	try {
		cout << "Пытаемся получить 10-го клиента...\n";
		cout << clientNames[10] << "\n";
	}
	catch (const out_of_range& e) {
		cerr << e.what() << "\n";
	}

	cout << "\n======================================================================\n";
	cout << "СТАТУС: Тестирование класса MyList завершено успешно.\n";
	system("pause");
	return 0;
}
