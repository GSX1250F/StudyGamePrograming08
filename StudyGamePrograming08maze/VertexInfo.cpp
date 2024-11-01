#include "VertexInfo.h"
#include <glew.h>

VertexInfo::VertexInfo(const float* verts, unsigned int numVerts,
					   const unsigned int* indices,unsigned int numIndices)
	: mNumVerts(numVerts)
	, mNumIndices(numIndices)
{
	// バーテックス配列オブジェクトをOpenGLに生成し、そのIDをメンバー変数mVertexArrayに保存する
	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);

	// インデックスバッファをOpenGLに生成し、そのIDをメンバー変数mIndexBufferに保存する
	glGenBuffers(1, &mIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,			// インデックスバッファの指定
		mNumIndices * sizeof(unsigned int),	// データのサイズ
		indices,							// コピー元の配列
		GL_STATIC_DRAW						// このデータを1回だけロードしてその後頻繁に使うとき
	);

	//VertexAttribute layout0 = position
	unsigned int cnt = 8;					//要素数	
	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(
		GL_ARRAY_BUFFER,							// バッファの種類にバーテックスバッファを指定
		cnt * mNumVerts * sizeof(float),			// コピーするバイト数
		verts,									// コピー元の配列
		GL_STATIC_DRAW								// このデータを1回だけロードしてその後頻繁に使うとき
	);
	glVertexAttribPointer(
		0,											// 属性インデックス（１つ目はゼロ）
		3,											// 要素数
		GL_FLOAT,									// 要素の型
		GL_FALSE,									// 整数型のみ使用する。
		cnt * sizeof(float),						// ストライド（通常は各バーテックス属性のデータ数
		0											// 頂点データの開始位置からこの属性までのオフセット
	);
	glEnableVertexAttribArray(0);

	// VertexAttribute layout1 = Normal Vector
	glVertexAttribPointer(
		1,											// 属性インデックス（１つ目はゼロ）
		3,											// 要素数
		GL_FLOAT,									// 要素の型
		GL_FALSE,									// 整数型のみ使用する。
		cnt * sizeof(float),						// ストライド（通常は各バーテックス属性のデータ数
		reinterpret_cast<void*>(sizeof(float) * 3)	// 頂点データの開始位置からこの属性までのオフセット
	);
	glEnableVertexAttribArray(1);

	// VertexAttribute layout2 = TexCoord
	glVertexAttribPointer(
		2,											// 属性インデックス（１つ目はゼロ）
		2,											// 要素数
		GL_FLOAT,									// 要素の型
		GL_FALSE,									// 整数型のみ使用する。
		cnt * sizeof(float),						// ストライド（通常は各バーテックス属性のデータ数
		reinterpret_cast<void*>(sizeof(float) * 6)	// 頂点データの開始位置からこの属性までのオフセット
	);
	glEnableVertexAttribArray(2);

}


VertexInfo::~VertexInfo()
{
	glDeleteBuffers(1, &mVertexBuffer);
	glDeleteBuffers(1, &mIndexBuffer);
	glDeleteVertexArrays(1, &mVertexArray);
}

void VertexInfo::SetActive()
{
	glBindVertexArray(mVertexArray);
}