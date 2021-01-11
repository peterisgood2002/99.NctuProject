/******************************************
FILE
    Filter.h
PURPOSE
    對矩陣做filtering的動作
	進行相當簡單的filtering 如果有需要 可以在自行加入FFT Wavelet等
	其裡面都屬於static member function

	目前加入了1-D FFT


NOTE
	

AUTHOR
    9757553 顏志晟
*******************************************/

#ifndef _FILTER_
#define _FILTER_

#include "Kernel.h"


/*=======================
Filter
=========================*/
class Filter {

public:
	/*
	 *Do box filtering
	  It is part of the Filtering process
	 *Parameter：the data need to do filtering
	 *Return：the result of the filtering 
			  
	*/
	static double BoxFiltering( double* * const d ,int row ,int col);
	/**
	 *
	  Perform the basic filtering process to the whole data
	 *Parameter：the data need to do filtering
	 *Return：the result
	*/
	static float* * BasicFiltering( float* * const data ,int row ,int col ,const Kernel* k );

};
#endif