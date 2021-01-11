varying vec3 normal, lightDir, eyeDir ;

void main()
{	
	normal = gl_NormalMatrix * gl_Normal;

	vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);
	//screenxy = vec2 ( glModelViewProjectionMatrix * gl_Vertex );

	lightDir = vec3(gl_LightSource[0].position.xyz - vVertex);
	eyeDir = -vVertex;

	gl_TexCoord[0].xy = gl_MultiTexCoord0.xy;//�NTexture coordinate�M���жb��X�_��
	gl_TexCoord[1].xy = gl_MultiTexCoord1.xy;
	
	gl_Position = ftransform();//�i�H���ͩMOpen GL���ͪ��I�t���h���I�X��
}