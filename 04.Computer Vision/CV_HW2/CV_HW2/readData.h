/******************************************
FILE
    readData.h
PURPOSE
    �q�ɮפ�Ū���������

NOTE
	

AUTHOR
    9757553 �C����
*******************************************/

#ifndef _READDATA_
#define _READDATA_

#include <vector>
#include <string>
#include "Picture.h"
#include "Struct.h"
using std::vector;
using std::string;



/*
  ���o��i�Ϫ��I���� �H��K��MF
  �ǤJ�Gcorrespondence ���ɮצ�m
  �ǥX�G�۹������I�y�ж��X
*/
vector<CorPoint> getCorrespondPoint(const string cfile); 
/*
  Ū����Ƨ��U������
  �ǤJ�G��Ƨ��W��
  �ǥX�G���ɶ��X
*/
vector<Picture>  getPicture( const string dname );
#endif