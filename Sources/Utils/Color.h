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

	virtual void setRGBColor(const int _r, const int _g, const int _b) = 0;
	virtual void setHSVColor(const int _h, const int _s, const int _v) = 0;
	virtual unsigned int getColor() const = 0;
	virtual RGBColor getRGBColor() const = 0;
	virtual HSVColor getHSVColor() const = 0;

	virtual int getR() const = 0;
	virtual int getG() const = 0;
	virtual int getB() const = 0;
	virtual int getH() const = 0;
	virtual int getS() const = 0;
	virtual int getV() const = 0;

	virtual void setR(const int _r) = 0;
	virtual void setG(const int _g) = 0;
	virtual void setB(const int _b) = 0;
	virtual void setH(const int _h) = 0;
	virtual void setS(const int _s) = 0;
	virtual void setV(const int _v) = 0;
	virtual void setColor(unsigned int _color_code) = 0;

	virtual void shiftR(const int _value) = 0;
	virtual void shiftG(const int _value) = 0;
	virtual void shiftB(const int _value) = 0;
	virtual void shiftH(const int _value) = 0;
	virtual void shiftS(const int _value) = 0;
	virtual void shiftV(const int _value) = 0;

};


/// <summary>
/// RGBカラークラス
/// </summary>
class RGBColor : public Color {

private:
	int r;
	int g;
	int b;

public:
	RGBColor(const int _r, const int _g, const int _b);
	RGBColor(const unsigned int _color_code);
	~RGBColor() = default;

	void setRGBColor(const int _r, const int _g, const int _b);
	void setHSVColor(const int _h, const int _s, const int _v);
	unsigned int getColor() const;
	RGBColor getRGBColor() const;
	HSVColor getHSVColor() const;

	int getR() const;
	int getG() const;
	int getB() const;
	int getH() const;
	int getS() const;
	int getV() const;

	void setR(const int _r);
	void setG(const int _g);
	void setB(const int _b);
	void setH(const int _h);
	void setS(const int _s);
	void setV(const int _v);
	void setColor(unsigned int _color_code);

	void shiftR(const int _value);
	void shiftG(const int _value);
	void shiftB(const int _value);
	void shiftH(const int _value);
	void shiftS(const int _value);
	void shiftV(const int _value);

};


/// <summary>
/// HSVカラークラス
/// </summary>
class HSVColor : public Color {

private:
	int h;
	int s;
	int v;

public:
	HSVColor(const int _h, const int _s, const int _v);
	HSVColor(const unsigned int _color_code);
	~HSVColor() = default;

	void setRGBColor(const int _r, const int _g, const int _b);
	void setHSVColor(const int _h, const int _s, const int _v);
	unsigned int getColor() const;
	RGBColor getRGBColor() const;
	HSVColor getHSVColor() const;

	int getR() const;
	int getG() const;
	int getB() const;
	int getH() const;
	int getS() const;
	int getV() const;

	void setR(const int _r);
	void setG(const int _g);
	void setB(const int _b);
	void setH(const int _h);
	void setS(const int _s);
	void setV(const int _v);
	void setColor(unsigned int _color_code);

	void shiftR(const int _value);
	void shiftG(const int _value);
	void shiftB(const int _value);
	void shiftH(const int _value);
	void shiftS(const int _value);
	void shiftV(const int _value);
};


#endif // !COLOR_DEFINED
