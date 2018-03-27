#version 330 core

in VS_OUT{
    vec3 FragPos;
    vec3 Normals;
    vec2 texCoordinate;
    vec4 FragPosLightSpace;
}fs_in;


out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 partColor;
uniform vec3 viewPos;
uniform vec3 LightPos;
uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;
uniform bool texOn;
uniform bool shadowOn;


float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(fs_in.Normals);
    vec3 lightDir = normalize(LightPos - fs_in.FragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
    
    return shadow;
}

void main()
{
    float shadow=ShadowCalculation(fs_in.FragPosLightSpace);
    //vec3 color=partColor;
    //vec3 color=texture(horseTex,fs_in.texCoordinate).rgb;
    vec3 color;
    if (texOn)
    {
        color=texture(diffuseTexture,fs_in.texCoordinate).rgb;
    }
    else
    {
        color=partColor;
    }
    //ambient
    float ambientDegree=0.3;
    vec3 ambient=ambientDegree*color;
    
    //diffuse
    vec3 norm=normalize(fs_in.Normals);
    //vec3 lightDir=normalize(fs_in.FragPos-LightPos);
    vec3 lightDir=normalize(LightPos-fs_in.FragPos);
    //vec3 lightDir=normalize(-LightPos);
    //vec3 lightDir=vec3(0,-24,0);
    float diff=max(dot(lightDir,norm),0.0);
    vec3 diffuse=diff*lightColor;
    
    //specular
    //float specularStrength=0.5f;
    vec3 viewDir=normalize(viewPos-fs_in.FragPos);
    vec3 reflectDir=reflect(-lightDir,norm);
    float spec=0.0;
    vec3 halfwayDir=normalize(lightDir+viewDir);
    spec=pow(max(dot(norm,halfwayDir),0.0),64.0);
    vec3 specular=spec*lightColor;
    
    
    //vec3 lightResult=(ambient+diffuse+specular)*color;
    vec3 lightResult;
    if(shadowOn)
    {
        lightResult=(ambient+(1.0-shadow)*(diffuse+specular))*color;
    }
    else
    {
        lightResult=(ambient+diffuse+specular)*color;
    }
    
    FragColor = vec4(lightResult, 1.0);
    
}




