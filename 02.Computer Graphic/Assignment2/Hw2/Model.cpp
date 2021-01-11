/******************************************
FILE
    Model.cc
Purpose
    �i��Model���@�Ǭ����B�� �]�t�I���P�_����

NOTE
	

Author
    9757553 �C����
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
  ��XHitpoint������T ��diffuse���ȥH��normal����
  �ǤJ�G �� �H�� ap = alpha*(ab) + beta* (ac) �Q�ήt�Ȫk��X�ݭn����T
*/
void 
Model::calHitPoint(int facei ,double alpha , double beta) {
	Vertex* vec = faceList[ facei ].v;

	float* a = vList[ vec[0].v ].ptr;
	float* b = vList[ vec[1].v ].ptr;
	float* c = vList[ vec[2].v ].ptr;
	
	double gamma = 1 - alpha - beta;

	/*�p����*/
	hp.hit = true;
	hp.ns = gamma * mat[ vec[0].m ].Ns + alpha * mat[ vec[1].m ].Ns + beta * mat[ vec[2].m ].Ns;

	double n = 0;
	for ( int i = 0; i < 4; i++ ) {//�Q�Τ����p��Xnormal
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
  �ˬd�O�_���I�� �ðO��t���ȩ�Ray��
  �ǤJ�G�g�u ,�g�u�_�I �g�u���I
  �ǥX�G�p�G�I���ǥX�I����face�O���@��
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
  �����g�u�O�_Collision��
  r.a + r.t * r.b = a + beta * b + alpha * c
  ��z����
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

	double vec1[3] ,vec2[3] ,vec3[3];//�Ȧs���
	//�p��det(A)
	for ( int i = 0; i < as; i++ ) {
		vec1[i] = a[i] - b[i];
		vec2[i] = a[i] - c[i];
		vec3[i] = r.b[i];
	}
	double detA = det3by3 ( vec1 ,vec2 ,vec3 );
	if ( detA == 0 ) return false;//�L�Ѧ^��

	//�p��alpha
	for ( int i = 0; i < as; i++ ) {
		vec1[i] = a[i] - b[i];
		vec2[i] = a[i] - r.e[i];
		vec3[i] = r.b[i];
	}
	alpha = det3by3 ( vec1 ,vec2 ,vec3 ) / detA;

	//�p��beta
	for ( int i = 0; i < as; i++ ) {
		vec1[i] = a[i] - r.e[i];
		vec2[i] = a[i] - c[i];
		vec3[i] = r.b[i];
	}
	beta = det3by3 ( vec1 ,vec2 ,vec3 ) / detA;

	//�p��t
	for ( int i = 0; i < as; i++ ) {
		vec1[i] = a[i] - b[i];
		vec2[i] = a[i] - c[i];
		vec3[i] = a[i] - r.e[i];
	}
	double t = det3by3 ( vec1 ,vec2 ,vec3 ) / detA;

	//�P�_�I���P�_
	if ( t < tmin || t > tmax || t > r.t )//t�p�G��ثe��t�٨ӱo�j�N��ܥثe�w�g���@�Ӹ��񪺤F ���N��F
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
  �M�wdet(A)
  �䤤
       [vec1]    T
    A =[vec2] = A
	   [vec3]
*/
double
Model::det3by3(const double* vec1, const double* vec2, const double* vec3) {
	double total = 0;
	
	//��������
	for ( int i = 0; i < 3; i++ ) 
		total += vec1[ i%3 ] * vec2[ (i+1)%3 ] * vec3[ (i+2)%3 ];
	
	//�t������
	for ( int i = 0; i < 3; i++ )
		total -= vec3[ i%3] * vec2[ (i+1)%3 ] * vec1[ (i+2)%3 ];

	return total;
}

/*
  �z�Lincident ray reflection ray��Phong Light
  Phong Lighting
  diffuse = md * ld * ( r * n )
  ambient = ma * la
  specular = ms * ls * (h * n)^p
  h = ( e + l ) / |e+l|
  �ǤJ�Gincident ray ���쪺face�I �H�Υ��������]�w��
        �O�_�nmirror �Y�O�nmirror�п�J����
  �ǥX�G���I���C���RGBA
*/
const Color*
Model::getPtrColor( Ray r ,const double* p ,int facei ,const Light& l , bool shadow ,bool ismirror ,Scene* const scene ) {

	if ( !hp.hit ) return false;//��ܦb��L�p�⤤�����~ 
	Color* color = new Color();
	Vertex* vec = faceList[ facei ].v;
	

	//�̾ڳ��I�Z������ȨD�X ���I��material�Y��
	/*�[�JAmbient*/
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
			//�p��Lighting�����Ѽ� 
			const double* normal = hp.normal;
			double ptol[3] = { 0 };//�V�qpl
			double ptoe[3] = { 0 };//�V�qpe
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

			/*�[�J�C��*/
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
					/*�q���I���ͤϮg��Ray*/
					Ray tmp = reflection(p ,ptoe ,normal);
					double lp[3] = { 0 };//���u�y��
					for ( int i = 0; i < 3; i++ )
						lp[i] = l.lightp[i];

					const Color* c = scene->getDLColor(tmp ,lp );
					if ( c != NULL ) {
						color->r += c->r;
						color->g += c->g;
						color->b += c->b;
					}

					delete c;
				} else throw "�Ϯg�ɨS������ �N���Ϳ��~\n";
			}
		

		
	}

	return color;
}

/*
  �z�L�����k�o����I��normal��
  �ǤJ�G���I �H�θ��I�Ҧb����
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
  �z�L�����k�D�X���I��diffuse��
  �ǤJ�G���I �H�θ��I�Ҧb����
  �ǥX�G���I��diffuse��
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
  �NHitPoint�]��Default
*/
void 
Model::setDefault() {
	hp.hit = false;//�S������
	hp.ns = NOTANUMBER;
	for ( int i = 0; i < 4; i++ ) {
		hp.ma[i] = NOTANUMBER;
		hp.ms[i] = NOTANUMBER;
		hp.md[i] = NOTANUMBER;
	}
	for ( int i = 0; i < 3;i++ ) 
		hp.normal[i] = NOTANUMBER;
	
}