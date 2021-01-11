/******************************************
FILE
    SaliencyMap.h
PURPOSE
    this class will do the Salience map operation in
	"a model of Saliecny-Based Visual Attention for Rapid Scence Analysis"

NOTE
	It first do gaussian pyramids to image and separate an image to colors ,intensity and orientation.
	Compute each Feature map and combine to Conspicuity Map
	Then perform linear combinations to obtain Salience Map

	This class is free from memory leakage , the maintenace shouldn't need to worry about it

AUTHOR
    9757553 �C����
*******************************************/

#ifndef _SALIENCYMAP_
#define _SALIENCYMAP_

#include "Essential/Filter.h"
#include "Essential/Matrix.h"
#include "Essential/GaussianPyramid.h"
#include "Essential/Kernel.h"
#include <string>


using std::string;
#using <System.Drawing.dll>
using namespace System::Drawing;

class SaliencyMap {  

public:
	/*It is only two possibility. A file with image format and a file with HDR format*/
	SaliencyMap( const string& fname );
	/*The Big Three*/
	/*It only copy the color. It doesn't copy the Map needed plz take care of this.*/
	SaliencyMap (const SaliencyMap& v );
	virtual ~SaliencyMap();
	SaliencyMap& operator= (const SaliencyMap& v );
	
public:
	/*Perform the center-surround differences and normalization to obtain feature map*/
	void doIntensityFeatureMap( const GaussianKernel* k = NULL );
	void doColorfeatureMap( const GaussianKernel* k = NULL );
	void doOrientationFeatureMap( const GaussianKernel* k = NULL );
	/*Perform the across-scale combination and normalization to obtain Conspicuity map*/
	Matrix* getIConspicuityMap();//Conspicuity map of intensity
	Matrix* getCConspicuityMap();//Conspicuity map of color
	Matrix* getOConspicuityMap();//Conspicuity map of orientation

	/*
	  Perform the salience map algorithm to get the salience map
	  It'll perform normalized in the final step
	*/
	virtual Matrix* getSaliencyMap();
public:
	/*Display map in order to debug*/
	static void displayMap( const Matrix& m ,const string fname);

protected:
	/*������Ƶ��c */
	struct RGB {

	private:
		double R;
		double G;
		double B;
	public:
		friend class SaliencyMap;
	};
protected:
	RGB* * rgb;//�x�s��Bitmap�άO
	int row;
	int col;
	bool ishdr;
protected:
	/*�U��Feature map*/
	Matrix* rgfeaturemap;//Color��Feature map
	Matrix* byfeaturemap;
	int cfmsize;//color feature map size
	Matrix* intensityfeaturemap;
	int ifmsize;
	/*
	 �����ƦC�O�|�Ӥ@�դ��O�|�O0 45 90 135�o�˨̦��ƤU�h
	 ��k�O �p�U

	  c2		    c3				c4
	  ----------------------------------------------
	  |				|				|				|
	  |s5	s6		|s6		s7		|s7		s8		|
	  ----------------------------------------------
	  |		|		|		|		|		|		|
      ----------------------------------------------
	*/
	Matrix* orientationfeaturemap;
	int ofmsize;

	/*�U��Conspicuity Map�H���n���⪺�·�*/
	Matrix* intensitycm;
    Matrix* colorcm;
	Matrix* orientationcm;

	GaussianPyramid* GI;//Gaussian Pyramid of the intensity


protected:
	void setRGB( const string& fname );
	void init( int row , int col );
	void dispose();
	void makeIntensityPyramid( const GaussianKernel* k );
	Matrix getOriginalSaliencyMap();

protected:
	/*
	 *Interpolation the coarse matrix m into the fine matrix row * col.
	  It'll use the bilinear
	 *Parameter�Gthe coarse matrix information and the finelevel's row and col
	 *Return�Gnew matrix
	*/
	static Matrix* interpolationFromCoarseToFine(const Matrix& cm ,int row ,int col ); 
	/*
	 *It'll subsample the matrix m to row * col
	 *Parameter�Gthe matrix needed to subsample
	 *Return�Gthe subsample result
	*/
	static Matrix* getSubSample( const Matrix& cm ,int row ,int col );
	/*
	 *Perform the Normalization operator in the paper section 2.2.
	  It'll have the following instructions
	  (1) normalizing the values to a fixed range [ 0 .. M ]
	  (2) finding the maximum pixel of the image and the average of all it's local maximum u
	  (3) multiplying the map by ( M - u ) ^2
     *Parameter�G
	     This doesn't manipulate the matrix's array.
		 It only perform one matrix. Please take care of this!
	     the matrix need to do normalize operation.
	     Be careful that it'll change the matrix
	*/
	static void performNormalization( Matrix& m );
    /*
	 *Take the absolute value of the matrix
	*/
	static void ABS( Matrix& m );
	/*
	 *use the GaborKernel k to perform Gabor filtering on m 
	*/
	static Matrix performGaborFiltering( const Matrix* m , const GaborKernel* k ); 
	/*
	 *�N�Ƕi�Ӫ�Matrix�i��Normalize���ʧ@
	  �N���mapping��[-1..1]
	*/
	static void  normalize( Matrix& m );



};
#endif