#include "Field.h"
#include <Framework\DirectX11.h>



Field::Field() {
	auto device = DirectX11::Get().GetDevice().Get();

	// �R�����X�e�[�g���쐬����
	m_states = std::make_unique<DirectX::CommonStates>(device);
	// �G�t�F�N�g�t�@�N�g���[���쐬����
	auto fxFactory = std::make_unique<DirectX::EffectFactory>(device);
	// CMO��ǂݍ���ŃX�J�C�h�[�����쐬����
	m_skydome = DirectX::Model::CreateFromCMO(device, L"Resources/Models/Protected/skydome.cmo", *fxFactory);

	m_skydome->UpdateEffects([](DirectX::IEffect* effect) {
		//DirectX::IEffectLights* lights = dynamic_cast<DirectX::IEffectLights*>(effect);
		//if (lights) {
		//	lights->SetLightingEnabled(true);
		//	lights->SetPerPixelLighting(true);
		//	lights->SetAmbientLightColor(DirectX::Colors::White);
		//}

		DirectX::IEffectLights* lights = dynamic_cast<DirectX::IEffectLights*>(effect);
		if (lights)
		{
			// ���C�g�̉e�����Ȃ��� 
			lights->SetAmbientLightColor(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f));
			lights->SetLightEnabled(0, false);
			lights->SetLightEnabled(1, false);
			lights->SetLightEnabled(2, false);
		}
		DirectX::BasicEffect* basicEffect = dynamic_cast<DirectX::BasicEffect*>(effect);
		if (basicEffect)
		{
			// �G�~�b�V�����F�𔒂ɐݒ肷�� 
			basicEffect->SetEmissiveColor(DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f));
		}
	});
}

Field::~Field() {
}

void Field::Update() {
}

void Field::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const {
	auto context = DirectX11::Get().GetContext().Get();
	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::CreateScale(1.0f);
	m_skydome->Draw(context, *m_states, world, view, proj);
}
