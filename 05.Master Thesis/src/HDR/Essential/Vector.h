/***************************************************************************
    Name
        Vetcor.h
    Purpose
        to declare a class called Vector
    Author
        92306026 ÃC§ÓÑÔ

****************************************************************************/
#ifndef _GVECTOR_
#define _GVECTOR_


#include <iostream>

using std::ostream;

class Vector {
    public:

        //Constructor
        /**
         *Constructor or Default Constructor
         *Parameter¡Gdimension
         */
        Vector( int d=3 );

        /**
         *Constructor in order to transform array into Vector
         *Parameter¡Gdimesion and array
         */
        Vector (int d ,const double* array );

        //the "Big Three"
        /**
         *Copy Constructor
         *Parameter¡Ganother Vector to copy
         */
        Vector (const Vector& v );

        /**
         *Destructor
         */
        ~Vector();

        /**
         *Overloading the assignment operator
         *Parameter¡GVector you wanna assign
         */
        Vector& operator= (const Vector& v );

        //operator[]
        /**
         *Overload the operator[]
          if it out of range it'll return the first element
         *Parameter¡Gwhich one do you wanna access
         *Return¡Gvalue you wanna access
         */
        double& operator[] (int i) const;

        //Accessing/Changing dimensionality
        /**
         *Get the dimensionality of a Vector
         */
        inline int getDimension() const { return dimension; }
		/**
         *Get the data of a Vector
		  It'll reproduce  a new array to return
		  please delete it when you didn't need it
         */
		double* getData() const;

        /**
         *Set the dimensionality of a Vector
         *Parameter¡Gdimension you wanna set
         *Return¡Gif successful return true else return false
         */
        bool setDimension(int d);

        //Arithemetic Operators
        /**
         *Unary negation operator
         */
        Vector operator-() const;
        /**
         *Overloading the + and += operator
         *Parameter¡Ganother Vector to add
         *Return¡G
            if the dimensions are not the same it'll return the zero Vector
         */
        Vector operator+( const Vector& v2 ) const;
        /**
         *Return¡Gif the dimensions are not the same
                  it'll return the orginal Vector
         */
        Vector& operator+= ( const Vector& v2 );
        /**
         *Overloading the - and  -= operator
         *Parameter¡Ganother Vector to substract
         *Return¡G
            if the dimensions are not the same it'll return the zero Vector
         */
        Vector operator-( const Vector& v2 ) const;
        Vector& operator-= ( const Vector& v2 );
        /**
         *Scalar Multiplication¡G
             it'll overload operator* .this's consider Vector*scalar part
         *Parameter¡GScalar
         */
        Vector operator*( double s ) const;

        /**
         *it'll implement the scalar* Vector part
         */
        friend Vector operator* (double s ,const Vector& v);
        Vector& operator*= (double s );

        //Equality operators
        /**
         *Overloading == operator
         *Parameter¡Ganother Vector wanting comparison
         *Return¡G
            if the coordinate and dimension are the same return true
            else return false
         */
        bool operator== (const Vector& v2) const;
        /**
         *Overloading != operator
         *Parameter¡Ganother Vector wanting comparison
         *Return¡G
            if the coordinate and dimension are not the same return true
            else return false
         */
        bool operator!= (const Vector& v2) const;
        //Length
        /**
         *Compute the length of Vector.
         *Return¡Gthe length of Vector
         */
        double length();

        //Operaotr<<
        friend ostream& operator<< (ostream& out ,const Vector& v );
    private:
        int dimension;//Vector's dimesion
        double* coordinate;//dynamic Vector
};

/**
 *it'll implement the scalar* Vector part
 *Parameter¡GScalar and Vector
 */
Vector operator* (double s ,const Vector& v);
/**
 *Overloading the global operator<<
*/
ostream& operator<< (ostream& out ,const Vector& v );


/*=============================
     InLine Function Definition


	 Member functions can be inline, but it cannot be called before its definition due to its internal linkage. 
	 Therefore, inline member functions are usually defined right after class definition in the .h file
===============================*/
#endif
