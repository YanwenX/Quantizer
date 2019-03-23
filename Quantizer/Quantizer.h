#ifndef QUANTIZER_H
#define QUANTIZER_H

#include<vector>
#include<fstream>
#include<string>
using std::vector;
using std::ifstream;
using std::ofstream;
using std::string;

typedef struct PixelInfoEightBit {
	int intensity;		// intensity value
	long count;			// # of pixels with a certain intensity
	float freq;			// frequency of a certain intensity
	PixelInfoEightBit(int I, long c, float f) : intensity(I), count(c), freq(f) {}
}*PI8B;

class Quantizer
{
protected:
	vector<PI8B> pixelHist;			// histogram of the training set
	ifstream inFile;
	ofstream outFile;
	int quanBit, intensityBit;		// # of bits for quantizer and the original image intensities
	vector<float> intervalBound;	// quantization interval bounds
	vector<float> reconstructLevel;	// reconstruction levels
public:
	Quantizer(int, int);			// constructor
	void loadHist(string);			// load the histogram of the training set
	void computeFreq();				// compute the frequency of each intensity value
	void printHist();
	void uniformInitial();			// initialize the quantizer uniformly
	void trainQuantizer(float);
	void printLevels();
	void quantize(string, string);	// quantize the image using the trained quantizer
};

#endif	// QUANTIZER_H