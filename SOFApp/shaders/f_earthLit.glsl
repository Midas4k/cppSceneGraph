#version 330
in vec3 pixelPos;
in vec3 pixelNorm;
in vec2 pixelUV;
in vec3 pixelTangent;

out vec4 finalColor;
uniform vec3 lightK; // intensities for ambient, diffuse, specular
uniform float alpha; // specular power
uniform mat4 lightPos; // light position, world space
uniform vec3 cameraPos; // camera position, world space
uniform mat4 ambientColor;
uniform mat4 lightColor; // assume diffuse and specular the same
uniform vec4 attenuation; // 1/d_a**2
uniform sampler2D texture1; 
uniform sampler2D texture2;
uniform sampler2D textureSampler;
uniform sampler2D normalSampler;

void main()
{
// normals and normal mapping
	vec3 n = normalize(pixelNorm); // surface normal
	vec3 t = normalize(pixelTangent); // surface tangent 
	vec3 b = cross(t, n); // bitangent 
	vec4 normalColor = texture(normalSampler, pixelUV); 
	n = 2*(normalColor.r-0.5)*t + 2*(normalColor.g-0.5)*b + 2*(normalColor.b-0.5)*n; 
	n = normalize(n); 
	
    vec4 materialColor;
	finalColor = vec4(0.0f,0.0f,0.0f,0.0f);
    float kambient = lightK.x;
    

    for ( int i = 0; i < 4; i++ )
    {
       vec3 l = normalize(vec3(lightPos[i]) - pixelPos); // light vector
       vec3 e = normalize(cameraPos - pixelPos); // eye vector
       vec3 r = reflect(-l,n); // reflected light vector

       float d = length(vec3(lightPos[i]) - pixelPos);
       float fAttenuation = 1.0f/(1.0f + d*d*attenuation[i]);
       float kdiffuse = max(dot(l,n),0) * lightK.y * fAttenuation;
       float kspecular = pow(max(dot(r,e),0.0f),alpha) * lightK.z * fAttenuation;
       //Here we check the diffuse level to see how bright the object is
       if(kdiffuse > 0.04f){ materialColor = texture(texture1, pixelUV);} //if the pixel if bright enough we are setting the map to be the daytime (this is passed through as Texture 1)
       else{ materialColor = texture(texture2, pixelUV);} //otherwise we are setting the map to be night time (this is passed through as Texture 2)
       finalColor += materialColor*lightColor[i]*kdiffuse +kspecular*lightColor[i];
    }
    
    finalColor += materialColor *kambient;
    finalColor.a = 1.0f;
}