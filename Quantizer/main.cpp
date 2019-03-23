#include<fstream>
#include<iostream>
#include<string>
#include<vector>
#include"Quantizer.h"
using namespace std;

int main()
{
	/*ifstream inFile;
	ofstream outFile;
	char readNext;
	vector<int> pixelValue;
	long countLength = 0;*/

	/*inFile.open("audio.dat", ios::binary);
	while (inFile >> noskipws >> readNext) {
		++countLength;
		pixelValue.push_back(int(readNext));
	}
	inFile.close();

	outFile.open("audio_vector.dat", ios::binary);
	for (long i = 0; i < countLength; ++i) {
		outFile << pixelValue[i] << '\n';
	}
	outFile.close();*/

	/* chem */
	/*inFile.open("chem.256", ios::binary);
	while (inFile >> noskipws >> readNext) {
		++countLength;
		pixelValue.push_back(int(readNext));
	}
	inFile.close();

	outFile.open("chem.dat", ios::binary);
	for (long i = 0; i < countLength; ++i) {
		outFile << pixelValue[i] << '\n';
	}
	outFile.close();*/

	/* house */
	/*pixelValue.clear();
	inFile.open("house.256", ios::binary);
	while (inFile >> noskipws >> readNext) {
		pixelValue.push_back(int(readNext));
	}
	inFile.close();
	
	outFile.open("house.dat", ios::binary);
	for (long i = 0; i < countLength; ++i) {
		outFile << pixelValue[i] << '\n';
	}
	outFile.close();*/

	/* moon */
	/*pixelValue.clear();
	inFile.open("moon.256", ios::binary);
	while (inFile >> noskipws >> readNext) {
		pixelValue.push_back(int(readNext));
	}
	inFile.close();

	outFile.open("moon.dat", ios::binary);
	for (long i = 0; i < countLength; ++i) {
		outFile << pixelValue[i] << '\n';
	}
	outFile.close();*/

	int ib = 8, qb_1 = 2, qb_2 = 4;
	float eps = 0.001;
	Quantizer quan_2bit(ib, qb_1), quan_4bit(ib, qb_2);

	/* 2-bit quantizer training */
	quan_2bit.loadHist("chem.256");
	quan_2bit.loadHist("house.256");
	quan_2bit.loadHist("moon.256");
	quan_2bit.computeFreq();
	quan_2bit.uniformInitial();
	cout << "2-bit quantizer:" << endl;
	quan_2bit.trainQuantizer(eps);
	quan_2bit.printLevels();
	cout << endl;

	/* 4-bit quantizer training */
	quan_4bit.loadHist("chem.256");
	quan_4bit.loadHist("house.256");
	quan_4bit.loadHist("moon.256");
	quan_4bit.computeFreq();
	quan_4bit.uniformInitial();
	cout << "4-bit quantizer:" << endl;
	quan_4bit.trainQuantizer(eps);
	quan_4bit.printLevels();
	cout << endl;

	/* quantization */
	cout << "chem_2bit:" << '\t';
	quan_2bit.quantize("chem.256", "chem_2bit.dat");
	cout << "house_2bit:" << '\t';
	quan_2bit.quantize("house.256", "house_2bit.dat");
	cout << "moon_2bit:" << '\t';
	quan_2bit.quantize("moon.256", "moon_2bit.dat");
	cout << "couple_2bit:" << '\t';
	quan_2bit.quantize("couple.256", "couple_2bit.dat");
	cout << "elaine_2bit:" << '\t';
	quan_2bit.quantize("elaine.256", "elaine_2bit.dat");
	cout << "f16_2bit:" << '\t';
	quan_2bit.quantize("f16.256", "f16_2bit.dat");

	cout << "chem_4bit:" << '\t';
	quan_4bit.quantize("chem.256", "chem_4bit.dat");
	cout << "house_4bit:" << '\t';
	quan_4bit.quantize("house.256", "house_4bit.dat");
	cout << "moon_4bit:" << '\t';
	quan_4bit.quantize("moon.256", "moon_4bit.dat");
	cout << "couple_4bit:" << '\t';
	quan_4bit.quantize("couple.256", "couple_4bit.dat");
	cout << "elaine_4bit:" << '\t';
	quan_4bit.quantize("elaine.256", "elaine_4bit.dat");
	cout << "f16_4bit:" << '\t';
	quan_4bit.quantize("f16.256", "f16_4bit.dat");

	return 0;
}