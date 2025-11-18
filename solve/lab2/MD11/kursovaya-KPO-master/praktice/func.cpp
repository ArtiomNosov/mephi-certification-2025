#include "header.h"

#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>

namespace {
namespace fs = std::filesystem;

bool HasAllowedExtension(const fs::path& path) {
	static const std::array<const char*, 4> kExtensions{ ".jpg", ".jpeg", ".png", ".bmp" };
	auto ext = path.extension().string();
	std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char ch) { return static_cast<char>(std::tolower(ch)); });
	return std::any_of(kExtensions.begin(), kExtensions.end(), [&](const char* allowed) { return ext == allowed; });
}

void ApplyConvolution3x3(const cv::Mat& source, cv::Mat& dst, const cv::Matx33f& kernel, double delta) {
	CV_Assert(source.depth() == CV_8U);
	const int channels = source.channels();
	CV_Assert(channels == 1 || channels == 3 || channels == 4);

	dst.create(source.size(), source.type());
	cv::Mat padded;
	cv::copyMakeBorder(source, padded, 1, 1, 1, 1, cv::BORDER_REPLICATE);

	for (int row = 0; row < source.rows; ++row) {
		const uchar* upper = padded.ptr<uchar>(row);
		const uchar* middle = padded.ptr<uchar>(row + 1);
		const uchar* lower = padded.ptr<uchar>(row + 2);
		uchar* output = dst.ptr<uchar>(row);

		for (int col = 0; col < source.cols; ++col) {
			double acc[4] = { delta, delta, delta, delta };

			for (int kRow = 0; kRow < 3; ++kRow) {
				const uchar* srcPtr = (kRow == 0 ? upper : (kRow == 1 ? middle : lower)) + col * channels;
				for (int kCol = 0; kCol < 3; ++kCol) {
					const float weight = kernel(kRow, kCol);
					for (int ch = 0; ch < channels; ++ch) {
						acc[ch] += weight * srcPtr[ch];
					}
					srcPtr += channels;
				}
			}

			for (int ch = 0; ch < channels; ++ch) {
				output[ch] = cv::saturate_cast<uchar>(acc[ch]);
			}
			output += channels;
		}
	}
}
}  // namespace

bool CheckFormat(const std::string& imagePath, cv::Mat& source, std::string& errorMessage) {
	fs::path path(imagePath);
	if (path.empty()) {
		errorMessage = "Путь к изображению не указан.";
		return false;
	}
	if (!fs::exists(path)) {
		errorMessage = "Файл не найден.";
		return false;
	}
	if (!fs::is_regular_file(path)) {
		errorMessage = "Указанный путь не является файлом.";
		return false;
	}
	if (!HasAllowedExtension(path)) {
		errorMessage = "Поддерживаются только JPG, PNG и BMP файлы.";
		return false;
	}
	try {
		source = cv::imread(path.string(), cv::IMREAD_UNCHANGED);
	}
	catch (const cv::Exception& e) {
		errorMessage = std::string("Ошибка OpenCV при чтении файла: ") + e.what();
		source.release();
		return false;
	}
	catch (const std::exception& e) {
		errorMessage = std::string("Системная ошибка чтения файла: ") + e.what();
		source.release();
		return false;
	}

	if (source.empty()) {
		errorMessage = "Не удалось декодировать изображение.";
		return false;
	}

	if (source.depth() != CV_8U) {
		source.convertTo(source, CV_8U);
	}

	return true;
}

void Sharpening(const cv::Mat& source, cv::Mat& dst) {
	static const cv::Matx33f kKernel(0.f, -1.f, 0.f,
		-1.f, 5.f, -1.f,
		0.f, -1.f, 0.f);
	ApplyConvolution3x3(source, dst, kKernel, 0.0);
}

void Emboss(const cv::Mat& source, cv::Mat& dst) {
	static const cv::Matx33f kKernel(1.f, 0.f, 0.f,
		0.f, 0.f, 0.f,
		0.f, 0.f, -1.f);
	ApplyConvolution3x3(source, dst, kKernel, 128.0);
}

void Sobel(const cv::Mat& source, cv::Mat& dst) {
	static const cv::Matx33f kKernel(-1.f, 0.f, 1.f,
		-2.f, 0.f, 2.f,
		-1.f, 0.f, 1.f);
	ApplyConvolution3x3(source, dst, kKernel, 0.0);
}

void BoxBlur(const cv::Mat& source, cv::Mat& dst, cv::Size kernelSize) {
	cv::blur(source, dst, kernelSize, cv::Point(-1, -1), cv::BORDER_DEFAULT);
}

bool CheckOutput(const std::string& filePath, std::string& errorMessage) {
	fs::path path(filePath);
	if (path.empty()) {
		errorMessage = "Путь к выходному файлу не указан.";
		return false;
	}
	if (!path.has_filename()) {
		errorMessage = "Необходимо указать имя выходного файла.";
		return false;
	}
	fs::path parent = path.parent_path();
	if (!parent.empty() && !fs::exists(parent)) {
		errorMessage = "Каталог для сохранения не существует.";
		return false;
	}

	const bool existed = fs::exists(path);
	std::ofstream stream(path, std::ios::binary | std::ios::app);
	if (!stream.is_open()) {
		errorMessage = "Нет доступа для записи в указанный путь.";
		return false;
	}
	stream.close();

	if (!existed) {
		std::error_code ec;
		fs::remove(path, ec);
	}

	return true;
}