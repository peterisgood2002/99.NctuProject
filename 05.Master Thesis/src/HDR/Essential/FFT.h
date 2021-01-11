/******************************************
FILE
    FFT.h
PURPOSE
    �i�榳����Fourier Transformation�������B��
	���{���N�|�]�A1-D FFT 2-D FFT �H��1-D iFFT 2-D iFFT�������禡
	


NOTE
	

AUTHOR
    9757553 �C����
*******************************************/

#ifndef _FFT_
#define _FFT_

#include <iostream>
#include "ComplexNumber.h"

using std::ostream;


//typedef ComplexNumber* CNumberArray;//�ΨӪ��2D��Vector �H��K�ϥ� ��O²�g�a ���аO��o�ä��O���Ъ��覡
//typedef CNumberArray* CNumberMatrix;

class FFT {
public:
	static enum Mode { FORWARD = -1 ,INVERSE = 1 };
	/*
	 *Given an array ,this function will perform 1-D Fast Fourier Transformation
	  this algorithm will include the inverse Fast Fourier Transformation for convenience
	 *Parameter�Gan data array and does this function perform iFFT
	 *Return�GFFT array
	          if iFFT is performed ,it'll return the real value of iFFT * M
	*/
	static CNumberArray FFT_1D( const CNumberArray&  f ,Mode m = FORWARD);
	/*Perform a Radix-2 FFT algorithm
	  Parameter�Git'll treat the matrix as a 1-D element 
	             which perform Radix-2 FFT according to the row element
				 plz ensure the row size is 2^n or it'll abort the program
	  Return�Gthe result of FFT
	*/
	//static CNumberArray Radix_2_FFT_1D( const CNumberMatrix& d  ,Mode m);
	/*Perform the inverse Fourier transformation of the array*/
	static CNumberArray iFFT_1D( const CNumberArray&  F );
	/*Given an 2D array ,this function will perform 2-D Fast Fourier Transformation
	  this algorithm will include the inverse Fast Fourier Transformation for convenience
	 *Parameter�Gan 2D array and does this function perform iFFT
	 *Return�GFFT array
	           if iFFT is performed ,it'll return the real value of iFFT * M
	 */
	static CNumberMatrix FFT_2D( const CNumberMatrix& f ,Mode m = FORWARD );
	static CNumberMatrix iFFT_2D( const CNumberMatrix& F  );
	
	
};


#endif
