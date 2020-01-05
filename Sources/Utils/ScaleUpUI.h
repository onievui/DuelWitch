#pragma once
#ifndef SCALE_UP_UI_DEFINED
#define SCALE_UP_UI_DEFINED


#include "UISubject.h"


/// <summary>
/// �}�E�X������Ă���Ԋg�傷��UI�N���X
/// </summary>
class ScaleUpUI : public UISubject {
public:
	// �R���X�g���N�^
	ScaleUpUI(UIEventID eventID = UIEventID::Null,
		int layer = 0,
		const DirectX::SimpleMath::Vector2& pos = DirectX::SimpleMath::Vector2::Zero,
		const DirectX::SimpleMath::Vector2& size = DirectX::SimpleMath::Vector2(100.0f, 100.0f),
		const TextureResource* pTexture = nullptr,
		int textureIndex = 0);

public:
	// UI���X�V����
	void Update(const DX::StepTimer& timer) override;
	// UI��`�悷��
	void Render(DirectX::SpriteBatch* spriteBatch) const override;

protected:
	// �N���b�N������
	void OnClick() override;

protected:
	float m_clickWaitTime;
};

#endif // !SCALE_UP_UI_DEFINED
