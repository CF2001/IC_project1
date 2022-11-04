#include <iostream>
#include <cstring>
#include <fstream>

#include "BitStream.h"

using namespace std;


int main(int argc, char *argv[])
{
	
	if (argc != 4)
	{
		cerr << "Usage: " << argv[argc-4] << " <text file> <binary file> <mode type> \n";
		return 1;
	}
	
	char *textFile = argv[argc-3];
	char *binaryFile = argv[argc-2];
	char  *modeType = argv[argc-1];		
	char mode;
	
	if (strcmp(modeType,"-r") == 0)
	{
		mode = 'r';
	}else if (strcmp(modeType,"-w") == 0)
	{	
		mode = 'w';
	}else{
		cerr << "Mode type wrong. Mode type is either \'-r\' or \'-w\'.\n";
		return 1;
	}
	
	
	BitStream bs { binaryFile, mode};

	/* 
	* Mode = 'r' represents the decoder, i.e, convert the contents of 
	*  the binary file to a text file representing only 0s and 1s.
	*
	* -----------------------
	*
	* Mode = 'w' represents the encoder, i.e,  convert a text file 
	* containing only 0s and 1s into the binary equivalent.
	*
	*/
	if (mode == 'r')
	{
		ofstream ofs { textFile };
		
		int bitsToRead;
		cout << "How many bits do you want to read ?\n";
		cin >> bitsToRead;
		
		vector<int> totalBits = bs.read_Nbits(bitsToRead);
		
		//cout << totalBits.size() << endl;
		
		for(size_t i = 0; i < totalBits.size(); i++){
			if(i % 8 == 0 && i != 0)	// separar em 1 byte
			{
				ofs << endl;
			}
			ofs << totalBits[i];
		}
		ofs << endl;

		// -- Read 1 bit 
		/*
		int bit;
		while((bit = bs.read_bit()) != '0')
		{
			ofs << bit << endl;
		}
		*/
		
		
		ofs.close(); 
		
	}else	 
	{
		ifstream ifs { textFile };
		
		// -- write N bits 
		string bits;
		while(ifs >> bits)
		{
			//cout << "File bits: " << bits << endl;
			bs.write_Nbits(bits);
		}
		
		cout << "Size binary file: " << bs.binaryFile_size() << endl;
		
		
		/*
		// -- write 1 bit 
		int val;	// 0 or 1
		while(ifs >> val)
		{
			//cout << "text File: \n";
			//cout << val << endl;
			bs.write_bit(val);
		}
		*/
		
		ifs.close();
	}
	return 1;
}
