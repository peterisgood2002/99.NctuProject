/******************************************
FILE
    Stereopsis.cc
PURPOSE
    struct from motion�ݭn��������T

NOTE
	

AUTHOR
    9757553 �C����
*******************************************/
#include "Stereopsis.h"
#include "DPAlgo.h"
#include <windows.h>
#include <gdiplus.h>
#include <vector>
#include <cmath>
#include <iostream>

using std::cout;
using std::vector;
using Gdiplus::Color;
/*
  �ھڨ�i�Ϩ��o3D�y�� �p�⪺��k�p�U
  �Q��DP��X�����I�ᰲ�]�O(u ,v) (U ,V)
  d = | u - U |
  �A�Q�ΤT���ƪk
  z = -f(B/d)
  P = -f(B/d) * p
  �ǤJ�G
      ��i�� ���O�N���ϩM�k�� 
	  Optical center�첾�h��
	  �n������windows size�h�j
	  �H���@�ӹϬ���ǹw�]���H����(i = 0)���� i = 1���k��
  �ǥX�G�ھڭ��@�ӹ϶ǥX3D�y��
*/
Vertex* * 
get3DValue(Picture& pl , Picture& pr ,double T, int wwidth ,int wheight ,int rorc ,double f){
	int width = pl.getWidth();
	int height = pl.getHeight();
	
	if ( rorc != 0 ) {
		width = pr.getWidth();
		height = pr.getHeight();
	}

	//��l��Vertex
	Vertex* * v = create3DArray ( height ,width);
	for ( int i =0; i < height; i++ ) {
		printf("���X��%d�C\n",i);
		vector<WColor*>* lc = pl.getOneLineColors(i ,wwidth ,wheight);
		vector<WColor*>* rc = pr.getOneLineColors(i ,wwidth ,wheight);
		cout<<"���X����\n";

		double* * dpresult = dpAlgorithm(*lc ,*rc );
		printf("�p���%d�C��Correspondence\n",i);
		int* cor  = 0;
		if ( rorc == 0 ) cor = getCorrespondence(dpresult ,(int)lc->size() ,(int)rc->size() ,ROW);//�H���䬰��
		else cor = getCorrespondence(dpresult ,(int)lc->size() ,(int)rc->size() ,COL);//�H���䬰��
		cout<<"�p��Correspondence����\n";
		
		/*for ( size_t x = 0; x < lc->size(); x++ ) 
			cout<< cor[x] <<" ";*/
		/*��X3D�y�Э�*/

		for ( int j = 0; j < (int)lc->size(); j++ ) {
			if ( cor[j] != (int)NOTANUMBER ){
				int diff = (int)abs( cor[j] - j );
				if ( diff != 0 ) {
					double z = f * T / diff;
					v[i][j].x = z * j / f;
					v[i][j].y = z * i / f;
					v[i][j].z = z * 1;
				} else {
					if ( j > 0 ) {
						v[i][j].x = v[i][j-1].x;
						v[i][j].y = v[i][j-1].y;
						v[i][j].z = v[i][j-1].z;
					}
				}
			}
			else {
				if ( j > 0 ) {
						v[i][j].x = v[i][j-1].x;
						v[i][j].y = v[i][j-1].y;
						v[i][j].z = v[i][j-1].z;
				}
			}
			

		}
		
		delete cor;
		for ( size_t z = 0; z < lc->size(); z++ ) {
			WColor* tmp = (*lc)[z];
			delete tmp;
			tmp = (*rc)[z];
			delete dpresult[z];
		}

		delete lc;
		delete rc;
		delete dpresult;
		cout<<"==================\n";
	}

	return v;
}
