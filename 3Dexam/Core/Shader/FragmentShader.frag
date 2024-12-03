#version 330 core
out vec4 FragColor;

//the colour of the pixel
in vec3 Colour;

//the normal of the vertex
in vec3 Normal;

//the position of the fragment
in vec3 FragPos;

//the texture coordinates of the pixel
in vec2 TexCoord;

//the texture to use
uniform sampler2D OurTexture;

uniform int Flag;
uniform int LightOn;
uniform vec3 LightPos;
uniform mat4 View;

void main()
{

	//check if we are using the flat colour mode
	if(Flag == 0)
	{
		FragColor = vec4(Colour, 1.0);
	}
	//check if we are using the flat texture mode
	else if (Flag == 1)
	{
		//use the texture value
		vec4 texColor = texture(OurTexture, TexCoord);

		//check if the pixel is transparent
		if(texColor.a < 0.1)
		{
			//prevent drawing transparent pixels as black
			discard;
		}

		FragColor = texture(OurTexture, TexCoord);
	}

	//much of this code is from https://learnopengl.com/Advanced-Lighting/Basic-Lighting)

	vec3 LightColor = vec3(1);

	// ambient
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * Colour * LightColor;
  	
    // diffuse 
	vec3 lightDir = normalize(LightPos - FragPos);
    vec3 norm = normalize(Normal);
    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = diff * Colour * LightColor;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(vec3(View) - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * LightColor;  
        
    if (LightOn == 1)
	{
		vec3 result = (ambient + diffuse + specular) * vec3(FragColor);
		FragColor = vec4(result, 1.0);
	}
	else
	{
		vec3 result = (ambient) * vec3(FragColor);
		FragColor = vec4(result, 1.0);
	}
}