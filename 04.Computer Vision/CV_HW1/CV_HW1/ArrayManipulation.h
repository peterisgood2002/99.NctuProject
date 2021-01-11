/******************************************
FILE
    ArrayManipulation.h
PURPOSE
    �w�q�ާ@Array�������禡

NOTE
	

AUTHOR
    9757553 �C����
*******************************************/

struct Vector3{
public:
	double x;
	double y;
	double z;
};

typedef Vector3 Normal;
/*
  �o��@��3D array
  �ǤJ�Grow col
  �ǥX�G3D array
*/
Vector3* *create3DArray( int row ,int col );
/*
  �o��@��2D double array
  �ǤJ�Grow col
  �ǥX�G2D double array
*/
double* *create2DArray( int row ,int col );
/*
  �NArray�q2D�ন1D ����Column major �H�K�MMatlab�X��
  �ǤJ�G2D double array row col��
  �ǥX�G1D double array
*/
double* _2DDouble2M1D( double** _2d ,int row ,int col );