/***************************************************************************
    Name
        Histogram.h
    Purpose
        to declare a class called Histogrm
		It'll perform some histogram function include Histogram Equalization
    Author
        92306026 顏志晟

****************************************************************************/

#include <iostream>
#include <cmath>
#include "Histogram.h"

using std::cerr;

const int Histogram::NOTANUMBER = -1;

//Constructor
/*
 *According to d ,calculate the density function of d
  it'll create a 
 *Parameter：
	*the data you want to calculate histogram from 
	*number of bins you would wanna seperate
	
*/
Histogram::Histogram( double* * const d ,int row ,int col ,int n) {
	numberofbins = n;
	totalsamples = row * col;
	bins = new int[ numberofbins ];

	for ( int i = 0; i < numberofbins; i++ )
		bins[i] = 0;

	//記得打開
	setHistogram( d , row ,col );
}
/**
 *given an  array and its max and min of X axis create an Histogram
*/
Histogram::Histogram(const int* d ,int n ,double xmax ,double xmin ) {
	numberofbins = n;
	maxXvalue = xmax;
	minXvalue = xmin;

	totalsamples = 0;
	bins = new int[ numberofbins ];

	for ( int i = 0; i < numberofbins; i++ ) {
		bins[i] = d[i];
		totalsamples += d[i];
	}

	

}
//Copy Constuctor
Histogram::Histogram( const Histogram& h ) {

	numberofbins = h.numberofbins;
	maxXvalue = h.maxXvalue;
	minXvalue = h.minXvalue;
	totalsamples = h.totalsamples;

	bins = new int [ numberofbins ];

	for ( int i = 0; i < numberofbins; i++ ) 
		bins[i] = h.bins[i];

	

}
//operator assignment
Histogram& 
Histogram::operator= (const Histogram& h ) {
	if ( this == &h ) return *this;

	numberofbins = h.numberofbins;
	maxXvalue = h.maxXvalue;
	minXvalue = h.minXvalue;
	totalsamples = h.totalsamples;

	delete bins;

	bins = new int [ numberofbins ];

	for ( int i = 0; i < numberofbins; i++ ) 
		bins[i] = h.bins[i];

	return *this;
}
//Destructor
Histogram::~Histogram() {
	delete [] bins;
}

//取得相關資訊
int 
Histogram::getBinValue( int i ) const{
	if ( i > numberofbins ) {
		cerr << "access deny about the histogram ,it'll return NOTANUMBER value\n";
		return NOTANUMBER;
	}
	return bins[i];
}
int 
Histogram::getNumbersOfBin() const{
	return numberofbins;
}
//取得所有Bin中個數總合
int 
Histogram::getTotalSamples() const {
	return totalsamples;
}
//取得一個Bin的範圍
double
Histogram::getBinRangeSize() const {
	return ( maxXvalue - minXvalue ) / numberofbins;
}
//取得所有bin Amount中超過Ceiling的個數
int 
Histogram::getTotalAmountExcess( double ceiling ) const {
	int total = 0;

	for ( int i = 0; i < numberofbins; i++ ) {
		int binamount = getBinValue(i);
		if ( binamount > ceiling ) 
			total += int ( floor( binamount - ceiling ) );
	}

	return total;
}
/**
 *According to value return the probability
 *Parameter：value
 *Return：Probability 
		  if the value is not within the range it'll return 0
*/
double 
Histogram::getProbability(double value ) const {
	int i = binBelongTo(value);

	return (double)getBinValue(i) / getTotalSamples();

}
/*
  get the x coordinate of bin i
  if i is excess numberofbins it'll return NOTANUMBER
*/
double 
Histogram::getXOfBin(int i ) const{
	if ( i > numberofbins ) {
		cerr << "access deny about the histogram ,it'll return NOTANUMBER value\n";
		return NOTANUMBER;
	}

	return i * numberofbins + minXvalue;
}
/**
 *根據value找出他應該屬於哪一個bin
  如果超過Bin的範圍 他會根據範圍Assign最大值或最小值
*/
int 
Histogram::binBelongTo( double value ) const {
	if ( value > maxXvalue ) {
		return numberofbins - 1;
	}
	else if ( value < minXvalue ) {
		return 0;
	}
	return (int) floor( ( value - minXvalue ) / getBinRangeSize() );
}
//get the cumulative distribution function of this histogram
Histogram 
Histogram::getCumulativeDistribution() {
	
	int* cdf = new int [ numberofbins ];

	cdf[0] = bins[0];
	for ( int i = 1; i < numberofbins; i++ )
		cdf[i] = cdf[i-1] + bins[i];

	Histogram h(cdf ,numberofbins ,maxXvalue ,minXvalue );

	delete cdf;
	return h;

}

//設定相關資訊
/*
 it'll set the value of bins[i]
 if i is bigger than numberofbins it'll do nothing
*/
void 
Histogram::setBinValue(int i ,int value) {
	if ( i > numberofbins ) {
		cerr << "access deny about the histogram ,it'll do nothing about setting value\n";
		return;
	}

	int trimmings =  value - bins[i];
	bins[i] = value;
	totalsamples -= trimmings;//少了trimmings個sample
}
/**
  truncate bin values to ceiling if the bin values is bigger than ceiling
  Parameter：ceiling value
  Return：the amount of total bins to be truncated
*/
double
Histogram::truncateExcess( double ceiling ) {
	double trimmings = 0;
	for ( int i = 0; i < numberofbins; i++ ) {
		int amount = getBinValue(i);
		if (amount > ceiling ) {
			trimmings += amount - ceiling;
			setBinValue(i ,int( ceiling) );//看到只剩下ceiling
		}
	}

	return trimmings;
}

/*使用CLR的東西*/
#using <mscorlib.dll>
using System::Double;

void 
Histogram::setHistogram( double*  *const d ,int row ,int col ) {
	//設定起始值
	maxXvalue = Double::MinValue;
	minXvalue = Double::MaxValue;

	
	for ( int i = 0; i < row; i++ ) {
		for ( int j = 0; j < col; j++ ) {
			if ( d[i][j] < minXvalue ) 
				minXvalue = d[i][j];
			if ( d[i][j] > maxXvalue )
				maxXvalue = d[i][j];
		}
	}

	//開始計數
	double range = maxXvalue - minXvalue;
	for ( int i = 0; i < row; i++ ) {
		for ( int j = 0; j < col; j++ ) {
			double data = d[i][j];
			int index =  binBelongTo( data );

			if ( index < 0 )
				bins[0]++;
			else if ( index >= numberofbins )
				bins[ numberofbins-1 ]++;
			else bins[ index ]++;
		}
	}
}

ostream& operator<<( ostream& out ,const Histogram& h ) {
	out << "各個bin的值為\n";
	for ( int i = 0; i < h.numberofbins; i++ )
		out << h.bins[i] <<" ";
	out << "\n";

	return out;
}
