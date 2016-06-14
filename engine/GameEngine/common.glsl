#ifndef COMMON_GLSL
#define COMMON_GLSL

#define PI (3.141592f)
#define TO_RADIANS(x) (x * PI / 180)

uniform mat4 World;
uniform mat4 View;
uniform mat4 Projection;

uniform float ForceWireframe = 0;


float saturate(float val)
{
	return max(min(val, 1), 0);
}

vec3 saturate(vec3 val)
{
	return vec3(saturate(val.x), saturate(val.y), saturate(val.z));
}

vec4 saturate(vec4 val)
{
	return vec4(saturate(val.xyz), saturate(val.w));
}

#endif