/******************************************
FILE
    Struct.h
PURPOSE
    �w�q�зǸ�Ƶ��c

NOTE
	

AUTHOR
    9757553 �C����
*******************************************/

#ifndef _STRUCT_
#define _STRUCT_

struct Vector3{
public:
	double x;
	double y;
	double z;
};

typedef Vector3 Vertex;


/*�x�s�۹������I���X*/
struct CorPoint {
public:
	Vertex pl;
	Vertex pr;
	
};


#endif