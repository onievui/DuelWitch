#pragma once
#ifndef ERROR_MESSAGE_DEFINED
#define ERROR_MESSAGE_DEFINED


#define ErrorMessage

#ifdef _DEBUG

	// �G���[�����b�Z�[�W�{�b�N�X�ŕ\������
	#define ERROR_TYPE_MESSAGEBOX
	// �G���[���A�T�[�g�ŕ\������i�������I������j
	//#define ERROR_TYPE_ASSERT
	
	#if defined(ERROR_TYPE_MESSAGEBOX) && defined(ERROR_TYPE_ASSERT)
	static_assert(false, "You can only use one ERROR_TYPE!");
	#endif

	#ifdef ERROR_TYPE_MESSAGEBOX
		#ifndef _WINUSER_
			#include <Windows.h>
		#endif
		#undef ErrorMessage
		#define ErrorMessage(message) ErrorMessageBoxW(message)
		inline void ErrorMessageBoxW(LPCWSTR message) {
			MessageBoxW(NULL, message, L"", MB_OK);
		}
	#endif
	
	#ifdef ERROR_TYPE_ASSERT
		#include <cassert>
		#undef ErrorMessage
		#define ErrorMessage(message) assert(!message)
	#endif

#endif 


#endif //! ERROR_MESSAGE_DEFINED
