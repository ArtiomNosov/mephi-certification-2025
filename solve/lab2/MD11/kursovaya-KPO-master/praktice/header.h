#pragma once

#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include <string>

bool CheckFormat(const std::string& imagePath, cv::Mat& source, std::string& errorMessage);
void Sharpening(const cv::Mat& source, cv::Mat& dst);
void Emboss(const cv::Mat& source, cv::Mat& dst);
void Sobel(const cv::Mat& source, cv::Mat& dst);
void BoxBlur(const cv::Mat& source, cv::Mat& dst, cv::Size kernelSize);
bool CheckOutput(const std::string& filePath, std::string& errorMessage);
