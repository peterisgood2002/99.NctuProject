varying vec3 normal, lightDir, eyeDir ;

void main()
{	
	normal = gl_NormalMatrix * gl_Normal;

	vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);
	//screenxy = vec2 ( glModelViewProjectionMatrix * gl_Vertex );

	lightDir = vec3(gl_LightSource[0].position.xyz - vVertex);
	eyeDir = -vVertex;

	gl_TexCoord[0].xy = gl_MultiTexCoord0.xy;//將Texture coordinate和坐標軸整合起來
	gl_TexCoord[1].xy = gl_MultiTexCoord1.xy;
	
	gl_Position = ftransform();//可以產生和Open GL產生的點差不多的點出來
}