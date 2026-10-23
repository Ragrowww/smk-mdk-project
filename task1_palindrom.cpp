#include <windows.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class SMKJournal {
private:
	string filename;

	
	string getTimestamp() {
		SYSTEMTIME st;
		GetLocalTime(&st);
		char buffer[32];
		snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d %02d:%02d:%02d",
			st.wYear, st.wMonth, st.wDay,
			st.wHour, st.wMinute, st.wSecond);
		return string(buffer);
	}

	
	void showError(const string& message) {
		DWORD errorCode = GetLastError();
		cerr << "ОШИБКА: " << message << endl;
		cerr << "Код ошибки: " << errorCode << endl;

		char* errorMsg = nullptr;
		DWORD result = FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			nullptr,
			errorCode,
			0,
			(LPSTR)&errorMsg,
			0,
			nullptr
		);
		if (result != 0 && errorMsg) {
			cerr << "Описание: " << errorMsg;
			LocalFree(errorMsg);
		}
	}

public:
	SMKJournal() : filename("smk_notes.txt") {}


	bool saveEvent(const string& eventText) {
		if (eventText.empty()) {
			cerr << "ОШИБКА: Описание события не может быть пустым!" << endl;
			return false;
		}

	
		HANDLE hFile = CreateFileA(
			filename.c_str(),
			GENERIC_WRITE,
			FILE_SHARE_READ,
			nullptr,
			OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			nullptr
		);

		if (hFile == INVALID_HANDLE_VALUE) {
			showError("Не удалось создать/открыть файл журнала СМК");
			return false;
		}

		
		string record = "[" + getTimestamp() + "] " + eventText + "\n";

	
		SetFilePointer(hFile, 0, nullptr, FILE_END);

		DWORD bytesWritten = 0;
	
		if (!WriteFile(hFile, record.c_str(), (DWORD)record.length(),
			&bytesWritten, nullptr)) {
			showError("Не удалось записать данные в файл");
			CloseHandle(hFile);
			return false;
		}

		if (bytesWritten != record.length()) {
			cerr << "ОШИБКА: Записано не все данные!" << endl;
			CloseHandle(hFile);
			return false;
		}

		CloseHandle(hFile);
		cout << "УСПЕШНО: Событие зафиксировано в журнале -> " << record;
		return true;
	}

	
	bool readJournal() {
		
		HANDLE hFile = CreateFileA(
			filename.c_str(),
			GENERIC_READ,
			FILE_SHARE_READ,
			nullptr,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			nullptr
		);

		if (hFile == INVALID_HANDLE_VALUE) {
			if (GetLastError() == ERROR_FILE_NOT_FOUND) {
				cerr << "ОШИБКА: Журнал событий не найден!" << endl;
				cerr << "Создайте хотя бы одну запись (событие)." << endl;
			}
			else {
				showError("Не удалось открыть файл журнала для чтения");
			}
			return false;
		}

	
		DWORD fileSize = GetFileSize(hFile, nullptr);
		if (fileSize == INVALID_FILE_SIZE) {
			showError("Не удалось определить размер файла");
			CloseHandle(hFile);
			return false;
		}
		if (fileSize == 0) {
			cout << "Журнал пуст. Событий пока нет." << endl;
			CloseHandle(hFile);
			return true;
		}

		vector<char> buffer(fileSize + 1, 0);
		DWORD bytesRead = 0;

	
		if (!ReadFile(hFile, buffer.data(), fileSize, &bytesRead, nullptr)) {
			showError("Не удалось прочитать файл");
			CloseHandle(hFile);
			return false;
		}
		CloseHandle(hFile);

		cout << "\n========================================================" << endl;
		cout << "          ЖУРНАЛ СОБЫТИЙ ИНСПЕКТОРА СМК" << endl;
		cout << "========================================================" << endl;
		cout << string(buffer.data(), bytesRead);
		cout << "========================================================" << endl;
		return true;
	}
};

int main() {

	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	SMKJournal journal;
	int choice;

	while (true) {
		cout << "\n========================================================" << endl;
		cout << "      СТРАХОВАЯ МЕДИЦИНСКАЯ КОМПАНИЯ (СМК)" << endl;
		cout << "          Система учета работы инспектора" << endl;
		cout << "========================================================" << endl;
		cout << "1. Добавить новое событие (запись)" << endl;
		cout << "2. Показать историю событий (журнал)" << endl;
		cout << "3. Выход" << endl;
		cout << "--------------------------------------------------------" << endl;
		cout << "Выберите действие (1-3): ";

		if (!(cin >> choice)) {
			cin.clear();
			cin.ignore(10000, '\n');
			cerr << "ОШИБКА: Введите число!" << endl;
			system("pause");
			continue;
		}
		cin.ignore();

		if (choice == 1) {
			cout << "\n========================================================" << endl;
			cout << "               РЕГИСТРАЦИЯ СОБЫТИЯ" << endl;
			cout << "========================================================" << endl;
			cout << "(Например: Клиент Петров направлен в клинику 'Медика')" << endl;
			cout << "Введите описание события: ";
			string text;
			getline(cin, text);
			journal.saveEvent(text);
			cout << "\n";
			system("pause");
		}
		else if (choice == 2) {
			journal.readJournal();
			cout << "\n";
			system("pause");
		}
		else if (choice == 3) {
			cout << "\n========================================================" << endl;
			cout << "              РАБОТА ЗАВЕРШЕНА. ДО СВИДАНИЯ!" << endl;
			cout << "========================================================" << endl;
			system("pause");
			break;
		}
		else {
			cerr << "ОШИБКА: Неверный выбор! Попробуйте снова." << endl;
			system("pause");
		}
	}
	return 0;
}
