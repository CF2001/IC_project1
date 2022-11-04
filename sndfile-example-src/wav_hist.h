#ifndef WAVHIST_H
#define WAVHIST_H

#include <iostream>
#include <vector>
#include <map>
#include <sndfile.hh>

class WAVHist {
  private:
  	// short: intensidade do som no determinado canal
  	// size_t: nº vezes q a intensidade aparece no determinado canal 
	std::vector<std::map<short, size_t>> counts;	
	std::vector<short> channel0;
	std::vector<short> channel1;
	std::map<short, size_t> midChannel;	
	std::map<short,size_t> sideChannel;

  public:
  	/* Constructor: receives an audio file as input */
  	/*
  	* resize(n): Resizes the container so that it contains ‘n’ elements.
  	*/
	WAVHist(const SndfileHandle& sfh) {
		counts.resize(sfh.channels());	// contains 2 channels
	}

	
	// n: permite saltar do canal 0 -> 1 e vs.
	// counts.size() : nº de canais que se tem 
	void update(const std::vector<short>& samples) {
		size_t n { };
		
		for(auto s : samples) {	// s: contém os dados da sample respetivo.
			counts[n++ % counts.size()][s]++;	// atualiza o counter c0 e c1 (size_t)
		}												
	}

	void dump(const size_t channel) const {
		for(auto [value, counter] : counts[channel])	// percorre apenas 1 canal
			std::cout << value << '\t' << counter << '\n';
	}
	
	
	/* 
	* Função que coloca todas as amostra do canal 0 no vetor channel 0 
	* Função que coloca todas as amostra do canal 1 no vetor channel 1
	* As amostras vêm da seguinte forma: esquerda, direita, esquerda...
	*/
	void separateSamples(const std::vector<short>& samples) {
		size_t n { };
		
		for (auto s : samples){
			if (n%2 == 0){	// Channel 0
				channel0.insert(channel0.end(), s);			
			}else{	// Channel 1
				channel1.insert(channel1.end(), s);
			}
			n++;
		}
	}
	
	/* histogram of the MID channel */
	// Calculate MID channel = (L + R)/2 
	void updateMIDchannel(){
		short L { };
		short R { };
		
		for (size_t i = 0; i < channel0.size(); i++){
			L = channel0[i];
			R = channel1[i];
			midChannel[(int)(L+R)/2]++;
		}	
	}
	
	/* histogram of the SIDE channel */
	// Calculate SIDE channel = (L - R)/2
	void updateSIDEchannel(){
		short L { };
		short R { };
		
		for (size_t i = 0; i < channel0.size(); i++){
			L = channel0[i];
			R = channel1[i];
			sideChannel[(int)(L-R)/2]++;
		}
	}
	
	void createHistogram(int histogramType)
	{
		if (histogramType == 2) // Build histogram of MID channel
		{
			for (auto [value, counter] : midChannel){
				std::cout << value << '\t' << counter << '\n';
			}
			
		}else{		// Build histogram of SIDE channel
		
			for (auto [value, counter] : sideChannel){
				std::cout << value << '\t' << counter << '\n';
			}
		}
	}
	
};

#endif



















