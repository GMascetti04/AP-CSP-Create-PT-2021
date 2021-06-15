#version 330 core
layout(location = 0) in vec4 aPos;

layout(location = 1) in vec4 Color;

uniform mat4 u_MVP;

out vec4 v_Color;

void main()
{
    gl_Position = u_MVP * aPos;
    v_Color = Color;
    
}