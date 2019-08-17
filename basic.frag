#version 330

in vec4 vertexColor;
in vec3 pos;
uniform float Time;

void main(){
    gl_FragColor  = (sin(Time)*pos.y)/100+vertexColor + vec4(sin(Time)/5, sin(Time)/2, cos(Time)/3, 0);
}