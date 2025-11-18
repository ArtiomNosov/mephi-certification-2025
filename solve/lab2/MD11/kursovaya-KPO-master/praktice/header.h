#pragma once
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <string>
bool CheckFormat(cv::String image,cv::Mat &source);
void Sharpening(cv::Mat* source, cv::Mat* dst, cv::Mat &kernel, cv::Point anchor, double delta);
void Emboss(cv::Mat* source, cv::Mat* dst, cv::Mat& kernel, cv::Point anchor, double delta);
void Sobel(cv::Mat* source, cv::Mat* dst, cv::Mat& kernel, cv::Point anchor, double delta);
bool CheckOutput(cv::String file);
