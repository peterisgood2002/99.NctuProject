/***************************************************************************
   NAME
     Posture.h
   PURPOSE
     �ŧi�FPosture���O 

   NOTES
     �Ψ��x�s�@�j����ਤ��
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
	vector<Vector3> bone_position; //Bone���@�ɮy�Ц�m

	vector<Vector3> axisX;
	vector<Vector3> axisY;
	vector<Vector3> axisZ;
};

#endif