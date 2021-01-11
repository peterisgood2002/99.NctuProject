/******************************************
FILE
    scence.h
Purpose
    負責場景的一些設定 並且render出場景

NOTE
	

Author
    9757553 顏志晟
*******************************************/

#include "Scene.h"
#include "function.h"
#include "glut.h"
#include "Struct.h"
#include <iostream>
#include "math.h"

using std::cout;

Scene::Scene(const char* const* filename ,const int size) : marray(size) {

	for ( int i=0; i<size;i++ ) {
		mesh* m = new Model( filename[i] );
		marray[i] = m;
	}

}

Scene::~Scene() {
	for ( unsigned i = 0 ;i < marray.size(); i++)
		delete marray[i];
	
	
}

/*根據mode來顯示場景
  傳入：要以甚麼mode顯示
*/
void 
Scene::display(Mode mode) {
	switch ( mode ) {
		case DLIGHTING:
			directLighting();
			break;
		case MIRROR:
			directLighting(true ,2);//對球做mirroring效果
			break;
		case INDIRECT:
			indirectLighting();
			cout<<"整體做indirect lighting\n";
			break;
		case DEFAULT:
		default:
			defaultDisplay();
			break;
	}
}

void 
Scene::defaultDisplay() {

	initCanvas();
	lighting();
	
	for ( unsigned i=0; i<marray.size(); i++) {
		mesh* model = marray[i];

		//畫出model
		for ( int i = 0; i < model->fTotal; i++ ) {
			glBegin(GL_POLYGON);
				for (int j=0;j<3;j++)
				{
					if ( model != NULL) {
						//設定面的材質
						material m = model->mat[ model->faceList[i][0].m ];
						glMaterialfv(GL_FRONT, GL_SPECULAR, m.Ks);
						glMaterialfv(GL_FRONT, GL_DIFFUSE, m.Kd);
						glMaterialfv(GL_FRONT, GL_AMBIENT, m.Ka); 
						glMaterialf(GL_FRONT, GL_SHININESS,m.Ns);
						
						glTexCoord2fv( model->tList[ model->faceList[i][j].t ].ptr );
						//設點的normal
						glNormal3fv( model->nList[ model->faceList[i][j].n].ptr );
						//設點
						glVertex3fv( model->vList[ model->faceList[i][j].v].ptr );	
					}
				}
			glEnd();
		}
	}

}
/*取得Ray對場景碰撞的點
  傳入：Ray 
  傳回：有無打到 打到的mesh和face在哪裡
*/
const double* 
Scene::RayToPoint(Ray& r ,int& modeli ,int& facei ) {
	
	double* vec = NULL;

	//對所有的model偵測有無碰撞 確定到底打到的是甚麼鬼東西 並取得是哪一個model的那一個面被打到
	for ( unsigned i = 0; i < marray.size(); i++ ) {
	
		Model* model = (Model*) marray[i];
		double tface = model->collideFace(r ,par->getNearPlane() ,par->getFarPlane() );
		if ( tface != Model::NOTANUMBER ) {//表示有打到東西
			facei = tface;
			modeli = i;
		}
	}
	
	if ( modeli != -1 && facei != Model::NOTANUMBER ) {
		vec = new double[3];
		for ( int i = 0; i < 3; i++ )
			vec[i] = r.e[i] + r.t * r.b[i];
		
	}
	return vec;
}

/*進行direct Lighting
  傳入：是否要對某一個model產生mirror效果
*/
void 
Scene::directLighting( bool ismirror ,int mi) {

	initCanvas();

	double lp[3] = { 0 };//光線座標
	for ( int i = 0; i < 3; i++ )
		lp[i] = par ->getLight().lightp[i];
	/*儲存pixel color值*/
	int total = par->getGLWindowWidth() *par->getGLWindowHeight() * 3;
	float* color = new float[ total];
	for ( int i = 0; i < total; i++ )
		color[i] = 0;

	cout<<"Direct Lighting開始\n";
	for ( int j = 0; j < par->getGLWindowHeight(); j++  ) {//對每一個點進行render
		for ( int i = 0; i < par->getGLWindowWidth(); i++) {	
			//int i = 386,j =152;
			
			Ray r(i , j ,par );//產生一條射線
			
			const Color* c = getDLColor( r ,lp , ismirror  ,mi);

			if ( c != NULL ) {
				printf("%d %d 需要上顏色 \n" ,i ,j);
				//上色方法是原點在左下角
				/*
				 /|\
				  |
				  |
				  --------------->
				  所以j要改變
				*/
				int cell = ( par->getGLWindowWidth()) * (par->getGLWindowHeight() - j ) + i;
				
				color[ cell * 3 ] = c->r;
				color[ cell * 3 + 1] = c->g;
				color[ cell * 3 + 2] = c->b;
				//cout<<"==============================\n";

				
			}

			delete c;
		}
	}
	cout<<"Direct Lighting結束\n";
	glDrawPixels(par->getGLWindowWidth() ,par->getGLWindowHeight() ,GL_RGB ,GL_FLOAT ,color); 
}
/*indirect lighting
  傳入：是否要對某一個model產生shadow效果
*/
void 
Scene::indirectLighting( ) {
	initCanvas();

	double lp[3] = { 0 };//光線座標
	for ( int i = 0; i < 3; i++ )
		lp[i] = par ->getLight().lightp[i];
	/*儲存pixel color值*/
	int total = par->getGLWindowWidth() *par->getGLWindowHeight() * 3;
	float* color = new float[ total];
	for ( int i = 0; i < total; i++ )
		color[i] = 0;

	cout<<"Indirect Lighting開始\n";
	/*----------------Begin Indirect Lighting----------------------*/
	for ( int j = 0; j < par->getGLWindowHeight(); j++  ) {//對每一個點進行render
		for ( int i = 0; i < par->getGLWindowWidth(); i++) {
			//int i = 134,j =79;
			
			Ray r(i , j ,par );//產生一條射線
			int modeli = -1 ,facei = -1;
			const double* vec = RayToPoint( r ,modeli ,facei );//找到射線找出的那個點
			const Color* c = getDLColor( r ,lp ,true ,2);//先找出打到的點他的顏色值
			Color* obil = NULL;
			if ( vec != NULL ) {//表示有打到東西
				/*製作反射線中 請稍候......*/
				Model* model = (Model*) marray[ modeli ];
				const double* normal = model->getNormal( vec ,facei );//以內插方式計算normal
				
				double ptoe[3] = { 0 };//向量pe
				for ( int i = 0; i < 3; i++ ) 
					ptoe[i] = r.e[i] - vec[i];
				double l = sqrt( ptoe[0] * ptoe[0] + ptoe[1] * ptoe[1] + ptoe[2] * ptoe[2] );//normalize
				for ( int i = 0; i < 3; i++ )
					ptoe[i] = ptoe[i] / l;

				Ray tmp = reflection( vec ,ptoe ,normal );
				/*反射線製作完成*/
				/*將Color加上新光源的貢獻值
				  新光源的貢獻量=新光源的強度 *要畫出的點的diffuse * cos(ray方向與該點法向量夾角)  
				*/
				const Color* newlight = getDLColor(tmp ,lp ,true ,2);//有打到顏色
				if ( newlight != NULL ) {
					const Color* d = model->getDiffuse(vec ,facei );
					double cost = innerProduct ( normal , tmp.normalizeB() );//Cos theta
					obil = new Color();

					obil->r = newlight->r * d->r * cost * 0.5;
					obil->g = newlight->g * d->g * cost * 0.5;
					obil->b = newlight->b * d->b * cost * 0.5;

					delete d;
				}

				//model->setDefault();//設回預設
				delete newlight;
				
					
			}

			delete vec;
			/*上色了真開心!!*/
			int cell = ( par->getGLWindowWidth()) * (par->getGLWindowHeight() - j ) + i;
			if ( c != NULL ) {
				printf("%d %d 需要上顏色 \n" ,i ,j);
				//上色方法是原點在左下角
				/*
				 /|\
				  |
				  |
				  --------------->
				  所以j要改變
				*/
				
				color[ cell * 3 ] += c->r;
				color[ cell * 3 + 1] += c->g;
				color[ cell * 3 + 2] += c->b;
				cout<<"==============================\n";
			}
			if ( obil != NULL ) {
				color[ cell * 3 ] += obil->r;
				color[ cell * 3 + 1] += obil->g;
				color[ cell * 3 + 2] += obil->b;
			}

			
			delete c;
			delete obil;
		}
		
	}/*----------------End Indirect Lighting----------------------*/
	cout<<"Inirect Lighting結束\n";
	glDrawPixels(par->getGLWindowWidth() ,par->getGLWindowHeight() ,GL_RGB ,GL_FLOAT ,color);

}
/*得到該點的Direct light的color
  傳入：一條Ray Light的位置 要不要對某個mesh產生shadow
  傳回：Ray打到東西的顏色值 如果不需要上顏色 將會回傳NULL
*/
const Color*
Scene::getDLColor(Ray& r ,const double* lp ,bool ismirror ,int mi ) {
	int modeli = -1 ,facei = -1;
	const double* vec = RayToPoint( r ,modeli ,facei );//找到射線找出的那個點
	
	/*畫顏色*/	
	const Color* c = NULL;

	if ( vec != NULL) {
		//找出shadow ray
		Ray sr( vec , lp );//屬於線段

		//檢查shawdow ray是否與場景發生碰撞 並且紀錄sr.t值已利往後計算
		Model* model = NULL;
		for ( unsigned t = 0; t < marray.size(); t++ ) {
			model = (Model*) marray[t];
			model->collideFace(sr ,par->getNearPlane() ,par->getFarPlane() );
		}

		/*打光*/
		model = ( Model* ) marray[ modeli ];
		
		if ( sr.t < 1 ) //表示物體在光線和點之間 則此點是陰影部分 要是暗的
			c = model->getPtrColor( r ,vec ,facei ,par->getLight() , true );
		else {//打光 終於做到這一步了
			if ( ismirror && modeli == mi ) //如果有要mirror且又剛好是這個model被打到 就mirror
				c = model->getPtrColor( r ,vec ,facei ,par->getLight() , false ,true ,this);
			else c = model->getPtrColor( r ,vec ,facei ,par->getLight() ,false );
		}
		//printf("其打到的點為 %lf %lf %lf \n" ,vec[0] ,vec[1] ,vec[2] );
		//printf("其顏色值為%f %f %f %f \n" ,c->r ,c->g ,c->b ,c->a);
	}

	delete vec;
	return c;
}
/*
將某個場景進行平移動作
傳入：平移哪個場景 平移多少
*/
void 
Scene::tMeshTo(int i ,double to[3] ) {
	mesh* model = marray[i];
	
	for ( int i=0; i<model->vTotal;i++) {
		model->vList[i].ptr[0] += to[0];
		model->vList[i].ptr[1] += to[1];
		model->vList[i].ptr[2] += to[2];
	}

}



///*進行函數測試用*/
//void 
//Scene::test(Ray & r) {
//		Model* m = ( Model*) marray[0];
//		int j = m->collideFace(r ,1 ,1000);
//
//		if ( j != Model::NOTANUMBER ) {
//			cout<<"你打到的面是"<<j<<" ";
//			for ( int i = 0; i<3;i++ )
//				cout<<r.e[i] + r.t * r.b[i]<<" ";
//			cout<<"\n";
//		}
//
//	
//
//}