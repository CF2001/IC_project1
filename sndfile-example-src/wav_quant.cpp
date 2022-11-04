#include <iostream>		
#include <vector>
#include <sndfile.hh>
#include <string>
#include "wav_quant.h"

using namespace std;

/* 
* Each of those frames contains 16-bits of resolution. 
* Buffer for reading/writing frames/samples
*/
constexpr size_t FRAMES_BUFFER_SIZE = 65536;

int main(int argc, char *argv[]) {	

	if(argc != 4) {
		cerr << "Usage: " << argv[0] << " <input file> <output file> <resolution bits>\n";
		return 1;
	}
	
	char *inputFile = argv[argc-3];
	char *outputFile = argv[argc-2];
	int resolutionBits = stoi(argv[argc-1]);


	/* Abrir o ficheiro de audio */
	SndfileHandle sfhIn { inputFile };		
	if(sfhIn.error()) {
		cerr << "Error: invalid input file\n";
		return 1;
    	}

	/* Verifica se esta no formato wav */
	if((sfhIn.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
		cerr << "Error: file is not in WAV format\n";
		return 1;
	}

	/* Verifica se sao os 16 bits de amostra */
	if((sfhIn.format() & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16) {
		cerr << "Error: file is not in PCM_16 format\n";
		return 1;
	}

	/* Cria ficheiro para escrita */
	SndfileHandle sfhOut { outputFile, SFM_WRITE, sfhIn.format(), 
				sfhIn.channels(), sfhIn.samplerate() };
	if(sfhOut.error()) {
		cerr << "Error: invalid output file\n";
		return 1;
    	}
    	
    	/** Garantir que resolution bits estão entre 0 a 16 !!!!! */
    	if (resolutionBits < 0 || resolutionBits > 16)
    	{
    		cerr << "Error: invalid resolution bits.\n";
		return 1;
    	}

	size_t nFrames;
	vector<short> samples(FRAMES_BUFFER_SIZE * sfhIn.channels()); 
	
	
	/* Only to get the histogram of channel 0 or 1 of the compressed audio file */
	//WAVQuant quant { sfhIn, FRAMES_BUFFER_SIZE * sfhIn.channels() };
	
	WAVQuant quant { FRAMES_BUFFER_SIZE * sfhIn.channels() };
	
	
	while((nFrames = sfhIn.readf(samples.data(), FRAMES_BUFFER_SIZE))){ 
		samples.resize(nFrames * sfhIn.channels());
	
		
		sfhOut.writef(quant.quantization(samples,resolutionBits).data(), nFrames);
		
		/* Only to get the histogram of channel 0 or 1 of the compressed audio file */
		//quant.update(quant.newsamples());
	}	
	
	/* Only to get the histogram of channel 0 or 1 of the compressed audio file */
	//quant.dump(0); // channel0: dump(0) ; channel1: dump(1)
	
	return 0;
}













