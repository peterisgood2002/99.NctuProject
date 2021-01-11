/******************************************
FILE
    Model.cc
Purpose
    進行Model的一些相關運算 包含碰撞與否等等

NOTE
	

Author
    9757553 顏志晟
*******************************************/
#include "Model.h"
#include "math.h"
#include "function.h"
#include <iostream>

using std::cout;

/*Model*/
/*Constructor & Destructor*/
Model::Model(const char *str) : mesh(str) {
	setDefault();
}

Model::~Model() {
	mesh::~mesh();
}
/*
  算出Hitpoint相關資訊 有diffuse等值以及normal的值
  傳入： 面 以及 ap = alpha*(ab) + beta* (ac) 利用差值法算出需要的資訊
*/
void 
Model::calHitPoint(int facei ,double alpha , double beta) {
	Vertex* vec = faceList[ facei ].v;

	float* a = vList[ vec[0].v ].ptr;
	float* b = vList[ vec[1].v ].ptr;
	float* c = vList[ vec[2].v ].ptr;
	
	double gamma = 1 - alpha - beta;

	/*計算資料*/
	hp.hit = true;
	hp.ns = gamma * mat[ vec[0].m ].Ns + alpha * mat[ vec[1].m ].Ns + beta * mat[ vec[2].m ].Ns;

	double n = 0;
	for ( int i = 0; i < 4; i++ ) {//利用內插計算出normal
		hp.md[i] = gamma * mat[ vec[0].m ].Kd[i] + alpha * mat[ vec[1].m ].Kd[i] + beta * mat[ vec[2].m ].Kd[i];
		hp.ma[i] = gamma * mat[ vec[0].m ].Ka[i] + alpha  * mat[ vec[1].m ].Ka[i] + beta * mat[ vec[2].m ].Ka[i];
		hp.ms[i] = gamma * mat[ vec[0].m ].Ks[i] + alpha * mat[ vec[1].m ].Ks[i] + beta * mat[ vec[2].m ].Ks[i];
	}
	for ( int i = 0; i < 3;i++ ) {
		hp.normal[i] = gamma * nList[ vec[0].n ].ptr[i] + beta * nList[ vec[1].n ].ptr[i] + alpha * nList[ vec[2].n ].ptr[i]; 
		n += hp.normal[i] * hp.normal[i];
	}
	n = sqrt( n );

	for ( int i = 0; i < 3; i++ )//normalize
		hp.normal[i] /= n;

}
/*
  檢查是否有碰撞 並記錄t的值於Ray中
  傳入：射線 ,射線起點 射線終點
  傳出：如果碰撞傳出碰撞的face是哪一個
*/
int
Model::collideFace( Ray& r ,double tmin ,double tmax) {

	int facei = NOTANUMBER;
	double alpha = NOTANUMBER;
	double beta = NOTANUMBER;

	for ( int i = 0; i < fTotal ; i++) {
		double ta = 0 ,tb = 0;
		Vertex* vec = faceList[i].v;
		float* a = vList[ vec[0].v ].ptr;
		float* b = vList[ vec[1].v ].ptr;
		float* c = vList[ vec[2].v ].ptr;
		if ( isCollide(r ,a ,b ,c ,tmin, tmax ,ta ,tb) ) {
			r.t = tmax;
			alpha = ta;
			beta = tb;
			facei = i;
		}
		
	}
	
	if ( facei != NOTANUMBER ) {
		setDefault();
		calHitPoint(facei ,alpha , beta);
	}
	return facei;
}
/*
  偵測射線是否Collision用
  r.a + r.t * r.b = a + beta * b + alpha * c
  整理之後
  A = [a - b a - c r.b]
  A[v] = r

  Cramer's rule 
         | a-r.a a-c r.b |
  beta = | a-r.a a-c r.b |    / det(A)
		 | a-r.a a-c r.b |
  
  alpha = | a-b a-r.a r.b | /det(A)
  t = | a-b a-c a-r.a | /det(A)
*/
bool 
Model::isCollide (Ray& r ,const float* a ,const float* b ,const float* c ,double& tmin ,double& tmax ,double& alpha ,double& beta ,int as ) {

	if ( as != 3 ) return false;

	double vec1[3] ,vec2[3] ,vec3[3];//暫存資料
	//計算det(A)
	for ( int i = 0; i < as; i++ ) {
		vec1[i] = a[i] - b[i];
		vec2[i] = a[i] - c[i];
		vec3[i] = r.b[i];
	}
	double detA = det3by3 ( vec1 ,vec2 ,vec3 );
	if ( detA == 0 ) return false;//無解回傳

	//計算alpha
	for ( int i = 0; i < as; i++ ) {
		vec1[i] = a[i] - b[i];
		vec2[i] = a[i] - r.e[i];
		vec3[i] = r.b[i];
	}
	alpha = det3by3 ( vec1 ,vec2 ,vec3 ) / detA;

	//計算beta
	for ( int i = 0; i < as; i++ ) {
		vec1[i] = a[i] - r.e[i];
		vec2[i] = a[i] - c[i];
		vec3[i] = r.b[i];
	}
	beta = det3by3 ( vec1 ,vec2 ,vec3 ) / detA;

	//計算t
	for ( int i = 0; i < as; i++ ) {
		vec1[i] = a[i] - b[i];
		vec2[i] = a[i] - c[i];
		vec3[i] = a[i] - r.e[i];
	}
	double t = det3by3 ( vec1 ,vec2 ,vec3 ) / detA;

	//判斷碰撞與否
	if ( t < tmin || t > tmax || t > r.t )//t如果比目前的t還來得大就表示目前已經有一個較近的了 那就算了
		return false;
	else if ( beta < 0 || beta > 1 )
		return false;
	else if( alpha < 0 || alpha > 1 - beta )
		return false;
	else {
		tmax = t;
		return true;
	}
}

/*
  決定det(A)
  其中
       [vec1]    T
    A =[vec2] = A
	   [vec3]
*/
double
Model::det3by3(const double* vec1, const double* vec2, const double* vec3) {
	double total = 0;
	
	//正的部分
	for ( int i = 0; i < 3; i++ ) 
		total += vec1[ i%3 ] * vec2[ (i+1)%3 ] * vec3[ (i+2)%3 ];
	
	//負的部分
	for ( int i = 0; i < 3; i++ )
		total -= vec3[ i%3] * vec2[ (i+1)%3 ] * vec1[ (i+2)%3 ];

	return total;
}

/*
  透過incident ray reflection ray打Phong Light
  Phong Lighting
  diffuse = md * ld * ( r * n )
  ambient = ma * la
  specular = ms * ls * (h * n)^p
  h = ( e + l ) / |e+l|
  傳入：incident ray 打到的face點 以及光的相關設定值
        是否要mirror 若是要mirror請輸入場景
  傳出：該點的顏色值RGBA
*/
const Color*
Model::getPtrColor( Ray r ,const double* p ,int facei ,const Light& l , bool shadow ,bool ismirror ,Scene* const scene ) {

	if ( !hp.hit ) return false;//表示在其他計算中有失誤 
	Color* color = new Color();
	Vertex* vec = faceList[ facei ].v;
	

	//依據頂點距離的比值求出 該點的material係數
	/*加入Ambient*/
	const float* m_ambient = hp.ma;
	color->r = (float)m_ambient[0] * l.lambient[0];
	color->g = (float)m_ambient[1] * l.lambient[1];
	color->b = (float)m_ambient[2] * l.lambient[2];
	color->a = (float)m_ambient[3] * l.lambient[3];
		
	if ( !shadow ){
			/*
			 Phong Lighting
			  diffuse = md * ld * ( r * n )
			  ambient = ma * la
			  specular = ms * ls * (h * n)^p
			  h = ( e + l ) / |e+l|

			  light = diffuse + ambient + specular
			*/
			//計算Lighting相關參數 
			const double* normal = hp.normal;
			double ptol[3] = { 0 };//向量pl
			double ptoe[3] = { 0 };//向量pe
			double h[3] = { 0 };
			for ( int i = 0; i < 3; i++ ) {
				ptoe[i] = r.e[i] - p[i];
				h[i] = ptoe[i] + ptol[i];
			}
			double lptol = sqrt ( ptol[0] * ptol[0] + ptol[1] * ptol[1] + ptol[2] * ptol[2] );
			double ltoe = sqrt ( ptoe[0] * ptoe[0] + ptoe[1] * ptoe[1] + ptoe[2] * ptoe[2] );
			double lh = sqrt ( h[0] * h[0] + h[1] * h[1] + h[2] * h[2] );
			for ( int i = 0; i < 3; i++ ) {
				ptol[i] /= lptol;
				ptoe[i] /= ltoe;
				h[i] /= lh;
			}

			/*加入顏色*/
			if ( !ismirror) {
				float* m_specular = hp.ms;
				float* m_diffuse = hp.md;
				float m_shine = hp.ns;
				double tmp[4] = { 0 };
				double rn = innerProduct( ptoe , normal );
				if ( rn <= 0 ) rn = 0;
				double hn = innerProduct( h , normal );
				for ( int i = 0; i < 4; i++ ) {
					tmp[i] += m_diffuse[i] * l.ldiffuse[i] * rn ;
					tmp[i] += m_specular[i] * l.lspecular[i] * pow(hn ,(double) m_shine);
				}
				color->r += (float)tmp[0];
				color->g += (float)tmp[1];
				color->b += (float)tmp[2];
				color->a += (float)tmp[3];
			} 
			else {
				if (scene != NULL ) {
					/*從此點產生反射的Ray*/
					Ray tmp = reflection(p ,ptoe ,normal);
					double lp[3] = { 0 };//光線座標
					for ( int i = 0; i < 3; i++ )
						lp[i] = l.lightp[i];

					const Color* c = scene->getDLColor(tmp ,lp );
					if ( c != NULL ) {
						color->r += c->r;
						color->g += c->g;
						color->b += c->b;
					}

					delete c;
				} else throw "反射時沒有場景 將產生錯誤\n";
			}
		

		
	}

	return color;
}

/*
  透過內插法得到該點的normal值
  傳入：該點 以及該點所在的面
*/
const double* 
Model::getNormal( const double* p ,int facei ) {
	Vertex* vec = faceList[ facei ].v;

	double* normal = new double[3];
	for ( int i = 0; i < 3; i++ )
		normal[i] = (double)hp.normal[i];

	return normal;
}
/*
  透過內插法求出該點的diffuse值
  傳入：該點 以及該點所在的面
  傳出：該點的diffuse值
*/
const Color*
Model::getDiffuse( const double* p ,int facei ) {

	Vertex* vec = faceList[ facei ].v;

	double* d = new double [4];

	for ( int i = 0; i < 4; i++ )
		d[i] = (double)hp.md[i];
	Color * c = new Color();
	c->r = d[0];
	c->g = d[1];
	c->b = d[2];
	c->a = d[3];
	return c;
}
/*
  將HitPoint設成Default
*/
void 
Model::setDefault() {
	hp.hit = false;//沒有打到
	hp.ns = NOTANUMBER;
	for ( int i = 0; i < 4; i++ ) {
		hp.ma[i] = NOTANUMBER;
		hp.ms[i] = NOTANUMBER;
		hp.md[i] = NOTANUMBER;
	}
	for ( int i = 0; i < 3;i++ ) 
		hp.normal[i] = NOTANUMBER;
	
}