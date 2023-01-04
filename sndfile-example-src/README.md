### Project 1 - Information and coding 

To build:
	make
	
	
To test Exercise 2:

	../sndfile-example-bin/wav_hist <input file> <histogram type>
	
	<input file> => wav audio file
	<histogram type>: 
			=> 0 (histogram of channel 0)
			=> 1 (histogram of channel 1)
			=> 2 (histogram of MID channel)
			=> 3 (histogram of SIDE channel)
			
  	To construct the histogram:
  	
	  	1 - ../sndfile-example-bin/wav_hist <input file> <histogram type> > histogram.txt
	  	
	  	2 - gnuplot: plot "histogram.txt" with boxes.
	  	
	Exemplification:
		../sndfile-example-bin/wav_hist ../audioFiles/sample01.wav 0 > histogram_0.txt


To test Exercise 3:

	../sndfile-example-bin/wav_quant <input file> <output file> <resolution bits>
	
	<input file> =>  original audio file
	<output file> => compressed audio file
	<resolution bits> => bits that represent each audio sample
	
	Exemplification:
		../sndfile-example-bin/wav_quant ../audioFiles/sample01.wav quant5_sample01.wav 5

	

To test Exercise 4:

	../sndfile-example-bin/wav_cmp <input file> <output file>
	
	<input file> =>  original audio file
	<output file> => compressed audio file
	
	Exemplification:
		../sndfile-example-bin/wav_cmp ../audioFiles/sample01.wav quant5_sample01.wav


To test Exercise 5:

	../sndfile-example-bin/wav_effects <input file> <output file> <effectType> [<gain> <delay>]
	
	<input file> => initial wav audio file
	<output file> => final wav audio file
	<effectType>:
		=> 0 (simple Eco)
		=> 1 (multiple Eco)
	<gain> => (gain < 1 || gain > -1)
	<delay> => higher than 0
	
	Exemplification:
		To test simple Eco:
		../sndfile-example-bin/wav_effects ../audioFiles/sample02.wav sample02_simpleEco.wav 0 0.9 10000

	
To test Exercise 6/7:

	../sndfile-example-bin/BitStream <text file> <binary file> <mode type>
	
	<text file> => .txt file
	<binary file> => .out file 
	<mode type>:
		=> -w (to test encoder)
		=> -r (to test decoder)
		
		
	Exemplification:	
		To test the ENCODER:
		../sndfile-example-bin/BitStream write64bits_toBinaryFile.txt binaryFile.out -w
		
		Note: Check the bits written to the binary file using the command: xxd -b <binary file>
		
		To test the DECODER:
		../sndfile-example-bin/BitStream readNbits_fromBinaryFile.txt binaryFile.out -r


To test Exercise 8:

	../sndfile-example-bin/wav_dct <wavFileIn> <wavFileOut> <binaryFile>
	
	<wavFileIn> => original audio file
	<wavFileOut> => final audio file
	<binaryFile> => .out file
	
	Exemplification:	
		../sndfile-example-bin/wav_dct ../audioFiles/sample.wav dct_sample.wav binaryFile.out
		
****
## Grade: 18,1
	
	
	
	
	
	
	
	

