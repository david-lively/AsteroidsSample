#version 150

in vec3 WorldPosition;
in vec4 Color;
out vec4 fragmentColor;

void main() {
    
    vec3 dx = normalize(dFdx(WorldPosition));
    vec3 dy = normalize(dFdy(WorldPosition));
    
    
    vec4 color = vec4(dx,1);
 
    
    
    fragmentColor = color;
}
