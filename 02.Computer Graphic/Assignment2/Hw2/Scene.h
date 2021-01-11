/******************************************
FILE
    scence.h
Purpose
    負責場景的一些設定 並且render出場景

NOTE
	

Author
    9757553 顏志晟
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
enum Mode { DEFAULT = 0 ,DLIGHTING ,MIRROR ,INDIRECT};//此程式的相關模式
#endif


class Scene {

private:
	vector<mesh*> marray;
	/*進行default輸出用*/
	void defaultDisplay();
	/*direct lighting
	  傳入：是否要對某一個model產生mirror效果
	*/
	void directLighting( bool ismirror = false ,int mi = 0 );
	/*indirect lighting
	*/
	void indirectLighting();
	/*取得Ray對場景碰撞的點
	  傳入：Ray 
	  傳回：有無打到 打到的mesh和face在哪裡 沒有打到回傳NULL
	*/
	const double* RayToPoint(Ray& r ,int& modeli ,int& facei );
public:
	/*Constructor*/
	/*
	  傳入：model所在的filename陣列
	*/
	Scene(const char* const * filename ,const int size);
	/*Destructor*/
	~Scene();
	/*根據mode來顯示場景
	  傳入：要以甚麼mode顯示
	*/
	void display(Mode mode);
	/*
	  將某個場景進行平移動作
	  傳入：平移哪個場景 平移多少
	*/
	void tMeshTo(int i ,double to[4] );
	/*得到該點的Direct light的color
	  傳入：一條Ray Light的位置 要不要對某個mesh產生mirror
	  傳回：Ray打到東西的顏色值 如果不需要上顏色 將會回傳NULL
	*/
	const Color* getDLColor( Ray& r ,const double* lp  ,bool ismirror = false ,int mi = 0);
	/*進行函數測試用*/
	//void test(Ray & r);


};

#endif

