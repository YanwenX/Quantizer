#include"Quantizer.h"
#include<iostream>
#include<cmath>
using std::pow;
using std::abs;
using std::log10;
using std::log2;
using std::cout;
using std::endl;
using std::ios;
using std::noskipws;

Quantizer::Quantizer(int ib, int qb)
{
	intensityBit = ib;
	quanBit = qb;
	for (int i = 0; i < pow(2, ib); ++i) {
		pixelHist.push_back(new PixelInfoEightBit(i, 0, 0));
	}
}

void Quantizer::loadHist(string fileName)
{
	inFile.open(fileName, ios::binary);
	
	unsigned char readNext;
	while (inFile >> noskipws >> readNext) {
		++pixelHist[int(readNext)]->count;
	}
	
	inFile.close();
}

void Quantizer::computeFreq()
{
	long sumCount = 0;
	for (int i = 0; i < pixelHist.size(); ++i) {
		sumCount += pixelHist[i]->count;
	}
	for (int i = 0; i < pixelHist.size(); ++i) {
		pixelHist[i]->freq = float(pixelHist[i]->count) / sumCount;
	}
}

void Quantizer::printHist()
{
	for (int i = 0; i < 256; ++i) {
		cout << pixelHist[i]->intensity << '\t'
			<< pixelHist[i]->count << '\t'
			<< pixelHist[i]->freq << endl;
	}
}

void Quantizer::uniformInitial()
{
	for (int i = 0; i <= pow(2, quanBit); ++i) {
		intervalBound.push_back(i * pow(2, intensityBit) / pow(2, quanBit));
	}
}

void Quantizer::trainQuantizer(float eps)
{
	float MSE_1 = 0, MSE_2 = 0;
	/* initial r's */
	for (int i = 0; i < pow(2, quanBit); ++i) {
		float weightedMean = 0, freqSum = 0;
		for (int j = intervalBound[i]; j < intervalBound[i + 1]; ++j) {
			weightedMean += pixelHist[j]->intensity * pixelHist[j]->freq;
			freqSum += pixelHist[j]->freq;
		}
		reconstructLevel.push_back(weightedMean / freqSum);
	}

	/* initial error */
	for (int i = 0; i < pow(2, quanBit); ++i) {
		float errorInOneLevel = 0;
		for (int j = intervalBound[i]; j < intervalBound[i + 1]; ++j) {
			errorInOneLevel += pixelHist[j]->freq *
				pow((pixelHist[j]->intensity - reconstructLevel[i]), 2);
		}
		MSE_2 += errorInOneLevel;
	}

	/* iteration */
	float PSNR;
	long countIteration = 0;
	cout << "epslon = " << eps << endl;
	while (abs(MSE_1 - MSE_2) / MSE_2 >= eps) {
		/* PSNR */
		PSNR = 10 * log10(pow(pow(2, intensityBit) - 1, 2) / MSE_2);
		cout << countIteration << '\t' << "PSNR = " << PSNR << endl;
		/* count iteration */
		++countIteration;

		/* update t's */
		for (int i = 1; i < pow(2, quanBit); ++i) {
			intervalBound[i] = (reconstructLevel[i - 1] + reconstructLevel[i]) / 2;
		}
		/* update r's */
		for (int i = 0; i < pow(2, quanBit); ++i) {
			float weightedMean = 0, freqSum = 0;
			for (int j = intervalBound[i]; j < intervalBound[i + 1]; ++j) {
				weightedMean += pixelHist[j]->intensity * pixelHist[j]->freq;
				freqSum += pixelHist[j]->freq;
			}
			reconstructLevel[i] = (weightedMean / freqSum);
		}
		/* update MSE */
		MSE_1 = MSE_2;		// MSE_1 for the previous MSE
		MSE_2 = 0;			// compute new MSE_2
		for (int i = 0; i < pow(2, quanBit); ++i) {
			float errorInOneLevel = 0;
			for (int j = intervalBound[i]; j < intervalBound[i + 1]; ++j) {
				errorInOneLevel += pixelHist[j]->freq *
					pow((pixelHist[j]->intensity - reconstructLevel[i]), 2);
			}
			MSE_2 += errorInOneLevel;
		}
	}
	cout << "Number of iterations: " << countIteration << endl;
}

void Quantizer::printLevels()
{
	for (int i = 0; i < intervalBound.size(); ++i) {
		cout << intervalBound[i] << '\t';
	}
	cout << endl;
	for (int i = 0; i < reconstructLevel.size(); ++i) {
		cout << reconstructLevel[i] << '\t';
	}
	cout << endl;
}

void Quantizer::quantize(string inFileName, string outFileName)
{
	inFile.open(inFileName, ios::binary);
	outFile.open(outFileName, ios::binary);
	unsigned char readNext;

	/* histogram for each image */
	vector<PI8B> imageHist;
	for (int i = 0; i < pow(2, intensityBit); ++i) {
		imageHist.push_back(new PixelInfoEightBit(i, 0, 0));
	}

	/* quantized output and update the histogram */
	long countPixel = 0;
	while (inFile >> noskipws >> readNext) {
		++countPixel;
		++imageHist[int(readNext)]->count;
		for (int i = 0; i < reconstructLevel.size(); ++i) {
			if (int(readNext) >= intervalBound[i] && int(readNext) < intervalBound[i + 1]) {
				outFile << reconstructLevel[i] << '\n';
				break;
			}
		}
	}

	/* frequencies */
	for (int i = 0; i < imageHist.size(); ++i) {
		imageHist[i]->freq = float(imageHist[i]->count) / countPixel;
	}

	/* MSE */
	float MSE = 0;
	for (int i = 0; i < pow(2, quanBit); ++i) {
		float errorInOneLevel = 0;
		for (int j = intervalBound[i]; j < intervalBound[i + 1]; ++j) {
			errorInOneLevel += imageHist[j]->freq *
				pow((imageHist[j]->intensity - reconstructLevel[i]), 2);
		}
		MSE += errorInOneLevel;
	}

	/* PSNR */
	float PSNR;
	PSNR = 10 * log10(pow(pow(2, intensityBit) - 1, 2) / MSE);
	cout << "PSNR = " << PSNR << '\t';

	/* Entropy */
	float Entropy = 0;
	for (int i = 0; i < imageHist.size(); ++i) {
		if (!imageHist[i]->freq) continue;
		else
			Entropy -= imageHist[i]->freq * log2(imageHist[i]->freq);
	}
	cout << "Entropy = " << Entropy << endl;

	outFile.close();
	inFile.close();
}