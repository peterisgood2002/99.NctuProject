/******************************************
FILE
    scence.h
Purpose
    �t�d�������@�ǳ]�w �åBrender�X����

NOTE
	

Author
    9757553 �C����
*******************************************/

#ifndef _SCENE_
#define _SCENE_

#include "Ray.h"
#include "Model.h"
#include <string>
#include <vector>

using std::vector;

#ifndef _MODE_
#define _MODE_
enum Mode { DEFAULT = 0 ,DLIGHTING ,MIRROR ,INDIRECT};//���{���������Ҧ�
#endif


class Scene {

private:
	vector<mesh*> marray;
	/*�i��default��X��*/
	void defaultDisplay();
	/*direct lighting
	  �ǤJ�G�O�_�n��Y�@��model����mirror�ĪG
	*/
	void directLighting( bool ismirror = false ,int mi = 0 );
	/*indirect lighting
	*/
	void indirectLighting();
	/*���oRay������I�����I
	  �ǤJ�GRay 
	  �Ǧ^�G���L���� ���쪺mesh�Mface�b���� �S������^��NULL
	*/
	const double* RayToPoint(Ray& r ,int& modeli ,int& facei );
public:
	/*Constructor*/
	/*
	  �ǤJ�Gmodel�Ҧb��filename�}�C
	*/
	Scene(const char* const * filename ,const int size);
	/*Destructor*/
	~Scene();
	/*�ھ�mode����ܳ���
	  �ǤJ�G�n�H�ƻ�mode���
	*/
	void display(Mode mode);
	/*
	  �N�Y�ӳ����i�業���ʧ@
	  �ǤJ�G�������ӳ��� �����h��
	*/
	void tMeshTo(int i ,double to[4] );
	/*�o����I��Direct light��color
	  �ǤJ�G�@��Ray Light����m �n���n��Y��mesh����mirror
	  �Ǧ^�GRay����F�誺�C��� �p�G���ݭn�W�C�� �N�|�^��NULL
	*/
	const Color* getDLColor( Ray& r ,const double* lp  ,bool ismirror = false ,int mi = 0);
	/*�i���ƴ��ե�*/
	//void test(Ray & r);


};

#endif

