#pragma once
#ifndef IMAGIC_DEFINED
#define IMAGIC_DEFINED


#include <Game\Object\IObject.h>
#include "MagicInfo.h"


class SphereCollider;
enum class MagicID;
enum class PlayerID;


/// <summary>
/// ���@�C���^�t�F�[�X�N���X
/// </summary>
class IMagic : public IObject {
public:
	virtual ~IMagic() = default;
	
public:
	// ���@�𐶐�����
	virtual void Create(const MagicInfo& magicInfo, const DirectX::SimpleMath::Vector3& pos, const DirectX::SimpleMath::Vector3& dir) = 0;
	// ���@���X�V����
	virtual void Update(const DX::StepTimer& timer) = 0;
	// ���@���J������
	virtual void Lost() = 0;	
	// ���@��`�悷��
	virtual void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) const = 0;
	// ���@�̏I���������s��
	virtual void Finalize() = 0;

public:
	// ���@ID���擾����
	virtual MagicID GetID() const = 0;
	// �v���C���[ID���擾����
	virtual PlayerID GetPlayerID() const = 0;
	// �_���[�W���擾����
	virtual float GetPower() const = 0;
	// �v���C���[�Ƃ̏Փˏ���
	virtual void HitPlayer(const Collider* collider) = 0;
	// ���@�Ƃ̏Փˏ���
	virtual void HitMagic(const IMagic* other) = 0;

	// ���@���g�p���Ă��邩�ǂ����擾����
	virtual bool IsUsed() const = 0;
	// ���@���g�p���邩�ǂ����ݒ肷��
	virtual void SetUsed(bool isUsed) = 0;
};


#endif // !IMAGIC_DEFINED


