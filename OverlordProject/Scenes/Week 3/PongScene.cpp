#include "stdafx.h"
#include "PongScene.h"

#include "Prefabs/CubePrefab.h"
#include "Prefabs/SpherePrefab.h"

PongScene::PongScene() :
	GameScene(L"ComponentTestScene") {}

void PongScene::Initialize()
{
	//m_SceneContext.settings.showInfoOverlay = true;
	//m_SceneContext.settings.drawPhysXDebug = true;
	//m_SceneContext.settings.drawGrid = true;
	m_SceneContext.settings.enableOnGUI = true;

	auto& pPhysx = PxGetPhysics();
	const auto pDefaultMaterial = pPhysx.createMaterial(0.f, 0.f, 1.f);

	constexpr XMFLOAT3 wallHorizontalDistance{ 0.f,6.f,0.f };
	constexpr XMFLOAT3 wallVerticalDistance{ 10.f,0.f,0.f };

	constexpr XMFLOAT3 wallVertical{ 1.f, wallHorizontalDistance.y * 2.f + 1.f, 1.f };
	constexpr XMFLOAT3 wallHorizontal{ wallVerticalDistance.x * 2.f, 1.f, 1.f };

	m_pLeftWall = new CubePrefab{ wallVertical, XMFLOAT4{Colors::White} };
	auto rigidbody = m_pLeftWall->AddComponent(new RigidBodyComponent{true});
	rigidbody->AddCollider(PxBoxGeometry{ wallVertical.x / 2, wallVertical.y / 2, wallVertical.z / 2 }, *pDefaultMaterial, true);
	m_pLeftWall->GetTransform()->Translate(wallVerticalDistance.x * -1, wallVerticalDistance.y * -1, wallVerticalDistance.z * -1);
	AddChild(m_pLeftWall);

	m_pRightWall = new CubePrefab{ wallVertical, XMFLOAT4{Colors::White} };
	rigidbody = m_pRightWall->AddComponent(new RigidBodyComponent{ true });
	rigidbody->AddCollider(PxBoxGeometry{ wallVertical.x / 2, wallVertical.y / 2, wallVertical.z / 2 }, *pDefaultMaterial, true);
	m_pRightWall->GetTransform()->Translate(wallVerticalDistance);
	
	AddChild(m_pRightWall);

	auto wall = new CubePrefab{ wallHorizontal, XMFLOAT4{Colors::White} };
	rigidbody = wall->AddComponent(new RigidBodyComponent{ true });
	rigidbody->AddCollider(PxBoxGeometry{ wallHorizontal.x / 2, wallHorizontal.y / 2, wallHorizontal.z / 2 }, *pDefaultMaterial);
	wall->GetTransform()->Translate(wallHorizontalDistance);
	AddChild(wall);

	wall = new CubePrefab{ wallHorizontal, XMFLOAT4{Colors::White} };
	rigidbody = wall->AddComponent(new RigidBodyComponent{ true });
	rigidbody->AddCollider(PxBoxGeometry{ wallHorizontal.x / 2, wallHorizontal.y / 2, wallHorizontal.z / 2 }, *pDefaultMaterial);
	wall->GetTransform()->Translate(wallHorizontalDistance.x * -1, wallHorizontalDistance.y * -1, wallHorizontalDistance.z * -1);
	AddChild(wall);

	m_pCamera = new FixedCamera();
	m_pCamera->GetTransform()->Translate(0, 0, -15);
	m_pCamera->GetTransform()->Rotate(0, 0, 0),
	AddChild(m_pCamera)->GetComponent<CameraComponent>()->SetActive();

	m_pBall = new SpherePrefab(0.2f, 10, XMFLOAT4(Colors::Red));
	AddChild(m_pBall)->GetTransform()->Translate(0.f, 0.f, 0.f);
	rigidbody = m_pBall->AddComponent(new RigidBodyComponent());
	rigidbody->SetConstraint(RigidBodyConstraint::TransZ, false);
	rigidbody->GetPxRigidActor()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
	rigidbody->AddCollider(PxSphereGeometry{ 0.2f }, *pDefaultMaterial);
	
	m_pPlayer0 = new CubePrefab{ wallVertical.x / 4.f,wallVertical.y / 5.f,wallVertical.z / 4.f, XMFLOAT4{Colors::White} };
	AddChild(m_pPlayer0);
	rigidbody = m_pPlayer0->AddComponent(new RigidBodyComponent());
	rigidbody->SetKinematic(true);
	rigidbody->AddCollider(PxBoxGeometry{ wallVertical.x / 4.f / 2.f,wallVertical.y / 5.f / 2.f,wallVertical.z / 4.f / 2.f }, *pDefaultMaterial);

	m_pPlayer1 = new CubePrefab{ wallVertical.x / 4.f,wallVertical.y / 5.f,wallVertical.z / 4.f, XMFLOAT4{Colors::White} };
	AddChild(m_pPlayer1);
	rigidbody = m_pPlayer1->AddComponent(new RigidBodyComponent());
	rigidbody->SetKinematic(true);
	rigidbody->AddCollider(PxBoxGeometry{ wallVertical.x / 4.f / 2.f,wallVertical.y / 5.f / 2.f,wallVertical.z / 4.f / 2.f }, *pDefaultMaterial);
}

void PongScene::Update()
{
	float player0{}, player1{};
	constexpr float speed = 5.f;
	const float deltaTime = m_SceneContext.pGameTime->GetElapsed();
	if (InputManager::IsKeyboardKey(InputState::pressed, VK_SPACE))
	{
		const auto rigidbody = m_pBall->GetComponent<RigidBodyComponent>();
		rigidbody->AddForce(XMFLOAT3{ 100,100,0 });
	}
	if (InputManager::IsKeyboardKey(InputState::pressed, 'R'))
	{
		Reset();
	}
	if (InputManager::IsKeyboardKey(InputState::down, VK_UP))
	{
		player0 += speed * deltaTime;
	}
	if (InputManager::IsKeyboardKey(InputState::down, VK_DOWN))
	{
		player0 -= speed * deltaTime;
	}
	if (InputManager::IsKeyboardKey(InputState::down, 'Z'))
	{
		player1 += speed * deltaTime;
	}
	if (InputManager::IsKeyboardKey(InputState::down, 'S'))
	{
		player1 -= speed * deltaTime;
	}

	XMFLOAT3 player0Pos = m_pPlayer0->GetTransform()->GetPosition();
	player0Pos.y += player0;
	m_pPlayer0->GetTransform()->Translate(player0Pos);

	XMFLOAT3 player1Pos = m_pPlayer1->GetTransform()->GetPosition();
	player1Pos.y += player1;
	m_pPlayer1->GetTransform()->Translate(player1Pos);
	//auto rigidbody = m_pBall->GetComponent<RigidBodyComponent>();
	
	//rigidbody->AddForce(XMFLOAT3{ 100,0,0 });
	//Optional
}

void PongScene::Draw()
{
	//Optional
}

void PongScene::OnGUI()
{
	ImGui::Text("This only activates if\n SceneSettings.enableOnGUI is True.\n\n");
	ImGui::Text("Use ImGui to add custom\n controllable scene parameters!");
	ImGui::ColorEdit3("Demo ClearColor", &m_SceneContext.settings.clearColor.x, ImGuiColorEditFlags_NoInputs);
}

void PongScene::PostInitialize()
{
	m_pRightWall->SetOnTriggerCallBack([=](GameObject*, GameObject* pOther, PxTriggerAction action)
		{
			if (action == PxTriggerAction::ENTER && pOther == m_pBall)
			{
				Reset();
			}
		});
	m_pLeftWall->SetOnTriggerCallBack([=](GameObject*, GameObject* pOther, PxTriggerAction action)
		{
			if (action == PxTriggerAction::ENTER && pOther == m_pBall)
			{
				Reset();
			}
		});
}

void PongScene::Reset()
{
	m_pBall->GetTransform()->Translate(XMFLOAT3{});

	m_pPlayer0->GetTransform()->Translate(-8.f, 0.f, 0.f);
	m_pPlayer1->GetTransform()->Translate(8.f, 0.f, 0.f);
}