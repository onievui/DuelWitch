#include "Subject.h"
#include "IObserver.h"
#include "ErrorMessage.h"


/// <summary>
/// オブザーバをアタッチする
/// </summary>
/// <param name="observer">オブザーバ</param>
void Subject::Attach(IObserver* observer) {
	if (!observer) {
		ErrorMessage(L"サブジェクトに無効な値をアタッチ出来ません");
		return;
	}
	m_list.push_back(observer);
}

/// <summary>
/// オブザーバをデタッチする
/// </summary>
/// <param name="observer">オブザーバ</param>
void Subject::Detach(IObserver* observer) {
	m_list.erase(std::remove(m_list.begin(), m_list.end(), observer), m_list.end());
}

/// <summary>
/// オブザーバに通知する
/// </summary>
void Subject::Notify() {
	for (std::vector<IObserver*>::iterator itr = m_list.begin(); itr != m_list.end(); itr++) {
		(*itr)->Update(this);
	}
}
