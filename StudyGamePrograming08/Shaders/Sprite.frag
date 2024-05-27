// Request GLSL 3.3
#version 330

// ���_�V�F�[�_�[����̃e�N�X�`�����W����
in vec2 fragTexCoord;

// ���_�V�F�[�_�[����̒��_�J���[����
in vec4 fragVertexColor;

// �o�͐F
out vec4 outColor;

// �e�N�X�`���T���v�����O�p
uniform sampler2D uTexture;

void main()
{
	// �e�N�X�`������F���T���v�����O����
	outColor = texture(uTexture, fragTexCoord);

	// practice. �e�N�X�`���F�ƒ��_�J���[�𕽋ς���
	// outColor = (texture(uTexture, fragTexCoord) + fragVertexColor) * 0.5f;
}
