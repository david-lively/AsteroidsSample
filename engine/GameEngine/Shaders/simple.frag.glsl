#version 150

in vec4 Color;
in vec4 DevicePosition;
out vec4 fragmentColor;

void main() {
    float scaledDepth = DevicePosition.z / 2 + 0.5f;
    fragmentColor.rgb = vec3(scaledDepth);
}
