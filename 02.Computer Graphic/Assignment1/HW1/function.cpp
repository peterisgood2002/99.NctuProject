#include "function.h"
#include <iostream>

using std::cout;
using std::string;
extern mesh* model;

/*將model畫出來
  傳入：是否加上texture
*/
void 
drawPolygon( bool withtexture ) {

	glClearColor(0.0f,0.0f,0.0f,0.0f);//設定background color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor3f(1.0, 1.0, 1.0);
	

	//畫圖
	for (int i=0;i < model->fTotal;i++)
	{
		glBegin(GL_POLYGON);
		for (int j=0;j<3;j++)
		{
			//設Texture座標
			if ( !withtexture) glTexCoord2fv( model->tList[ model->faceList[i][j].t ].ptr );
			else {
				glMultiTexCoord2fv(GL_TEXTURE0_ARB,  model->tList[ model->faceList[i][j].t ].ptr );
				glMultiTexCoord2fv(GL_TEXTURE1_ARB,  model->tList[ model->faceList[i][j].t ].ptr );
			}

			//設點的normal
			glNormal3fv( model->nList[model->faceList[i][j].n].ptr );
			//設點
			glVertex3fv( model->vList[model->faceList[i][j].v].ptr );	
		}
		glEnd();
	}
	
}

/*
 對作業一的呼叫
*/
void 
program1() {
	cout<<"目前進行lighting和material \n";
	drawPolygonWithoutTexture();
}

/*
 對作業二的呼叫
 */
void 
program2() {
	drawPolygonWithTexture();
}
///*
// 對作業三的呼叫
// */
//void 
//program3() {
//	bumpMapping();
//}

/*設定參數用*/
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

//設定材質狀況
void 
setMaterialParameter(GLfloat* mat_d ,GLfloat* mat_s ,GLfloat mat_sh ,GLfloat* mat_a ) {
	char yesno = 'n';
	cout<<"請設定相關材質顏色\n";
	cout<<"\t1.是否設定diffuse材質顏色或使用default value(RGBA = 1 ,0 ,0 ,1)(Y/N)：";
	cin >> yesno;
	yesno = tolower(yesno);
	if ( yesno =='y' ) {
		cout<<"\tDiffuse材質顏色(RGBA)：";
		cin >> mat_d[0] >>  mat_d[1] >>  mat_d[2] >>  mat_d[3];
	}
	cout<<"\t2.是否設定Specular材質顏色或使用default value(RGBA = 0, 0, 1, 1)(Y/N)：";
	cin >> yesno;
	yesno = tolower(yesno);
	if ( yesno =='y' ) {
		cout<<"\tSpecular材質顏色(RGBA)：";
		cin >> mat_s[0] >>  mat_s[1] >>  mat_s[2]  >>  mat_s[3];
	}
	cout<<"\t3.是否設定shinnig程度或使用default value(70)(Y/N)：";
	cin >> yesno;
	yesno = tolower(yesno);
	if ( yesno =='y' ) {
		cout<<"\tshinnig程度：";
		cin >> mat_sh;
	}

	if ( mat_a != NULL ){
		cout<<"\t4.是否設定ambient材質顏色或使用default value(RGBA = 0, 1, 0, 1)(Y/N)：";
		cin >> yesno;
		yesno = tolower(yesno);
		if ( yesno =='y' ) {
			cout<<"\tambient材質顏色(RGBA)：";
			cin >> mat_a[0] >>  mat_a[1] >>  mat_a[2]  >>  mat_a[3];
		}
	}
	
}

//設定光線狀況
void 
setLightingParameter(GLfloat* light_d  ,GLfloat* light_s ,GLfloat* light_a) {
	char yesno = 'n';
	cout<<"請設定相關光線顏色\n";
	cout<<"\t1.是否設定diffuse光線顏色或使用default value(RGBA = 1 ,1 ,1 ,1)(Y/N)：";
	cin >> yesno;
	yesno = tolower(yesno);
	if ( yesno =='y' ) {
		cout<<"\tDiffuse光線顏色(RGBA)：";
		cin >> light_d[0] >>  light_d[1] >>  light_d[2] >>  light_d[3];
	}
	cout<<"\t2.是否設定Specular光線顏色或使用default value(RGBA = 1 ,1 ,1 ,1)(Y/N)：";
	cin >> yesno;
	yesno = tolower(yesno);
	if ( yesno =='y' ) {
		cout<<"\tSpecular光線顏色(RGBA)：";
		cin >> light_s[0] >>  light_s[1] >>  light_s[2] >>  light_s[3];
	}

	if ( light_a != NULL ) {//如果有ambient參數時使用
		cout<<"\t3.是否設定Ambient光線顏色或使用default value(RGBA = 0 ,0 ,0 ,0)(Y/N)：";
		cin >> yesno;
		yesno = tolower(yesno);
		if ( yesno =='y' ) {
			cout<<"\tAmbient光線顏色(RGBA)：";
			cin >> light_a[0] >>  light_a[1] >>  light_a[2] >>  light_a[3];
		}
	}
}