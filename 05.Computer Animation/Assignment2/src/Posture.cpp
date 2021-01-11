#include "StdAfx.h"
#include "Posture.h"
#include "Motion.h"
#include <fstream>
#include <string>

using namespace std;

Posture::Posture(const Posture& rhs)
{
	root_pos = rhs.root_pos;

	bone_rotation = rhs.bone_rotation;
	bone_translation = rhs.bone_translation;
	bone_length = rhs.bone_length;
}

Posture& Posture::operator =(const Posture& rhs)
{
	root_pos = rhs.root_pos;

	bone_rotation = rhs.bone_rotation;
	bone_translation = rhs.bone_translation;
	bone_length = rhs.bone_length;

	return *this;
}