/******************************************
Purpose
    �w�q���������O �x�s�������U���Ѽ�

Author
    9757553 �C����
*******************************************/

#ifndef _CAMERA_
#define _CAMERA_

class Camera {
private :
	//eye camera��m
	double eyex;
	double eyey;
	double eyez;
	//center ���餤����m
	double centerx;
	double centery;
	double centerz;
	//up
	double upx;
	double upy;
	double upz;
	const double PI;
public :
	//default constructor�H
	Camera() ;
	Camera(double ex ,double ey ,double ez ,double cx ,double cy ,double cz ,double ux ,double uy ,double uz);
	//�]�w���������Ѽ�
	/**
	 *�]�w�����ݥX�h���V�q
	 *�ǤJ�G�����ݥX��(ex ,ey ,ez)
	*/
	void setCamera(double ex ,double ey ,double ez);
	/**
	 *�]�w�������ߦ�m�b����
	 *�ǤJ�G�������ߦ�m���y��
	*/
	void setCenter(double cx ,double cy ,double cz);
	/**
	 *�����¤W����m����
	*/
	void setUp(double ux ,double uy ,double uz);
	//�o�체�������Ѽ�
	double getEyeX() const;
	double getEyeY() const;
	double getEyeZ() const;
	double getCenterX() const;
	double getCenterY() const;
	double getCenterZ() const;
	double getUpX() const;
	double getUpY() const;
	double getUpZ() const;
	/**�]�w����zoom in
	  *�ǤJ�G�nzoom in�h�ֶZ��
	*/
	void zoomIn(double dist);
	/**�]�w����zoom out
	  *�ǤJ�G�nzoom out �h�ֶZ��
	*/
	void zoomOut(double dist);
	/**�V��
	  *�ǤJ�G�n�V���h�ֶZ��
	  */
	void moveLeft(double dist);
	/**�V�k
	  *�ǤJ�G�V�k�h�ֶZ��
	  */
	void moveRight(double dist);
	/**Camera�H�۪��餤�߱���
	  *�ǤJ�G��������q ��������q in degree
	  */
	void rotate(double rx ,double ry);
	/**Camera �P���鶡���Z��*/
	double distanceToObject();

};

#endif