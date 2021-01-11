/******************************************
FILE
    ArrayManipulation.h
PURPOSE
    �w�q�ާ@Array�������禡

NOTE
	

AUTHOR
    9757553 �C����
*******************************************/

#ifndef _ARRAYMANI_
#define _ARRAYMANI_

#include "Struct.h"

/*
  �o��@��3D array
  �ǤJ�Grow col
  �ǥX�G3D array
*/
Vector3* *create3DArray( int row ,int col );
/*
  �o��@��2D double array���w�Ȭ�0
  �ǤJ�Grow col ��l��ƹw�]��0
  �ǥX�G2D double array
*/
double* *create2DArray( int row ,int col ,double initdata = 0);
/*
  ����O����
  �ǤJ�G�@��2D array
*/
void dispose(double* * data ,int row ,int col);
/*
  �NArray�q2D�ন1D ����Column major �H�K�MMatlab�X��
  �ǤJ�G2D double array row col��
  �ǥX�G1D double array
*/
double* _2DDouble2M1D( double** _2d ,int row ,int col );
/*
  �NArray�q1D�ন2D ����Column major �H�K�MMatlab�X��
  �ǤJ�G1D double array 2D double array��row col��
  �ǥX�G2D double array
*/
double* *_M1DDouble22D(  double* M1d ,int row ,int col );


#endif