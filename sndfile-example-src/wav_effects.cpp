#include <iostream>
#include <vector>
#include <sndfile.hh>
#include "wav_effects.h"

using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE = 65536;  // Buffer for reading frames

int main(int argc, char *argv[]) {
	if(argc < 3) {
		cerr << "Usage: " << argv[0] << " <input file> <output file> <effectType> [<gain> <delay>]\n";
		//input output effect gain delay
		return 1;
	}

	SndfileHandle inFile { argv[1] };  //abre o ficheiro para ler(construtor)
	if(inFile.error()) {
		cerr << "Error: invalid input file\n";
		return 1;
    }

	//abrir para escrita, mesmo formato, mesmo numero de canais, mesmo samplerate
	SndfileHandle outFile { argv[2], SFM_WRITE, inFile.format(), inFile.channels(), inFile.samplerate() }; 	
	if(outFile.error()) {
		cerr << "Error: invalid output file\n";
		return 1;
    	}

	//verifica se formato é permitido
	if((inFile.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) { 	
		cerr << "Error: file is not in WAV format\n";
		return 1;
	}

	if((inFile.format() & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16) {
		cerr << "Error: file is not in PCM_16 format\n";
		return 1;
	}

	int effectType = atoi(argv[3]);		//efeitos vão de 0 a 2
	if(effectType > 2 || effectType < 0) {
		cerr << "Error: invalid effect type requested\n";
		return 1;
	}


	size_t nFrames;
	vector<short> samples;
	vector<short> bufferVector(FRAMES_BUFFER_SIZE * inFile.channels());
	WAVEffects effect { inFile };	 //cria um objeto do tipo wave effect

	switch (effectType)
	{
		case (0): 	//simple Eco
		{
			float gain = stof(argv[4]);
			//cerr<<gain <<"-> gain\n";
			if(gain > 1 || gain < -1) {
				cerr << "Error: invalid gain requested\n";
				return 1;
			}
			float delay = stof(argv[5]);
			//cerr<<delay <<"-> delay\n";
			if(delay <= 0) {
				cerr << "Error: invalid delay requested\n";
				return 1;
			}

			while((nFrames = inFile.readf(bufferVector.data(), FRAMES_BUFFER_SIZE))) {
				bufferVector.resize(nFrames * inFile.channels()); 	//redimenciona buffer para ter o tamanho real do póximo bloco
				for(auto value : bufferVector) {
					samples.push_back(value); 	//Adiciona cada valor do buffer no final do vetor samples
				}
			}
			effect.simpEco(samples, outFile, delay, gain);  //aplica o efeito
			break;
		}
		case (1):  //multiple Eco
		{
			float gain = stof(argv[4]);
			cerr<<gain <<"-> gain\n";
			if(gain > 1 || gain < -1) {
				cerr << "Error: invalid gain requested\n";
				return 1;
			}
			float delay = stof(argv[5]);
			cerr<<delay <<"-> delay\n";
			if(delay <= 0) {
				cerr << "Error: invalid delay requested\n";
				return 1;
			}
			
			while((nFrames = inFile.readf(bufferVector.data(), FRAMES_BUFFER_SIZE))) {
				for(auto value : bufferVector) {
					samples.push_back(value); 						
				}
				bufferVector.resize(nFrames * inFile.channels()); 
			}
			//effect.multipEco(samples, outFile, delay, gain); 		
			break;
		}
		case (2): //accelerate
		{
			/*
			short velocidade = 2;									//forma genérica que poderia ser um arg do terminal
			while((nFrames = inFile.readf(samples.data(), FRAMES_BUFFER_SIZE))) { //nframes = tamanho do próx buffer
				samples.resize(nFrames * inFile.channels());
				effect.accelUpdate(samples, velocidade); 			//faz update
			}
			effect.accelDump(outFile); 			//salva
			break;
			*/
		}
	}
	return 0;
}

