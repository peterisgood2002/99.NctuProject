/******************************************
FILE
    Model.h
Purpose
    �i��Model���@�Ǭ����B�� �]�t�I���P�_����

NOTE
	

Author
    9757553 �C����
*******************************************/

#ifndef _MODEL_
#define _MODEL_

#include "mesh.h"
#include "OpenGLParameter.h"
#include "Scene.h"
#include "Ray.h"


struct HitPoint {
public:
	bool hit;
	double normal[3];//�x�snormalize�L�᪺��
	float ma[4];
	float ms[4];
	float md[4];
	float ns;
};

class Model : public mesh {
	
private:
	
	/*�����O�_Collision��*/
	bool isCollide (Ray& r ,const float* a ,const float* b ,const float* c ,double& tmin ,double& tmax ,double& alpha ,double& beta ,int as = 3);
	double det3by3( const double* vec1 ,const double* vec2 ,const double* vec3 );//�M�w�x�}3*3 ��determine 
	static const int fpcount = 3;//face�`�@�O�ѴX���I�զ�
	HitPoint hp;//��ܼ����쪺�I

public:
	Model(const char* str);
	~Model();

	static const int NOTANUMBER = -1;
	/*
	  ��XHitpoint������T ��diffuse���ȥH��normal����
	  �ǤJ�G �� �H�� alpha*(ab) + beta* (ac) �Q�ήt�Ȫk��X�ݭn����T
	*/
	void calHitPoint(int facei ,double alpha , double beta);
	/*
	  �ˬd�O�_���I��
	  �ǤJ�G�g�u ,�g�u�_�I �g�u���I
	  �ǥX�G�p�G�I���ǥX�I����face�O���@��
	*/
	int collideFace( Ray& r ,double tmin ,double tmax );
	/*
	  �z�Lincident ray reflection ray��Phong Light
	  �ǤJ�Gincident ray ���쪺face�I �H�Υ��������]�w�� ���u���L�Q�B���M�w���ͪ��C��O�_�n�[�Wdiffuse�Mambient
	        �O�_�nmirror �Y�O�nmirror�п�J����
	  �ǥX�G���I���C���RGBA
	*/
	const Color* getPtrColor(Ray r ,const double* p ,int facei ,const Light& l, bool shadow  ,bool ismirror = false ,Scene* const scene = NULL );
	/*
	  �z�L�����k�o����I��normal��
	  �ǤJ�G���I �H�θ��I�Ҧb����
	*/
	const double* getNormal( const double* p ,int facei );
	/*
	  �z�L�����k�D�X���I��diffuse��
	  �ǤJ�G���I �H�θ��I�Ҧb����
	  �ǥX�G���I��diffuse��
	*/
	const Color* getDiffuse( const double* p ,int facei );
	/*
	  �NHitPoint�]��Default
	*/
	void setDefault();
};
#endif