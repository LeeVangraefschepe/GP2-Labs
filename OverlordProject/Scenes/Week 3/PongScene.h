#pragma once
class PongScene : public GameScene
{
public:
	PongScene();
	~PongScene() override = default;

	PongScene(const PongScene& other) = delete;
	PongScene(PongScene&& other) noexcept = delete;
	PongScene& operator=(const PongScene& other) = delete;
	PongScene& operator=(PongScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void OnGUI() override;
	void PostInitialize() override;
private:
	void Reset();
	FixedCamera* m_pCamera;
	GameObject* m_pBall;
	GameObject* m_pLeftWall;
	GameObject* m_pRightWall;
	GameObject* m_pPlayer0;
	GameObject* m_pPlayer1;
};