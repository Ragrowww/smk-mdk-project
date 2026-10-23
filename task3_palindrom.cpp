#include <windows.h>
#include <iostream>
#include <string>
#include <iomanip>

using namespace std;


struct SystemInfo {
	string compName;
	string userName;
	string winDir;
	string sysDir;
	string tempDir;
	string osName;
	string servicePack;
	int screenWidth;
	int screenHeight;
	int borderWidth;
	int doubleClickTime;
	int keyboardDelay;
	int keyboardSpeed;
	string colorWindow;
	string colorCaption;
	string colorMenu;
	string colorText;
};

SystemInfo g_info;
HANDLE hConsole;

string getColorStr(COLORREF color) {
	return "RGB(" + to_string(GetRValue(color)) + ", " +
		to_string(GetGValue(color)) + ", " +
		to_string(GetBValue(color)) + ")";
}


void collectInfo() {
	char buf[MAX_PATH];

	
	DWORD size = MAX_COMPUTERNAME_LENGTH + 1;
	GetComputerNameA(buf, &size);
	g_info.compName = buf;

	size = sizeof(buf);
	GetUserNameA(buf, &size);
	g_info.userName = buf;


	GetWindowsDirectoryA(buf, MAX_PATH);
	g_info.winDir = buf;
	GetSystemDirectoryA(buf, MAX_PATH);
	g_info.sysDir = buf;
	GetTempPathA(MAX_PATH, buf);
	g_info.tempDir = buf;


	OSVERSIONINFOEX osvi = { 0 };
	osvi.dwOSVersionInfoSize = sizeof(osvi);
#pragma warning(suppress : 4996)
	GetVersionEx((OSVERSIONINFO*)&osvi);
	if (osvi.dwMajorVersion == 10) g_info.osName = "Windows 10/11";
	else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 3) g_info.osName = "Windows 8.1";
	else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 2) g_info.osName = "Windows 8";
	else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 1) g_info.osName = "Windows 7";
	else g_info.osName = "Windows " + to_string(osvi.dwMajorVersion);

	g_info.servicePack = (osvi.wServicePackMajor > 0) ? to_string(osvi.wServicePackMajor) : "Нет";

	
	g_info.screenWidth = GetSystemMetrics(SM_CXSCREEN);
	g_info.screenHeight = GetSystemMetrics(SM_CYSCREEN);
	g_info.borderWidth = GetSystemMetrics(SM_CXBORDER);
	g_info.doubleClickTime = GetDoubleClickTime();

	
	UINT delay = 1, speed = 31;
	SystemParametersInfo(SPI_GETKEYBOARDDELAY, 0, &delay, 0);
	SystemParametersInfo(SPI_GETKEYBOARDSPEED, 0, &speed, 0);
	g_info.keyboardDelay = delay;
	g_info.keyboardSpeed = speed;

	
	g_info.colorWindow = getColorStr(GetSysColor(COLOR_WINDOW));
	g_info.colorCaption = getColorStr(GetSysColor(COLOR_ACTIVECAPTION));
	g_info.colorMenu = getColorStr(GetSysColor(COLOR_MENU));
	g_info.colorText = getColorStr(GetSysColor(COLOR_WINDOWTEXT));
}

void printMenu() {
	cout << "======================================================\n";
	cout << "       СТРАХОВАЯ МЕДИЦИНСКАЯ КОМПАНИЯ (СМК)\n";
	cout << "       УТИЛИТА СБОРА СИСТЕМНОЙ ИНФОРМАЦИИ\n";
	cout << "======================================================\n";
	cout << "1. Показать полную системную информацию\n";
	cout << "2. Изменить системные цвета\n";
	cout << "3. Показать таблицу\n";
	cout << "4. Выход\n";
	cout << "------------------------------------------------------\n";
	cout << "Выберите действие (1-4): ";
}

void printFullInfo() {
	collectInfo(); 
	cout << "\n--- СИСТЕМНАЯ ИНФОРМАЦИЯ ---\n\n";
	cout << "--- 1. ИМЯ КОМПЬЮТЕРА И ПОЛЬЗОВАТЕЛЯ ---\n";
	cout << "Имя компьютера: " << g_info.compName << "\n";
	cout << "Имя пользователя: " << g_info.userName << "\n\n";

	cout << "--- 2. ПУТИ К СИСТЕМНЫМ КАТАЛОГАМ ---\n";
	cout << "Каталог Windows: " << g_info.winDir << "\n";
	cout << "Системный каталог: " << g_info.sysDir << "\n";
	cout << "Каталог TEMP: " << g_info.tempDir << "\n\n";

	cout << "--- 3. ВЕРСИЯ ОПЕРАЦИОННОЙ СИСТЕМЫ ---\n";
	cout << "Операционная система: " << g_info.osName << "\n";
	cout << "Номер сервиспака: " << g_info.servicePack << "\n\n";

	cout << "--- 4. СИСТЕМНЫЕ МЕТРИКИ ---\n";
	cout << "Ширина экрана: " << g_info.screenWidth << " пикс.\n";
	cout << "Высота экрана: " << g_info.screenHeight << " пикс.\n";
	cout << "Ширина рамки окна: " << g_info.borderWidth << " пикс.\n";
	cout << "Время двойного щелчка: " << g_info.doubleClickTime << " мс\n";
	cout << "Задержка клавиатуры: " << g_info.keyboardDelay << " (0-3)\n";
	cout << "Скорость клавиатуры: " << g_info.keyboardSpeed << " (0-31)\n\n";

	cout << "--- 5. СИСТЕМНЫЕ ЦВЕТА ---\n";
	cout << "Цвет окна: " << g_info.colorWindow << "\n";
	cout << "Цвет заголовка: " << g_info.colorCaption << "\n";
	cout << "Цвет меню: " << g_info.colorMenu << "\n";
	cout << "Цвет текста: " << g_info.colorText << "\n";
	cout << "======================================================\n";
}

void changeColors() {
	cout << "\n--- ИЗМЕНЕНИЕ СИСТЕМНЫХ ЦВЕТОВ ---\n";
	cout << "1. Зеленый текст на черном фоне (СМК-стиль)\n";
	cout << "2. Красный текст на белом фоне (Тревога!)\n";
	cout << "3. Вернуть стандартный цвет\n";
	cout << "Выберите вариант (1-3): ";

	int c;
	if (!(cin >> c)) { cin.clear(); cin.ignore(10000, '\n'); return; }

	if (c == 1) {
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		cout << ">>> Цвет изменен на зеленый! Система СМК готова. <<<\n";
	}
	else if (c == 2) {
		SetConsoleTextAttribute(hConsole, BACKGROUND_RED | BACKGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		cout << ">>> ВНИМАНИЕ! Цвет изменен на белый/красный. <<<\n";
	}
	else {
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		cout << ">>> Стандартный цвет восстановлен. <<<\n";
	}
}

void printTable() {
	collectInfo();
	cout << "\n--- ТАБЛИЦА СИСТЕМНОЙ ИНФОРМАЦИИ ---\n";
	cout << "------------------------------------------------------\n";
	cout << "| " << left << setw(25) << "Параметр" << " | " << setw(20) << "Значение" << " |\n";
	cout << "------------------------------------------------------\n";
	cout << "| " << left << setw(25) << "Имя компьютера" << " | " << setw(20) << g_info.compName << " |\n";
	cout << "| " << left << setw(25) << "Имя пользователя" << " | " << setw(20) << g_info.userName << " |\n";
	cout << "| " << left << setw(25) << "Каталог Windows" << " | " << setw(20) << g_info.winDir << " |\n";
	cout << "| " << left << setw(25) << "Системный каталог" << " | " << setw(20) << g_info.sysDir << " |\n";
	cout << "| " << left << setw(25) << "Операционная система" << " | " << setw(20) << g_info.osName << " |\n";
	cout << "| " << left << setw(25) << "Ширина экрана" << " | " << setw(20) << (to_string(g_info.screenWidth) + " пикс") << " |\n";
	cout << "| " << left << setw(25) << "Высота экрана" << " | " << setw(20) << (to_string(g_info.screenHeight) + " пикс") << " |\n";
	cout << "| " << left << setw(25) << "Цвет окна" << " | " << setw(20) << g_info.colorWindow << " |\n";
	cout << "| " << left << setw(25) << "Цвет текста" << " | " << setw(20) << g_info.colorText << " |\n";
	cout << "------------------------------------------------------\n";
}

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	int choice;
	while (true) {
		printMenu();
		if (!(cin >> choice)) {
			cin.clear();
			cin.ignore(10000, '\n');
			continue;
		}
		cout << "\n";
		if (choice == 1) printFullInfo();
		else if (choice == 2) changeColors();
		else if (choice == 3) printTable();
		else if (choice == 4) {
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			cout << "======================================================\n";
			cout << "                  ДО СВИДАНИЯ!\n";
			cout << "======================================================\n";
			break;
		}
		cout << "\n";
		system("pause");
		system("cls"); 
	}
	return 0;

}
