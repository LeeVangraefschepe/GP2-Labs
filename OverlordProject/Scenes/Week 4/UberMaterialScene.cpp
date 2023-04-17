#include "stdafx.h"
#include "UberMaterialScene.h"

#include "Materials/UberMaterial.h"
#include "Prefabs/SpherePrefab.h"

UberMaterialScene::UberMaterialScene() :
	GameScene(L"UberMaterialScene") {}

void UberMaterialScene::Initialize()
{
	m_SceneContext.settings.showInfoOverlay = true;
	m_SceneContext.settings.drawPhysXDebug = false;
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.enableOnGUI = true;

	//auto& pPhysx = PxGetPhysics();
	//const auto pDefaultMaterial = pPhysx.createMaterial(0.5f, 0.5f, 0.2f);

	m_pSphere = new GameObject{};
	m_pMaterial = MaterialManager::Get()->CreateMaterial<UberMaterial>();
	const auto pModel = m_pSphere->AddComponent(new ModelComponent{ L"Meshes/Sphere.ovm" });
	pModel->SetMaterial(m_pMaterial);
	AddChild(m_pSphere);
	m_pSphere->GetTransform()->Scale(5.f);

	m_pCamera = new FixedCamera();
	m_pCamera->GetTransform()->Translate(0, 0, -15);
	m_pCamera->GetTransform()->Rotate(0, 0, 0),
	AddChild(m_pCamera)->GetComponent<CameraComponent>()->SetActive();
}

void UberMaterialScene::Update()
{
	const float totalTime = m_SceneContext.pGameTime->GetTotal();
	m_pSphere->GetTransform()->Rotate(0.f, totalTime, 0.f);
}

void UberMaterialScene::Draw()
{
}

void UberMaterialScene::OnGUI()
{
	ImGui::ColorEdit3("Demo ClearColor", &m_SceneContext.settings.clearColor.x, ImGuiColorEditFlags_NoInputs);
	m_pMaterial->DrawImGui();
}

void UberMaterialScene::PostInitialize()
{
}