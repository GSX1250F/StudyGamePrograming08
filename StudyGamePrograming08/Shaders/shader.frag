#version 330

out vec4 outColor;

in vec2 fragTexCoord;
in vec4 fragVertColor;

uniform sampler2D uTexture;

void main()
{
	// �e�N�X�`������F���T���v�����O����
	outColor = texture(uTexture, fragTexCoord);
	//outColor =  fragVertColor;
}