/******************************************
FILE
    Stereopsis.h
PURPOSE
    �w�qstruct from motion�ݭn��������T

NOTE
	

AUTHOR
    9757553 �C����
*******************************************/

#ifndef _STEREOPSIS_
#define _STEREOPSIS_

#include "ArrayManipulation.h"
#include "Picture.h"
/*
  �ھڨ�i�Ϩ��o3D�y��
  �ǤJ�G
      ��i�� ���O�N���ϩM�k�� 
	  Optical center�첾�h�� 
	  �n������windows size�h�j
	  �H���@�ӹϬ���ǹw�]���H����(i = 0)���� i = 1���k��
	  �J�Z
  �ǥX�G�ھڭ��@�ӹ϶ǥX3D�y��
*/
Vertex* * get3DValue(Picture& pl , Picture& pr ,double T, int wwidth = 1 ,int wheight = 1 ,int rorc = 0 ,double f = 1);

#endif