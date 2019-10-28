#pragma once
#ifndef FUZZY_OBJECT_DEFINED
#define FUZZY_OBJECT_DEFINED


template<class InputData, class OutputData>
/// <summary>
/// �t�@�W�[�I�u�W�F�N�g
/// </summary>
class FuzzyObject {
public:
	virtual ~FuzzyObject() = default;

public:
	// ���̓f�[�^��ݒ肷��
	void SetInputData(const InputData& inputData) { m_inputData = inputData; }
	// �o�̓f�[�^���擾����
	const OutputData& GetOutputData() const { return m_outputData; }
	// ���͂���o�͂��v�Z����
	virtual void Execute() {}

protected:
	InputData  m_inputData;
	OutputData m_outputData;
};


#endif // !FUZZY_OBJECT_DEFINED
