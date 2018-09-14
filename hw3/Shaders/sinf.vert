#version 420

layout(location = 0) in vec3 invertex;
layout(location = 1) in vec3 innormal;
layout(location = 2) in vec2 intexture;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 ModelView;
uniform vec3 Light;

flat out vec2 texturecoord;

flat out vec3 N;
flat out vec3 V;
void main() {	
	V = vec3(ModelView * vec4(invertex,1.0f));
	N = normalize(mat3(transpose(inverse(ModelView))) * innormal);
	texturecoord=intexture;
	
	gl_Position = Projection * ModelView * vec4(invertex, 1.0f);	
}
