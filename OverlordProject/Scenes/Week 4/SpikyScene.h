#pragma once
class SpikyMaterial;

class SpikyScene : public GameScene
{
public:
	SpikyScene();
	~SpikyScene() override = default;

	SpikyScene(const SpikyScene& other) = delete;
	SpikyScene(SpikyScene&& other) noexcept = delete;
	SpikyScene& operator=(const SpikyScene& other) = delete;
	SpikyScene& operator=(SpikyScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void OnGUI() override;
	void PostInitialize() override;

private:
	FixedCamera* m_pCamera;
	GameObject* m_pSphere;
	SpikyMaterial* m_pMaterial;

	float spikeLength{0.5f};
};

