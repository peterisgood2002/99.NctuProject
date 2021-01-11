/******************************************
FILE
    program23.cpp
Purpose
	提供子作業二的所有能力
	2.Render the Utah teapot using Gouraud shading. (15%)
	提供子作業三的所有能力
	4.Render the Utah teapot with bump mapping.(50%)
NOTE
	

Author
    9757553 顏志晟
*******************************************/
#include "function.h"

extern GLuint texture[NTEX];
extern bool openBumpmapping;
void 
drawPolygonWithTexture() {
	//打光
	lighting();
	glClear(GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT );
	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	
	//有Texture 則設定Texture mapping的方式
	//string str[ NTEX ] ={ "colorTexture" ,"bumpMap" };
	//找Texture bind進去 Shading
	//1.Get the sampler uniform location.
	GLint location = glGetUniformLocationARB(bumpMapping, "colorTexture" );
	//2.Bind the texture to texture unit i.
	glActiveTexture(GL_TEXTURE0 );
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	//3.Pass i as an integer by glUniform.
	if ( location == -1 ) printf("Cant find texture name: colorTexture\n");
	else glUniform1iARB(location, 0);
	
	if ( openBumpmapping ) {
		//1.Get the sampler uniform location.
		GLint location = glGetUniformLocationARB(bumpMapping, "bumpMap" );
		//2.Bind the texture to texture unit i.
		glActiveTexture(GL_TEXTURE1 );
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		//3.Pass i as an integer by glUniform.
		if ( location == -1 ) printf("Cant find texture name: colorTexture\n");
	}
	//畫圖
	drawPolygon( true );
}

