#version 330

layout(location=0) in vec3 vertex;
out vec3 texCoord;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position=projection*view*model*vec4(vertex,1.0);
    texCoord = vertex;
}
