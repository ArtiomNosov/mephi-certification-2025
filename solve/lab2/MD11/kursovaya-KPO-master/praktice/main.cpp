#pragma once

#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"

#include "header.h"

#include <iostream>
#include <limits>
#include <string>

int main(int argc, char** argv)
{
	setlocale(LC_ALL, "rus");

	cv::Mat src, dst;
	int choice = -1;
	std::string imagePath;
	std::string outputPath;
	std::string errorMessage;

	do {
		std::cout << "Введите путь к изображению: ";
		if (!std::getline(std::cin >> std::ws, imagePath)) {
			std::cout << "Ввод прерван.\n";
			break;
		}

		if (!CheckFormat(imagePath, src, errorMessage)) {
			std::cout << errorMessage << std::endl;
			continue;
		}

		std::cout << "\nВыберите фильтр:\n"
			<< "0) Выход\n"
			<< "1) Рельеф (emboss)\n"
			<< "2) Повышение резкости\n"
			<< "3) Размытие (5x5)\n"
			<< "4) Оператор Собеля\n"
			<< "Ваш выбор: ";

		if (!(std::cin >> choice)) {
			std::cout << "Некорректный ввод.\n";
			break;
		}
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		if (choice == 0) {
			std::cout << "Завершение работы.\n";
			break;
		}

		std::cout << "Введите путь для сохранения обработанного изображения: ";
		if (!std::getline(std::cin >> std::ws, outputPath)) {
			std::cout << "Ввод прерван.\n";
			break;
		}
		if (!CheckOutput(outputPath, errorMessage)) {
			std::cout << errorMessage << std::endl;
			continue;
		}

		try {
			switch (choice) {
			case 1:
				Emboss(src, dst);
				break;
			case 2:
				Sharpening(src, dst);
				break;
			case 3:
				BoxBlur(src, dst, cv::Size(5, 5));
				break;
			case 4:
				Sobel(src, dst);
				break;
			default:
				std::cout << "Неизвестный номер фильтра.\n";
				continue;
			}

			if (!cv::imwrite(outputPath, dst)) {
				std::cout << "Не удалось сохранить файл по указанному пути.\n";
				continue;
			}
			std::cout << "Файл успешно сохранён.\n";
		}
		catch (const cv::Exception& e) {
			std::cout << "Ошибка обработки изображения: " << e.what() << std::endl;
		}
	} while (true);

	return 0;
}