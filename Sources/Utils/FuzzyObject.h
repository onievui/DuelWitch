#pragma once
#ifndef FUZZY_OBJECT_DEFINED
#define FUZZY_OBJECT_DEFINED


template<class InputData, class OutputData>
/// <summary>
/// ファジーオブジェクト
/// </summary>
class FuzzyObject {
public:
	virtual ~FuzzyObject() = default;

public:
	// 入力データを設定する
	void SetInputData(const InputData& inputData) { m_inputData = inputData; }
	// 出力データを取得する
	const OutputData& GetOutputData() const { return m_outputData; }
	// 入力から出力を計算する
	virtual void Execute() {}

protected:
	InputData  m_inputData;
	OutputData m_outputData;
};


#endif // !FUZZY_OBJECT_DEFINED
