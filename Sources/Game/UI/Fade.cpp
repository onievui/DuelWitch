#include "Fade.h"
#include <Utils\MathUtils.h>
#include <Utils\ServiceLocater.h>
#include <Utils\Resource.h>
#include <Utils\ResourceManager.h>


/// <summary>
/// �R���X�g���N�^
/// </summary>
Fade::Fade() 
	: m_state(State::NoFade)
	, m_isRunning(false)
	, m_time()
	, m_endTime()
	, m_endAlpha()
	, m_fadeColor(DirectX::Colors::Black) {
}

/// <summary>
/// �t�F�[�h�N���X�̏�����
/// </summary>
/// <param name="state">�t�F�[�h�̎��</param>
/// <param name="endTime">�I������</param>
/// <param name="endAlpha">�ŏI�I�ȃA���t�@�l</param>
/// <param name="color">�t�F�[�h�J���[</param>
void Fade::Initialize(State state, float endTime, float endAlpha, const DirectX::SimpleMath::Color& color) {
	m_state = state;
	m_isRunning = true;
	m_time = 0;
	m_endTime = std::max(endTime, 0.0f);
	m_endAlpha = Math::Clamp(0.0f, endAlpha, 1.0f);
	m_fadeColor = color;
}

/// <summary>
/// �t�F�[�h��Ԃ��X�V����
/// </summary>
/// <param name="timer">�X�e�b�v�^�C�}�[</param>
void Fade::Update(const DX::StepTimer& timer) {
	float elapsed_time = static_cast<float>(timer.GetElapsedSeconds());

	// ��~���Ȃ珈�����Ȃ�
	if (!m_isRunning) {
		return;
	}

	// �o�ߎ��Ԃ��J�E���g����
	if (m_time < m_endTime) {
		m_time += elapsed_time;
		m_time = Math::Clamp(0.0f, m_time, m_endTime);
	}
}

/// <summary>
/// �t�F�[�h��`�悷��
/// </summary>
/// <param name="spriteBatch">�X�v���C�g�o�b�`</param>
void Fade::Render(DirectX::SpriteBatch* spriteBatch) const {
	const TextureResource* texture = ServiceLocater<ResourceManager<TextureResource>>::Get()->GetResource(TextureID::Fade);

	// �`�悷��F�ƃA���t�@�l���v�Z����
	DirectX::SimpleMath::Color color = m_fadeColor;
	color.w = GetAlpha();

	// �A���t�@�l��0��荂���Ȃ�`�悷��
	if (color.w > 0.0f) {
		spriteBatch->Draw(texture->GetResource().Get(),
			DirectX::SimpleMath::Vector2::Zero,
			nullptr, color, 0,
			DirectX::SimpleMath::Vector2::Zero,
			DirectX::SimpleMath::Vector2(1.0f, 1.0f));
	}
}

/// <summary>
/// ���Ԃ����Z�b�g����
/// </summary>
void Fade::ResetTime() {
	m_time = 0.0f;
}

/// <summary>
/// ���Ԃ��΂�
/// </summary>
/// <param name="timeRate">��΂����Ԃ̊���</param>
void Fade::SkipTime(float timeRate) {
	m_time += m_endTime * timeRate;
	m_time = Math::Clamp(m_time, 0.0f, m_endTime);
}

/// <summary>
/// �ĊJ����
/// </summary>
void Fade::Start() {
	m_isRunning = true;
}

/// <summary>
/// �ꎞ��~����
/// </summary>
void Fade::Stop() {
	m_isRunning = false;
}

/// <summary>
/// �������Ă��邩�ǂ����擾����
/// </summary>
/// <returns>
/// true : �������Ă���
/// false : �������ĂȂ�
/// </returns>
bool Fade::IsFinished() const {
	return m_time >= m_endTime;
}

/// <summary>
/// ���݂̃A���t�@�l���擾����
/// </summary>
/// <returns>
/// �A���t�@�l
/// </returns>
float Fade::GetAlpha() const {
	// �t�F�[�h���Ȃ��ꍇ�͂��̂܂܃A���t�@�l��Ԃ�
	if (m_state == State::NoFade) {
		return m_endAlpha;
	}

	// �ω��󋵂��v�Z����
	if (m_endTime <= 0.0f) {
		// �ݒ肳�ꂽ���Ԃ�0�ȉ��̏ꍇ�͕ω��ς݂Ɣ��f����
		return m_endAlpha;
	}
	else {
		float t = m_time / m_endTime;
		// �t�F�[�h�̎�ނɂ���Č�����ς���
		if (m_state == State::FadeIn) {
			return Math::Lerp(1.0f, m_endAlpha, t);
		}
		else {
			return Math::Lerp(0.0f, m_endAlpha, t);
		}
	}
}
