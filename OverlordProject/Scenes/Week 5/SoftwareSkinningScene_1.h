#pragma once
class BoneObject;

class SoftwareSkinningScene_1 : public GameScene
{
public:
	SoftwareSkinningScene_1();
	~SoftwareSkinningScene_1() override = default;

	SoftwareSkinningScene_1(const SoftwareSkinningScene_1& other) = delete;
	SoftwareSkinningScene_1(SoftwareSkinningScene_1&& other) noexcept = delete;
	SoftwareSkinningScene_1& operator=(const SoftwareSkinningScene_1& other) = delete;
	SoftwareSkinningScene_1& operator=(SoftwareSkinningScene_1&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void OnGUI() override;
	void PostInitialize() override;
private:
	BoneObject* m_pBone0{};
	BoneObject* m_pBone1{};

	XMFLOAT3 m_bone0Rot{}, m_bone1Rot{};

	bool m_autoRotate{true};
	float m_BoneRotation{};
	int m_RotationSign{ 1 };
};