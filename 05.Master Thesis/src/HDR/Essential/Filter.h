/******************************************
FILE
    Filter.h
PURPOSE
    ��x�}��filtering���ʧ@
	�i��۷�²�檺filtering �p�G���ݭn �i�H�b�ۦ�[�JFFT Wavelet��
	��̭����ݩ�static member function

	�ثe�[�J�F1-D FFT


NOTE
	

AUTHOR
    9757553 �C����
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
	 *Parameter�Gthe data need to do filtering
	 *Return�Gthe result of the filtering 
			  
	*/
	static double BoxFiltering( double* * const d ,int row ,int col);
	/**
	 *
	  Perform the basic filtering process to the whole data
	 *Parameter�Gthe data need to do filtering
	 *Return�Gthe result
	*/
	static float* * BasicFiltering( float* * const data ,int row ,int col ,const Kernel* k );

};
#endif