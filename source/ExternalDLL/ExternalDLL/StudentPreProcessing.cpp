#include "StudentPreProcessing.h"
#include "ImageIO.h"
#include "GrayscaleAlgorithm.h"
#include "ImageFactory.h"
#include "HereBeDragons.h"


IntensityImage * StudentPreProcessing::stepToIntensityImage(const RGBImage &src) const {
	GrayscaleAlgorithm grayScaleAlgorithm;
	IntensityImage * image = ImageFactory::newIntensityImage();
	grayScaleAlgorithm.doAlgorithm(src, *image);
	return image;
}


uchar middlepixel(const cv::Mat & boxsample)
{
	return boxsample.at<uchar>((boxsample.rows / 2), (boxsample.cols / 2));
}

uchar medianpixel(const cv::Mat & boxsample)
{
	std::vector<uchar> allvalues;
	for (int x = 0; x < boxsample.cols; ++x)
		for (int y = 0; y < boxsample.rows; ++y)
			allvalues.push_back(boxsample.at<uchar>(y, x));
	std::sort(allvalues.begin(), allvalues.end());
	int length = boxsample.cols*boxsample.rows;
	return length % 2 ? allvalues[length / 2] : (allvalues[length / 2] + allvalues[length / 2 - 1]) / 2;
}

uchar linear(const cv::Mat & boxsample);

uchar bilinear(const cv::Mat & boxsample);


cv::Mat resize(const cv::Mat & origin, const double & ratio)
{
	cv::Mat newMatrix;
	newMatrix.create(origin.cols*ratio, origin.rows*ratio, CV_8UC1); // nieuwe matrix aanmaken
	cv::Mat boxSample;
	double multiplier = 1.0 / ratio;
	for (int x = 0; x < newMatrix.cols; ++x)
		for (int y = 0; y < newMatrix.rows; ++y)
		{
			for (int i = 0; i < multiplier; ++i)
				for (int j = 0; j < multiplier; ++j)
					boxSample.at<uchar>(j, i) = origin.at<uchar>((multiplier*y+j), (multiplier*x+i));
			newMatrix.at<uchar>(y, x) = middlepixel(boxSample);
		}

	// pixels mappen
	// klaar
}

IntensityImage * StudentPreProcessing::stepScaleImage(const IntensityImage &image) const {
	cv::Mat matrix;
	// image inladen
	matrix.create(image.getHeight(), image.getWidth(), CV_8UC1); // CV_8UC1 ???
	for (int x = 0; x < matrix.cols; ++x)
		for (int y = 0; y < matrix.rows; ++y)
			matrix.at<uchar>(y, x) = image.getPixel(x, y); // at<uchar> ???

	int targetsize = 40000;
	int originsize = matrix.cols * matrix.rows;
	if (targetsize < originsize)
	{
		double ratio = 1.0 / sqrt(originsize / targetsize);	// ratio tussen oud en nieuw bepalen
		auto target = resize(matrix, ratio);
	}
	
	// daarmee pixels per pixel bepalen
	// waarde nieuwe pixel bepalen
	// nieuwe waarde toewijzen
}

IntensityImage * StudentPreProcessing::stepEdgeDetection(const IntensityImage &src) const {
	cv::Mat OverHillOverDale;
	HereBeDragons::HerLoveForWhoseDearLoveIRiseAndFall(src, OverHillOverDale);
	//cv::medianBlur(*image, *image, 3);
	//cv::GaussianBlur(*image, *image, cv::Size(3, 3), 0, 0, cv::BORDER_DEFAULT);
	cv::Mat ThoroughBushThoroughBrier = (cv::Mat_<float>(9, 9) << 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, -4, -4, -4, 1, 1, 1, 1, 1, 1, -4, -4, -4, 1, 1, 1, 1, 1, 1, -4, -4, -4, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0);
	cv::Mat OverParkOverPale;
	filter2D(OverHillOverDale, OverParkOverPale, CV_8U, ThoroughBushThoroughBrier, cv::Point(-1, -1), 0, cv::BORDER_DEFAULT);
	IntensityImage * ThoroughFloodThoroughFire = ImageFactory::newIntensityImage();
	HereBeDragons::NoWantOfConscienceHoldItThatICall(OverParkOverPale, *ThoroughFloodThoroughFire);
	return ThoroughFloodThoroughFire;
}

IntensityImage * StudentPreProcessing::stepThresholding(const IntensityImage &src) const {
	cv::Mat OverHillOverDale;
	HereBeDragons::HerLoveForWhoseDearLoveIRiseAndFall(src, OverHillOverDale);
	cv::threshold(OverHillOverDale, OverHillOverDale, 220, 255, cv::THRESH_BINARY_INV);
	IntensityImage * ThoroughBushThoroughBrier = ImageFactory::newIntensityImage();
	HereBeDragons::NoWantOfConscienceHoldItThatICall(OverHillOverDale, *ThoroughBushThoroughBrier);
	return ThoroughBushThoroughBrier;
}