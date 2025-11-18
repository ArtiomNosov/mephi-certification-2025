#include "pch.h"
#include "CppUnitTest.h"

#include "../praktice/header.h"

#include "opencv2/imgcodecs.hpp"

#include <filesystem>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:

		TEST_METHOD(CheckFormat_SucceedsForValidFile)
		{
			std::string path = "../../marvel.jpg";
			cv::Mat src;
			std::string error;
			Assert::IsTrue(CheckFormat(path, src, error));
			Assert::IsFalse(src.empty());
		}

		TEST_METHOD(CheckFormat_FailsForMissingFile)
		{
			cv::Mat src;
			std::string error;
			Assert::IsFalse(CheckFormat("../../missing_file.jpg", src, error));
			Assert::IsTrue(!error.empty());
		}

		TEST_METHOD(CheckOutput_AllowsExistingFile)
		{
			std::string error;
			Assert::IsTrue(CheckOutput("../../marvel5.jpg", error));
		}

		TEST_METHOD(CheckOutput_AllowsNewFileAndCleansUp)
		{
			namespace fs = std::filesystem;
			const std::string filename = "../../temporary_test_output.jpg";
			std::string error;
			Assert::IsTrue(CheckOutput(filename, error));
			Assert::IsFalse(fs::exists(filename));
		}
	};
}

