/******************************************
FILE
    Filter.cpp
PURPOSE
    ��x�}��filtering���ʧ@

NOTE
	

AUTHOR
    9757553 �C����
*******************************************/
#include "Filter.h"
#include "FFT.h"

double 
Filter::BoxFiltering( double* * const d ,int row ,int col) {
	double avg = 0;

	for ( int i = 0; i < row; i++ )
		for ( int j = 0; j < col; j++ )
			avg += d[i][j];

	avg /= row * col;

	return avg;
}
/**
 *Perform the basic filtering process to the whole data
 *Parameter�Gthe data need to do filtering
 *Return�Gthe result
*/
float* * 
Filter::BasicFiltering( float* * const data ,int row ,int col ,const Kernel* k ) {
	/*�إ߸��*/
	CNumberMatrix source( row ,col ,data );
	float* * kdata = k->getKernel();
	CNumberMatrix kernel( k->getRowSize() ,k->getColSize() ,kdata );
	dispose( kdata ,k->getRowSize() ,k->getColSize() );

	/*1.zero padding*/
	int r = row + k->getRowSize() - 1;
	int c = col + k->getColSize() - 1;
	source.resetSize( r , c );
	kernel.resetSize( r , c );

	/*2.�নFourier Domain*/
	CNumberMatrix fs = FFT::FFT_2D( source );
	//std::cout << fs;
	CNumberMatrix ks = FFT::FFT_2D( kernel );
	//std::cout << FFT::iFFT_2D(ks);

	/*3.G = K * F*/
	fs *= ks;
	
	//std::cout << fs;
	/*4.��iFFT(G)*/
	CNumberMatrix ifs = FFT::iFFT_2D( fs);
	//std::cout << ifs;
	float* * result = create2DArray( row ,col );

	/*
	  �]��Convolution�����G�ä��O�������O�ڭ̷Q�n��
	  ����������Ƥ~�O�ڭ̷Q�n��
	  
	*/
	int fr = k->getRowSize() / 2;
	int fc = k->getColSize() / 2;
	for ( int i = 0; i < row; i++ ) 
		for ( int j = 0; j < col; j++ ) 
			result[i][j] = ifs[fr + i][fc + j].getReal();

	return result;
}

