#pragma once
#ifndef SUBJECT_DEFINED
#define SUBJECT_DEFINED


#include "ISubject.h"


class IObserver;


/// <summary>
/// サブジェクトクラス
/// </summary>
class Subject : public ISubject {
public:
	// オブザーバをアタッチする
	void Attach(IObserver* observer) override;
	// オブザーバをデタッチする
	void Detach(IObserver* observer) override;
	// オブザーバに通知する
	void Notify() override;

private:
	// アタッチしているオブザーバのリスト
	std::vector<IObserver*> m_list;
};


#endif	// !SUBJECT_DEFINED
