#include <iostream>
#include <sndfile.hh>
#include <map>
#include <vector>

using namespace std;

class WAVQuant {

	private: 
		vector<short> newSamples; 
		short newSample;
		int bitsToreduce;
		
		/* Only to get the histogram of channel 0 or 1 of the compressed audio file */
		vector<map<short, size_t>> counts;

	public: 
		/* Only to get the histogram of channel 0 or 1 of the compressed audio file */
		WAVQuant(const SndfileHandle& sfh, size_t size) {
			counts.resize(sfh.channels());	
			newSamples.resize(size);
		}
		
		
		WAVQuant(size_t size)
		{
			newSamples.resize(size);
		}
		
		int bitsToReduce(int resolutionBits)
		{
			return 16 - resolutionBits;
		}
		
		vector<short> quantization(const std::vector<short>& samples, int resolutionBits){
			
			for (size_t i = 0; i < samples.size(); i++){
				//cout << s << endl;
				newSample = (samples[i] >> bitsToReduce(resolutionBits)) << bitsToReduce(resolutionBits);
				//cout << "NEW: " << newSample << endl;
				newSamples[i] = newSample;
			}
			return newSamples;
		}
		
		vector<short> newsamples()
		{
			return newSamples;
		}
		
		/* Only to get the histogram of channel 0 or 1 of the compressed audio file */
		void update(const std::vector<short>& samples) {
			size_t n { };
			
			for(auto s : samples) {	// s: contém os dados da sample respetivo.
				counts[n++ % counts.size()][s]++;	// atualiza o counter c0 e c1 (size_t)
			}												
		}

		/* Only to get the histogram of channel 0 or 1 of the compressed audio file */
		void dump(const size_t channel) const {
			for(auto [value, counter] : counts[channel])	// percorre apenas 1 canal
				cout << value << '\t' << counter << '\n';
		}
		
};




