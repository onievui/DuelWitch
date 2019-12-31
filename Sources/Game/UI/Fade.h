#pragma once
#ifndef FADE_DEFINED
#define FADE_DEFINED


#include <Framework\StepTimer.h>


// �t�F�[�h�N���X
class Fade {
public:
	// �t�F�[�h�̎��
	enum class State {
		NoFade,
		FadeIn,
		FadeOut,
		// �A���t�@�l�擾�p
		FadeInAlpha = FadeOut,
		FadeOutAlpha = FadeIn,
	};

public:
	// �R���X�g���N�^
	Fade();

public:
	// �t�F�[�h�N���X������������
	void Initialize(State state, float endTime, float endAlpha,
		const DirectX::SimpleMath::Color& color = DirectX::SimpleMath::Color(DirectX::Colors::Black));
	// �t�F�[�h��Ԃ��X�V����
	void Update(const DX::StepTimer& timer);
	// �t�F�[�h��`�悷��
	void Render(DirectX::SpriteBatch* spriteBatch) const;

public:
	// ���Ԃ����Z�b�g����
	void ResetTime();
	// ���Ԃ��΂�
	void SkipTime(float timeRate = 1.0f);
	// �ĊJ����
	void Start();
	// �ꎞ��~����
	void Stop();
	// ���s�����ǂ����擾����
	bool IsRunning() const { return m_isRunning; }
	// �������Ă��邩�ǂ����擾����
	bool IsFinished() const;
	// ���݂̃A���t�@�l���擾����
	float GetAlpha() const;

private:
	// �t�F�[�h�̎��
	State                      m_state;
	// ���s�����ǂ����̃t���O
	bool                       m_isRunning;
	// ���݂̎���
	float                      m_time;
	// �I������
	float                      m_endTime;
	// �ŏI�I�ȃA���t�@�l
	float                      m_endAlpha;
	// �t�F�[�h�J���[
	DirectX::SimpleMath::Color m_fadeColor;
};


#endif // !FADE_DEFINED



