/******************************************
FILE
    program14.cpp
Purpose
	提供子作業一的所有能力
	User can set various light and material parameters 
	  to render diffuse or specular lighting effects. (10%)
    1.Render the Utah teapot using Gouraud shading
	4.Phong Shading的功能
NOTE
	根據OpenGL API提及 GL_SMOOTH
	Smooth shading, the default, causes the computed colors of
	vertices to be interpolated as the primitive is rasterized,
	typically assigning different	colors to each resulting pixel
	fragment.
	也就是說點的顏色使用線性差值產生的 也就是說這是Gouraud shading的效果
	因此我們不需要改寫Rasteriztion的部分就可以產生作業一的能力

Author
    9757553 顏志晟
*******************************************/

#include "function.h"

using std::cout;
using std::cin;

extern bool openPhongShading;
extern GLhandleARB program;//program object for phong shading

/*進行作業一*/
static void lighting();
void 
drawPolygonWithoutTexture() {
	//打光
	lighting();
	//畫圖
	drawPolygon( false );

}
//打光
static bool setting = true;

void 
lighting() {
	//設定材質
	static GLfloat mat_diffuse[] = {0.2, 0.2, 0.6, 1.0};
	static GLfloat mat_specular[] = {0.6, 0.6, 0.6, 1.0};
	static GLfloat mat_ambient[] = {0.6, 0.6, 0.6, 1.0};
	static GLfloat mat_shininess = 160.0;
	//設定光線
	static GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};
	static GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
	static GLfloat light_ambient[] = {0.0, 0.0, 0.0, 1.0};
	//設定光線向量
	static GLfloat light_position[] = {0.0 ,20.0 ,27 , 1.0};

	if (! setting ) {
		setMaterialParameter(mat_diffuse ,mat_specular ,mat_shininess ,mat_ambient );
		setLightingParameter(light_diffuse ,light_specular ,light_ambient );
		cout<<"是否設定光線向量其default值為({0.0 1.0 0.0 )(Y/N)：";
		char yesno = 'n';
		cin >> yesno;
		if ( yesno == 'Y' || yesno == 'N' ) {
			cout<<"請輸入光線向量(x , y , z) = ";
			cin >> light_position[0] >> light_position[1] >>light_position[2];
		}
		setting = true;
	}
	
	glShadeModel( GL_SMOOTH );

	// enable lighting
	glEnable(GL_LIGHTING);
	// set light property
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);//Cl
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);//Csl
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);//Ca

	// set material property
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);//Cr   c = Cr  Cl  (n * r)
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);//Cs
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);//Cr'  c = Ca Cr' 
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);//p c = Csl Cs (h * n) ^ p
}
