/******************************************
FILE
    retificationh.h
PURPOSE
    �w�q�p��N�ϧηh�����禡�w

NOTE
	

AUTHOR
    9757553 �C����
*******************************************/

#ifndef _RECTIF_
#define _RECTIF_

#include "Picture.h"
//#include <vector>

//using std::vector;

/*
 ���o���઺�x�} R: 3 * 3
 �ǤJ�G����b ���ਤ��
 �ǥX�G����x�}
*/
double* * getRotation3By3( const double* axis ,double theta  );
/*
  �ھڤ@�ӦV�q��X�L�Px�b�����èD�XRotation matrix 2
  �ǤJ�G�V�q
  �ǥX�Grotation matrix
*/
double* *getRotation2(const double* v );
/*
 image epipole line�ॿ
 �ǤJ�G
     �v�� 
	 fundamental matrix 
	 translation vector 
	 rotation matrix
 �ǥX�G�ॿ���v��
*/
vector<Picture> getRectifyImage( vector<Picture>& pics , double* * f ,const double* t ,double* * r );

#endif
