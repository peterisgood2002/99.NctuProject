
#include "stdafx.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "loadOBJ.h"

///////////////////////////////////////////////////////////////////////////////
// Function:	ParseString
// Purpose:		Actually breaks a string of words into individual pieces �ھڪťդ��}�r��
// Arguments:	Source string in, array to put the words and the count

///////////////////////////////////////////////////////////////////////////////
void ParseString(char *buffer,CStringArray *words,int *cnt)
{
	/*http://www.ucancode.net/Visual_C_MFC_Example/CStringArray-CString.htm*/
/// Local Variables ///////////////////////////////////////////////////////////
	CString in = buffer, temp;
///////////////////////////////////////////////////////////////////////////////
	
	in.TrimLeft();//�p�G�e�����ťմN����
	in.TrimRight();//�p�G�̫ᦳ�ťմN����
	*cnt = 0;
	do 
	{
		/*http://msdn.microsoft.com/en-US/library/5w2d5xww%28v=VS.80%29.aspx*/
		temp = in.SpanExcluding(" \t");		// GET UP TO THE NEXT SPACE OR TAB
		words->Add(temp);
		if (temp == in) break;//�p�G�o�{�@�� �h��ܥL�᭱�S���b���s���r�� �Ҧp�GTTT 
		in = in.Right(in.GetLength() - temp.GetLength());//Extracts the right part of a string.
		in.TrimLeft();
		*cnt = *cnt + 1;			
	} while (1);
	*cnt = *cnt + 1;
}
//// ParseString //////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Procedure:	LoadMaterialLib
// Purpose:		Handles the Loading of a Material library
// Arguments:	Name of the Material Library
///////////////////////////////////////////////////////////////////////////////		
void LoadMaterialLib(CString name,t_Visual *visual)
{
/// Local Variables ///////////////////////////////////////////////////////////
	int cnt;
	char buffer[MAX_STRINGLENGTH];
	CStringArray words;
	CString temp;
	FILE *fp;
///////////////////////////////////////////////////////////////////////////////
	strcpy(visual->map,"");
	fp = fopen((LPCTSTR)name,"r");
	if (fp != NULL)
	{
		// FIRST PASS SETS UP THE NUMBER OF OBJECTS IN THE FILE
		while (!feof(fp))
		{
			fgets(buffer,MAX_STRINGLENGTH,fp);	// GET A STRING FROM THE FILE
			ParseString(buffer,&words,&cnt);	// BREAK THE STRING INTO cnt WORDS
			if (cnt > 0)						// MAKE SURE SOME WORDS ARE THERE
			{
				temp = words.GetAt(0);			// CHECK THE FIRST WORK
				if (temp.GetLength() > 0)
				{
					if (temp == "Ka")			// AMBIENT
					{
						visual->Ka.r = (float)atof(words.GetAt(1));
						visual->Ka.g = (float)atof(words.GetAt(2));
						visual->Ka.b = (float)atof(words.GetAt(3));
					}
					else if (temp == "Kd")		// DIFFUSE COLOR
					{
						visual->Kd.r = (float)atof(words.GetAt(1));
						visual->Kd.g = (float)atof(words.GetAt(2));
						visual->Kd.b = (float)atof(words.GetAt(3));
					}
					else if (temp == "Ks")		// SPECULAR COLOR
					{
						visual->Ks.r = (float)atof(words.GetAt(1));
						visual->Ks.g = (float)atof(words.GetAt(2));
						visual->Ks.b = (float)atof(words.GetAt(3));
					}
					else if (temp == "Ns")		// SPECULAR COEFFICIENT
					{
						visual->Ns = (float)atof(words.GetAt(1));
					}
					else if (temp == "map_Kd")	// TEXTURE MAP NAME
					{
						strcpy(visual->map,(LPCTSTR)words.GetAt(1));
					}
				}
			}
			words.RemoveAll();		// CLEAR WORD BUFFER
		}
		fclose(fp);
	}
}

///////////////////////////////////////////////////////////////////////////////
// Procedure:	HandleFace
// Purpose:		Handles the Face Line in an OBJ file.  Extracts index info to 
//				a face Structure
// Arguments:	Array of words from the face line, place to put the data
// Notes:		Not an Official OBJ loader as it doesn't handle anything other than
//				3-4 vertex polygons.
///////////////////////////////////////////////////////////////////////////////		
void HandleFace(CStringArray *words,t_faceIndex *face)
{
/// Local Variables ///////////////////////////////////////////////////////////
	int loop,loopcnt;
	CString temp;
	CString vStr,nStr,tStr;		// HOLD POINTERS TO ELEMENT POINTERS
	int nPos,tPos;
///////////////////////////////////////////////////////////////////////////////
	loopcnt = words->GetSize();
	
	// LOOP THROUGH THE 3 - 4 WORDS OF THE FACELIST LINE, WORD 0 HAS 'f'
	for (loop = 1; loop < loopcnt; loop++)
	{
		temp = words->GetAt(loop);			// GRAB THE NEXT WORD
		// FACE DATA IS IN THE FORMAT vertex/texture/normal
		tPos = temp.Find('/');				// FIND THE '/' SEPARATING VERTEX AND TEXTURE
		vStr = temp.Left(tPos);				// GET THE VERTEX NUMBER
		temp.SetAt(tPos,' ');				// CHANGE THE '/' TO A SPACE SO I CAN TRY AGAIN
		nPos = temp.Find('/');				// FIND THE '/' SEPARATING TEXTURE AND NORMAL
		tStr = temp.Mid(tPos + 1, nPos - tPos - 1);		// GET THE TEXTURE NUMBER
		nStr = temp.Right(temp.GetLength() - nPos - 1);	// GET THE NORMAL NUMBER
		face->v[loop - 1] = atoi(vStr);		// STORE OFF THE INDEX FOR THE VERTEX �x�s����Vertex
		face->t[loop - 1] = atoi(tStr);		// STORE OFF THE INDEX FOR THE TEXTURE �x�s����Texture Coordinate
		face->n[loop - 1] = atoi(nStr);		// STORE OFF THE INDEX FOR THE NORMAL �x�s����Normal
	}
	face->flags = 0;
	if (tStr.GetLength() > 0)	face->flags |= FACE_TYPE_TEXTURE;
	if (nStr.GetLength() > 0)	face->flags |= FACE_TYPE_NORMAL;
	if (loopcnt == 4) face->flags |= FACE_TYPE_TRI;
	else if (loopcnt == 5) face->flags |= FACE_TYPE_QUAD;
	else
	{
		::MessageBox(NULL,"Face found larger then a Quad\nSubstituting a Tri","ERROR",MB_OK);
		face->flags |= FACE_TYPE_TRI;
	}
}
///// HandleFace //////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Procedure:	LoadOBJ
// Purpose:		Load an OBJ file into the current bone visual
// Arguments:	Name of 0BJ file and pointer to bone, flags of what to load
// Notes:		Not an Official OBJ loader as it doesn't handle more then
//				3 vertex polygons or multiple objects per file.
//				Current flags are only (NULL, LOADOBJ_VERTEXONLY,LOADOBJ_REUSEVERTICES)
///////////////////////////////////////////////////////////////////////////////		
BOOL LoadOBJ(char *filename,t_Visual *visual, int flags)
{
/// Local Variables ///////////////////////////////////////////////////////////
	int loop,loop2,cnt;
	char buffer[MAX_STRINGLENGTH];//�̤jBuffer�q��255
	CStringArray words;//�N�@���}���@��r��
	CString temp;
	FILE *fp;
	long vCnt = 0, nCnt = 0, tCnt = 0, fCnt = 0;
	long vPos = 0, nPos = 0, tPos = 0, fPos = 0;
	tVector *vertex = NULL,*normal = NULL,*texture = NULL;//�x�sOBJ�ɩҦ������
	t_faceIndex *face = NULL;//��ܬ��@�ӭ����X
	float *data;
	unsigned short *indexData;
///////////////////////////////////////////////////////////////////////////////
	fp = fopen(filename,"r");
	if (fp != NULL)
	{
		// FIRST PASS SETS UP THE NUMBER OF OBJECTS IN THE FILE 1.��즳�X���I�M��
		while (!feof(fp))
		{
			fgets(buffer,MAX_STRINGLENGTH,fp);	// GET A STRING FROM THE FILE
			ParseString(buffer,&words,&cnt);	// BREAK THE STRING INTO cnt WORDS
			if (cnt > 0)						// MAKE SURE SOME WORDS ARE THERE
			{
				temp = words.GetAt(0);			// CHECK THE FIRST WORK
				if (temp.GetLength() > 0)
				{
					//�p�ƾ��W�[
					if (temp[0] == 'v')			// ONLY LOOK AT WORDS THAT START WITH v 
					{
						if (temp.GetLength() > 1 && temp[1] == 'n')			// vn IS A NORMAL
							nCnt++;
						else if (temp.GetLength() > 1 && temp[1] == 't')	// vt IS A TEXTURE 
							tCnt++;
						else
							vCnt++;											// v IS A VERTEX
					}
					else if (temp[0] == 'f')
						fCnt++;												// f IS A FACE
				}
			}
			words.RemoveAll();		// CLEAR WORD BUFFER
		}

		// NOW THAT I KNOW HOW MANY, ALLOCATE ROOM FOR IT ���F�I���Ӽ� �N�i�H���w�Ŷ�
		if (vCnt > 0)
		{
			vertex = (tVector *)malloc(vCnt * sizeof(tVector));
			if (nCnt > 0)
				normal = (tVector *)malloc(nCnt * sizeof(tVector));
			if (tCnt > 0)
				texture = (tVector *)malloc(tCnt * sizeof(tVector));
			if (fCnt > 0)
				face = (t_faceIndex *)malloc(fCnt * sizeof(t_faceIndex));

			fseek(fp,0,SEEK_SET);

			// NOW THAT IT IS ALL ALLOC'ED.  GRAB THE REAL DATA �����
			while (!feof(fp))
			{
				fgets(buffer,MAX_STRINGLENGTH,fp);
				ParseString(buffer,&words,&cnt);
				if (cnt > 0)
				{
					temp = words.GetAt(0);
					if (temp.GetLength() > 0)
					{
						if (temp[0] == 'v')		// WORDS STARTING WITH v
						{
							if (temp.GetLength() > 1 && temp[1] == 'n')	// vn NORMALS
							{
								normal[nPos].x = (float)atof(words.GetAt(1));
								normal[nPos].y = (float)atof(words.GetAt(2));
								normal[nPos].z = (float)atof(words.GetAt(3));
								nPos++;
							}
							else if (temp.GetLength() > 1 && temp[1] == 't')	// vt TEXTURES
							{
								texture[tPos].u = (float)atof(words.GetAt(1));
								texture[tPos].v = (float)atof(words.GetAt(2));
								tPos++;
							}
							else											// VERTICES
							{
								vertex[vPos].x = (float)atof(words.GetAt(1));
								vertex[vPos].y = (float)atof(words.GetAt(2));
								vertex[vPos].z = (float)atof(words.GetAt(3));
								vPos++;
							}
						}
						else if (temp[0] == 'f')			// f v/t/n v/t/n v/t/n	FACE LINE
						{
							if (words.GetSize() > 5)
							{
								sprintf(buffer,"Face %d has more than 4 vertices",fPos);
								MessageBox(NULL,buffer,"ERROR",MB_OK);
							}
							HandleFace(&words,&face[fPos]);//�N��Ū�J
							fPos++;
						}
						else if (temp == "mtllib")  // HANDLE THE MATERIAL LIBRARY
						{
							LoadMaterialLib(words.GetAt(1),visual);//�NMaterial����TŪ�J �é�m�bt_Visual��
						}
					}
				}
				words.RemoveAll();		// CLEAR WORD BUFFER
			}

			/*�N��ƶ���Ƶ��ct_Visual��*/
			// THIS IS BAD.  THINGS RUN NICER IF ALL THE POLYGONS HAVE THE SAME VERTEX COUNTS
			// ASSUME ALL HAVE THE SAME AS THE FIRST.  IT SHOULD TESSELATE QUADS TO TRIS IF
			// THERE ARE SOME TRIS,  BUT I KNOW MY DATABASE SO I MAKE MY LIFE EASIER
			if ((face[0].flags & FACE_TYPE_TRI)> 0) visual->vPerFace = 3;//�P�_�O�T���Φ��@�ӭ��άO�|��Φ��@�ӭ�
			else visual->vPerFace = 4;

			if (nCnt > 0 && (flags & LOADOBJ_VERTEXONLY) == 0)//��Normal
			{
				if (tCnt > 0)//��Texture coordinate
				{
					visual->dataFormat = GL_T2F_N3F_V3F;
					visual->vSize = 8;					// 2 texture, 3 normal, 3 vertex
				}
				else//�LTexture coordinate
				{
					visual->dataFormat = GL_N3F_V3F;
					visual->vSize = 6;					// 3 floats for normal, 3 for vertex
				}
			}
			else//�ƻ򳣨S���u���I
			{
				visual->dataFormat = GL_V3F;
				visual->vSize = 3;					// 3 floats for vertex
			}
			/*�B�z�������*/
			visual->faceCnt = fPos;
			if ((flags & LOADOBJ_REUSEVERTICES) > 0)
			{
				visual->reuseVertices = TRUE;
				visual->vertexData = (float *)malloc(sizeof(float) * visual->vSize * vPos);//Vertex�����j�p
				visual->vertexCnt = vPos;//�]�wVertex�ƶq
				visual->faceIndex = (unsigned short *)malloc(sizeof(unsigned short) * fPos * visual->vPerFace);//���������j�p
				if ((flags & LOADOBJ_VERTEXONLY) > 0)		// COPY VERTEX DATA
				{
					memcpy(visual->vertexData,vertex,sizeof(float) * visual->vSize * vPos);//�N�I�i��Copy
				}
				else		// SHOULD HANDLE CASE WHERE THERE IS NORMALS AND TEXTURE COORDS
				{
					visual->vertexData = NULL;		// TODO: I DON'T WANT TO DEAL WITH IT
				}
			}
			else//�ڭ̤@��i�H�ݨ쪺�覡1/1/1 
			{
				visual->reuseVertices = FALSE;
				visual->vertexData = (float *)malloc(sizeof(float) * visual->vSize * fPos * visual->vPerFace);
				visual->vertexCnt = fPos * visual->vPerFace;
				visual->faceIndex = NULL;
			}

			/*�Iassign���F �{�b��Assign��*/
			data = visual->vertexData;
			indexData = visual->faceIndex;
			for (loop = 0; loop < fPos; loop++)
			{
				// ERROR CHECKING TO MAKE SURE 
				if ((face[loop].flags & FACE_TYPE_TRI)> 0 && visual->vPerFace == 4)
					::MessageBox(NULL,"Face Vertex Count does not match","ERROR",MB_OK);
				if ((face[loop].flags & FACE_TYPE_QUAD)> 0 && visual->vPerFace == 3)
					::MessageBox(NULL,"Face Vertex Count does not match","ERROR",MB_OK);

				/*�}�lassign�����I*/
				for (loop2 = 0; loop2 < visual->vPerFace; loop2++)
				{
					// IF I DON'T WANT TO REUSE VERTICES, FILL IT ALL OUT
					if ((flags & LOADOBJ_REUSEVERTICES) == 0)
					{
						// ALL FACE INDICES ARE 1 BASED INSTEAD OF 0
						if (tCnt > 0)	// IF TEXTURE COORDS WRITE OUT THOSE
						{
							*data++ = texture[face[loop].t[loop2] - 1].u;
							*data++ = texture[face[loop].t[loop2] - 1].v;
						}
						if (nCnt > 0)	// IF THERE ARE NORMALS WRITE THOSE OUT
						{
							*data++ = normal[face[loop].n[loop2] - 1].x;
							*data++ = normal[face[loop].n[loop2] - 1].y;
							*data++ = normal[face[loop].n[loop2] - 1].z;
						}
						*data++ = vertex[face[loop].v[loop2] - 1].x;	// SAVE OUT VERTICES
						*data++ = vertex[face[loop].v[loop2] - 1].y;
						*data++ = vertex[face[loop].v[loop2] - 1].z;
					}
					else	// REUSE VERTICES SO JUST FILL OUT THE INDEX STRUCTURE
					{
						*indexData++ = face[loop].v[loop2] - 1;
					}
				}
			}

			if (vertex) free(vertex);
			if (normal) free(normal);
			if (texture) free(texture);
			if (face) free(face);
		}

		fclose(fp);
	}
	else
		return FALSE;
	return TRUE;
}