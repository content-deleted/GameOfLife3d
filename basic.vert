#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

varying vec4 vertexColor;
varying vec3 pos;
uniform mat4 MVP;
uniform float Time;

void main(){
    pos = position;
    gl_Position = MVP * vec4(position, 1.0f);
    vertexColor = color;
}