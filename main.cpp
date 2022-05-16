#include <opencv2/opencv.hpp>

#include <iostream>
#include <sstream>
#include <vector>

cv::Mat getSourceHDR(const cv::Mat& image)
{
	std::vector<cv::Mat> channels;
	cv::split(image, channels);
	std::vector<cv::Mat> bgr({ channels[0],channels[1],channels[2] });
	cv::Mat result;
	cv::merge(bgr, result);
	return result;
}

bool readInputDoubleValue(const std::string& input, double &value)
{
	std::istringstream is(input);
	is >> value;
	if (is.fail())
	{
		value = 1;
		return false;
	}
	return true;
}

cv::Mat ACES(const cv::Mat& v)
{
	const double A = 2.51;
	const double B = 0.03;
	const double C = 2.43;
	const double D = 0.59;
	const double E = 0.14;

	cv::Mat v2 = v.mul(v);
	cv::Mat result;
	cv::divide(A * v2 + B * v, C * v2 + D * v + E, result);
	return result;
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		std::cout << "usage: ToneMappingACES <HDR image file path> [exposure]" << std::endl;
		return 0;
	}

	std::string imagePath(argv[1]);
	cv::Mat image = cv::imread(imagePath, cv::IMREAD_UNCHANGED);
	if (image.empty())
	{
		std::cout << "[error] invalid image file: " << imagePath << std::endl;
		return 1;
	}

	int imageType = image.type();
	if (imageType != CV_32FC3 && imageType != CV_32FC4 && imageType != CV_64FC3 && imageType != CV_64FC4)
	{
		std::cout << "[error] invalid HDR image: " << argv[1] << std::endl;
		return 1;
	}

	double exposure = 1;
	if (argc >= 3)
	{
		if (!readInputDoubleValue(argv[2], exposure))
		{
			std::cout << "[warning] invalid exposure: " << argv[2] << std::endl;
		}
	}

	cv::Mat source = exposure * getSourceHDR(image);
	cv::Mat result = ACES(source);
	cv::pow(result, 1.0 / 2.2, result);
	result.convertTo(result, CV_8UC3, 255.0);

	std::ostringstream os;
	os << imagePath << "." << exposure << ".png";
	cv::imwrite(os.str(), result);

	return 0;
}