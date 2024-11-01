#include "MeshComponent.h"
#include "Shader.h"
#include "Mesh.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexInfo.h"

MeshComponent::MeshComponent(Actor* owner)
	:Component(owner)
	, mMesh(nullptr)
	, mTextureIndex(0)
	, mVisible(true)
{
	mOwner->GetGame()->GetRenderer()->AddMeshComp(this);
}

MeshComponent::~MeshComponent()
{
	mOwner->GetGame()->GetRenderer()->RemoveMeshComp(this);
}

void MeshComponent::Draw(Shader* shader)
{
	if (mMesh && mVisible)
	{
		// ワールド座標変換の設定
		shader->SetMatrixUniform("uWorldTransform", mOwner->GetWorldTransform());		
		// テクスチャの設定
		Texture* t = mMesh->GetTexture(mTextureIndex);
		if (t)
		{
			t->SetActive();
		}
		// 鏡面反射指数の設定 
		float sp = mMesh->GetSpecPower();
		shader->SetFloatUniform("uSpecPower", mMesh->GetSpecPower());
		// メッシュの頂点情報クラスをアクティブにする。
		VertexInfo* vi = mMesh->GetVertexInfo();
		vi->SetActive();
		// Draw
		glDrawElements(GL_TRIANGLES, vi->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}
