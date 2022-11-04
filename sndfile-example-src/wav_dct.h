#include <iostream>

class wavDCT {

	public: 

	/* Converts a binary value to decimal with sign */
	/* This function applies the sum of the weights of each bit to 
	*  determine the corresponding decimal value. To find out whether it is 
	*  positive or negative, it checks whether the most significant bit is 0 or 1 
	*/
	int binaryToSignedDecimal(const char binary[], int significantBits) 
	{
	    int power = pow(2,significantBits-1);
	    int signedDecimal = 0;

	    for (int i = 0; i < significantBits; i++)
	    {
	    	// Checks if the first bit is positive or negative (0:positive, 1:negative)
		if ( i==0 && binary[i]!='0')
		{
		    signedDecimal = power * -1;
		}
		else 
		{
		    // Applies the sum of the weight of each bit
		    signedDecimal += (binary[i]-'0') * power; // The -'0': convert to integer
		}
		power = power / 2;	// weight of the corresponding bit
	    }

	    return signedDecimal;
	}

}; 
