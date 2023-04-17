#include "stdafx.h"
#include "BoneObject.h"

BoneObject::BoneObject(BaseMaterial* pMaterial, float length) : m_pMaterial(pMaterial), m_length(length)
{
}
void BoneObject::AddBone(BoneObject* pBone)
{
	pBone->GetTransform()->Translate(m_length, 0.0f, 0.0f);
	AddChild(pBone);
}
void BoneObject::CalculateBindPose()
{
	const auto inverseWorldMatrix = XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&GetTransform()->GetWorld()));
	XMStoreFloat4x4(&m_bindPose, inverseWorldMatrix);

	for (BoneObject* pChild : GetChildren<BoneObject>())
	{
		pChild->CalculateBindPose();
	}
}
void BoneObject::Initialize(const SceneContext&)
{
	const auto pEmpty = new GameObject{};
	AddChild(pEmpty);

	const auto pModel = pEmpty->AddComponent(new ModelComponent{ L"Meshes/Bone.ovm" });
	pModel->SetMaterial(m_pMaterial);

	const auto pTransform = pEmpty->GetTransform();
	pTransform->Rotate(0.f, -90.f, 0.f);
	pTransform->Scale(m_length);
}