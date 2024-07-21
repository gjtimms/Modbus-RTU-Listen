// Function to return single precision float from from 8 hex chars, 4 bytes in 4 value integer array
#include <stdio.h>
#include <cstdint>
#include <string.h>
#include <iostream>
using namespace std;

float  float_from_8hex (int arr[])
{
  uint16_t 	digit;
  //uint16_t  digitraw;
  uint8_t 	ptr;						// Start output at lhs of string    
  uint8_t 	cnt;
  uint8_t   h12;
  std::string	result = "00000000000000000000000000000000";
  uint8_t sign;
  float answer;
  float mult;

  std::string bit4s[16] =
	{ "0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111",
	"1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111"};

  ptr = 0;						// Output pointer
  int j;						// Input pointer
  int h;
  
  for (j=0; j<4; j++) {         // Convert 4 Ascii integers to pairs of Hex values to 0s or 1s in binary result string
     for (h=0; h<2; h++) {
        digit = arr[j];

        if (h == 0) {
           digit = digit/16;            // High hex char in byte
           arr[j] -= digit*16;
        }
        else {
           digit = (digit - digit/16);  // Low hex char in byte
        }

	    // Convert the digit into binary 1s and 0s in the result string
	    if (digit >= 8) {
		   result[ptr] = '1';
	 	   digit -= 8;
	  	}
	    if (digit >= 4) {
		   result[ptr + 1] = '1';
		   digit -= 4;
		}
	    if (digit >= 2) {
		   result[ptr + 2] = '1';
		   digit -= 2;
		}
	    if (digit >= 1) {
		   result[ptr + 3] = '1';
		   digit -= 1;
		}

        ptr += 4;                      //Line up next digit in result
     }
  }

    // ********** Set sign flag **********
    if (result[0] == '0') {
      sign = 0;
      printf("+ve \n");
    }
    else {
      sign = 1;    
      printf("-ve \n");
    }
    
    // ********* Cut out 8 bit exponent from 32 bit string **********
    string exp = result.substr(1, 8);
    uint8_t borrow = 0;	
	ptr = 7;                        //subtract bias of 127 = 1111111

	do {
	   if (exp[ptr] == '0')
	     {
	       if (borrow == 1)
	         {
	           exp[ptr] = '0';   
	         }
	       else
	         {
	           exp[ptr] = '1';
	           borrow = 1;
	         }
	     }
       else
	     if (borrow == 0)
	       {
	         exp[ptr] = '0';
	       }
	     else
	       {
	         exp[ptr] = '1';    
	       }
	   ptr--;
	}
	while (ptr>0);
	
	// ******** Evaluate exponent powers of 2
    int8_t addin = 1;
    int8_t expow = 0;

    for (ptr=7; ptr>0; ptr--)
      {
         if (exp[ptr] == '1') {
            expow += addin; 
         }
         addin *= 2;
      }
      if ((expow != 0) && (exp[0] == '0')) {
         expow -= 128; 
      }

    mult = 1;
    while (expow > 0)                // Calculate +ve powers of 2 multiplication factor
    {
       mult *= 2;
       expow--;
    }
    while (expow < 0)                // Calculate -ve powers of 2 multiplication factor
    {
       mult *= 0.5;
       expow++;
    }
    if (sign == 1) {mult = -mult;}

    // ********* Cut out 23 bit fraction from 32 bit string **********
    string fract = result.substr(9, 23);
     
    // ********* construct answer ************
    answer = 1;
    float fraction = 0.5;
    for (ptr=0; ptr<23; ptr++)  {
       if (fract[ptr] == '1')  {
          answer += fraction;
       }
       //ptr++;
       fraction *= 0.5;
    }

	return mult * answer;
  }
