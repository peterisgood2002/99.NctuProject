/******************************************
FILE
    GaussianKernel.cpp
PURPOSE
    �ΨӲ��ͤ@��Gaussian Kernel��

NOTE
	

AUTHOR
    9757553 �C����
*******************************************/

#include "Kernel.h"
#include <cmath>

GaussianKernel::GaussianKernel( int row,int col , double s ) : Kernel(row ,col ) {
	
	sigma = s;

	constructKernel();
}

void 
GaussianKernel::constructKernel() {
	/*
	  -------------->x
	  |
	  |
	  |
	  |
	  |
	  v
	  y

	  Gaussian Function = A * e ^ - (  D^2(x ,y ) / 2 * sigma )
	*/

	int col = getColSize();
	int row = getRowSize();
	int cx = col / 2;
	int cy = row / 2;
	//double amplitude = 1.0 / ( 2*3.1415926 * sigma * sigma );
	double total = 0;
	for ( int y = 0; y < row; y++ ) {
		for ( int x = 0; x < col; x++ ) {
			double d = pow( double(x-cx) ,2 ) + pow ( double(y-cy) ,2 );
			d /= (2 * sigma * sigma);

			//double result = amplitude * exp( -d );
			double result = exp(-d);
			total += result;
			kernel.setData(y ,x , result );

		}
	}

	/*normalize���G*/
	kernel.scale( 1/total );

}
