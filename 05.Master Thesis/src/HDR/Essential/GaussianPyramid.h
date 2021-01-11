/******************************************
FILE
    GaussianPyramid.h
PURPOSE
    儲存進行Gaussian Pyramid的類別庫


NOTE
	

AUTHOR
    9757553 顏志晟
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
      *Parameter：the level wanted to get
	  *Return：
	     the data or NULL 
	*/
	Matrix* getLevelData( int l ) const;//複製一份 不會動到物件內的資料
	const Matrix* getLevelDataWithoutCopy( int l ) const;//此會動到物件內的資料 plz don't use the delete operation
private:
	int tlevel;//total scale of the gaussian pyramid
	Matrix* pyramids;

private:
	/*Perform Gaussian Pyramid Alforithm*/
	void setGaussianPyramid( const Matrix& m ,int level ,const GaussianKernel* k  );
};

ostream& operator<< (ostream& out ,const GaussianPyramid& gp );

#endif