/******************************************
FILE
    HDRVideo.h
PURPOSE
    this define the tone mapping of my thesis.
	I hope it'll work in order to graduate.

NOTE
	this is for HDR Video
	

AUTHOR
    9757553 ÃC§ÓÑÔ
*******************************************/

#ifndef _HDRVIDEO_
#define _HDRVIDEO_

#include "ThesisToneMapping.h"
#include <string>

using std::string;

class HDRVideo {

public:
	/*
	  Constructor
	*/
	HDRVideo( const string& dname );

public:
	/*
	  Perform the tone mapping with Temporal effect
	  *Parameter: The destination directory of the result
	*/
	void performWithTemporal( const string& dstdir );
	/*
	  Perform the tone mapping without Temporal effect
	  *Parameter: The destination directory of the result
	*/
	void performWithoutTemporal( const string& dstdir );

private:
	string dirname;

};
#endif