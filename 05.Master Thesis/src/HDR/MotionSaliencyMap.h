/******************************************
FILE
    MotionSaliencyMap.h
PURPOSE
    this class add the motion information to the salience map
	according to "Realistic Avatar Eye and Head Animation Using a Neurobiological Model of Visual Attention"

NOTE
	We use the fast implementation uses the intensity as input.
	We will perfer the reader to infer the "RegSailence.h" in 
	     http://ilab.usc.edu/toolkit/doc-nosrc/html/db/d6c/classRegSaliency.html#z156_17
    We also use the static variable to store the previous motion information.

AUTHOR
    9757553 顏志晟
*******************************************/

#ifndef _MOTIONSALIENCYMAP_
#define _MOTIONSALIENCYMAP_

#include "SaliencyMap.h"
class MotionSaliencyMap : public SaliencyMap {

public:
	/*
	  Contructor
	*/
	MotionSaliencyMap(const string& fname ,bool islastFrame = false);//是不是最後的Frame 以便於刪除static data member
	/*
	  Copy Constrctor as the SaliencyMap
	*/
	MotionSaliencyMap (const MotionSaliencyMap& v );
	MotionSaliencyMap& operator= (const MotionSaliencyMap& v );

	MotionSaliencyMap (const SaliencyMap & v );

public:
	/*
	  Destructor
	*/
	~MotionSaliencyMap();

public:
	/*Perform the center-surround differences and normalization to obtain feature map*/
	void doFlickerFeatureMap( const GaussianKernel* k = NULL );
	void doMotionFeatureMap( const GaussianKernel* k = NULL );
	/*Perform the across-scale combination and normalization to obtain Conspicuity map*/
	Matrix* getFConspicuityMap();//Conspicuity map of Flicker
	Matrix* getMConspicuityMap();//Conspicuity map of Motion

public:
	/*Perform the salience map algorithm to get the salience map*/
	Matrix* getSaliencyMap();

private:
	bool lastframe;
	/*Flicker Feature map*/
	int ffmsize;
	Matrix* flickerfeaturemap;
	
	/*Motion Feature map*/
	int mfmsize;
	Matrix* motionfeaturemap;
	

	/*Conspicuity map:It is not an array*/
	Matrix* flickercm;
	Matrix* motioncm;

private:
	void init();
	void dispose();

private:
	/*Build the shiftd pyramids according to the note above*/
	Matrix* buildShiftedPyr();
	Matrix* buildReichardtModel( const Matrix* SP );

/*static data member*/
private:
	static GaussianPyramid* preGI;//用來給Flicker Map用的
	static Matrix* preSP;//previous Shifted Pyramids it's an array
};

#endif