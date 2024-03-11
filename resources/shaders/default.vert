#version 330 core

// coordinates / positions
layout (location = 0) in vec3 aPos;
// colors
layout (location = 1) in vec3 aColor;
// texture coords
layout (location = 2) in vec2 aTex;

out vec3 color;
out vec2 texCoord;

// controls the scale of the vertices
uniform float scale;

// inputs the matrices needed for 3D viewing with perspective
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    // outputs the positions / coordinates of all vertices
    gl_Position = proj * view * model * vec4(aPos, 1.0);
    
    color = aColor;
    texCoord = aTex;
}