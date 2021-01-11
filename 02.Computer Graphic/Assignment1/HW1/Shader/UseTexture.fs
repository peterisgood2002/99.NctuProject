uniform sampler2D colorTexture;
uniform sampler2D bumpMap;
varying vec3 normal, lightDir, eyeDir;//取線上的座標

void main ()
{
	vec4 Puv = texture2D( colorTexture, gl_TexCoord[0].xy ).rgba;
	vec4 Duv = texture2D( bumpMap, gl_TexCoord[1].xy ).rgba;
	
	
	// x座標偏移量
	vec2 uvx = vec2 ( gl_TexCoord[0].x ,  gl_TexCoord[0].y + (1.0/512.0) );//取上面的值
	float dy =  texture2D(bumpMap ,uvx ).r- Duv.r ;
	
	float deltay = abs(dy) * 2;
	//y座標偏移量
	vec2 uvy = vec2 ( gl_TexCoord[0].x + (1.0/512.0)  ,gl_TexCoord[0].y ); //取下面的值
	float dx1 =  texture2D(bumpMap ,uvy ).r - Duv.r ;
	float deltax = abs (dx1);
	//z座標便宜量
	vec3 N = normalize(normal);
	vec3 Newnormal;
	Newnormal.y = N.y + deltay;
	Newnormal.x = cos(deltax ) *N.x + sin(deltax ) * N.z;
	Newnormal.z = -sin(deltax ) * N.x + cos(deltax ) * N.z; 
	
	//進行Phong Shading
	vec4 final_color = 
	(gl_FrontLightModelProduct.sceneColor * gl_FrontMaterial.ambient) + 
	(gl_LightSource[0].ambient * gl_FrontMaterial.ambient);
							
	N = normalize ( Newnormal );
	vec3 L = normalize(lightDir);
	
	float lambertTerm = dot(N,L);
	
	if(lambertTerm > 0.0)
	{
		final_color += gl_LightSource[0].diffuse * 
		               Puv * 
					   lambertTerm;	
		
		vec3 E = normalize(eyeDir);
		vec3 R = reflect(-L, N);
		float specular = pow( max(dot(R, E), 0.0), 
		                 gl_FrontMaterial.shininess );
		final_color += gl_LightSource[0].specular * 
		               gl_FrontMaterial.specular * 
					   specular;	
	}

	gl_FragColor = final_color;		
	
}