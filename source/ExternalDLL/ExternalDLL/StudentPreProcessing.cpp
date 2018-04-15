#include "StudentPreProcessing.h"
#include "ImageIO.h"
#include "ImageFactory.h"
#include <math.h>
#include <iostream>
#include <ctime>

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

Intensity average(const IntensityImage * boxsample)
{
	int sum = 0;
	for (int x = 0; x < boxsample->getWidth(); ++x)
		for (int y = 0; y < boxsample->getHeight(); ++y)
			sum += boxsample->getPixel(x, y);
	return sum / (boxsample->getHeight()*boxsample->getWidth());
}

Intensity gaussian(const IntensityImage * boxsample)
{
	//double d = boxsample->getWidth();
	//double r = d / 2;
	//double cumulative = 0.0;
	//double totalfactor = 0.0;
	////std::cout << "center: " << r << std::endl;
	//for (int x = 0; x < d; ++x)
	//	for (int y = 0; y < d; ++y)
	//	{
	//		double x_p = abs(r - x);
	//		double y_p = abs(r - y);
	//		double dist = sqrt(x_p*x_p + y_p*y_p);
	//		double weight = 1 - dist / r;
	//		cumulative += boxsample->getPixel(x, y) * weight;
	//		totalfactor += weight;
	//		//std::cout << "(" << x << ", " << y << "): " << weight << std::endl;
	//	}

	const double pi = atan(1) * 4;
	const double e = exp(1);
	const double sigma = boxsample->getWidth()/3;
	const double size = boxsample->getWidth();
	double cumulative = 0;
	double totalfactor = 0;
	for (int x = 0; x < size; ++x)
		for (int y = 0; y < size; ++y)
		{
			// calculate the Gaussian value for this pixel (x, y)
			double factor = (1.0 / (2.0 * pi*sigma*sigma))*pow(e, (-(((x - sigma / 2)*(x - sigma / 2) + (y - sigma / 2)*(y - sigma / 2)) / (2.0 * sigma*sigma))));
			// add 
			cumulative += boxsample->getPixel(x, y) * factor;
			totalfactor += factor;
		}
	return cumulative / totalfactor;
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
			result->setPixel(x, y, gaussian(boxsample));
		}
}

IntensityImage * StudentPreProcessing::stepScaleImage(const IntensityImage &image) const {
	std::clock_t start;
	double duration;
	start = std::clock();
	IntensityImage * result = ImageFactory::newIntensityImage();
	int targetsize = 40000;
	int originsize = image.getWidth() * image.getHeight();
	
	//if (targetsize < originsize)
	//{
		double ratio = 1.0 ;// sqrt(originsize / targetsize);	// ratio tussen oud en nieuw bepalen
		result->set(ratio * image.getWidth(), ratio * image.getHeight());
		resize(image, result, ratio);
	//}
	//else { std::cout << image.getWidth() << ", " << image.getHeight() << std::endl; }
	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	std::cout << "Time: " << duration << std::endl;
	return result;
}

IntensityImage * StudentPreProcessing::stepEdgeDetection(const IntensityImage &src) const { return nullptr; }

IntensityImage * StudentPreProcessing::stepThresholding(const IntensityImage &src) const { return nullptr; }