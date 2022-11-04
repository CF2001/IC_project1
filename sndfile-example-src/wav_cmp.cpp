#include <iostream>		
#include <vector>
#include <sndfile.hh>
#include <string>
#include <math.h>

using namespace std;

/* 
* Each of those frames contains 16-bits of resolution. 
* Buffer for reading/writing frames/samples
*/
constexpr size_t FRAMES_BUFFER_SIZE = 65536;

int main(int argc, char *argv[]) {	

	if(argc != 3) {
		cerr << "Usage: " << argv[0] << " <original file> <compressed file>\n";
		return 1;
	}
	
	char *originalFile = argv[1];
	char *compressedFile = argv[2];

	
	SndfileHandle sfhOriginal { originalFile };	/* Abrir o ficheiro de audio original */	
	if(sfhOriginal.error()) {
		cerr << "Error: invalid input file\n";
		return 1;
    	}

	/* Verifica se esta no formato wav */
	if((sfhOriginal.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
		cerr << "Error: file is not in WAV format\n";
		return 1;
	}

	/* Verifica se sao os 16 bits de amostra */
	if((sfhOriginal.format() & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16) {
		cerr << "Error: file is not in PCM_16 format\n";
		return 1;
	}
	
	
	SndfileHandle sfhCompressed { compressedFile };  /* Abrir o ficheiro de audio comprimido */
	if(sfhCompressed.error()) {
		cerr << "Error: invalid input file\n";
		return 1;
    	}

	/* Verifica se esta no formato wav */
	if((sfhCompressed.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
		cerr << "Error: file is not in WAV format\n";
		return 1;
	}


	size_t nFrames;
	
	/* Amostras do fichiro original */
	vector<short> OrigSamples(FRAMES_BUFFER_SIZE * sfhOriginal.channels()); 
	/* Amostras do ficheiro comprimido */
	vector<short> ComprSamples(FRAMES_BUFFER_SIZE * sfhCompressed.channels()); 
	
	/* Energia do sinal */
	double signalSum = 0;	// Soma das amostras originais ao quadrado
	/* Energia do erro do sinal */ 
	double errorSum = 0;	// Soma da diferenca entre a amostra original e comprimida ao quadrado
	/* Signal-to-noise ratio */
	double SNR = 0;  	
	/* Maximum per sample absolute error */
	double tmpError = 0;
	double maxError = 0;

	
	while((nFrames = sfhOriginal.readf(OrigSamples.data(), FRAMES_BUFFER_SIZE)))
	{
		// read compressed samples
		sfhCompressed.readf(ComprSamples.data(), FRAMES_BUFFER_SIZE);
		
		// Nota: OrigSamples.size() = ComprSamples.size()
		for (size_t i = 0; i < OrigSamples.size(); i++)
		{
			signalSum += pow(OrigSamples[i],2);	
			errorSum += pow((OrigSamples[i] - ComprSamples[i]),2);
			tmpError = abs(OrigSamples[i] - ComprSamples[i]);
			
			if (tmpError > maxError)
			{
				maxError = tmpError;
			}
		}
	}
	
	SNR = 10*log10(signalSum/errorSum);
	
	cout << "Signal-to-noise ratio: " << SNR << " dB" << endl;
	cout << "Maximum per sample absolute error: " << maxError << endl;
		
	return 0;
}

