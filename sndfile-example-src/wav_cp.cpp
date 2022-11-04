#include <iostream>		
#include <vector>
#include <sndfile.hh>

using namespace std;		// para n escrever std::

/* Each of those frames contains 16-bits of resolution. */
constexpr size_t FRAMES_BUFFER_SIZE = 65536; // Buffer for reading/writing frames/samples

int main(int argc, char *argv[]) {

	bool verbose { false };	// igual a verbose=false

	/** Este programa tem 3 argumentos **/
	if(argc < 3) {
		cerr << "Usage: wav_cp [ -v (verbose) ]\n";
		cerr << "              wavFileIn wavFileOut\n";
		return 1;
	}

	for(int n = 1 ; n < argc ; n++)
		if(string(argv[n]) == "-v") {	// string() converte string de c para c++
			verbose = true;
			break;
		}

	/* Abrir o ficheiro de audio */
	/*
	 * argc: n de argumentos no terminal
	 * argv: vai buscar o nome do audio em funcao do indice
	 *
	*/
	SndfileHandle sfhIn { argv[argc-2] };		
	if(sfhIn.error()) {
		cerr << "Error: invalid input file\n";
		return 1;
    	}

	/* Verifica se esta no formato wav */
	/*
	 * SF_FORMAT_WAV: Microsoft WAV format (little endian)
	*/
	if((sfhIn.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
		cerr << "Error: file is not in WAV format\n";
		return 1;
	}

	/* Verifica se sao os 16 bits de amostra */
	/*
	 * SF_FORMAT_PCM_16: Signed 16 bit data
	*/
	if((sfhIn.format() & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16) {
		cerr << "Error: file is not in PCM_16 format\n";
		return 1;
	}

	/* Frames de um ficheiro --> pares de amostras, conjunto de valores q ocorrem 
					 temporalmente no mesmo instante. */
	/*
	* 529200 frames
	* 44100 samples per second
	* 2 channels
	*/
	if(verbose) {
		cout << "Input file has:\n";
		cout << '\t' << sfhIn.frames() << " frames\n";
		cout << '\t' << sfhIn.samplerate() << " samples per second\n"; // freq de amostragem
		cout << '\t' << sfhIn.channels() << " channels\n";
	}

	/* Cria ficheiro para escrita */
	/*
	 * argv[argc-1]: nome do ficheiro de saida 
	 * SFM_WRITE: write only mode
	*/
	SndfileHandle sfhOut { argv[argc-1], SFM_WRITE, sfhIn.format(), 
				sfhIn.channels(), sfhIn.samplerate() };
	if(sfhOut.error()) {
		cerr << "Error: invalid output file\n";
		return 1;
    }

	/* Le os blocos do ficheiro de entrada e escreve-os no ficheiro de saida */
	size_t nFrames;
	
	/*
	* O vetor contém um bloco de amostras de comprimento FRAMES_BUFFER_SIZE 
	* vezes o número de canais do áudio em causa.
	* Em cada posição do vetor vai haver um inteiro, com sinal, de 16 bits, 
	* ou seja, vai conter um valor entre -2^15 e (2^15)-1, que representa a 
	* intensidade do som nesse canal e nesse instante de tempo.
	*
	*/
	vector<short> samples(FRAMES_BUFFER_SIZE * sfhIn.channels()); 
	
	while((nFrames = sfhIn.readf(samples.data(), FRAMES_BUFFER_SIZE))) 
		sfhOut.writef(samples.data(), nFrames);
		
	return 0;
}


















