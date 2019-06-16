#define _CRT_SECURE_NO_WARNINGS

#include "MyGame.h"
#include "DebugCamera.h"
#include "GridFloor.h"
#include "Player.h"
#include "ElementManager.h"
#include "TargetCamera.h"
#include "Field.h"

// �R���X�g���N�^
MyGame::MyGame(int width, int height) : m_width(width), m_height(height), Game(width, height)
{
}

// �f�X�g���N�^
MyGame::~MyGame() = default;

// MyGame�I�u�W�F�N�g����������
void MyGame::Initialize(int width, int height) 
{
	// ���N���X��Initialize���Ăяo�� 
	Game::Initialize(width, height);

	// CommonStates�I�u�W�F�N�g�𐶐�����
	m_commonStates = std::make_unique<DirectX::CommonStates>(m_directX.GetDevice().Get());
	// EffectFactory�I�u�W�F�N�g�𐶐�����
	m_effectFactory = std::make_unique<DirectX::EffectFactory>(m_directX.GetDevice().Get());
	// �L�[�{�[�h�𐶐�����
	m_keyboard = std::make_unique<DirectX::Keyboard>();
	// �}�E�X�𐶐�����
	m_mouse = std::make_unique<DirectX::Mouse>();
	m_mouse->SetWindow(DirectX11::Get().GetHWnd());

	m_world = DirectX::SimpleMath::Matrix::Identity;
}

// ���\�[�X�𐶐�����
void MyGame::CreateResources()
{
	// ���N���X��CreateResources���Ăяo��
	Game::CreateResources();

	// �r���[���W�ϊ��s��𐶐�����
	// ���_, �����_, 
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(DirectX::SimpleMath::Vector3(2.0f, 2.0f, 2.0f),
		DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::Up);
	// �ˉe���W�ϊ��s��𐶐�����
	m_projection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(DirectX::XM_PI / 4.0f,
		float(m_width) / float(m_height), 0.1f, 500.0f);
	
	// ���f���I�u�W�F�N�g�𐶐�����
	m_model = std::make_unique<Player>();
	m_model->Create(L"bloom.cmo", L"Resources/Models/Protected");
	// �G�������g�}�l�[�W�����쐬����
	m_elementManager = std::make_unique<ElementManager>();
	m_elementManager->Initialize();

	//�f�o�b�O�J�����𐶐�����
	m_debugCamera = std::make_unique<DebugCamera>(m_width, m_height);
	//�^�[�Q�b�g�J�����𐶐�����
	m_targetCamera = std::make_unique<TargetCamera>(m_model.get(), DirectX::SimpleMath::Vector3(0.0f, 2.0f, -5.0f),
		DirectX::SimpleMath::Vector3(0.0f, 0.0f, 2.0f), DirectX::SimpleMath::Vector3::UnitY,
		Math::HarfPI*0.5f, float(m_width) / float(m_height), 0.1f, 10000.0f);
	//�O���b�h���𐶐�����
	m_gridFloor = std::make_unique<GridFloor>(m_commonStates.get(), 200.0f, 50);

	// �t�B�[���h�𐶐�����
	m_field = std::make_unique<Field>();

}

// �Q�[�����X�V����
void MyGame::Update(const DX::StepTimer& timer) 
{
	// �v���C���[�̍X�V
	m_model->Update(timer);
	// �G�������g�}�l�[�W���̍X�V
	m_elementManager->Update(timer);

	if (timer.GetFrameCount() % 30 == 0) {
		DirectX::SimpleMath::Vector3 area_offset(0, 0, 80);
		DirectX::SimpleMath::Vector3 area_start = DirectX::SimpleMath::Vector3::One*-3.0f+area_offset;
		DirectX::SimpleMath::Vector3 area_end = DirectX::SimpleMath::Vector3::One*3.0f + area_offset;
		m_elementManager->CreateElement(area_start, area_end, 1);
	}

	// �f�o�b�O�J�����̍X�V
	m_debugCamera->Update();
	// �^�[�Q�b�g�J�����̍X�V
	m_targetCamera->Update();
	// �t�B�[���h�̍X�V
	m_field->Update();
}

// �Q�[����`�悷��
void MyGame::Render(const DX::StepTimer& timer) 
{
	// �ŏ��̍X�V�̑O�͉����`�悵�Ȃ��悤�ɂ���
	if (timer.GetFrameCount() == 0) 
		return;

	// TODO: �����_�����O�R�[�h��ǉ�����
	float time = float(timer.GetTotalSeconds());

	// Z���ɑ΂��ĉ�]������s��𐶐�����
	m_world = DirectX::SimpleMath::Matrix::CreateRotationZ(cosf(time) * 1.0f);

	// �o�b�t�@���N���A����
	Clear();
	// �X�v���C�g�o�b�`���J�n����
	GetSpriteBatch()->Begin(DirectX::SpriteSortMode_Deferred, m_commonStates->NonPremultiplied());
	// FPS��`�悷��
	DrawFPS(timer);
	//�r���[�s����擾����
	//m_view = m_debugCamera->GetCameraMatrix();
	m_view = m_targetCamera->GetViewMatrix();
	m_projection = m_targetCamera->GetProjectionMatrix();
	//�O���b�h����`�悷��
	m_gridFloor->Render(m_view, m_projection);
	// �t�B�[���h��`�悷��
	m_field->Render(m_view, m_projection);
	// ���f����`�悷��
	m_model->Render(m_view, m_projection);
	// �G�������g��`�悷��
	m_elementManager->Render(m_view, m_projection);

	// �X�v���C�g�o�b�`���I������
	GetSpriteBatch()->End();
	// �o�b�N�o�b�t�@��\������
	Present();
}

// ��n��������
void MyGame::Finalize() 
{
	// ���N���X��Finalize���Ăяo��
	Game::Finalize();

	m_model.reset();
	m_keyboard.reset();
	m_mouse.reset();
	m_debugCamera.reset();
	m_gridFloor.reset();
}

// FPS��`�悷��
void MyGame::DrawFPS(const DX::StepTimer& timer)
{
	// FPS������𐶐�����
	std::wstring fpsString = L"fps = " + std::to_wstring((unsigned int)timer.GetFramesPerSecond());
	// FPS��`�悷��
	GetSpriteFont()->DrawString(GetSpriteBatch(), fpsString.c_str(), DirectX::SimpleMath::Vector2(0, 0), DirectX::Colors::White);
}
