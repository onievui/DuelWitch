#pragma once
#ifndef SHARD_DATA_DEFINED
#define SHARD_DATA_DEFINED


/// <summary>
/// シーン間で共有するデータ
/// </summary>
class ShareData {
public:
	// コンストラクタ
	ShareData() : m_selectCharaId() {}

public:
	// 選択したキャラのIDを取得する
	const std::vector<int>& GetSelectCharaID() const             { return m_selectCharaId; }

	// 選択したキャラのIDを設定する
	void SetSelectCharaID(const std::vector<int>& selectCharaId) { m_selectCharaId = selectCharaId; }

private:
	// 選択したキャラのID
	std::vector<int> m_selectCharaId;

};


#endif // !SHARD_DATA_DEFINED

