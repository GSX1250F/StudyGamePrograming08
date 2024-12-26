#version 330

struct DirectionalLight
{
	vec3 mDirection;
	vec3 mDiffuseColor;
	vec3 mSpecColor;
};

struct PointLight
{
	vec3 mPosition;
	vec3 mDiffuseColor;
	vec3 mSpecColor;
	float mAttenuation;
};

struct SpotLight
{
	vec3 mPosition;
	vec3 mDirection;
	vec3 mDiffuseColor;
	vec3 mSpecColor;
	float mAttenuation;
	float mCornAngle;
	float mFalloff;
};

in vec3 fragWorldPos;
in vec3 fragNormal;
in vec2 fragTexCoord;

const int uDirLightNum = 2;
const int uPointLightNum = 4;
const int uSpotLightNum = 4;
uniform sampler2D uTexture;
uniform vec3 uAmbientLight;
uniform DirectionalLight uDirLights[uDirLightNum];
uniform PointLight uPointLights[uPointLightNum];
uniform SpotLight uSpotLights[uSpotLightNum];
uniform vec3 uCameraPos;
uniform float uSpecPower;

out vec4 outColor;

void main()
{
	vec3 N = normalize(fragNormal);
	vec3 V = normalize(uCameraPos - fragWorldPos);
	vec3 lightColor = uAmbientLight;
	vec3 Diffuse;
	vec3 Specular;
	vec3 L;
	vec3 R;
		
	for (int i = 0; i < uDirLightNum; i++)
	{
		if (length(uDirLights[i].mDirection) > 0)
		{
			L = normalize(-uDirLights[i].mDirection);
			R = normalize(reflect(-L, N));

			Diffuse = uDirLights[i].mDiffuseColor * max(0.0, dot(N, L));
			Specular = uDirLights[i].mSpecColor * pow(max(0.0, dot(R, V)), uSpecPower);
			lightColor += Diffuse + Specular;
		}
	}
	
	vec3 pDir;
	float pLen;
	float pAtt;
	for (int i = 0; i < uPointLightNum; i++)
	{
		if (uPointLights[i].mAttenuation > 0)
		{
			pDir = uPointLights[i].mPosition - fragWorldPos;
			pLen = length(pDir) * 0.001;
			pAtt = 1.0 / (uPointLights[i].mAttenuation * pLen * pLen);
			L = normalize(pDir);
			R = normalize(reflect(-L, N));
			Diffuse = uPointLights[i].mDiffuseColor * max(0.0, dot(N, L));
			Specular = uPointLights[i].mSpecColor * pow(max(0.0, dot(R, V)), uSpecPower);
			lightColor += pAtt * (Diffuse + Specular);
		}		
	}

	vec3 sDir;
	float dAng;
	float sAtt;
	for (int i = 0; i < uSpotLightNum; i++)
	{
		if (uSpotLights[i].mAttenuation > 0)
		{
			pDir = uSpotLights[i].mPosition - fragWorldPos;
			pLen = length(pDir) * 0.001;
			pAtt = 1.0 / (uSpotLights[i].mAttenuation * pLen * pLen);
			L = normalize(pDir);
			sDir = normalize(uSpotLights[i].mDirection);
			if(dot(-L, sDir) >=0)
			{
				dAng = acos(dot(-L, sDir));
				if(dAng <= uSpotLights[i].mCornAngle / 2)
				{
					sAtt = 1.0f;
				}
				else
				{
					sAtt = pow(cos(dAng - uSpotLights[i].mCornAngle / 2) , uSpotLights[i].mFalloff);
				}				
				R = normalize(reflect(-L, N));
				Diffuse = uSpotLights[i].mDiffuseColor * max(0.0, dot(N, L));
				Specular = uSpotLights[i].mSpecColor * pow(max(0.0, dot(R, V)), uSpecPower);
				lightColor += pAtt * (Diffuse + Specular) * sAtt;
			}			
		}		
	}

	outColor = texture(uTexture, fragTexCoord) * vec4(lightColor, 1.0);	
}