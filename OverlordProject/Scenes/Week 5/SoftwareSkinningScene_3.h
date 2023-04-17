#pragma once
class BoneObject;
class SoftwareSkinningScene_3 : public GameScene
{
public:
	SoftwareSkinningScene_3();
	~SoftwareSkinningScene_3() override = default;

	SoftwareSkinningScene_3(const SoftwareSkinningScene_3& other) = delete;
	SoftwareSkinningScene_3(SoftwareSkinningScene_3&& other) noexcept = delete;
	SoftwareSkinningScene_3& operator=(const SoftwareSkinningScene_3& other) = delete;
	SoftwareSkinningScene_3& operator=(SoftwareSkinningScene_3&& other) noexcept = delete;

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
		VertexSoftwareSkinned(const XMFLOAT3& position, const XMFLOAT3& normal, const XMFLOAT4 color, float bone0weight) : transformedVertex{ position, normal, color }, originalVertex{ position, normal, color }, blendWeight{ bone0weight }
		{
			
		}

		VertexPosNormCol transformedVertex{};
		VertexPosNormCol originalVertex{};
		float blendWeight{};
	};

	void InitializeVertices(float length);

	MeshDrawComponent* m_pMeshDrawer{};
	std::vector<VertexSoftwareSkinned> m_SkinnedVertices{};
};