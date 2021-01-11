/******************************************
FILE
    GaussianPyramid.h
PURPOSE
    �x�s�i��Gaussian Pyramid�����O�w


NOTE
	

AUTHOR
    9757553 �C����
*******************************************/

#ifndef _GAUSSIANPYRAMID_
#define _GAUSSIANPYRAMID_

#include <iostream>
#include "Matrix.h"
#include "Filter.h"

std::ostream;


class GaussianPyramid {

public:
	/*
	  Given an data matrix and a level it'll compute the gaussian pyramid
	  If the data cannot support the level ,It'll produce the detaila as possible
	*/
	GaussianPyramid( float* * const data ,int row ,int col ,int level ,const GaussianKernel* k = NULL );
	GaussianPyramid( const Matrix& m ,int level ,const GaussianKernel* k = NULL );
	GaussianPyramid( const GaussianPyramid& g );
	~GaussianPyramid();
	GaussianPyramid& operator= ( const GaussianPyramid& g );
	

public:
	int getLevelAmount() const;
	/* 
	  *Get The level l of the Gaussian Pyramid.
	   If there is no the level , it'll return NULL
      *Parameter�Gthe level wanted to get
	  *Return�G
	     the data or NULL 
	*/
	Matrix* getLevelData( int l ) const;//�ƻs�@�� ���|�ʨ쪫�󤺪����
	const Matrix* getLevelDataWithoutCopy( int l ) const;//���|�ʨ쪫�󤺪���� plz don't use the delete operation
private:
	int tlevel;//total scale of the gaussian pyramid
	Matrix* pyramids;

private:
	/*Perform Gaussian Pyramid Alforithm*/
	void setGaussianPyramid( const Matrix& m ,int level ,const GaussianKernel* k  );
};

ostream& operator<< (ostream& out ,const GaussianPyramid& gp );

#endif