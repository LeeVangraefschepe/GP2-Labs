#pragma once
class UberMaterial;

class UberMaterialScene : public GameScene
{
public:
	UberMaterialScene();
	~UberMaterialScene() override = default;

	UberMaterialScene(const UberMaterialScene& other) = delete;
	UberMaterialScene(UberMaterialScene&& other) noexcept = delete;
	UberMaterialScene& operator=(const UberMaterialScene& other) = delete;
	UberMaterialScene& operator=(UberMaterialScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void OnGUI() override;
	void PostInitialize() override;
private:
	FixedCamera* m_pCamera;
	GameObject* m_pSphere;
	UberMaterial* m_pMaterial;
};