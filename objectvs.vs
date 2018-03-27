#version 330 core
layout(location=0) in vec3 aPos;
layout (location = 1) in vec3 normals;
layout (location=2) in vec2 aTexCoord;

out VS_OUT{
    vec3 FragPos;
    vec3 Normals;
    vec2 texCoordinate;
    vec4 FragPosLightSpace;
} vs_out;

//out vec3 Normals;
//out vec3 FragPos;
//out vec3 LightPos;
//out vec2 texCoordinate;
//out vec4 FragPosLightSpace;

//uniform vec3 lightPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

void main()
{
    gl_Position=projection*view*model*vec4(aPos,1.0);
    vs_out.FragPos=vec3(model * vec4(aPos, 1.0));
    vs_out.Normals=transpose(inverse(mat3(model)))*normals;
    //LightPos=vec3(view*vec4(lightPos,1.0f));
    vs_out.texCoordinate=aTexCoord;
    vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
}



