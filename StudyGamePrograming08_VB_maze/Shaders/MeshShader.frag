#version 330

struct DirectionalLight
{
	vec3 mDirection;
	vec3 mDiffuseColor;
	vec3 mSpecColor;
};

in vec3 fragWorldPos;
in vec3 fragNormal;
in vec2 fragTexCoord;

uniform sampler2D uTexture;
uniform vec3 uAmbientLight;
uniform DirectionalLight uDirLight;
uniform vec3 uCameraPos;
uniform float uSpecPower;

out vec4 outColor;

void main()
{
	vec3 N = normalize(fragNormal);
    vec3 L = normalize(-uDirLight.mDirection);
	vec3 V = normalize(uCameraPos - fragWorldPos);
	vec3 R = normalize(reflect(-L, N));

	vec3 lightColor = uAmbientLight;
	vec3 Diffuse = uDirLight.mDiffuseColor * max(0.0, dot(N, L));
	vec3 Specular = uDirLight.mSpecColor * pow(max(0.0, dot(R, V)), uSpecPower);
	lightColor += Diffuse + Specular;
	
	outColor = texture(uTexture, fragTexCoord) * vec4(lightColor, 1.0);	
}