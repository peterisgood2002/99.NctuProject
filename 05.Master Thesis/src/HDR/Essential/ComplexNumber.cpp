/******************************************
FILE
    ComplexNumber.cpp
PURPOSE
    此檔案將會包括一些ComplexNumber的函式 以便於管理以及添加新事物


NOTE
	

AUTHOR
    9757553 顏志晟
*******************************************/
#include "ComplexNumber.h"

ComplexNumber::ComplexNumber( double real , double imagery ) {
	setReal(real);
	setImagery(imagery);
}
void 
ComplexNumber::setReal( double r ) {
	real = r;
}
void 
ComplexNumber::setImagery( double i ) {
	imagery = i;
}
void 
ComplexNumber::setData( double r ,double i ) {
	setReal(r);
	setImagery(i);
}
double 
ComplexNumber::getReal() const{
	return real;
}
double 
ComplexNumber::getImagery() const{
	return imagery;
}
double 
ComplexNumber::getLength() const{
	return real * real + imagery * imagery;
}
/*perform s * R = (s*real + i s* imagery*/
ComplexNumber 
ComplexNumber::operator* ( double scale ) const{
	return ComplexNumber( real*scale ,imagery*scale );
}
ComplexNumber& 
ComplexNumber::operator*= ( double scale ) {
	real *= scale;
	imagery *= scale;

	return *this;
}
ComplexNumber 
ComplexNumber::operator* ( const ComplexNumber& c2 ) const {
	double r = real * c2.real - imagery * c2.imagery;
	double i = real * c2.imagery + imagery * c2.real;

	return ComplexNumber( r , i );
}
ComplexNumber& 
ComplexNumber::operator*= ( const ComplexNumber& c2 ) {
	double r = real;
	double i = imagery;
	real = r * c2.real - i * c2.imagery;
	imagery = r * c2.imagery + i * c2.real;

	return *this;
}
ComplexNumber 
ComplexNumber::operator/ ( double scale ) const {
	return ComplexNumber( real/scale ,imagery/scale );
}
ComplexNumber& 
ComplexNumber::operator/= ( double scale ) {
	real /= scale;
	imagery /= scale;

	return *this;
}
ComplexNumber 
ComplexNumber::operator+ ( const ComplexNumber& r1 ) const {
	return ComplexNumber( real+r1.real ,imagery+r1.imagery );
}
ComplexNumber& 
ComplexNumber::operator+= ( const ComplexNumber& r1) {
	real += r1.real;
	imagery += r1.imagery;

	return *this;
}
ComplexNumber 
ComplexNumber::operator- ( const ComplexNumber& r1 ) const {
	return ComplexNumber( real-r1.real ,imagery-r1.imagery );
}
ComplexNumber& 
ComplexNumber::operator-= ( const ComplexNumber& r1) {
	real -= r1.real;
	imagery -= r1.imagery;

	return *this;
}
ostream& 
operator<< ( ostream& out ,const ComplexNumber& r ) {
	out <<"( " << r.getReal() << "+" << r.getImagery() <<"i )";

	return out;
}