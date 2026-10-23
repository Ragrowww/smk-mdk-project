#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <windows.h>

using namespace std;


string formatMemory(unsigned long long bytes) {
	unsigned long long gb = bytes / (1024 * 1024 * 1024);
	unsigned long long mb = (bytes % (1024 * 1024 * 1024)) / (1024 * 1024);
	return to_string(gb) + " ГБ " + to_string(mb) + " МБ";
}

int main() {
	
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	cout << "========================================================\n";
	cout << "       СТРАХОВАЯ МЕДИЦИНСКАЯ КОМПАНИЯ (СМК)\n";
	cout << "  Модуль мониторинга ИТ-инфраструктуры (Задание 2)\n";
	cout << "========================================================\n\n";


	typedef LONG(WINAPI* RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);
	HMODULE hMod = GetModuleHandleW(L"ntdll.dll");

	string osName = "Windows (неизвестная версия)";
	string servicePack = "Нет (не установлен)";

	if (hMod) {
		RtlGetVersionPtr fxPtr = (RtlGetVersionPtr)GetProcAddress(hMod, "RtlGetVersion");
		if (fxPtr != nullptr) {
			RTL_OSVERSIONINFOW rovi = { 0 };
			rovi.dwOSVersionInfoSize = sizeof(rovi);
			if (fxPtr(&rovi) == 0) {
				
				if (rovi.dwMajorVersion == 10 && rovi.dwBuildNumber >= 22000) osName = "Windows 11";
				else if (rovi.dwMajorVersion == 10) osName = "Windows 10";
				else if (rovi.dwMajorVersion == 6 && rovi.dwMinorVersion == 3) osName = "Windows 8.1";
				else if (rovi.dwMajorVersion == 6 && rovi.dwMinorVersion == 2) osName = "Windows 8";
				else if (rovi.dwMajorVersion == 6 && rovi.dwMinorVersion == 1) osName = "Windows 7";
				else osName = "Windows " + to_string(rovi.dwMajorVersion) + "." + to_string(rovi.dwMinorVersion);

				
				if (rovi.szCSDVersion[0] != 0) {
					wstring ws(rovi.szCSDVersion);
					servicePack = string(ws.begin(), ws.end());
				}
			}
		}
	}


	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	string freeMemory = "Ошибка чтения";
	string totalMemory = "Ошибка чтения";

	if (GlobalMemoryStatusEx(&memInfo)) {
		freeMemory = formatMemory(memInfo.ullAvailPhys);
		totalMemory = formatMemory(memInfo.ullTotalPhys);
	}

	
	cout << "Сформирован отчет для ИТ-отдела СМК:\n";
	cout << "--------------------------------------------------------\n";
	cout << left << setw(22) << "Операционная система"
		<< setw(22) << "Номер сервиспака"
		<< "Свободная память" << endl;
	cout << "--------------------------------------------------------\n";
	cout << left << setw(22) << osName
		<< setw(22) << servicePack
		<< freeMemory << endl;
	cout << "--------------------------------------------------------\n";

	cout << "\n(Справочно: всего оперативной памяти на ПК: " << totalMemory << ")\n";
	cout << "СТАТУС: Проверка завершена. Рабочее место пригодно для работы.\n";

	system("pause");
	return 0;
}
