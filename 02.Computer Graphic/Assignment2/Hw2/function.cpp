/******************************************
FILE
    program.h
Purpose
    

NOTE
	

Author
    9757553 顏志晟
*******************************************/
#include "function.h"
#include "glut.h"

/*初始化Canvas好畫東西*/
void 
initCanvas() {

	glClearColor(0.0f,0.0f,0.0f,0.0f);//設定background color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor3f(0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);// 記得啟動深度測試
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();//之後的matrix可以pop掉
		const Light light = par->getLight();
		glTranslatef( light.lightp[0] / light.lightp[3]  ,light.lightp[1] / light.lightp[3] ,light.lightp[2] / light.lightp[3] );
		
		glutSolidSphere(0.5, 10, 8);	// the Planet, 此時的矩陣是 I * ModelView * R * T * R
	glPopMatrix();//pop最上面的matrix (由上面三個矩陣相乘而來),會回到 I * ModelView
	
}

/*default打光用*/
void 
lighting() {
	const Light light = par->getLight();
	//設定光線
	static const GLfloat* light_specular = light.lspecular;
	static const GLfloat* light_diffuse = light.ldiffuse;
	static const GLfloat* light_ambient = light.lambient;
	//設定光線向量
	static const GLfloat* lightp =  light.lightp;
	
	printf("光的位置在(%f ,%f ,%f ,%f)\n" ,lightp[0] ,lightp[1] ,lightp[2] ,lightp[3] );

	glShadeModel( GL_SMOOTH );

	// enable lighting
	glEnable(GL_LIGHTING);
	// set light property
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light.lightp);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);//Cl
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);//Csl
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);//Ca

}

/*inner product*/
double 
innerProduct(const double* v1 ,const double* v2) {
	double ip = 0;
	for ( int i = 0; i < 3; i++ )
		ip += v1[i] * v2[i];

	return ip;
}
/*反射
  傳入：起始位置 入射線 法線
  傳出：反射線
*/
Ray
reflection( const double * p ,const double* ptoe ,const double* normal ) {
	double ray[3] = { 0 };
	double dn = innerProduct( ptoe , normal );
	for ( int i = 0; i < 3; i++ ) 
		ray[i] = 2 * dn * normal[i] - ptoe[i];
	//算出終點座標
	for ( int i = 0; i < 3; i++ )
		ray[i] += p[i];
	
	Ray tmp( p ,ray ,true , 100000);//屬於Ray而非線段

	return tmp;
}
