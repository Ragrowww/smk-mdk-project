#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <filesystem>
#include <chrono>
#include <ctime>

using namespace std;
namespace fs = std::filesystem;


struct WavHeader {
	char chunkID[4];     
	uint32_t chunkSize;
	char format[4];      
};

struct FmtChunk {
	char subchunk1ID[4];  
	uint32_t subchunk1Size;
	uint16_t audioFormat;
	uint16_t numChannels;
	uint32_t sampleRate;
	uint32_t byteRate;
	uint16_t blockAlign;
	uint16_t bitsPerSample;
};

int main() {
	setlocale(LC_ALL, "Russian");

	cout << "======================================================================\n";
	cout << "            СТРАХОВАЯ МЕДИЦИНСКАЯ КОМПАНИЯ (СМК)\n";
	cout << "            Модуль аудио-аналитики звонков клиентов\n";
	cout << "======================================================================\n\n";

	string filename;
	cout << "Введите имя аудиофайла: ";
	cin >> filename;

	if (!fs::exists(filename)) {
		cerr << "ОШИБКА: Файл не найден!\n";
		system("pause");
		return 1;
	}

	try {
		auto fileSize = fs::file_size(filename);
		auto ftime = fs::last_write_time(filename);
		auto sctp = chrono::time_point_cast<chrono::system_clock::duration>(
			ftime - fs::file_time_type::clock::now() + chrono::system_clock::now()
			);
		time_t cftime = chrono::system_clock::to_time_t(sctp);
		tm timeinfo;
		localtime_s(&timeinfo, &cftime);

		cout << "\n--- СИСТЕМНАЯ ИНФОРМАЦИЯ О ФАЙЛЕ ---\n";
		cout << "Размер файла: " << fileSize << " байт\n";
		cout << "Дата изменения: " << put_time(&timeinfo, "%Y-%m-%d %H:%M:%S") << "\n";

		ifstream file(filename, ios::binary);
		if (!file.is_open()) {
			cerr << "ОШИБКА: Не удалось открыть файл.\n";
			system("pause");
			return 1;
		}

		
		WavHeader header;
		file.read(reinterpret_cast<char*>(&header), sizeof(WavHeader));

		if (string(header.chunkID, 4) != "RIFF" || string(header.format, 4) != "WAVE") {
			cerr << "ОШИБКА: Это не WAV файл!\n";
			return 1;
		}

		FmtChunk fmt;
		uint32_t dataSize = 0;
		bool fmtFound = false, dataFound = false;
		char chunkName[4];

		
		while (file.read(chunkName, 4)) {
			uint32_t chunkSize = 0;
			file.read(reinterpret_cast<char*>(&chunkSize), 4);

			if (string(chunkName, 4) == "fmt ") {
				file.read(reinterpret_cast<char*>(&fmt.audioFormat), chunkSize);
				fmtFound = true;
			}
			else if (string(chunkName, 4) == "data") {
				dataSize = chunkSize;
				dataFound = true;
				break; 
			}
			else {
			
				file.seekg(chunkSize, ios::cur);
			}
		}

		if (!fmtFound || !dataFound) {
			cerr << "ОШИБКА: Не удалось найти параметры звука в файле.\n";
			return 1;
		}

		cout << "\n--- ХАРАКТЕРИСТИКИ АУДИОЗАПИСИ ---\n";
		cout << "Количество каналов: " << fmt.numChannels;
		if (fmt.numChannels == 1) cout << " (Моно)\n";
		else if (fmt.numChannels == 2) cout << " (Стерео)\n";

		cout << "Частота дискретизации: " << fmt.sampleRate << " Гц\n";
		cout << "Разрядность: " << fmt.bitsPerSample << " бит\n";

		
		if (fmt.sampleRate > 0 && fmt.numChannels > 0 && fmt.bitsPerSample > 0) {
			double duration = static_cast<double>(dataSize) /
				(fmt.sampleRate * fmt.numChannels * (fmt.bitsPerSample / 8.0));

			cout << "Длительность записи: " << fixed << setprecision(2) << duration << " сек.\n";
		}

		file.close();
	}
	catch (const fs::filesystem_error& e) {
		cerr << "Системная ошибка: " << e.what() << "\n";
	}

	cout << "\n======================================================================\n";
	cout << "СТАТУС: Анализ записи завершен.\n";
	system("pause");
	return 0;
}
