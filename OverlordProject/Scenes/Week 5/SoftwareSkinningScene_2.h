#pragma once
class BoneObject;

class SoftwareSkinningScene_2 : public GameScene
{
public:
	SoftwareSkinningScene_2();
	~SoftwareSkinningScene_2() override = default;

	SoftwareSkinningScene_2(const SoftwareSkinningScene_2& other) = delete;
	SoftwareSkinningScene_2(SoftwareSkinningScene_2&& other) noexcept = delete;
	SoftwareSkinningScene_2& operator=(const SoftwareSkinningScene_2& other) = delete;
	SoftwareSkinningScene_2& operator=(SoftwareSkinningScene_2&& other) noexcept = delete;

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

	bool m_autoRotate{ true };
	float m_BoneRotation{};
	int m_RotationSign{ 1 };

	struct VertexSoftwareSkinned
	{
		VertexSoftwareSkinned(const XMFLOAT3& position, const XMFLOAT3& normal, const XMFLOAT4 color) : transformedVertex{ position, normal, color }, originalVertex{ position, normal, color }
		{
			
		}
		VertexPosNormCol transformedVertex{};
		VertexPosNormCol originalVertex{};
	};

	void InitializeVertices(float length);

	MeshDrawComponent* m_pMeshDrawer{};
	std::vector<VertexSoftwareSkinned> m_SkinnedVertices{};
};