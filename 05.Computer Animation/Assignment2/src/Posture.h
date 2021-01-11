/***************************************************************************
   NAME
     Posture.h
   PURPOSE
     宣告了Posture類別 

   NOTES
     用來儲存一大堆旋轉角度
   AUTHOR
     
   HISTORY
     Zhi-Cheng Yan - Match 3, 2010: Modified.
***************************************************************************/

#ifndef POSTURE_H
#define POSTURE_H

#include <vector>
#include "Math3D.h"

using namespace std;

class Posture
{
public:
	Posture() {}
	Posture(const Posture&);
	~Posture() {}
	
	Posture& operator =(const Posture& rhs);							// assign operator

public:
	Vector3 root_pos;													// the root position

	vector<Vector3> bone_rotation;
	vector<Vector3> bone_translation;
	vector<Vector3> bone_length;
	vector<Vector3> bone_position; //Bone的世界座標位置

	vector<Vector3> axisX;
	vector<Vector3> axisY;
	vector<Vector3> axisZ;
};

#endif