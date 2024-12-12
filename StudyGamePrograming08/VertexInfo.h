#pragma once
class VertexInfo
{
public:
	VertexInfo(const float* vertPos, const float* texCoord,
			   const float* vertColor, const unsigned int* indices,
			   unsigned int numVerts, unsigned int numIndices);
	~VertexInfo();

	// ���̃o�[�e�b�N�X�z����A�N�e�B�u�ɂ���i�`��ł���悤�ɂ���j
	void SetActive();

	unsigned int GetNumVerts() const { return mNumVerts; }
	unsigned int GetNumIndices() const { return mNumIndices; }

private:	
	unsigned int mNumVerts;			// ���_�̐�
	unsigned int mNumIndices;		// �C���f�b�N�X�̐�
	unsigned int mVertexArray;		// �o�[�e�b�N�X�z��I�u�W�F�N�g(VAO)��OpenGL ID
	unsigned int mIndexBuffer;		// �C���f�b�N�X�o�b�t�@(EBO)��OpenGL ID	
	unsigned int mVertPosBuffer;	// ���_���W�o�b�t�@(VBO)��OpenGL ID
	unsigned int mTexCoordBuffer;	// �e�N�X�`�����W�o�b�t�@(VBO)��OpenGL ID
	unsigned int mVertColorBuffer;	// ���_�J���[�o�b�t�@(VBO)��OpenGL ID		
};