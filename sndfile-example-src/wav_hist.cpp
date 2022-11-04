#include <iostream>
#include <vector>
#include <sndfile.hh>
#include "wav_hist.h"

using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE = 65536; // Buffer for reading frames


int main(int argc, char *argv[]) {

	
	if(argc != 3) {
		cerr << "Usage: " << argv[0] << " <input file> <histogram type>\n";
		return 1;
	}
	
	char *inputFile = argv[argc-2];
	int histogramType = stoi(argv[argc-1]);

	SndfileHandle sndFile { inputFile };
	if(sndFile.error()) {
		cerr << "Error: invalid input file\n";
		return 1;
    }

	if((sndFile.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
		cerr << "Error: file is not in WAV format\n";
		return 1;
	}

	if((sndFile.format() & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16) {
		cerr << "Error: file is not in PCM_16 format\n";
		return 1;
	}
	
	if ((histogramType < 0) || (histogramType > 3)){
		cerr << "Error: invalid histogram type requested\n";
		return 1;
	}
	
	size_t nFrames;
	vector<short> samples(FRAMES_BUFFER_SIZE * sndFile.channels());
	WAVHist hist { sndFile };
	
	while((nFrames = sndFile.readf(samples.data(), FRAMES_BUFFER_SIZE))) {
		samples.resize(nFrames * sndFile.channels());
		
		hist.update(samples);		// Updates the counts vector
		hist.separateSamples(samples); // separate samples in channels
	}

	/* histogram of channel 0 or channel 1 */
	if (histogramType == 0 || histogramType == 1){
		hist.dump(histogramType);
	}	
	
	/* histogram of the average of the channels (mono version) */ 
	if (histogramType == 2){
		hist.updateMIDchannel(); 
		hist.createHistogram(2);
	}
	
	// histogram of the difference of the channels (stereo version)
	if (histogramType == 3){
		hist.updateSIDEchannel();
		hist.createHistogram(3);
	}
	
	
	return 0;
}





















