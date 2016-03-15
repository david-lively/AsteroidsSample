#version 150

in vec3 WorldPosition;
in vec4 Color;
out vec4 fragmentColor;

void main() {
    
    vec3 dx = normalize(dFdx(WorldPosition));
    vec3 dy = normalize(dFdy(WorldPosition));

	vec3 normal = cross(dx,dy);

	vec3 lightDirection = normalize(vec3(1,1,0));
	vec3 lightColor = vec3(1,0,1);

	float intensity = dot(normal,lightDirection);

	vec4 color = vec4(intensity  * lightColor,1);
    
    fragmentColor = color;
}
