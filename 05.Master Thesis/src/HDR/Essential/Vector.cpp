/***************************************************************************
    Name
        Vetcor.h
    Purpose
        to implement a class called Vector
    Author
        92306026 ÃC§ÓÑÔ

****************************************************************************/
#include "Vector.h"
#include <iostream>

extern "C" {
    #include <math.h>
}

using std::cout;
using std::endl;
using std::ostream;
/*Constructor*/
//initialize the vector's dimensionality to d and all cooedinate is 0
Vector::Vector( int d ) {
    if ( d<=0) {
        cout<<"you may want to have a negative dimensional vector.";
        cout<<"this's a mistake\n"; exit(-1);
    }

    dimension = d;
    coordinate = new double[dimension];
    for (int i=0; i<dimension; i++)
        coordinate[i] = 0.0;
}

//initialize a vector with the given array
Vector::Vector (int d ,const double* array) {
    if ( d<=0) {
        cout<<"you may want to have a negative dimensional vector.";
        cout<<"this's a mistake\n"; exit(-1);
    }

    dimension = d;
    coordinate = new double [ dimension];
    for (int i=0; i<dimension; i++)
        coordinate[i] = array[i];
}

/*the "Big Three"*/
//copy all v's element to vector
Vector::Vector (const Vector& v ) {
    dimension = v.dimension;
    coordinate = new double [dimension];

    for (int i=0; i<dimension; i++)
        coordinate[i] =v.coordinate[i];
}

//delete the vector
Vector::~Vector() {
    delete [] coordinate;
}

//assign a vector to another vector
Vector& Vector::operator= (const Vector& v ) {
    if (this == &v ) return *this;

	delete [] coordinate;//delete original array

    dimension = v.dimension;
    coordinate = new double [dimension];

    for (int i=0; i<dimension; i++)
        coordinate[i] =v.coordinate[i];

    return *this;
}

/*operator[]*/
//it'll access the i'th element in the vector.
//If it out of range then return the first element of vector
double& 
Vector::operator[] ( int i) const{
    if ( i>=0 && i <dimension)
        return coordinate[i];
    cout<<"it out of range return the first element of vector\n";
    return coordinate[0];
}

/*Accessing/Changing dimensionality*/
/**
 *Get the data of a vector
 */
double* 
Vector::getData() const{
	double* d = new double[dimension];
	for ( int i = 0; i < dimension; i++ )
		d[i] = coordinate[i];

	return d;
}
/*set Dimension to Vector
  if the new one is bigger than previous one
  the new coordinate are appended to the old vector
  and their value are all set to 0
  else it'll truncate extra coordinate
*/
bool 
Vector::setDimension(int d) {
    //append or truncate
    if ( d >dimension ) {
        int dim = dimension;//store the old dimension
        dimension =d;
        double* tmp = coordinate;//store the previous vector
        //create another vector array
        coordinate = new double[dimension];
        if (coordinate ==NULL) {
            cout<<"Memory Allocation fault\n";
            return false;
        }

        //copy the old one to new one
        int i = 0;
        for (i=0; i<dim; i++)
            coordinate[i] = tmp[i];
        while( i<dimension )
            coordinate[i++] = 0.0;
        delete tmp;//delete the old vector
    }
    else if (d < dimension ) {
        double* tmp = &coordinate[d];//pointer to d'th element
        delete [] tmp;//delete all of element following d'th element
        dimension = d;
    }

    return true;
}

/*Arithemetic Operators*/
//negate the entire coordinates of vector
Vector Vector::operator-() const {
    double* array= new double [dimension];

    for (int i=0; i<dimension; i++ )
        array[i] = -coordinate[i];
    Vector tmp(dimension ,array);
    delete [] array;

    return tmp;
}

//add  the individual coordinates of two vector
//if they cannot add ,return default vector
Vector Vector::operator+( const Vector& v2 ) const {
    if ( dimension == v2.dimension ) {
        double* array =new double [dimension];
        for (int i=0; i<dimension; i++)
            array[i] =coordinate[i] + v2.coordinate[i];

		Vector v( dimension ,array );
		delete [] array;
        return v;
    }

    cout<<"the dimension of the two Vectors are not the same"<<endl;
    return Vector();

}

//if they cannot add  ,return itself
Vector& Vector::operator+= ( const Vector& v2 ) {
    if ( dimension == v2.dimension ) {
        for (int i=0; i<dimension; i++)
           coordinate[i]+= v2.coordinate[i];
    }
    else
        cout<<"the dimension of the two Vectors are not the same"<<endl;
    return *this;
}

//substract  the individualcoordinates of two vector
//if they cannot substract ,return default vector
Vector Vector::operator-( const Vector& v2 ) const {
    if ( dimension == v2.dimension ) {
        double* array =new double [dimension];
        for (int i=0; i<dimension; i++)
            array[i] =coordinate[i] - v2.coordinate[i];

        Vector v( dimension ,array );
		delete [] array;
        return v;
    }

    cout<<"the dimension of the two Vectors are not the same"<<endl;
    return Vector();
}

//if they cannot substract  ,return itself
Vector& Vector::operator-= ( const Vector& v2 ) {
    if ( dimension == v2.dimension ) {
        for (int i=0; i<dimension; i++)
           coordinate[i]-= v2.coordinate[i];
    }
    else
        cout<<"the dimension of the two Vectors are not the same"<<endl;

    return *this;
}

//it'll make all coordinate multiply scalar considered vector*scalar part
Vector Vector::operator*( double s ) const {
    double* array = new double [dimension];
    for (int i=0; i<dimension ;i++)
        array[i] = coordinate[i] * s;
    
	Vector v( dimension ,array );
	delete [] array;
    return v;
}

//Scalar * vector
Vector operator* (double s ,const Vector& v) {
    return v * s;
}
Vector& Vector::operator*= (double s ) {
    for (int i=0; i<dimension; i++)
        coordinate[i] *= s;
    return *this;
}

/*Equality operators*/
//it'll first compare dimension.if the same ,it'll compare coordinate
bool Vector::operator== (const Vector& v2) const {
    if ( dimension == v2.dimension ) {
        for (int i=0; i<dimension; i++)
            if ( coordinate[i] != v2.coordinate[i] ) return false;
        return true;
    }

    return false;
}

//if v1==v2 then return false else return true
bool Vector::operator!= (const Vector& v2) const {
    if (*this ==v2) return false; else return true;
}

//it'll use LA method to compute the length of a vector
double Vector::length() {
    double ssum=0;

    for ( int i=0; i<dimension; i++)
        ssum+= coordinate[i]*coordinate[i];

    return sqrt(ssum);
}
//Overloading the global operator<<
ostream& operator<< ( ostream& out ,const Vector&  v ) {
    out<<"(";

    for (int i=0; i<v.dimension; i++ ) {
        out<<v.coordinate[i]<<", ";
    }

    out<<"\b\b)\n";

    return out;
}


