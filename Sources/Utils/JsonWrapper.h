#pragma once
#ifndef JSON_WRAPPER_DEFINED
#define JSON_WRAPPER_DEFINED


#include "picojson.h"


namespace JsonWrapper {

	/// <summary>
	/// JSON�f�[�^�N���X
	/// </summary>
	class value {
	public:
		// �R���X�g���N�^
		value(picojson::value& val)
			: m_val(val) {
		}

		// �v�f�ɃA�N�Z�X����
		JsonWrapper::value operator[](const char* name) {
			return JsonWrapper::value(m_val.get<picojson::object>()[name]);
		}

		// �z��ɃA�N�Z�X����
		JsonWrapper::value operator[](int index) {
			return JsonWrapper::value(m_val.get<picojson::array>()[index]);
		}

		template <class T>
		// �f�[�^���擾����
		T& Get() {
			return m_val.get<T>();
		}

		// �f�[�^���擾����
		int GetNumI() {
			return static_cast<int>(m_val.get<double>());
		}

		//�f�[�^���擾����
		float GetNumF() {
			return static_cast<float>(m_val.get<double>());
		}

		// �f�[�^���擾����
		DirectX::SimpleMath::Vector2 GetVector2() {
			const picojson::array& vec2 = m_val.get<picojson::array>();
			return DirectX::SimpleMath::Vector2(
				static_cast<float>(vec2[0].get<double>()),
				static_cast<float>(vec2[1].get<double>())
			);
		}

		// �f�[�^���擾����
		DirectX::SimpleMath::Vector3 GetVector3() {
			const picojson::array& vec3 = m_val.get<picojson::array>();
			return DirectX::SimpleMath::Vector3(
				static_cast<float>(vec3[0].get<double>()),
				static_cast<float>(vec3[1].get<double>()),
				static_cast<float>(vec3[2].get<double>())
			);
		}

		// �f�[�^���擾����
		DirectX::SimpleMath::Vector4 GetVector4() {
			const picojson::array& vec4 = m_val.get<picojson::array>();
			return DirectX::SimpleMath::Vector4(
				static_cast<float>(vec4[0].get<double>()),
				static_cast<float>(vec4[1].get<double>()),
				static_cast<float>(vec4[2].get<double>()),
				static_cast<float>(vec4[3].get<double>())
			);
		}

		template <class T>
		// �f�[�^��ݒ肷��
		void Set(const T& val) {
			m_val = picojson::value(val);
		}

		// picojson�I�u�W�F�N�g���擾����
		picojson::value& GetValue() {
			return m_val;
		}

		// �V���A���C�Y���s��
		std::string Serialize(bool prettify) const {
			return m_val.serialize(prettify);
		}

	private:
		// ���f�[�^�ւ̎Q��
		picojson::value& m_val;
	};


	/// <summary>
	/// JSON�f�[�^�N���X�i���[�g�p�j
	/// </summary>
	class root : public JsonWrapper::value {
	public:
		// �R���X�g���N�^
		root()
			: JsonWrapper::value(m_root) {
		}

	private:
		// ���f�[�^
		picojson::value m_root;
	};

	// JSON�f�[�^���o�͂���
	extern std::ostream &operator << (std::ostream& os, JsonWrapper::value &value);

	// JSON�f�[�^��ǂݍ���
	extern std::istream &operator >> (std::istream& is, JsonWrapper::value& value);

	// �t�@�C������JSON�f�[�^��ǂݍ���
	extern bool LoadCheck(JsonWrapper::value& value, const std::wstring& fileName);

}

#endif // !JSON_WRAPPER_DEFINED

