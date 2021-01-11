/******************************************
FILE
    ArrayManipulation.h
PURPOSE
    定義操作Array的相關函式

NOTE
	

AUTHOR
    9757553 顏志晟
*******************************************/

struct Vector3{
public:
	double x;
	double y;
	double z;
};

typedef Vector3 Normal;
/*
  得到一個3D array
  傳入：row col
  傳出：3D array
*/
Vector3* *create3DArray( int row ,int col );
/*
  得到一個2D double array
  傳入：row col
  傳出：2D double array
*/
double* *create2DArray( int row ,int col );
/*
  將Array從2D轉成1D 此為Column major 以便和Matlab合體
  傳入：2D double array row col值
  傳出：1D double array
*/
double* _2DDouble2M1D( double** _2d ,int row ,int col );