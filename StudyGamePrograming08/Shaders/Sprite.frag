// Request GLSL 3.3
#version 330

// 頂点シェーダーからのテクスチャ座標入力
in vec2 fragTexCoord;

// 頂点シェーダーからの頂点カラー入力
in vec4 fragVertexColor;

// 出力色
out vec4 outColor;

// テクスチャサンプリング用
uniform sampler2D uTexture;

void main()
{
	// テクスチャから色をサンプリングする
	outColor = texture(uTexture, fragTexCoord);

	// practice. テクスチャ色と頂点カラーを平均する
	// outColor = (texture(uTexture, fragTexCoord) + fragVertexColor) * 0.5f;
}
