
#include "stdafx.h"
#include "VectorRn.h"

VectorRn VectorRn::WorkVector;

double VectorRn::MaxAbs () const
{
	double result = 0.0;
	double* t = x;
	for ( long i = length; i>0; i-- ) {
		if ( (*t) > result ) {
			result = *t;
		}
		else if ( -(*t) > result ) {
			result = -(*t);
		}
		t++;
	}
	return result;
}