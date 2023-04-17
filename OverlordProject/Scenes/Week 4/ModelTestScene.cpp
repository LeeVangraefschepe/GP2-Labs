#include "stdafx.h"
#include "ModelTestScene.h"

#include "Materials/ColorMaterial.h"
#include "Materials/DiffuseMaterial.h"

ModelTestScene::ModelTestScene() :
	GameScene(L"ModelTestScene") {}

void ModelTestScene::Initialize()
{
	//m_SceneContext.settings.showInfoOverlay = true;
	//m_SceneContext.settings.drawPhysXDebug = true;
	//m_SceneContext.settings.drawGrid = true;
	m_SceneContext.settings.enableOnGUI = true;

	auto& pPhysx = PxGetPhysics();
	const auto pBouncyMat = pPhysx.createMaterial(.5f, .5f, 1.f);

	//ground plane
	GameSceneExt::CreatePhysXGroundPlane(*this, pBouncyMat);

	DiffuseMaterial* pMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	//const UINT materialId = pMaterial->GetMaterialId();
	//auto pColorVariable = pMaterial->GetVariable(L"gColor");
	//pMaterial->SetVariable_Vector(L"gColor", XMFLOAT4{ Colors::Red });
	pMaterial->SetDiffuseTexture(L"Textures/Chair_Dark.dds");
	const auto pModel = new ModelComponent{ L"Meshes/Chair.ovm" };
	pModel->SetMaterial(pMaterial);

	m_pChair = new GameObject{};
	m_pChair->AddComponent(pModel);
	const auto rb = m_pChair->AddComponent(new RigidBodyComponent{});
	const auto pConvexMesh = ContentManager::Load<PxConvexMesh>(L"Meshes/Chair.ovpc");
	rb->AddCollider(PxConvexMeshGeometry{pConvexMesh}, *pBouncyMat);
	m_pChair->GetTransform()->Translate( 0.f,5.f,0.f );
	
	
	//rb->AddCollider()
	AddChild(m_pChair);



	//auto& pPhysx = PxGetPhysics();
	//const auto pDefaultMaterial = pPhysx.createMaterial(0.5f, 0.5f, 0.2f);
}

void ModelTestScene::Update()
{
}

void ModelTestScene::Draw()
{
}

void ModelTestScene::OnGUI()
{
	ImGui::Text("This only activates if\n SceneSettings.enableOnGUI is True.\n\n");
	ImGui::Text("Use ImGui to add custom\n controllable scene parameters!");
	ImGui::ColorEdit3("Demo ClearColor", &m_SceneContext.settings.clearColor.x, ImGuiColorEditFlags_NoInputs);
}

void ModelTestScene::PostInitialize()
{
}