#pragma once
#ifndef IOBSERVER_DEFINED
#define IOBSERVER_DEFINED


class ISubject;


/// <summary>
/// オブザーバインタフェースクラス
/// </summary>
class IObserver {
public:
	// オブザーバを更新する
	virtual void Update(ISubject* subject) = 0;
};


#endif	// !IOBSERVER_DEFINED
