/******************************************
FILE
    GarborKernel.cpp
PURPOSE
    用來產生一個Gaussian Kernel的

NOTE
	

AUTHOR
    9757553 顏志晟
*******************************************/

#include "Kernel.h"

#using <mscorlib.dll>
using System::Math;
/*
 *Constructor
 *Paramteter
  theta：the orientation of the normal to the parallel stripes
  row：the row of the kernel 
  col：the column of the kernel
  lamda：the wavelength of the cosine facto
  w：phase offset
  sigma：sigma of the Gaussian envelope
  gamma：spatial aspect ratio
*/
GaborKernel::GaborKernel( double t ,int row ,int col ,double l ,double w ,double s ,double g ) : Kernel( row ,col ){
	
	radian = Math::PI / 180 * t;//轉成Radian儲存
	lamda = l;//the wavelength of the cosine factor
	psi = Math::PI / 180 * w;//phase offset
	sigma = s;//sigma of the Gaussian envelope
	gamma = g;//spatial aspect ratio
}
void 
GaborKernel::constructKernel() {
	
	/*
	  Kernel
					x'^2 + r2*y'2					x'
	  g = exp (-  ------------------- ) * cos( 2PI----- + psi )
	                    2 * sigma^2					l
	*/
	int row = getRowSize();
	int col = getColSize();

	int cy = row / 2;
	int cx = col / 2;

	for ( int y = 0; y < row; y++ ) {
		for ( int x = 0; x < col; x++ ) {
			double xproun = (x - cx)* Math::Cos(radian) + (y - cy) * Math::Sin(radian);
			double yproun = -(x - cx) * Math::Cos(radian) + (y - cy) * Math::Cos(radian);
			double result = - ( xproun * xproun + gamma * gamma * yproun * yproun) / ( 2 * sigma * sigma );
			result = Math::Exp( result );
			result *= Math::Cos( 2 * Math::PI * xproun / lamda + psi );

			kernel.setData( y ,x ,result );
		}
	}
}