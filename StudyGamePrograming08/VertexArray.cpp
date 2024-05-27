#include "VertexArray.h"
#include <gl/glew.h>

VertexArray::VertexArray(const float* verts, unsigned int numVerts,
	const unsigned int* indices, unsigned int numIndices)
	:mNumVerts(numVerts)
	, mNumIndices(numIndices)
{
	// 頂点配列オブジェクトを作成し、そのIDをメンバー変数mVertexArrayに保存する
	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);

	// 頂点配列オブジェクトができたら、頂点バッファを作成できる
	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(
		GL_ARRAY_BUFFER,	// バッファの種類
		numVerts * 9 * sizeof(float),	//コピーするバイト数
		verts,	//コピー元（ポインタ）
		GL_STATIC_DRAW	//このデータの利用方法
	);

	// インデックスバッファを作る。
	glGenBuffers(1, &mIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,		//インデックスバッファの指定
		numIndices * sizeof(unsigned int),		//データのサイズ
		indices, GL_STATIC_DRAW);

	// 頂点属性を指定する。
	// 今回のレイアウトは、位置座標として３個のfloat値を使う。
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,						// 属性インデックス（１つ目はゼロ）
		3,						// 要素数（ここでは3）
		GL_FLOAT,				// 要素の型
		GL_FALSE,				// 整数型のみ使用する。
		sizeof(float) * 9,		// 新しいストライドはfloat5個分
		0						// 頂点データの開始位置からこの属性までのオフセット
	);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,						// 頂点属性インデックス
		2,						// 成分の数（uvは２個）
		GL_FLOAT,				// 各成分の型
		GL_FALSE,				// GL_FLOATには使わない
		sizeof(float) * 9,		// ストライド（通常は各頂点のサイズ）
		reinterpret_cast<void*>(sizeof(float) * 3)		// オフセットポインタ
	);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2,						// 頂点属性インデックス
		4,						// 成分の数
		GL_FLOAT,				// 各成分の型
		GL_FALSE,				// GL_FLOATには使わない
		sizeof(float) * 9,		// ストライド（通常は各頂点のサイズ）
		reinterpret_cast<void*>(sizeof(float) * 5)		// オフセットポインタ
	);

}

VertexArray::~VertexArray()
{
	glDeleteBuffers(1, &mVertexBuffer);
	glDeleteBuffers(1, &mIndexBuffer);
	glDeleteVertexArrays(1, &mVertexArray);
}

void VertexArray::SetActive()
{
	glBindVertexArray(mVertexArray);
}
