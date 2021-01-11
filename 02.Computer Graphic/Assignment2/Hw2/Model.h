/******************************************
FILE
    Model.h
Purpose
    進行Model的一些相關運算 包含碰撞與否等等

NOTE
	

Author
    9757553 顏志晟
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
	double normal[3];//儲存normalize過後的值
	float ma[4];
	float ms[4];
	float md[4];
	float ns;
};

class Model : public mesh {
	
private:
	
	/*偵測是否Collision用*/
	bool isCollide (Ray& r ,const float* a ,const float* b ,const float* c ,double& tmin ,double& tmax ,double& alpha ,double& beta ,int as = 3);
	double det3by3( const double* vec1 ,const double* vec2 ,const double* vec3 );//決定矩陣3*3 的determine 
	static const int fpcount = 3;//face總共是由幾個點組成
	HitPoint hp;//表示撞擊到的點

public:
	Model(const char* str);
	~Model();

	static const int NOTANUMBER = -1;
	/*
	  算出Hitpoint相關資訊 有diffuse等值以及normal的值
	  傳入： 面 以及 alpha*(ab) + beta* (ac) 利用差值法算出需要的資訊
	*/
	void calHitPoint(int facei ,double alpha , double beta);
	/*
	  檢查是否有碰撞
	  傳入：射線 ,射線起點 射線終點
	  傳出：如果碰撞傳出碰撞的face是哪一個
	*/
	int collideFace( Ray& r ,double tmin ,double tmax );
	/*
	  透過incident ray reflection ray打Phong Light
	  傳入：incident ray 打到的face點 以及光的相關設定值 光線有無被遮蔽決定產生的顏色是否要加上diffuse和ambient
	        是否要mirror 若是要mirror請輸入場景
	  傳出：該點的顏色值RGBA
	*/
	const Color* getPtrColor(Ray r ,const double* p ,int facei ,const Light& l, bool shadow  ,bool ismirror = false ,Scene* const scene = NULL );
	/*
	  透過內插法得到該點的normal值
	  傳入：該點 以及該點所在的面
	*/
	const double* getNormal( const double* p ,int facei );
	/*
	  透過內插法求出該點的diffuse值
	  傳入：該點 以及該點所在的面
	  傳出：該點的diffuse值
	*/
	const Color* getDiffuse( const double* p ,int facei );
	/*
	  將HitPoint設成Default
	*/
	void setDefault();
};
#endif