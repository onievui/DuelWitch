#include "InputCodeLoader.h"
#include <Utils\ServiceLocater.h>
#include <Utils\InputManager.h>


/// <summary>
/// ì¸óÕÉRÅ[ÉhÇì«Ç›çûÇﬁ
/// </summary>
void InputCodeLoader::Load() {
	constexpr InputManager::KeyCode key_codes[static_cast<int>(InputID::Num)] = {
		static_cast<InputManager::KeyCode>(InputManager::NONE_USE),
		static_cast<InputManager::KeyCode>(InputManager::NONE_USE),
		static_cast<InputManager::KeyCode>(InputManager::NONE_USE),
		InputManager::KeyCode::A,
		InputManager::KeyCode::D,
		InputManager::KeyCode::W,
		InputManager::KeyCode::S,
		static_cast<InputManager::KeyCode>(InputManager::NONE_USE),
		static_cast<InputManager::KeyCode>(InputManager::NONE_USE),
		InputManager::KeyCode::A,
		InputManager::KeyCode::D,
		InputManager::KeyCode::S,
		InputManager::KeyCode::Escape,
	};

	constexpr InputManager::PadCode pad_codes[static_cast<int>(InputID::Num)] = {
		InputManager::PadCode::A,
		InputManager::PadCode::B,
		InputManager::PadCode::X,
		InputManager::PadCode::LStickLeft,
		InputManager::PadCode::LStickRight,
		InputManager::PadCode::LStickUp,
		InputManager::PadCode::LStickDown,
		InputManager::PadCode::RTrigger,
		InputManager::PadCode::A,
		InputManager::PadCode::LShoulder,
		InputManager::PadCode::RShoulder,
		InputManager::PadCode::X,
		InputManager::PadCode::Start
	};

	constexpr InputManager::MouseCode mouse_codes[static_cast<int>(InputID::Num)] = {
		InputManager::MouseCode::Left,
		static_cast<InputManager::MouseCode>(InputManager::NONE_USE),
		InputManager::MouseCode::Right,
		static_cast<InputManager::MouseCode>(InputManager::NONE_USE),
		static_cast<InputManager::MouseCode>(InputManager::NONE_USE),
		static_cast<InputManager::MouseCode>(InputManager::NONE_USE),
		static_cast<InputManager::MouseCode>(InputManager::NONE_USE),
		InputManager::MouseCode::Left,
		InputManager::MouseCode::Right,
		static_cast<InputManager::MouseCode>(InputManager::NONE_USE),
		static_cast<InputManager::MouseCode>(InputManager::NONE_USE),
		static_cast<InputManager::MouseCode>(InputManager::NONE_USE),
		static_cast<InputManager::MouseCode>(InputManager::NONE_USE),
	};

	InputManager* input_manager = ServiceLocater<InputManager>::Get();

	for (int i = 0; i < static_cast<int>(InputID::Num); ++i) {
		input_manager->SetInputCode(static_cast<InputID>(i), key_codes[i], pad_codes[i], mouse_codes[i]);
	}

}
