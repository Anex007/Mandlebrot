#version 400 core
layout(location = 0) in vec3 a_Pos;

//out vec3 fragPos;

void main()
{
    //fragPos = vec3(u_model * vec4(a_Pos, 1.0));
    gl_Position = vec4(a_Pos, 1.0);
}