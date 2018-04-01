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

Intensity middlepixel(const IntensityImage * boxsample)
{
	return boxsample->getPixel((boxsample->getWidth() / 2), (boxsample->getHeight() / 2));
}

Intensity medianpixel(const IntensityImage * boxsample)
{
	std::vector<uchar> allvalues;
	for (int x = 0; x < boxsample->getWidth(); ++x)
		for (int y = 0; y < boxsample->getHeight(); ++y)
			allvalues.push_back(boxsample->getPixel(x, y));
	std::sort(allvalues.begin(), allvalues.end());
	int length = allvalues.size();
	return length % 2 ? allvalues[length / 2] : (allvalues[length / 2] + allvalues[length / 2 - 1]) / 2;
}

Intensity linear(const IntensityImage * boxsample);

Intensity bilinear(const IntensityImage * boxsample);


IntensityImage * resize(const IntensityImage & origin, const IntensityImage * result, const double & ratio)
{
	//cv::Mat newMatrix;
	//newMatrix.create(origin.cols*ratio, origin.rows*ratio, CV_8UC1); // nieuwe matrix aanmaken
	//cv::Mat boxSample;
	double multiplier = 1.0 / ratio;
	IntensityImage * boxsample = ImageFactory::newIntensityImage();
	boxsample->set(multiplier, multiplier);
	//std::array<uchar, px> pixels;
	

	for (int x = 0; x < boxsample->getWidth(); ++x)
		for (int y = 0; y < boxsample->getHeight; ++y)
		{
			for (int i = 0; i < multiplier; ++i)
				for (int j = 0; j < multiplier; ++j)
					boxsample->setPixel(i, j, origin.getPixel(multiplier*x + i, multiplier*y + j));
					//boxSample.at<uchar>(j, i) = origin.at<uchar>((multiplier*y+j), (multiplier*x+i));
			result->setPixel(x, y, middlepixel(boxsample));	// zegt een Intensity nodig te hebben, is een Intensity, maar zeurt dat het geen IntensityImage is?
				//newMatrix.at<uchar>(y, x) = middlepixel(boxSample);
		}

	// pixels mappen
	// klaar
}

IntensityImage * StudentPreProcessing::stepScaleImage(const IntensityImage &image) const {
	IntensityImage * result = ImageFactory::newIntensityImage();

	int targetsize = 40000;
	int originsize = image.getWidth() * image.getHeight();
	
	if (targetsize < originsize)
	{
		double ratio = 1.0 / sqrt(originsize / targetsize);	// ratio tussen oud en nieuw bepalen
		result->set(ratio * image.getWidth(), ratio * image.getHeight());
		resize(image, result, ratio);
	}

	//cv::Mat matrix;
	// image inladen
	//matrix.create(image.getHeight(), image.getWidth(), CV_8UC1); // CV_8UC1 ???
	//for (int x = 0; x < matrix.cols; ++x)
		//for (int y = 0; y < matrix.rows; ++y)
			//matrix.at<uchar>(y, x) = image.getPixel(x, y); // at<uchar> ???

	
		
		//auto target = resize(matrix, ratio); // daarmee pixels per pixel bepalen
	}


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