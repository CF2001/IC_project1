#ifndef WAVEffects_H
#define WAVEffects_H

#include <iostream>
#include <vector>
#include <map>
#include <sndfile.hh>
#include <fstream>

class WAVEffects {
  private:
  	std::vector<short> simpleEco;
  	std::vector<short> multipleEco;
  	//std::vector<std::vector<int>> accelerate;


  public:
	WAVEffects(const SndfileHandle& sfh) {
		multipleEco.resize(sfh.channels());  //a primeira dimensão é o número de canais
	}
	
	void simpEco(const std::vector<short>& samples, SndfileHandle& outFile, uint32_t delay, uint32_t ganho) { //processa e salva eco simples

		simpleEco.resize(samples.size()+(outFile.channels()*delay));

		for (int i = 0; i< delay*outFile.channels(); i++) {
			simpleEco[i] = samples[i];
		}

		//std::cerr<<outFile.channels()<<" ->outFile.channels()\n";
		//std::cerr<< samples.size()<<" ->samples.size()\n";

		short s;
		for (int i = delay*outFile.channels(); i< samples.size()+(delay*outFile.channels()); i++) {

			//std::cerr<<i<<" ->i\n";
			//std::cerr<<samples[i]<<" ->samples[i]\n";
			//std::cerr<< i - delay*outFile.channels()<<" ->i-delay * channels\n";
			//std::cerr<< samples[i-delay*outFile.channels()]<<" ->samples[i-delay * channels]\n";//*outFile.channels())]<<"\n";
			s = samples[i-(delay*outFile.channels())];
			simpleEco[i] = (samples[i] + ganho*s) / (1+ganho);

			//std::cerr<< simpleEco[i]<<" ->simpleEco[i]\n\n";
		}
		//std::cerr<<"ended\n";
		outFile.writef(simpleEco.data(), samples.size()/outFile.channels() + delay);

	}
	void multipEco(const std::vector<short>& samples, SndfileHandle& outFile, uint32_t delay, uint32_t ganho) {//processa e salva eco multiplo
		multipleEco.resize(samples.size()+(outFile.channels()*delay));

		for (int i = 0; i< delay*outFile.channels(); i++) {
			multipleEco[i] = samples[i];
		}

		//std::cerr<<outFile.channels()<<" ->outFile.channels()\n";
		//std::cerr<< samples.size()<<" ->samples.size()\n";

		short s;
		for (int i = delay*outFile.channels(); i< samples.size()+(delay*outFile.channels()); i++) {

			//std::cerr<<i<<" ->i\n";
			//std::cerr<<samples[i]<<" ->samples[i]\n";
			//std::cerr<< i - delay*outFile.channels()<<" ->i-delay * channels\n";
			//std::cerr<< samples[i-delay*outFile.channels()]<<" ->samples[i-delay * channels]\n";//*outFile.channels())]<<"\n";
			s = multipleEco[i-(delay*outFile.channels())];
			multipleEco[i] = (samples[i] + ganho*s) / (1+ganho);

			//std::cerr<< multipleEco[i]<<" ->multipleEco[i]\n\n";
		}
	}
	
	/*
	void accelUpdate(const std::vector<short>& samples, short velocidade) {//atualiza
		
	}
	void accelDump(SndfileHandle outFile) {//salva em arquivo

	}*/
};

#endif

