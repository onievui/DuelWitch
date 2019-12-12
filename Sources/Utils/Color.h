#pragma once
#ifndef COLOR_DEFINED
#define COLOR_DEFINED


class RGBColor;
class HSVColor;


/// <summary>
/// カラークラス
/// </summary>
class Color {
public:
	Color() = default;
	virtual ~Color() = default;

public:
	virtual void SetRGBColor(const int r, const int g, const int b) = 0;
	virtual void SetHSVColor(const int h, const int s, const int v) = 0;
	virtual unsigned int GetColor() const = 0;
	virtual RGBColor GetRGBColor() const = 0;
	virtual HSVColor GetHSVColor() const = 0;

	virtual int GetR() const = 0;
	virtual int GetG() const = 0;
	virtual int GetB() const = 0;
	virtual int GetH() const = 0;
	virtual int GetS() const = 0;
	virtual int GetV() const = 0;

	virtual void SetR(const int r) = 0;
	virtual void SetG(const int g) = 0;
	virtual void SetB(const int b) = 0;
	virtual void SetH(const int h) = 0;
	virtual void SetS(const int s) = 0;
	virtual void SetV(const int v) = 0;
	virtual void SetColor(unsigned int colorcode) = 0;

	virtual void ShiftR(const int value) = 0;
	virtual void ShiftG(const int value) = 0;
	virtual void ShiftB(const int value) = 0;
	virtual void ShiftH(const int value) = 0;
	virtual void ShiftS(const int value) = 0;
	virtual void ShiftV(const int value) = 0;

};


/// <summary>
/// RGBカラークラス
/// </summary>
class RGBColor : public Color {
public:
	RGBColor(const int r, const int g, const int b);
	RGBColor(const unsigned int colorcode);
	~RGBColor() = default;

public:
	void SetRGBColor(const int r, const int g, const int b);
	void SetHSVColor(const int h, const int s, const int v);
	unsigned int GetColor() const;
	RGBColor GetRGBColor() const;
	HSVColor GetHSVColor() const;

	int GetR() const;
	int GetG() const;
	int GetB() const;
	int GetH() const;
	int GetS() const;
	int GetV() const;

	void SetR(const int r);
	void SetG(const int g);
	void SetB(const int b);
	void SetH(const int h);
	void SetS(const int s);
	void SetV(const int v);
	void SetColor(unsigned int colorcode);

	void ShiftR(const int value);
	void ShiftG(const int value);
	void ShiftB(const int value);
	void ShiftH(const int value);
	void ShiftS(const int value);
	void ShiftV(const int value);


private:
	int m_r;
	int m_g;
	int m_b;

};


/// <summary>
/// HSVカラークラス
/// </summary>
class HSVColor : public Color {
public:
	HSVColor(const int h, const int s, const int v);
	HSVColor(const unsigned int colorcode);
	~HSVColor() = default;

public:
	void SetRGBColor(const int r, const int g, const int b);
	void SetHSVColor(const int h, const int s, const int v);
	unsigned int GetColor() const;
	RGBColor GetRGBColor() const;
	HSVColor GetHSVColor() const;

	int GetR() const;
	int GetG() const;
	int GetB() const;
	int GetH() const;
	int GetS() const;
	int GetV() const;

	void SetR(const int r);
	void SetG(const int g);
	void SetB(const int b);
	void SetH(const int h);
	void SetS(const int s);
	void SetV(const int v);
	void SetColor(unsigned int colorcode);

	void ShiftR(const int value);
	void ShiftG(const int value);
	void ShiftB(const int value);
	void ShiftH(const int value);
	void ShiftS(const int value);
	void ShiftV(const int value);

private:
	int m_h;
	int m_s;
	int m_v;

};


#endif // !COLOR_DEFINED
