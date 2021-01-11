/******************************************
FILE
    PhotoStereo.h
PURPOSE
    �ŧiPhotometric Stereo�������禡

NOTE
	

AUTHOR
    9757553 �C����
*******************************************/
#include <vector>
#include "Picture.h"
#include "ArrayManipulation.h"
using std::vector;


/*
  �إߤ@��HeightMap
  �ǤJ�G�n�إ�HeightMap������img
  �ǥX�GHeightmap
*/
Vector3** buildNormalMap( const vector<Picture>& imgs );
/*
  ��(x ,y)�i��Photometric stereo��X��normalize��normal
  �ǤJ�G( x ,y ) �٦�����
  �ǥX�G��normalize��normal
*/
Normal calNormal( int x ,int y ,const vector<Picture>& imgs );
/*
  �NNormal normalize
  �ǤJ�G�@�ө|��Normalize��normal
  �Ǧ^�G����normal�öǦ^Normalize�����G
*/
Normal* *normalizeN( Normal* * map ,int row ,int col);
/*
  ���(-df/dx ,-df/dy ,1 )
  �ǤJ�G�@�ө|��Normalize��normal row col
  �Ǧ^�G����normal�öǦ^Normalize�����G
*/
Normal* *findF( Normal* * map ,int row ,int col);
/*
  �إ�Height Map
  �ǤJ�GNormal Map
  �ǥX�GHeight Map
*/
double* *buildHeightMap( Normal* *nmap ,int row ,int col );
/*
  �i��Correlation
  ----------->x
  |
  |
  |
  |
  y
  �ǤJ�Gheight map filter �nfilter����m
  �ǥX�Gcoorelation�����G

*/
double* *smoothHMap( double* * hmap ,int row ,int col );
