#version 420

layout(binding = 0) uniform sampler2D Texture;
in vec2 texturecoord;
out vec4 outColor;

in vec3 N;
in vec3 V;

uniform vec3 EView;
uniform mat4 View;
uniform vec3 Light;

void main() {

	vec4 texturecolor = texture2D(Texture, texturecoord);
	vec4 light_pos= View * vec4(Light,1.0);
	
	vec3 L = normalize(light_pos.xyz-V);
	vec3 E = normalize(-V);
	vec3 R = -reflect(L,N);
	
	float ambient = 0.1;
	float diffuse = max(dot(N,L), 0.0);
	float spec = pow(max(dot(R,E),0.0),15);
	float lcolor = ambient+diffuse+spec;	
	
	outColor =texturecolor*lcolor;
}