#include "stdafx.h"
#include "SoftwareSkinningScene_2.h"

#include "Materials/ColorMaterial.h"
#include "Prefabs/BoneObject.h"

SoftwareSkinningScene_2::SoftwareSkinningScene_2() :
	GameScene(L"SoftwareSkinningScene_2") {}

void SoftwareSkinningScene_2::Initialize()
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

	m_pBone0->CalculateBindPose();

	GameObject* pBoxDrawer{ new GameObject{} };
	AddChild(pBoxDrawer);

	m_pMeshDrawer = pBoxDrawer->AddComponent(new MeshDrawComponent{ 24, true });
	InitializeVertices(boneSize);
}

void SoftwareSkinningScene_2::Update()
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


	const auto bone0Transform{ XMLoadFloat4x4(&m_pBone0->GetBindPose()) * XMLoadFloat4x4(&m_pBone0->GetTransform()->GetWorld()) };
	const auto bone1Transform{ XMLoadFloat4x4(&m_pBone1->GetBindPose()) * XMLoadFloat4x4(&m_pBone1->GetTransform()->GetWorld()) };

	for (int i{}; i < m_SkinnedVertices.size(); ++i)
	{
		auto& skinnedVertex = m_SkinnedVertices[i];
		const auto& curBoneTransform= i / 24 == 0 ? bone0Transform : bone1Transform;
		const auto transformedVertex= XMVector3TransformCoord(XMLoadFloat3(&skinnedVertex.originalVertex.Position), curBoneTransform);
		XMStoreFloat3(&skinnedVertex.transformedVertex.Position, transformedVertex);
	}

	m_pMeshDrawer->RemoveTriangles();
	
	for (int i{}; i < m_SkinnedVertices.size(); i += 4)
	{
		const QuadPosNormCol quad
		{
			m_SkinnedVertices[i + 0].transformedVertex,
			m_SkinnedVertices[i + 1].transformedVertex,
			m_SkinnedVertices[i + 2].transformedVertex,
			m_SkinnedVertices[i + 3].transformedVertex
		};
		m_pMeshDrawer->AddQuad(quad);
	}
	m_pMeshDrawer->UpdateBuffer();
}

void SoftwareSkinningScene_2::Draw()
{
}

void SoftwareSkinningScene_2::OnGUI()
{
	ImGui::BeginDisabled(m_autoRotate);
	ImGui::DragFloat3("Bone 0 - ROT", reinterpret_cast<float*>(&m_bone0Rot));
	ImGui::DragFloat3("Bone 1 - ROT", reinterpret_cast<float*>(&m_bone1Rot));
	ImGui::EndDisabled();

	ImGui::Checkbox("Auto Rotate", &m_autoRotate);
}

void SoftwareSkinningScene_2::PostInitialize()
{
}

void SoftwareSkinningScene_2::InitializeVertices(float length)
{
	auto pos = XMFLOAT3(length / 2.f, 0.f, 0.f);
	const auto offset = XMFLOAT3(length / 2.f, 2.5f, 2.5f);
	auto col = XMFLOAT4(1.f, 0.f, 0.f, 0.5f);
#pragma region BOX 1
	//FRONT
	XMFLOAT3 norm = { 0, 0, -1 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	//BACK
	norm = { 0, 0, 1 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	//TOP
	norm = { 0, 1, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	//BOTTOM
	norm = { 0, -1, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	//LEFT
	norm = { -1, 0, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	//RIGHT
	norm = { 1, 0, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
#pragma endregion

	col = { 0.f, 1.f, 0.f, 0.5f };
	pos = { 22.5f, 0.f, 0.f };
#pragma region BOX 2
	//FRONT
	norm = { 0, 0, -1 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	//BACK
	norm = { 0, 0, 1 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	//TOP
	norm = { 0, 1, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	//BOTTOM
	norm = { 0, -1, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	//LEFT
	norm = { -1, 0, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ -offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	//RIGHT
	norm = { 1, 0, 0 };
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, -offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, offset.z + pos.z }, norm, col);
	m_SkinnedVertices.emplace_back(XMFLOAT3{ offset.x + pos.x, -offset.y + pos.y, -offset.z + pos.z }, norm, col);
#pragma endregion
}
