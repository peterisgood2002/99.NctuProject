/******************************************
Purpose
    定義眼睛的類別 儲存眼睛的各項參數

Author
    9757553 顏志晟
*******************************************/

#ifndef _CAMERA_
#define _CAMERA_

class Camera {
private :
	//eye camera位置
	double eyex;
	double eyey;
	double eyez;
	//center 物體中間位置
	double centerx;
	double centery;
	double centerz;
	//up
	double upx;
	double upy;
	double upz;
	const double PI;
public :
	//default constructor以
	Camera() ;
	Camera(double ex ,double ey ,double ez ,double cx ,double cy ,double cz ,double ux ,double uy ,double uz);
	//設定眼睛相關參數
	/**
	 *設定眼睛看出去的向量
	 *傳入：眼睛看出的(ex ,ey ,ez)
	*/
	void setCamera(double ex ,double ey ,double ez);
	/**
	 *設定眼睛中心位置在哪裡
	 *傳入：眼睛中心位置的座標
	*/
	void setCenter(double cx ,double cy ,double cz);
	/**
	 *眼睛朝上的位置為何
	*/
	void setUp(double ux ,double uy ,double uz);
	//得到眼睛相關參數
	double getEyeX() const;
	double getEyeY() const;
	double getEyeZ() const;
	double getCenterX() const;
	double getCenterY() const;
	double getCenterZ() const;
	double getUpX() const;
	double getUpY() const;
	double getUpZ() const;
	/**設定眼睛zoom in
	  *傳入：要zoom in多少距離
	*/
	void zoomIn(double dist);
	/**設定眼睛zoom out
	  *傳入：要zoom out 多少距離
	*/
	void zoomOut(double dist);
	/**向左
	  *傳入：要向左多少距離
	  */
	void moveLeft(double dist);
	/**向右
	  *傳入：向右多少距離
	  */
	void moveRight(double dist);
	/**Camera隨著物體中心旋轉
	  *傳入：水平旋轉量 垂直旋轉量 in degree
	  */
	void rotate(double rx ,double ry);
	/**Camera 與物體間的距離*/
	double distanceToObject();

};

#endif