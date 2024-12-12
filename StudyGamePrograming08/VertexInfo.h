#pragma once
class VertexInfo
{
public:
	VertexInfo(const float* vertPos, const float* texCoord,
			   const float* vertColor, const unsigned int* indices,
			   unsigned int numVerts, unsigned int numIndices);
	~VertexInfo();

	// このバーテックス配列をアクティブにする（描画できるようにする）
	void SetActive();

	unsigned int GetNumVerts() const { return mNumVerts; }
	unsigned int GetNumIndices() const { return mNumIndices; }

private:	
	unsigned int mNumVerts;			// 頂点の数
	unsigned int mNumIndices;		// インデックスの数
	unsigned int mVertexArray;		// バーテックス配列オブジェクト(VAO)のOpenGL ID
	unsigned int mIndexBuffer;		// インデックスバッファ(EBO)のOpenGL ID	
	unsigned int mVertPosBuffer;	// 頂点座標バッファ(VBO)のOpenGL ID
	unsigned int mTexCoordBuffer;	// テクスチャ座標バッファ(VBO)のOpenGL ID
	unsigned int mVertColorBuffer;	// 頂点カラーバッファ(VBO)のOpenGL ID		
};