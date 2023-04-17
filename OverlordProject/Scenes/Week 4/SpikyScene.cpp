#include "stdafx.h"
#include "SpikyScene.h"
#include "Materials/SpikyMaterial.h"

SpikyScene::SpikyScene() :
	GameScene(L"SpikyScene") {}

void SpikyScene::Initialize()
{
	m_SceneContext.settings.showInfoOverlay = true;
	m_SceneContext.settings.drawPhysXDebug = false;
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.enableOnGUI = true;

	m_pSphere = new GameObject{};
	m_pMaterial = MaterialManager::Get()->CreateMaterial<SpikyMaterial>();
	const auto pModel = m_pSphere->AddComponent(new ModelComponent{ L"Meshes/Sphere.ovm" });
	pModel->SetMaterial(m_pMaterial);
	AddChild(m_pSphere);
	m_pSphere->GetTransform()->Scale(5.f);

	m_pCamera = new FixedCamera();
	m_pCamera->GetTransform()->Translate(0, 0, -15);
	m_pCamera->GetTransform()->Rotate(0, 0, 0),
		AddChild(m_pCamera)->GetComponent<CameraComponent>()->SetActive();
}

void SpikyScene::Update()
{
	const float totalTime = m_SceneContext.pGameTime->GetTotal();
	m_pSphere->GetTransform()->Rotate(0.f, totalTime, 0.f);
	m_pMaterial->SetSpikeLength(spikeLength);
}

void SpikyScene::Draw()
{
}

void SpikyScene::OnGUI()
{
	ImGui::ColorEdit3("Demo ClearColor", &m_SceneContext.settings.clearColor.x, ImGuiColorEditFlags_NoInputs);
	ImGui::SliderFloat("Spike length", &spikeLength, 0.f, 1.f);
}

void SpikyScene::PostInitialize()
{
}