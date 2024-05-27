#pragma once
#include <string>
#include <gl/glew.h>
#include "Math.h"

class Shader
{
public:
	Shader();
	~Shader();
	//指定された名前の頂点/フラグメントシェーダーを読み込む
	bool Load(const std::string& vertName,
		const std::string& fragName);
	void Unload();
	//アクティブなシェーダープログラムとして設定
	void SetActive();
	// マトリックスユニフォームを設定
	void SetMatrixUniform(const char* name, const Matrix4& matrix);


private:
	//シェーダーをコンパイルする
	bool CompileShader(const std::string& fileName,
					   GLenum shaderType,
					   GLuint& outShader);
	//シェーダーのコンパイルに成功したかの判定
	bool IsCompiled(GLuint shader);
	//頂点/フラグメントプログラムのリンクを確認
	bool IsValidProgram();
	//シェーダーオブジェクトのIDを格納
	GLuint mVertexShader;
	GLuint mFragShader;
	GLuint mShaderProgram;
};

