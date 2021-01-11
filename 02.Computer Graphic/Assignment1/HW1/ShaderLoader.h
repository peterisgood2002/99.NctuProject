#include <stdio.h>
#include <stdlib.h>
#include "glew.h"
#include "wglew.h"
#include "glut.h"

bool ShaderLoad(GLhandleARB h_program, char* shader_file, GLenum shader_type)
{
	FILE *fp;
	GLhandleARB h_shader;
	GLcharARB *shader_string;
	GLint str_length, maxLength;
	GLint isCompiled = GL_FALSE, isLinked = GL_FALSE;
	GLcharARB *pInfoLog;

	// open the file of shader source code
	if((fp=fopen(shader_file, "r")) == NULL)
	{
		fprintf(stderr, "Error : Failed to read the OpenGL shader source \"%s\".\n", shader_file);
		return false;
	}

	// allocate memory for program string and load it.將所有source code全部讀入
	shader_string = (GLcharARB*) malloc(sizeof(GLcharARB) * 65536);
	str_length = (GLint) fread(shader_string, 1, 65536, fp);
	fclose(fp);

	// Create and load shader string. 將Source code放到Shadre object上
	h_shader = glCreateShaderObjectARB(shader_type);//根據Shader Type建立我們想要的Shader Object
	if(h_shader == 0)
	{
		fprintf(stderr, "Error : Failed to create OpenGL shader object \"%s\".\n", shader_file);
		return false;
	}
	//將Shader Source code送到Shader Object方法
	glShaderSourceARB(h_shader, 1, (const GLcharARB**)&shader_string, &str_length);
	free(shader_string);

	// Compile the vertex shader, print out the compiler log message.
	glCompileShaderARB(h_shader);//Compile Source code

	// get compile state information
	glGetObjectParameterivARB(h_shader, GL_OBJECT_COMPILE_STATUS_ARB, &isCompiled);

	if(!isCompiled)
	{
		fprintf(stderr, "Error : Failed to compile OpenGL shader source \"%s\".\n", shader_file);
		glGetObjectParameterivARB(h_shader, GL_OBJECT_INFO_LOG_LENGTH_ARB, &maxLength);
		pInfoLog = (GLcharARB *) malloc(maxLength * sizeof(GLcharARB));
		glGetInfoLogARB(h_shader, maxLength, &str_length, pInfoLog);
		fprintf(stderr, "%s\n", pInfoLog);
		free(pInfoLog);
		return false;
	}
	glAttachObjectARB(h_program, h_shader);//Link到object

	// delete the shader object, since we have attached it with the program object.
	glDeleteObjectARB(h_shader);

	// Link the program and print out the linker log message
	glLinkProgramARB(h_program);//將Program object做連結
	glGetObjectParameterivARB(h_program, GL_OBJECT_LINK_STATUS_ARB, &isLinked);

	if(!isLinked)
	{
		fprintf(stderr, "Error : Failed to link OpenGL shader \"%s\".\n", shader_file);
		glGetObjectParameterivARB(h_program, GL_OBJECT_INFO_LOG_LENGTH_ARB, &maxLength);
		pInfoLog = (GLcharARB *) malloc(maxLength * sizeof(GLcharARB));
		glGetInfoLogARB(h_program, maxLength, &str_length, pInfoLog);
		fprintf(stderr, "%s\n", pInfoLog);
		free(pInfoLog);
		return false;
	}

	return true;
}
