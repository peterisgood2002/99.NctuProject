/***************************************************************************
    Name
        Histogram.h
    Purpose
        to declare a class called Histogrm
		It'll perform some histogram function include Histogram Equalization
    Author
        92306026 顏志晟

****************************************************************************/

#ifndef _HISTOGRAM_
#define _HISTOGRAM_

#include <iostream>

using std::ostream;

class Histogram {

public:
	//Constructor
	/**
	 *According to d ,calculate the density function of d
	  it'll create a 
	 *Parameter：
		*the data you want to calculate histogram from 
		*number of bins you would wanna seperate 
		
	*/
	Histogram( double* *const d ,int row ,int col ,int n = 256 );
	///**
	// *given an  array and its max and min of X axis create an Histogram
	//*/
	Histogram(const int* d ,int n ,double xmax ,double xmin );
	//Copy Constuctor
	Histogram( const Histogram& h );
	//operator assignment
	Histogram& operator= (const Histogram& h );
	//Destructor
	~Histogram();

	//取得相關資訊
	/*
	  Get the value of  bin i 
	  if i is excess numberofbins it'll return NOTANUMBER
	*/
	int getBinValue( int i ) const;
	int getNumbersOfBin() const;
	//get total amount of all Bins
	int getTotalSamples() const;
	//get the range size of the bin
	double getBinRangeSize() const;
	//取得所有bin Amount中超過Ceiling的個數
	int getTotalAmountExcess( double ceiling ) const;
	/**
	 *According to value return the probability
	 *Parameter：value
	 *Return：Probability 
			  if the value is not within the range it'll return 0
    */
	double getProbability(double value ) const;
	/*
	  get the x coordinate of bin i
	  if i is excess numberofbins it'll return NOTANUMBER
	*/
	double getXOfBin(int i ) const;
	//---------------
	
	//get the cumulative distribution function of this histogram
	Histogram getCumulativeDistribution();

	//設定相關資訊
	/*
	 it'll set the value of bins[i]
	 if i is bigger than numberofbins it'll do nothing
	*/
	void setBinValue(int i ,int value);
	/**
	 according to d ,set this histogram to another histogram
	 Parameter：
	*/
	void setHistogram( double*  *const d ,int row ,int col );
	
	/**
	  truncate bin values to ceiling if the bin values is bigger than ceiling
	  Parameter：ceiling value
	  Return：the amount of total bins to be truncated
	*/
	double truncateExcess( double ceiling );

	friend ostream& operator<<( ostream& out ,const Histogram& h );

private:
	static const int NOTANUMBER;
	
private:
	int* bins;
	int numberofbins;
	int totalsamples;//全部sample的量
	//X軸的最大值與最小值
	double maxXvalue;
	double minXvalue;

private:
	/**
	 *根據value找出他應該屬於哪一個bin
	  如果超過Bin的範圍 他會根據範圍Assign最大值或最小值
	*/
	int binBelongTo( double value ) const;
};

//方便Debug 輸出histogram用
ostream& operator<<( ostream& out ,const Histogram& h );

#endif