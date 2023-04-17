#include "stdafx.h"
#include "ComponentTestScene.h"

#include "Prefabs/CubePrefab.h"
#include "Prefabs/SpherePrefab.h"
#include "Prefabs/TorusPrefab.h"


ComponentTestScene::ComponentTestScene() :
	GameScene(L"ComponentTestScene") {}

void ComponentTestScene::Initialize()
{
	//m_SceneContext.settings.showInfoOverlay = true;
	//m_SceneContext.settings.drawPhysXDebug = true;
	//m_SceneContext.settings.drawGrid = true;
	m_SceneContext.settings.enableOnGUI = true;

	auto& pPhysx = PxGetPhysics();
	const auto pBouncyMaterial = pPhysx.createMaterial(.5f, .5f, 1.f);

	//Ground plane
	GameSceneExt::CreatePhysXGroundPlane(*this, pBouncyMaterial);

	//Sphere red (group 0)
	auto pSphere = new SpherePrefab(1.f, 10, XMFLOAT4(Colors::Red));
	AddChild(pSphere)->GetTransform()->Translate(0.f,30.f,0.f);
	auto rigidBody = pSphere->AddComponent(new RigidBodyComponent());
	rigidBody->AddCollider(PxSphereGeometry{ 1.f }, *pBouncyMaterial);
	rigidBody->SetCollisionGroup(CollisionGroup::Group0);
	rigidBody->SetCollisionIgnoreGroups(CollisionGroup::Group1 | CollisionGroup::Group2);

	//Sphere Green (group 1)
	pSphere = new SpherePrefab(1.f, 10, XMFLOAT4(Colors::Green));
	AddChild(pSphere)->GetTransform()->Translate(0.f, 20.f, 0.f);
	rigidBody = pSphere->AddComponent(new RigidBodyComponent());
	rigidBody->AddCollider(PxSphereGeometry{ 1.f }, *pBouncyMaterial);
	rigidBody->SetCollisionGroup(CollisionGroup::Group1);
	rigidBody->SetConstraint(RigidBodyConstraint::TransX | RigidBodyConstraint::TransZ, false);

	//Sphere Blue (group 2)
	pSphere = new SpherePrefab(1.f, 10, XMFLOAT4(Colors::Blue));
	AddChild(pSphere)->GetTransform()->Translate(0.f, 10.f, 0.f);
	rigidBody = pSphere->AddComponent(new RigidBodyComponent());
	rigidBody->SetConstraint(RigidBodyConstraint::TransX | RigidBodyConstraint::TransZ, false);
	rigidBody->AddCollider(PxSphereGeometry{ 1.f }, *pBouncyMaterial);
	rigidBody->SetCollisionGroup(CollisionGroup::Group2);
}

void ComponentTestScene::Update()
{
	//Optional
}

void ComponentTestScene::Draw()
{
	//Optional
}

void ComponentTestScene::OnGUI()
{
	ImGui::Text("This only activates if\n SceneSettings.enableOnGUI is True.\n\n");
	ImGui::Text("Use ImGui to add custom\n controllable scene parameters!");
	ImGui::ColorEdit3("Demo ClearColor", &m_SceneContext.settings.clearColor.x, ImGuiColorEditFlags_NoInputs);
}
