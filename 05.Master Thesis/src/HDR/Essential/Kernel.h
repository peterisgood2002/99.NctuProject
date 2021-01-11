/******************************************
FILE
    Kernel.h
PURPOSE
    this define some kernel in order to perform filtering

NOTE
	If you wanna create a new kernel ,please inherit the Kernel class and implement your own constructKernel()
	

AUTHOR
    9757553 ÃC§ÓÑÔ
*******************************************/

#ifndef _KERNEL_
#define _KERNEL_


#include "Matrix.h"
#include <iostream>

using std::ostream;

/*=======================
    define Kernel 
=========================*/
class Kernel {
public:
	/*Create a Kernel which member is all 1*/
	Kernel(int row ,int col ,double data = 1 );
public:
	int getRowSize() const;
	int getColSize() const;
	/*Get the kernel it'll replicate the data */
	float* * getKernel() const;
protected:
	Matrix kernel;

public:
	friend ostream& operator<< (ostream& out ,const Kernel& v );

protected:
	virtual void constructKernel() = 0;
};
ostream& operator<< (ostream& out ,const Kernel& v );

/*===========================
	define a Box Kernel
========================*/
class BoxKernel : public Kernel {

public:
	/*Create a Box Kernel*/
	BoxKernel( int row ,int col ) : Kernel( row ,col ,1.0/(row*col) ){};
protected:
	void constructKernel(){};
};

/*===========================
	define a Gaussian Kernel
========================*/
class GaussianKernel : public Kernel {

public:
	/*Consturctor*/
	/*
	  given  sigma ,it'll give you a gaussian kernel
	*/
	GaussianKernel( int row = 31,int col = 31 , double sigma = 5);
	
private:
	double sigma;

protected:
	void constructKernel();

};

/*======================
    define a Gabor Kernel 
	It'll according to the parameter obtain a Gabor kernel 
    http://en.wikipedia.org/wiki/Gabor_filter
========================*/
class GaborKernel : public Kernel {

public:
	/*
	 *Constructor
	 *Paramteter
	  theta¡Gthe orientation of the normal to the parallel stripes
	  row¡Gthe row of the kernel 
	  col¡Gthe column of the kernel
	  lamda¡Gthe wavelength of the cosine factor
	  psi¡Gphase offset
	  sigma¡Gsigma of the Gaussian envelope
	  gamma¡Gspatial aspect ratio
	*/
	GaborKernel( double theta ,int row = 7 ,int col = 7 ,double lamda = 7 ,double psi = 0 ,double sigma = 2.333 ,double gamma = 1);

private:
	double radian;//the orientation of the normal to the parallel stripes in radian
	double lamda;//the wavelength of the cosine factor
	double psi;//phase offset
	double sigma;//sigma of the Gaussian envelope
	double gamma;//spatial aspect ratio

protected:
	void constructKernel();
};

#endif
