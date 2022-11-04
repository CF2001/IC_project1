#include <iostream>
#include <vector>
#include <cmath>
#include <fftw3.h>
#include <sndfile.hh>
#include <bitset>
#include <cstring>

#include "BitStream.h"
#include "wav_dct.h"

using namespace std;

int main(int argc, char *argv[]) {

	bool verbose { false };
	size_t bs { 1024 };		// tamanho de cada bloco; cada bloco tem 1024 frames
	double dctFrac { 0.2 };

	if(argc != 4) {
		cerr << "Usage: wav_dct [ -v (verbose) ]\n";
		cerr << "               [ -bs blockSize (def 1024) ]\n";
		cerr << "               [ -frac dctFraction (def 0.2) ]\n";
		cerr << "               wavFileIn wavFileOut binaryFile\n";
		return 1;
	}

	for(int n = 1 ; n < argc ; n++)
		if(string(argv[n]) == "-v") {
			verbose = true;
			break;
		}

	for(int n = 1 ; n < argc ; n++)
		if(string(argv[n]) == "-bs") {
			bs = atoi(argv[n+1]);
			break;
		}

	for(int n = 1 ; n < argc ; n++)
		if(string(argv[n]) == "-frac") {
			dctFrac = atof(argv[n+1]);
			break;
		}

	SndfileHandle sfhIn { argv[argc-3] };
	if(sfhIn.error()) {
		cerr << "Error: invalid input file\n";
		return 1;
    }

	if((sfhIn.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
		cerr << "Error: file is not in WAV format\n";
		return 1;
	}

	if((sfhIn.format() & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16) {
		cerr << "Error: file is not in PCM_16 format\n";
		return 1;
	}

	
	SndfileHandle sfhOut { argv[argc-2], SFM_WRITE, sfhIn.format(),
	  sfhIn.channels(), sfhIn.samplerate() };
	if(sfhOut.error()) {
		cerr << "Error: invalid output file\n";
		return 1;
    	}

	if(verbose) {
		cout << "Input file has:\n";
		cout << '\t' << sfhIn.frames() << " frames\n";
		cout << '\t' << sfhIn.samplerate() << " samples per second\n";
		cout << '\t' << sfhIn.channels() << " channels\n";
	}

	size_t nChannels { static_cast<size_t>(sfhIn.channels()) };
	size_t nFrames { static_cast<size_t>(sfhIn.frames()) };

	// Read all samples: c1 c2 ... cn c1 c2 ... cn ...
	// Note: A frame is a group c1 c2 ... cn
	vector<short> samples(nChannels * nFrames);
	sfhIn.readf(samples.data(), nFrames);	// ler as amostras e inseri-las no vetor samples

	// Cada bloco tem 1024 frames
	size_t nBlocks { static_cast<size_t>(ceil(static_cast<double>(nFrames) / bs)) };

	// Do zero padding, if necessary
	samples.resize(nBlocks * bs * nChannels);

	// Vector for holding all DCT coefficients, channel by channel
	// vetor externo:  c1    			c2		=> tamanhoM = 2
	// vetor interno:  DCT coefficients 		DCT coefficients => tamnhoM = nFrames
	vector<vector<double>> x_dct(nChannels, vector<double>(nBlocks * bs));
 	
	
	// Vector for holding DCT computations
	vector<double> x(bs);
	
	// Direct DCT
	fftw_plan plan_d = fftw_plan_r2r_1d(bs, x.data(), x.data(), FFTW_REDFT10, FFTW_ESTIMATE);
	for(size_t n = 0 ; n < nBlocks ; n++)
	{	
		for(size_t c = 0 ; c < nChannels ; c++) {	
			
			for(size_t k = 0 ; k < bs ; k++)
				x[k] = samples[(n * bs + k) * nChannels + c];

			fftw_execute(plan_d);
			// Keep only "dctFrac" of the "low frequency" coefficients
			for(size_t k = 0 ; k < bs * dctFrac ; k++){
				x_dct[c][n * bs + k] = x[k] / (bs << 1);
			}
		}
		
	}
	
	
	/************** LOSSY ENCODER **************/
	
	// Binary File for writing
	char *binaryFileW = argv[argc-1];
	BitStream bsW { binaryFileW , 'w'};
	string binary;
		
	//size_t count = 0;
	// Write the resulting DCT bits to a binary file
	for (size_t n = 0; n < nBlocks; n++)
	{
		for (size_t c = 0; c < nChannels; c++)
		{
			for(size_t k = 0; k < bs; k++)
			{	
				// Convert DCT coefficients from double->int and then convert to 16-bit binary
				binary = bitset<16>(round(x_dct[c][n * bs + k])).to_string();  // double->int->binary  
				// write the 16 bits to a binary file 
				bsW.write_Nbits(binary); 
			}
		}   
	}	

	
	/************** LOSSY DECODER **************/
	
	
	// Binary file for reading
	char *binaryFileR = argv[argc-1];
	BitStream bsR { binaryFileR , 'r'};
	
	int bitsToRead = (bsR.binaryFile_size() *8); 
		
	vector<int> totalBits = bsR.read_Nbits(bitsToRead);	
	
	string bin;
	int significantBits = 16;
	int decimal;
	vector<int> x_dct_binaryFile;
	
	
	/* The values in the array totalBits are integers, so we convert 
	 each bit of that array to '0' or '1' and join all those bits in the bin string */
	for(int i = 0; i < bitsToRead; i++){
		if (totalBits[i] == 0)
		{
			//cout << "is 0 " << endl;
			bin += '0';
		}else if (totalBits[i] == 1)
		{
			//cout << "is 1 " << endl;
			bin += '1';
		}
	}
	 
	wavDCT dct {};
	
	
	/* Each binary value is represented with 16 bits, so we get all 16 bits 
	  of the string bin and each 16-bit value is converted to an signed int */
	for (int i = 0; i < bitsToRead; i++)
	{
		if (i == 0)
		{
			//cout << bin.substr(i,16) << endl;
			decimal = dct.binaryToSignedDecimal(bin.substr(i,16).c_str(),significantBits);
			//cout << "Decimal: " << decimal << endl;
			x_dct_binaryFile.push_back(decimal);
		}else if (i % 16 == 0)
		{
			//cout << "multiplo 16 : " << i << endl;
			//cout << bin.substr(i, 16) << endl;
			decimal = dct.binaryToSignedDecimal(bin.substr(i,16).c_str(),significantBits);
			//cout << "Decimal: " << decimal << endl;
			x_dct_binaryFile.push_back(decimal);
		}
	}
	
	vector<vector<double>> x_dct_test(nChannels, vector<double>(nBlocks * bs));
	
	size_t count = 0;
	for(size_t n = 0; n < nBlocks; n++) {
		for(size_t c = 0; c < nChannels; c++) {
	    		for (size_t k = 0; k < bs; k++) {
				x_dct_test[c][n*bs + k] = x_dct_binaryFile[count];
				count++;
	    		}
		}
	}
	

	// Inverse DCT
	fftw_plan plan_i = fftw_plan_r2r_1d(bs, x.data(), x.data(), FFTW_REDFT01, FFTW_ESTIMATE);
	for(size_t n = 0 ; n < nBlocks ; n++)
		for(size_t c = 0 ; c < nChannels ; c++) {
			for(size_t k = 0 ; k < bs ; k++){
				x[k] = x_dct_test[c][n*bs + k];
				//x[k] = x_dct[c][n * bs + k];

			}
			fftw_execute(plan_i);
			for(size_t k = 0 ; k < bs ; k++){
				samples[(n * bs + k) * nChannels + c] = static_cast<short>(round(x[k]));
			}	

		}

	sfhOut.writef(samples.data(), sfhIn.frames());
	return 0;
}








