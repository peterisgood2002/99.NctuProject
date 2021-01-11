/******************************************
FILE
    ThesisToneMapping.h
PURPOSE
    this define the tone mapping of my thesis.
	I hope it'll work in order to graduate.

NOTE
	
	

AUTHOR
    9757553 ÃC§ÓÑÔ
*******************************************/

#ifndef _THESIS_
#define _THESIS_

#include "SaliencyMap.h"
#include "./Loader/Loader.h"
#include "./Essential/Matrix.h"
#include <string>

using std::string;

class ThesisToneMapping {

public:
	/*
	  Constructor
	  @Parameter fname: the file name
	  @Parameter isvideo: this tone mapping operation is for video or not
	  @Parameter islastframe: is this file last frame
	*/
	ThesisToneMapping( const string& fname ,bool isvideo = false ,bool islastframe = false);
	ThesisToneMapping( ThesisToneMapping& t );
	ThesisToneMapping& operator= (const ThesisToneMapping& v );
	~ThesisToneMapping();

public:
	/*
	 It 'll perform my tone mapping It'll change the value of the HDRLoader 
	 *Parameter: when performing in HDR video ,it should need to have ratio to represent the attentional difference
	*/
	void perform( const Matrix* ratio = NULL );
	Matrix* getAttentionMap();
	void saveResult( string fname );

private:
	string filename;
	SaliencyMap* saliencemap;
	Loader* image;
	bool isforvideo;
	Matrix* ld;
	float* *ap;//the Attention Part of the images It'll calculate during performing

private:
	/*
	 *It'll get the scale value which the contrast need to add
	*/
	float* * getAttentionEffect();
	/*normalize the matrix to [0 1]*/
	Matrix* normalize( Matrix* m ,double max ,double min);
	/*apply gamma correction*/
	void gamma( Matrix* r ,Matrix* g ,Matrix* b );
	/*apply gamma correction and normalize*/
	void applyGammaAndNormalize( Matrix* r ,Matrix* g ,Matrix* b ,const Matrix* l);



};
#endif