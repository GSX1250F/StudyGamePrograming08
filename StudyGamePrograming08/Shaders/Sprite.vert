// Request GLSL 3.3
#version 330

// ���[���h�ϊ��ƃr���[�ˉe�̂��߂�uniform�s��
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

// ����0�͈ʒu�A����1�̓e�N�X�`�����W�A����2�͒��_�J���[
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;
layout(location = 2) in vec4 inVertexColor;

// �o�͂Ƀe�N�X�`�����W��ǉ�
out vec2 fragTexCoord;

// �o�͂ɒ��_�J���[��ǉ�
out vec4 fragVertexColor;

void main()
{
	// �ʒu���W�𓯎����W�n�ɕϊ�
	vec4 pos = vec4(inPosition, 1.0);
	// �ʒu�����[���h��ԂɁA�����ăN���b�v��Ԃɕϊ�
	gl_Position = pos * uWorldTransform * uViewProj;

	// �e�N�X�`�����W���t���O�����g�V�F�[�_�[�ɓn��
	fragTexCoord = inTexCoord;

	// ���_�J���[���t���O�����g�V�F�[�_�[�ɓn��
	fragVertexColor = inVertexColor;
}
