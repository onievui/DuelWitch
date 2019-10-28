#pragma once
#ifndef FIELD_DATA_DEFINED
#define FIELD_DATA_DEFINED


class Element;


/// <summary>
/// �t�B�[���h���\����
/// </summary>
struct FieldData {
	DirectX::SimpleMath::Vector3 fieldCenter;
	float                        fieldRadius;
	const std::vector<Element*>* pElements;
};


#endif // !FIELD_DATA_DEFINED
