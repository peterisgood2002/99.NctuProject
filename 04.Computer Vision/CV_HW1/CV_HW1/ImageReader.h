/******************************************
FILE
    ImageReader.h
PURPOSE
    Ū�JImage�������{��

NOTE
	

AUTHOR
    9757553 �C����
*******************************************/
#include <vector>
#include <string>
#include "Picture.h"


using std::string;
using std::vector;
/*
  Ū��Image
  �ǥX�@��Picture�}�C
*/
vector<Picture> readIMG();
/*
  Ū��Light�y�Ф��ɮ�
  �ǤJ�GŪ��fname�ɮ�
  �ǥX�G�N�ɮצs��string
*/
string readLightFile(const string fname);
/*
  Ū�����u��m
  �ǤJ�G�s�����u�y�Ъ��r�� �Ϫ��W�r
  �ǥX�G���u��m
*/
Light readLP(const string lp ,const string imgn );

