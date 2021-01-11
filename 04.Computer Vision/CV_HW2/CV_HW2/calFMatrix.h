/******************************************
FILE
    calFMatrix.h
PURPOSE
    �w�q�p��p��fundamental matrix������T
	F = T * R
	�èD�XT R

NOTE
	1.�z�L�U�������p��F
      p'�Gimage 2 ���Y�I
      p�Gimage 1 ���Y�I
      p' ������T * p'
      �]���b�S�������Ѽƪ����p�U
      0 = p' (T * p') = p' [T * (Rp + T) ] = p' (T * R)   p = p' E  p
      �b�������Ѽƪ����p�U
      0 = p'  (K' E K)  p = p' F  p
    2.���W��correspondent point�i�H���o��[����
      �Z�����|�W�Lsqrt(2) �]�N�O
      scale = sqrt(2) / Avgdis
          [     scale           0     -avg*scale ]
	   S =[       0           scale   -avg*scale ]
	      [       0             0		   1     ]

AUTHOR
    9757553 �C����
*******************************************/

#ifndef _CALF_
#define _CALF_

#include <vector>
#include <string>
#include "Picture.h"
#include "Struct.h"

using std::vector;
using std::string;

/*
  ���oCorPoint���U����������
  �ǤJ�G�Ҧ����I�������X
  �ǥX�G�@��CorPoint�x�s�U������������
*/
CorPoint* getAverage( const vector<CorPoint>& cp );
/*
 ���W��correspondent point�n�i��B��
 �Z�����|�W�Lsqrt(2) �]�N�O
 scale = �v�����שμe��
 
 �ǤJ�G�I�y�� �۹����I�Ҧ��������� �H�ι��ɪ����׼e��
 �ǥX�Gnormalize�L�᪺�I�y�� �H��Normalize�L�᪺�x�} nl nr
*/
vector<CorPoint> normalizeCP( const vector<CorPoint>& cp  ,double imgw ,double imgh ,double* * nl ,double* * nr ,CorPoint* avg = NULL);

/*
  �z�L�U�������p��F
  p'�Gimage 2 ���Y�I
  p�Gimage 1 ���Y�I
  p' ������T * p'
  �]���b�S�������Ѽƪ����p�U
  0 = p' (T * p') = p' [T * (Rp + T) ] = p' (T * R)   p = p' E  p
  �b�������Ѽƪ����p�U
  0 = p'  (K' E K)  p = p' F  p
  �ǤJ�Gcorrespondent point
  �ǥX�GFundamental matrix
*/
double* *calF(const vector<CorPoint>& cp );
/*
 �p��K' F K
 �ǤJ�G���䪺normalize matrix Fundamental matrix �k�䪺normalize
 �ǥX�G��X����
*/
double* *calKFK( double* *nl , double* * f , double* *nr ,int row ,int col);
/*
  �ھ�fundamental matrix�NT�ѥX �z�L�����I�ѥX��V
  �������G
    sum(T * Xi') * (F Xi) < 0 ==> T =  -T
  �ǤJ�Gfundamental matrix
  �ǥX�Gtranslation vector
*/
double* calT(double* * f ,const vector<CorPoint>& cp ,int row ,int col);
/*
  �ھ�fundamental matrix�MT�Ӹ� R
  �����G
    R = W = [ E1 * T + E2 * T        E2 * T + E3 * E1      E3 * T + E1 * E2 ]
	��noise ==> || R - W|| �̤p(�ثe�èS����)
  �ǤJ�Gfundamental matrix translation vector
  �ǥX�GR
*/
double* *calR(double* * f ,const double* t ,bool noise = false);
/*
  �ھ�fundamental matrix �b�ϤW�e�X�@��Epipolar line
  �ǤJ�G�� �ɦW fundamental matrix �����I �H�γo�@�i�ϬO�����٬O�k�� 
*/
void drawEpiLine(const Picture& p ,const string fname ,double* * f ,vector<CorPoint>& cp ,bool isL = true);
#endif