#version 330

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;

uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

out vec3 fragWorldPos;
out vec3 fragNormal;
out vec2 fragTexCoord;

void main()
{
	vec4 pos = vec4(inPosition, 1.0);
	pos = pos * uWorldTransform;
	gl_Position = pos * uViewProj;
	fragWorldPos = pos.xyz;	
	
	vec4 norm = vec4(inNormal, 0.0);
	norm = norm * uWorldTransform;
	fragNormal =  norm.xyz;

	fragTexCoord = inTexCoord;
}