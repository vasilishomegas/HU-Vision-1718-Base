#include "StudentPreProcessing.h"
#include "ImageIO.h"
#include "ImageFactory.h"
#include <math.h>
#include <iostream>

IntensityImage * StudentPreProcessing::stepToIntensityImage(const RGBImage &src) const { return nullptr; }

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

Intensity linear(const IntensityImage * boxsample)
{
	int sum = 0;
	for (int x = 0; x < boxsample->getWidth(); ++x)
		for (int y = 0; y < boxsample->getHeight(); ++y)
			sum += boxsample->getPixel(x, y);
	return sum / (boxsample->getHeight()*boxsample->getWidth());
}

Intensity bilinear(const IntensityImage * boxsample)
{
	double sigma = 10;
	const double pi = atan(1) * 4;
	const double e = exp(1);
	const double euler = std::exp(1.0);
	std::cout << e << std::endl;
	int x = boxsample->getWidth();
	int y = boxsample->getHeight();
	return ((1 / (2 * pi*sigma*sigma))*pow(e, (-((x*x + y*y) / (2 * sigma*sigma)))))
	
	//return (1/2*pi*sigma*sigma)*pow(e, (-;
}

void resize(const IntensityImage & origin, IntensityImage * result, const double & ratio)
{
	double multiplier = 1.0 / ratio;
	IntensityImage * boxsample = ImageFactory::newIntensityImage();
	boxsample->set(multiplier, multiplier);
	for (int y = 0; y < result->getHeight(); ++y)
		for (int x = 0; x < result->getWidth(); ++x)
		{
			for (int j = 0; j < multiplier; ++j)
				for (int i = 0; i < multiplier; ++i)			
					boxsample->setPixel(i, j, origin.getPixel(multiplier*x + i, multiplier*y + j));
			result->setPixel(x, y, linear(boxsample));
		}
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
	return result;
}

IntensityImage * StudentPreProcessing::stepEdgeDetection(const IntensityImage &src) const { return nullptr; }

IntensityImage * StudentPreProcessing::stepThresholding(const IntensityImage &src) const { return nullptr; }