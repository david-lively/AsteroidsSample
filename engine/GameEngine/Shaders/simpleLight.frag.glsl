#version 150

uniform mat4 View;

uniform float ForceWireframe = 0;

in vec4 WorldPosition;
out vec4 fragmentColor;

//vec3 ProcessLights(vec3 normal)
//{
//	vec4 color;
//
//	for (int i = 0; i < LightCount; ++i)
//	{
//		vec4 colorIntensity = LightColorIntensity[i];
//		vec4 position = vec4(0, 0, 0, 1);
//
//		position = View * LightTransform[i] * position;
//
//		vec3 direction = normalize(position.xyz - WorldPosition.xyz);
//
//		float intensity = dot(normal, direction);
//		color.rgb += intensity * colorIntensity.rgb;
//	}
//
//	color.a = 1;
//
//	return color;
//}


void main() {
	fragmentColor = vec4(1, 0, 1, 1);


	vec3 fragmentPosition = WorldPosition.xyz;

    vec3 dx = normalize(dFdx(fragmentPosition));
    vec3 dy = normalize(dFdy(fragmentPosition));

	vec3 normal = cross(dx,dy);

	vec3 lightDirect = vec3(-1, -1, 1);
	float intensity = dot(lightDirect, normal);

	vec4 lightColor = vec4(1, 1, 0, 1);

	fragmentColor = lightColor * intensity;
	//vec3 color = ProcessLights(normal);
	//
	//float force = clamp(ForceWireframe, 0, 1);

	//color.rgb = (1 - force) * color + force * vec3(1);

	//fragmentColor = vec4(color,1);
}
