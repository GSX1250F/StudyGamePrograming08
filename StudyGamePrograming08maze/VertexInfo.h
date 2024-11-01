#pragma once
class VertexInfo
{
public:
	VertexInfo(const float* verts, unsigned int numVerts,
		       const unsigned int* indices,unsigned int numIndices);
	~VertexInfo();

	// このバーテックス配列をアクティブにする（描画できるようにする）
	void SetActive();

	unsigned int GetNumVerts() const { return mNumVerts; }
	unsigned int GetNumIndices() const { return mNumIndices; }

private:
	unsigned int mVertexArray;		// バーテックス配列オブジェクトのOpenGL ID
	unsigned int mNumVerts;			// 頂点の数
	unsigned int mNumIndices;		// インデックスの数	
	unsigned int mVertexBuffer;		// 頂点バッファのOpenGL ID	
	unsigned int mIndexBuffer;		// インデックスバッファのOpenGL ID	
};