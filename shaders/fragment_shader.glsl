#version 460 core
out vec4 FragColor;
in vec3 vertexColor; // input variable from vs (same name and type)
void main()
{
    FragColor = vec4(vertexColor, 1.0);
}