
#include "pch.h"
#include "CppUnitTest.h"
#include "../praktice/header.h"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <string.h>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:
		
		// проверка исходного файла
		TEST_METHOD(TestMethod1)
		{
			std::string path = "../../marvel.jpg";
			cv::Mat src;
			bool test = CheckFormat(path,src);
			Assert::AreEqual(test, true);
		}

		TEST_METHOD(TestMethod2)
		{
			std::string path = "../../12345.jpg";
			cv::Mat src;
			bool test = CheckFormat(path, src);
			Assert::AreEqual(test, false);
		}

		// проверка выходного файла
		TEST_METHOD(TestMethod3)
		{
			std::string filename = "../../marvel5.jpg";
			bool test = CheckOutput(filename);
			Assert::AreEqual(test, true);
		}

		TEST_METHOD(TestMethod4)
		{
			std::string filename = "../../12345.jpg";
			bool test = CheckOutput(filename);
			Assert::AreEqual(test, false);
		}


	};
}
