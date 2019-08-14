#pragma once
#ifndef SERVICE_LOCATER_DEFINED
#define SERVICE_LOCATER_DEFINED


#include "Singleton.h"


template <class T>
class ServiceLocater : public Singleton<ServiceLocater<T>> {
	friend class Singleton<ServiceLocater<T>>;


};


#endif // !SERVICE_LOCATER_DEFINED