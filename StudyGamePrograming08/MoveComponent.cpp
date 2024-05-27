#include "MoveComponent.h"
#include "Actor.h"
#include "Math.h"

MoveComponent::MoveComponent(Actor* owner, int updateOrder) 
	: Component(owner), 
	mVelocity(Vector2::Zero),		// èdêSà⁄ìÆë¨ìx
	mRotSpeed(0.0f),				// âÒì]ë¨ìx
	mMass(1.0f),					// éøó 
	mMoveForce(Vector2::Zero),		// èdêSÇ…Ç©Ç©ÇÈóÕ
	mMoveAccel(Vector2::Zero),		// èdêSâ¡ë¨ìx	=èdêSÇ…Ç©Ç©ÇÈóÕ / éøó 
	mRotForce(0.0f),				// âÒì]ï˚å¸ÇÃóÕF +ï˚å¸ÇÕCCW
	mRotAccel(0.0f),				// âÒì]â¡ë¨ìx
	mMoveResist(0.0f),				// èdêSë¨ìxíÔçRó¶(%)
	mRotResist(0.0f),				// âÒì]ë¨ìxíÔçRó¶(%)
	mTorque(0.0f),					// ÉgÉãÉN=âÒì]ï˚å¸ÇÃóÕ * îºåa = äµê´ÉÇÅ[ÉÅÉìÉg * âÒì]â¡ë¨ìx
	mImoment(0.0f)					// äµê´ÉÇÅ[ÉÅÉìÉg
{
	
}

MoveComponent::~MoveComponent()
{
}

void MoveComponent::Update(float deltatime)
{
	// ActorÇÃà íuÇçXêV
	mOwner->SetPosition(mOwner->GetPosition() + mVelocity * deltatime);		//x = xo + vt
	// ActorÇÃï˚å¸ÇçXêV
	mOwner->SetRotation(mOwner->GetRotation() + mRotSpeed * deltatime);		//É¶ = É¶o + É÷t

	// èdêSë¨ìxÇçXêV
	if (!Math::NearZero(mMass)) 
	{
		//èdêSâ¡ë¨ìxÇÃåvéZÅ@F=ma  a=F*(1/m)
		mMoveAccel = mMoveForce * (1.0f / mMass);		
		//íÔçRóÕ = ë¨Ç≥*íÔçRåWêî    å∏ë¨ = -ë¨Ç≥*íÔçRåWêî/éøó 
		Vector2 movedecel = mVelocity * mMoveResist * 0.01f * (1 / mMass);
		mMoveAccel -= movedecel;
	}
	else { mMoveAccel = Vector2::Zero; }
	
	// ï˚å¸ÇçXêV
	// äµê´ÉÇÅ[ÉÅÉìÉgåvéZ	 Å¶2éüå≥Ç…Ç®Ç¢ÇƒÇÕÅAàÍólñßìxÇÃâ~î¬Ç∆Ç∑ÇÈÅB I=0.5*éøó *îºåa^2
	mImoment = 0.5f * mMass * mOwner->GetRadius() * mOwner->GetRadius();
	if (!Math::NearZero(mImoment)) 
	{
		// ÉgÉãÉNåvéZÅ@Å@ÉgÉãÉN=âÒì]ï˚å¸ÇÃóÕ * îºåa
		mTorque = mRotForce * mOwner->GetRadius();
		// âÒì]â¡ë¨ìxÇÃåvéZÅ@âÒì]â¡ë¨ìx = ÉgÉãÉN / äµê´ÉÇÅ[ÉÅÉìÉg
		mRotAccel = mTorque / mImoment;		//âÒì]â¡ë¨ìxÇÃåvéZ Fr=Ia  a=Fr/I
		//íÔçRóÕ = ë¨Ç≥*íÔçRåWêî    å∏ë¨ = -ë¨Ç≥*íÔçRåWêî*îºåa/äµê´ÉÇÅ[ÉÅÉìÉg
		float rotdecel = mRotSpeed * mOwner->GetRadius() * mRotResist / mImoment;
		mRotAccel -= rotdecel;
	}
	else { mRotAccel = 0.0f; }
	mVelocity += mMoveAccel * deltatime;	//v = vo + at
	mRotSpeed += mRotAccel * deltatime;		//É÷ = É÷o + bt

}
