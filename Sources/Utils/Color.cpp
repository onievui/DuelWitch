#include "Color.h"
#include "MathUtils.h"


RGBColor::RGBColor(const int _r, const int _g, const int _b) {
	setRGBColor(_r, _g, _b);
}

RGBColor::RGBColor(const unsigned int _color_code) {
	setColor(_color_code);
}

void RGBColor::setRGBColor(const int _r, const int _g, const int _b) {
	r = Math::Clamp(_r, 0, 255);
	g = Math::Clamp(_g, 0, 255);
	b = Math::Clamp(_b, 0, 255);
}

void RGBColor::setHSVColor(const int _h, const int _s, const int _v) {
	HSVColor hsv(_h, _s, _v);
	*this = hsv.getRGBColor();

}

unsigned int RGBColor::getColor() const {
	//return GetColor(r, g, b);
	return 0;
}

RGBColor RGBColor::getRGBColor() const {
	return *this;
}

HSVColor RGBColor::getHSVColor() const {
	int h, s, v, max, min;

	//最大値・最小値の設定
	if (r > g) {
		max = r;
		min = g;
	}
	else {
		max = g;
		min = r;
	}
	if (b > max) {
		max = b;
	}
	else if (b < min) {
		min = b;
	}

	//色相の計算
	if (max == min) {
		h = 0;
	}
	else if (max == r) {
		h = (int)(60 * ((g - b) / (float)(max - min)));
	}
	else if (max == g) {
		h = (int)(60 * ((b - r) / (float)(max - min)) + 120);
	}
	else {
		h = (int)(60 * ((r - g) / (float)(max - min)) + 240);
	}

	if (h < 0) {
		h += 360;
	}

	//彩度の計算
	s = (max - min) / max * 255;

	//明度の計算
	v = max;

	return HSVColor(h, s, v);
}

int RGBColor::getR() const {
	return r;
}

int RGBColor::getG() const {
	return g;
}

int RGBColor::getB() const {
	return b;
}

int RGBColor::getH() const {
	return getHSVColor().getH();
}

int RGBColor::getS() const {
	return getHSVColor().getS();
}

int RGBColor::getV() const {
	return getHSVColor().getV();
}

void RGBColor::setR(const int _r) {
	r = Math::Clamp(_r, 0, 255);
}

void RGBColor::setG(const int _g) {
	g = Math::Clamp(_g, 0, 255);
}

void RGBColor::setB(const int _b) {
	b = Math::Clamp(_b, 0, 255);
}

void RGBColor::setH(const int _h) {
	HSVColor hsv = getHSVColor();
	hsv.setH(_h);
	*this = hsv.getRGBColor();
}

void RGBColor::setS(const int _s) {
	HSVColor hsv = getHSVColor();
	hsv.setS(_s);
	*this = hsv.getRGBColor();
}

void RGBColor::setV(const int _v) {
	HSVColor hsv = getHSVColor();
	hsv.setV(_v);
	*this = hsv.getRGBColor();
}

void RGBColor::setColor(unsigned int _color_code) {
	int r, g, b;
	r = (_color_code >> 16) & 0xFF;
	g = (_color_code >>  8) & 0xFF;
	b = (_color_code >>  0) & 0xFF;
	setRGBColor(r, g, b);
}

void RGBColor::shiftR(const int _value) {
	r = Math::Clamp(r + _value, 0, 255);
}

void RGBColor::shiftG(const int _value) {
	g = Math::Clamp(g + _value, 0, 255);
}

void RGBColor::shiftB(const int _value) {
	b = Math::Clamp(b + _value, 0, 255);
}

void RGBColor::shiftH(const int _value) {
	HSVColor hsv = getHSVColor();
	hsv.shiftH(_value);
	*this = hsv.getRGBColor();
}

void RGBColor::shiftS(const int _value) {
	HSVColor hsv = getHSVColor();
	hsv.shiftS(_value);
	*this = hsv.getRGBColor();
}

void RGBColor::shiftV(const int _value) {
	HSVColor hsv = getHSVColor();
	hsv.shiftV(_value);
	*this = hsv.getRGBColor();
}


HSVColor::HSVColor(const int _h, const int _s, const int _v) {
	setHSVColor(_h, _s, _v);
}

HSVColor::HSVColor(const unsigned int _color_code) {
	setColor(_color_code);
}

void HSVColor::setRGBColor(const int _r, const int _g, const int _b) {
	RGBColor rgb(_r, _g, _b);
	*this = rgb.getHSVColor();
}

void HSVColor::setHSVColor(const int _h, const int _s, const int _v) {
	h = _h;
	s = Math::Clamp(_s, 0, 255);
	v = Math::Clamp(_v, 0, 255);

	while (h < 0) {
		h += 360;
	}
	h %= 360;
}

unsigned int HSVColor::getColor() const{
	return getRGBColor().getColor();
}

RGBColor HSVColor::getRGBColor() const {
	int r, g, b, max, min;

	//最大値・最小値の計算
	max = v;
	min = (int)(max - ((s / 255.f) * max));

	//RGBカラーの計算
	if (h <= 60) {
		r = max;
		g = (int)((h / 60.f) * (max - min) + min);
		b = min;
	}
	else if (h <= 120) {
		r = (int)(((120 - h) / 60.f) * (max - min) + min);
		g = max;
		b = min;
	}
	else if (h <= 180) {
		r = min;
		g = max;
		b = (int)(((h - 120) / 60.f) * (max - min) + min);
	}
	else if (h <= 240) {
		r = min;
		g = (int)(((240 - h) / 60.f) * (max - min) + min);
		b = max;
	}
	else if (h <= 300) {
		r = (int)(((h - 240) / 60.f) * (max - min) + min);
		g = min;
		b = max;
	}
	else {
		r = max;
		g = min;
		b = (int)(((360 - h) / 60.f) * (max - min) + min);
	}

	return RGBColor(r, g, b);
}

HSVColor HSVColor::getHSVColor() const {
	return *this;
}

int HSVColor::getR() const {
	return getRGBColor().getR();
}

int HSVColor::getG() const {
	return getRGBColor().getG();
}

int HSVColor::getB() const {
	return getRGBColor().getB();
}

int HSVColor::getH() const {
	return h;
}

int HSVColor::getS() const {
	return s;
}

int HSVColor::getV() const {
	return v;
}

void HSVColor::setR(const int _r) {
	RGBColor rgb = getRGBColor();
	rgb.setR(_r);
	*this = rgb.getHSVColor();
}

void HSVColor::setG(const int _g) {
	RGBColor rgb = getRGBColor();
	rgb.setG(_g);
	*this = rgb.getHSVColor();
}

void HSVColor::setB(const int _b) {
	RGBColor rgb = getRGBColor();
	rgb.setB(_b);
	*this = rgb.getHSVColor();
}

void HSVColor::setH(const int _h) {
	h = _h;
	while (h < 0) {
		h += 360;
	}
	h %= 360;
}

void HSVColor::setS(const int _s) {
	s = Math::Clamp(_s, 0, 255);
}

void HSVColor::setV(const int _v) {
	v = Math::Clamp(_v, 0, 255);
}

void HSVColor::setColor(unsigned int _color_code) {
	RGBColor rgb(0, 0, 0);
	rgb.setColor(_color_code);
	*this = rgb.getHSVColor();
}

void HSVColor::shiftR(const int _value) {
	RGBColor rgb = getRGBColor();
	rgb.shiftR(_value);
	*this = rgb.getHSVColor();
}

void HSVColor::shiftG(const int _value) {
	RGBColor rgb = getRGBColor();
	rgb.shiftG(_value);
	*this = rgb.getHSVColor();
}

void HSVColor::shiftB(const int _value) {
	RGBColor rgb = getRGBColor();
	rgb.shiftB(_value);
	*this = rgb.getHSVColor();
}

void HSVColor::shiftH(const int _value) {
	h += _value;
	while (h < 0) {
		h += 360;
	}
	h %= 360;
}

void HSVColor::shiftS(const int _value) {
	s = Math::Clamp(s + _value, 0, 255);
}

void HSVColor::shiftV(const int _value) {
	v = Math::Clamp(v + _value, 0, 255);
}
