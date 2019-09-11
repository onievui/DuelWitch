#pragma once
#ifndef ISUBJECT_DEFINED
#define ISUBJECT_DEFINED


class IObserver;


/// <summary>
/// サブジェクトインタフェースクラス
/// </summary>
class ISubject {
public:
	// オブザーバをアタッチする
	virtual void Attach(IObserver* observer) = 0;
	// オブザーバをデタッチする
	virtual void Detach(IObserver* observer) = 0;
	// オブザーバに通知する
	virtual void Notify() = 0;

};


#endif	// !ISUBJECT_DEFINED
