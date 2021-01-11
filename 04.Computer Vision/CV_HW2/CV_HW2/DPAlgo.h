/******************************************
FILE
    DPAlgo.h
PURPOSE
    �i��Dynamic programming algorithm���@�Ǩ禡
	

NOTE
	

AUTHOR
    9757553 �C����
*******************************************/

#ifndef _DPALGO_
#define _DPALGO_

#include <windows.h>
#include <gdiplus.h>
#include <vector>

using Gdiplus::Color;
using std::vector;

typedef vector<Color> WColor;

enum Direction { COL=-3 ,UP=-3 ,ROW=-2  ,LEFT=-2 ,D ,NOTDIR = 0};//��O�L���̤p�ȬO�q�W���άO����άO�﨤�u�Ӫ�
const double NOTANUMBER = System::Double::MaxValue;
/*
 �qColor����M�̱����I �H�U�C����
 double l[4] = { 4 ,3 ,2 ,1};
 double r[4] = { 5 ,4 ,2 ,1};
 ���X��
        5 4 2 1
      4
	  3
	  2
	  1
 �C�@��cell��ܥL���U�@����Ƹӫ��]
 �ǤJ�G���ϩM�k�Ϫ�epipolar line��color��
 �ǥX�GDynamic programming�����G 
*/
double* *dpAlgorithm(const vector<WColor*>& l , const vector<WColor*>& r ); 
/*
 �z�LDP algorithm���G ��X�۹��������
 �ǤJ�GDP algo���G 
      �n�Hrow���� ��۹�������٬Ocol d = UP��Hrow d = LEFT��Hcol
 �ǥX�G�Hd���Ǫ��۹������G
*/
int* getCorrespondence(double* * dpresult ,int row ,int col ,Direction d);
/*
  �t�d�p��cost�Ϊ�function �p�G���ݭn�Цۦ�override
  �ǤJ�G�ⵧ���
  �ǥX�Gcost
*/
double costFunc(const double* l ,const double* r ,int wsize);
/*
 Dynamic programming�D�n��@�B
 �ǤJ�G��Ӱ}�C�H��window size
 �ǥX�GDynamic programming���G
*/
double* *dpAlgorithm(const vector<double*>& left ,const vector<double*>& right ,int wsize);
/*Output data �� ������*/
void printData(double* * const data ,int r ,int c);

/*���ե�*/
double* * dpAlgorithm(double* l ,int llength ,double* r ,int rlength );
#endif