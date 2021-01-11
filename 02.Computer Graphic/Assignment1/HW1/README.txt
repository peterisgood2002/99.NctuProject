�o�N�|�����@��GLSL�����n�[��
1.GLSL�O�Ψӭ��s�]�wVertex Shader�MFragment Shader�W
  (1)Vertex Shader�G��gTransformation�Mlighting���覡
  (2)Fragment Shader�G��gTexture Setting���覡
  
2.GLSL���T�jinput�Moutput
  (1)Uniform�G�brendering�ɳ����|���ܪ��@���ܼ�
     gl_ModelViewMatrix					4x4 Matrix representing the model-view matrix.
	 gl_ModelViewProjectionMatrix		4x4 Matrix representing the model-view-projection matrix.
	 gl_NormalMatrix					3x3 Matrix representing the inverse transpose model-view matrix.
										This matrix is used for normal transformation.
  (2)Attribute�G�u��X�{�bVertex Shader�� ����X�{�bFragment shader�W
     gl_Vertex					4D vector representing the vertex position
	 gl_Normal					3D vector representing the vertex normal
	 gl_Color					4D vector representing the vertex color
	 gl_MultiTexCoordX			4D vector representing the texture coordinate of texture unit X
  (3)Varing�G�����P�ɥX�{�bVertex Shader �MFragment Shader�W
     gl_FrontColor				4D vector representing the primitives front color
	 gl_BackColor				4D vector representing the primitives back color
	 gl_TexCoord[X]				4D vector representing the Xth texture coordinate��X��texture coordinate
	 
3.There are four main types: float, int, bool and sampler.
  (1)Samplers are types representing textures
  (2)For shader type
     gl_Position				4D vector representing the final processed vertex position. Only 
								available in vertex shader.
	 gl_FragColor				4D vector representing the final color which is written in the frame 
								buffer. Only available in fragment shader.
	 gl_FragDepth				float representing the depth which is wrtten in the depth buffer.
								Only available in fragment shader.
  (3)�o��type��곣�|������OpenGL���@�Ǩ禡
  
4.������������ഫ���覡�o�� �u���ഫ���覡 �O�����������l�ƪ��覡

5.Vector with matrix multiplication is also available.
  Matrix * Vector will threat the vector as a column-vector (OpenGL standard)
  Vector * Matrix will threat the vector as a row-vector (DirectX standard)

6.���@�ǯS�O��function�i�H��
  dot		a simple dot product
  cross		a simple cross product
  texture2D	used for sampling a texture
  normalize	normalize a vector
  clamp		clamping a vector to a minimum and a maximum

7.�{�b�i�H�z�L�|���X�R�ӱN�L�X�R��OpenGL�{���W
  (1)GL_ARB_shader_objects
  (2)GL_ARB_shading_language_100
  (3)GL_ARB_vertex_shader
  (4)GL_ARB_fragment_shader
  
8.GLSL�]���MC++�ܹ� �ҥH�]�ݭncompile link���ʧ@
  (1)A shader object represents your source code
  (2)You are able to pass your source code to a shader object and compile the shader object. 
     �NSource code���Shader object�W
     GLhandleARB glCreateShaderObjectARB(GLenum shaderType)
	 *shaderType equal to GL_VERTEX_SHADER_ARB or GL_FRAGMENT_SHADER_ARB. 
  (3)A program object represents a useable part of render pipeline.
     �N��Render Pipeline�i�ϥΪ��@����
     GLhandleARB glCreateProgramObjectARB() 
  (4)�NShader Source code�e��Shader Object��k
	 void glShaderSourceARB(GLhangleARB shader, GLuint number_strings, const GLcharARB** strings, Glint * length);
		shader is the target shader object.
     *number_strings is the number of strings you want to pass to the GL.�r��ƶq
     *strings is a pointer to a pointer of chars where the source code is saved. source code�Q�x�s���a��b����
     *length is a pointer to ints where the length of each string is saved. 
      If length is equal to NULL then the strings are assumed to be null terminated. 
  (5)Compile Source code
     void glCompileShader(GLhandleARB shader);
  (6)Link��object
     void glAttachObjectARB(GLhandleARB program, GLhandleARB shader);
  (7)�NProgram object���s��
     void glLinkProgramARB(GLhandleARB program);

9.�ϥΤ�k�G
  (1)Program Object�i�H�o�˳Q�ϥ�
     void glUseProgramObjectARB(GLhandleARB program);
	 *If program is 0 then standard fixed function OpenGL is used 
  (2)If all shaders were compiled successfully and the program object was linked successfully 
     all renderings after glUseProgramObjectARB will be done using our shader. 
  (3)�i�R��Program Object
     void glDeleteObjectARB(GLhandleARB object)

10.��L
  (1)glGetInfoLogARB(GLhandleARB object, GLsizei maxLenght, GLsizei *length, GLcharARB *infoLog)
     *object is a shader or program object.
	 *maxLenght is the maximum count of chars which will be written to infoLog.
	 *length is the count of chars which were written to InfoLog.
	 *infoLog is a pointer to chars where the info log will be saved. 
  (2)Uniforms can be passed to the GL using
     void glUniform{1|2|3|4}{f|i}ARB(GLint location, TYPE val)
     void glUniform{1|2|3|4}{f|i}vARB(GLint location, GLuint count, const TYPE * vals)
     void glUniformMatrix{2|3|4|}fvARB(GLint location, GLuint count, GLboolean transpose, const GLfloat * vals)
     *location is the location of the uniform.��m
       ���olocation����k��
       GLint glGetUniformLocationARB(GLhandleARB program, const GLcharARB * name)
	 *count is the number of values from type TYPE you want to pass.�Ȫ��ƶq
	 *val is a value from TYPE.
	 *vals is a pointer to values of TYPE (for matrices only float are available).
	 *transpose specifies if the passed matrices should be transposed. 
	 *�O������Ǥw�g���ئn��uniform
  (3)Generic Attributes are just like Uniforms:
	 void glVertexAttrib{1|2|3|4}{s|f|d}ARB(GLuint index, TYPE val)
	 void glVertexAttrib{1|2|3|4}{s|f|d}vARB(GLuint index, const TYPE * vals)
     *index is the location if the attribute.
      ���o��k�G
      GLint glGetAttribLocationARB(GLhandleARB program, const GLcharARB* name)
     *val is a value from TYPE.
     *vals is a pointer to values of TYPE (for matrices only float are available) 
     
11.�p��ϥ�Texture
   (1)Texture���|�����Q�ǶiGLSL�� ���O�ڭ̩ĥi�H�ǤJTexture unit
      ��k�p�U
      *Get the sampler uniform location.
      *Bind the texture to texture unit i.
      *Pass i as an integer by glUniform.
   (2)�d�ҡG
      glUseProgramObjectARB(my_program);
	  int my_sampler_uniform_location = glGetUniformLocationARB(my_program, "my_color_texture");

	  glActiveTexture(GL_TEXTURE0 + i);
	  glBindTexture(GL_TEXTURE_2D, my_texture_object);
	  glUniform1iARB(my_sampler_uniform_location, i);
	  
	  i is the texture unit where I want to bind my texture. 
 
     