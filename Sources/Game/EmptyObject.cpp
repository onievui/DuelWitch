#include "EmptyObject.h"
#include "Collider.h"


/// <summary>
/// コンストラクタ
/// </summary>
EmptyObject::EmptyObject() 
	: m_transform()
	, m_collider()
	, m_world(){
}

/// <summary>
/// デストラクタ
/// </summary>
EmptyObject::~EmptyObject() {
	m_collider.reset();
}

/// <summary>
/// オブジェクトを更新する
/// </summary>
/// <param name="timer">ステップタイマー</param>
void EmptyObject::Update(const DX::StepTimer& timer) {
	timer;
	m_world = m_transform.GetMatrix();
}

