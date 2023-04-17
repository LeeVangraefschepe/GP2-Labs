#include "stdafx.h"
#include "SoftwareSkinningScene_1.h"

#include "Materials/ColorMaterial.h"
#include "Prefabs/BoneObject.h"

SoftwareSkinningScene_1::SoftwareSkinningScene_1() :
	GameScene(L"SoftwareSkinningScene_1") {}

void SoftwareSkinningScene_1::Initialize()
{
	m_SceneContext.settings.showInfoOverlay = true;
	m_SceneContext.settings.drawPhysXDebug = true;
	m_SceneContext.settings.drawGrid = true;
	m_SceneContext.settings.enableOnGUI = true;

	const auto pMaterial = MaterialManager::Get()->CreateMaterial<ColorMaterial>();
	pMaterial->SetColor(XMFLOAT4{ 1.f, 1.f, 1.f, 1.0f });

	GameObject* pRoot{ new GameObject{} };
	AddChild(pRoot);

	constexpr float boneSize{ 15.f };

	m_pBone0 = new BoneObject{ pMaterial, boneSize };
	pRoot->AddChild(m_pBone0);

	m_pBone1 = new BoneObject{ pMaterial, boneSize };
	m_pBone0->AddBone(m_pBone1);
}

void SoftwareSkinningScene_1::Update()
{
	if (m_autoRotate)
	{
		constexpr float rotationSpeed{ 45.0f };
		constexpr float maxAngle{ 45.0f };
		m_BoneRotation += rotationSpeed * static_cast<float>(m_RotationSign) * m_SceneContext.pGameTime->GetElapsed();

		if (abs(m_BoneRotation) > maxAngle)
		{
			m_BoneRotation -= static_cast<float>(m_RotationSign) * (abs(m_BoneRotation) - maxAngle);
			m_RotationSign = -m_RotationSign;
		}

		m_bone0Rot = XMFLOAT3{ 0.0f, 0.0f, m_BoneRotation };
		m_bone1Rot = XMFLOAT3{ 0.0f, 0.0f, -m_BoneRotation * 2.0f };
	}

	m_pBone0->GetTransform()->Rotate(m_bone0Rot);
	m_pBone1->GetTransform()->Rotate(m_bone1Rot);
}

void SoftwareSkinningScene_1::Draw()
{
}

void SoftwareSkinningScene_1::OnGUI()
{
	ImGui::BeginDisabled(m_autoRotate);
	ImGui::DragFloat3("Bone 0 - ROT", reinterpret_cast<float*>(&m_bone0Rot));
	ImGui::DragFloat3("Bone 1 - ROT", reinterpret_cast<float*>(&m_bone1Rot));
	ImGui::EndDisabled();

	ImGui::Checkbox("Auto Rotate", &m_autoRotate);
}

void SoftwareSkinningScene_1::PostInitialize()
{
}