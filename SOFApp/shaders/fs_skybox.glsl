#version 330

// very simple shader which kust transforms the position
uniform samplerCube cubeMap;
in vec3 texCoord;
out vec4 finalColor;

void main() {
	vec4 color = texture(cubeMap, texCoord);
    finalColor = color;
}
