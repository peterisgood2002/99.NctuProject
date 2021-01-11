/******************************************
FILE
    program14.cpp
Purpose
	���Ѥl�@�~�@���Ҧ���O
	User can set various light and material parameters 
	  to render diffuse or specular lighting effects. (10%)
    1.Render the Utah teapot using Gouraud shading
	4.Phong Shading���\��
NOTE
	�ھ�OpenGL API���� GL_SMOOTH
	Smooth shading, the default, causes the computed colors of
	vertices to be interpolated as the primitive is rasterized,
	typically assigning different	colors to each resulting pixel
	fragment.
	�]�N�O���I���C��ϥνu�ʮt�Ȳ��ͪ� �]�N�O���o�OGouraud shading���ĪG
	�]���ڭ̤��ݭn��gRasteriztion�������N�i�H���ͧ@�~�@����O

Author
    9757553 �C����
*******************************************/

#include "function.h"

using std::cout;
using std::cin;

extern bool openPhongShading;
extern GLhandleARB program;//program object for phong shading

/*�i��@�~�@*/
static void lighting();
void 
drawPolygonWithoutTexture() {
	//����
	lighting();
	//�e��
	drawPolygon( false );

}
//����
static bool setting = true;

void 
lighting() {
	//�]�w����
	static GLfloat mat_diffuse[] = {0.2, 0.2, 0.6, 1.0};
	static GLfloat mat_specular[] = {0.6, 0.6, 0.6, 1.0};
	static GLfloat mat_ambient[] = {0.6, 0.6, 0.6, 1.0};
	static GLfloat mat_shininess = 160.0;
	//�]�w���u
	static GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};
	static GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
	static GLfloat light_ambient[] = {0.0, 0.0, 0.0, 1.0};
	//�]�w���u�V�q
	static GLfloat light_position[] = {0.0 ,20.0 ,27 , 1.0};

	if (! setting ) {
		setMaterialParameter(mat_diffuse ,mat_specular ,mat_shininess ,mat_ambient );
		setLightingParameter(light_diffuse ,light_specular ,light_ambient );
		cout<<"�O�_�]�w���u�V�q��default�Ȭ�({0.0 1.0 0.0 )(Y/N)�G";
		char yesno = 'n';
		cin >> yesno;
		if ( yesno == 'Y' || yesno == 'N' ) {
			cout<<"�п�J���u�V�q(x , y , z) = ";
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
