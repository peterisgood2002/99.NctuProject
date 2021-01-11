/******************************************
FILE
    Struct.h
PURPOSE
    定義標準資料結構

NOTE
	

AUTHOR
    9757553 顏志晟
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


/*儲存相對應的點集合*/
struct CorPoint {
public:
	Vertex pl;
	Vertex pr;
	
};


#endif