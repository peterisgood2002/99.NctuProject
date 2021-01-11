/***************************************************************************
    Name
        Histogram.h
    Purpose
        to declare a class called Histogrm
		It'll perform some histogram function include Histogram Equalization
    Author
        92306026 �C����

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
 *Parameter�G
	*the data you want to calculate histogram from 
	*number of bins you would wanna seperate
	
*/
Histogram::Histogram( double* * const d ,int row ,int col ,int n) {
	numberofbins = n;
	totalsamples = row * col;
	bins = new int[ numberofbins ];

	for ( int i = 0; i < numberofbins; i++ )
		bins[i] = 0;

	//�O�o���}
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

//���o������T
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
//���o�Ҧ�Bin���Ӽ��`�X
int 
Histogram::getTotalSamples() const {
	return totalsamples;
}
//���o�@��Bin���d��
double
Histogram::getBinRangeSize() const {
	return ( maxXvalue - minXvalue ) / numberofbins;
}
//���o�Ҧ�bin Amount���W�LCeiling���Ӽ�
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
 *Parameter�Gvalue
 *Return�GProbability 
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
 *�ھ�value��X�L�����ݩ���@��bin
  �p�G�W�LBin���d�� �L�|�ھڽd��Assign�̤j�ȩγ̤p��
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

//�]�w������T
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
	totalsamples -= trimmings;//�֤Ftrimmings��sample
}
/**
  truncate bin values to ceiling if the bin values is bigger than ceiling
  Parameter�Gceiling value
  Return�Gthe amount of total bins to be truncated
*/
double
Histogram::truncateExcess( double ceiling ) {
	double trimmings = 0;
	for ( int i = 0; i < numberofbins; i++ ) {
		int amount = getBinValue(i);
		if (amount > ceiling ) {
			trimmings += amount - ceiling;
			setBinValue(i ,int( ceiling) );//�ݨ�u�ѤUceiling
		}
	}

	return trimmings;
}

/*�ϥ�CLR���F��*/
#using <mscorlib.dll>
using System::Double;

void 
Histogram::setHistogram( double*  *const d ,int row ,int col ) {
	//�]�w�_�l��
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

	//�}�l�p��
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
	out << "�U��bin���Ȭ�\n";
	for ( int i = 0; i < h.numberofbins; i++ )
		out << h.bins[i] <<" ";
	out << "\n";

	return out;
}
