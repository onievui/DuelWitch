#include "Color.h"
#include "MathUtils.h"


RGBColor::RGBColor(const int r, const int g, const int b) {
	SetRGBColor(r, g, b);
}

RGBColor::RGBColor(const unsigned int colorcode) {
	SetColor(colorcode);
}

void RGBColor::SetRGBColor(const int r, const int g, const int b) {
	m_r = Math::Clamp(r, 0, 255);
	m_g = Math::Clamp(g, 0, 255);
	m_b = Math::Clamp(b, 0, 255);
}

void RGBColor::SetHSVColor(const int h, const int s, const int v) {
	HSVColor hsv(h, s, v);
	*this = hsv.GetRGBColor();

}

unsigned int RGBColor::GetColor() const {
	return (m_r << 16) + (m_g << 8) + (m_b << 0);
}

RGBColor RGBColor::GetRGBColor() const {
	return *this;
}

HSVColor RGBColor::GetHSVColor() const {
	int h, s, v, max, min;

	//最大値・最小値の設定
	if (m_r > m_g) {
		max = m_r;
		min = m_g;
	}
	else {
		max = m_g;
		min = m_r;
	}
	if (m_b > max) {
		max = m_b;
	}
	else if (m_b < min) {
		min = m_b;
	}

	//色相の計算
	if (max == min) {
		h = 0;
	}
	else if (max == m_r) {
		h = (int)(60 * ((m_g - m_b) / (float)(max - min)));
	}
	else if (max == m_g) {
		h = (int)(60 * ((m_b - m_r) / (float)(max - min)) + 120);
	}
	else {
		h = (int)(60 * ((m_r - m_g) / (float)(max - min)) + 240);
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

int RGBColor::GetR() const {
	return m_r;
}

int RGBColor::GetG() const {
	return m_g;
}

int RGBColor::GetB() const {
	return m_b;
}

int RGBColor::GetH() const {
	return GetHSVColor().GetH();
}

int RGBColor::GetS() const {
	return GetHSVColor().GetS();
}

int RGBColor::GetV() const {
	return GetHSVColor().GetV();
}

void RGBColor::SetR(const int r) {
	m_r = Math::Clamp(r, 0, 255);
}

void RGBColor::SetG(const int g) {
	m_g = Math::Clamp(g, 0, 255);
}

void RGBColor::SetB(const int b) {
	m_b = Math::Clamp(b, 0, 255);
}

void RGBColor::SetH(const int h) {
	HSVColor hsv = GetHSVColor();
	hsv.SetH(h);
	*this = hsv.GetRGBColor();
}

void RGBColor::SetS(const int s) {
	HSVColor hsv = GetHSVColor();
	hsv.SetS(s);
	*this = hsv.GetRGBColor();
}

void RGBColor::SetV(const int v) {
	HSVColor hsv = GetHSVColor();
	hsv.SetV(v);
	*this = hsv.GetRGBColor();
}

void RGBColor::SetColor(unsigned int colorcode) {
	int rr, gg, bb;
	rr = (colorcode >> 16) & 0xFF;
	gg = (colorcode >>  8) & 0xFF;
	bb = (colorcode >>  0) & 0xFF;
	SetRGBColor(rr, gg, bb);
}

void RGBColor::ShiftR(const int value) {
	m_r = Math::Clamp(m_r + value, 0, 255);
}

void RGBColor::ShiftG(const int value) {
	m_g = Math::Clamp(m_g + value, 0, 255);
}

void RGBColor::ShiftB(const int value) {
	m_b = Math::Clamp(m_b + value, 0, 255);
}

void RGBColor::ShiftH(const int value) {
	HSVColor hsv = GetHSVColor();
	hsv.ShiftH(value);
	*this = hsv.GetRGBColor();
}

void RGBColor::ShiftS(const int value) {
	HSVColor hsv = GetHSVColor();
	hsv.ShiftS(value);
	*this = hsv.GetRGBColor();
}

void RGBColor::ShiftV(const int value) {
	HSVColor hsv = GetHSVColor();
	hsv.ShiftV(value);
	*this = hsv.GetRGBColor();
}


HSVColor::HSVColor(const int h, const int s, const int v) {
	SetHSVColor(h, s, v);
}

HSVColor::HSVColor(const unsigned int colorcode) {
	SetColor(colorcode);
}

void HSVColor::SetRGBColor(const int r, const int g, const int b) {
	RGBColor rgb(r, g, b);
	*this = rgb.GetHSVColor();
}

void HSVColor::SetHSVColor(const int h, const int s, const int v) {
	m_h = h;
	m_s = Math::Clamp(s, 0, 255);
	m_v = Math::Clamp(v, 0, 255);

	while (m_h < 0) {
		m_h += 360;
	}
	m_h %= 360;
}

unsigned int HSVColor::GetColor() const{
	return GetRGBColor().GetColor();
}

RGBColor HSVColor::GetRGBColor() const {
	int r, g, b, max, min;

	//最大値・最小値の計算
	max = m_v;
	min = (int)(max - ((m_s / 255.f) * max));

	//RGBカラーの計算
	if (m_h <= 60) {
		r = max;
		g = (int)((m_h / 60.f) * (max - min) + min);
		b = min;
	}
	else if (m_h <= 120) {
		r = (int)(((120 - m_h) / 60.f) * (max - min) + min);
		g = max;
		b = min;
	}
	else if (m_h <= 180) {
		r = min;
		g = max;
		b = (int)(((m_h - 120) / 60.f) * (max - min) + min);
	}
	else if (m_h <= 240) {
		r = min;
		g = (int)(((240 - m_h) / 60.f) * (max - min) + min);
		b = max;
	}
	else if (m_h <= 300) {
		r = (int)(((m_h - 240) / 60.f) * (max - min) + min);
		g = min;
		b = max;
	}
	else {
		r = max;
		g = min;
		b = (int)(((360 - m_h) / 60.f) * (max - min) + min);
	}

	return RGBColor(r, g, b);
}

HSVColor HSVColor::GetHSVColor() const {
	return *this;
}

int HSVColor::GetR() const {
	return GetRGBColor().GetR();
}

int HSVColor::GetG() const {
	return GetRGBColor().GetG();
}

int HSVColor::GetB() const {
	return GetRGBColor().GetB();
}

int HSVColor::GetH() const {
	return m_h;
}

int HSVColor::GetS() const {
	return m_s;
}

int HSVColor::GetV() const {
	return m_v;
}

void HSVColor::SetR(const int r) {
	RGBColor rgb = GetRGBColor();
	rgb.SetR(r);
	*this = rgb.GetHSVColor();
}

void HSVColor::SetG(const int g) {
	RGBColor rgb = GetRGBColor();
	rgb.SetG(g);
	*this = rgb.GetHSVColor();
}

void HSVColor::SetB(const int b) {
	RGBColor rgb = GetRGBColor();
	rgb.SetB(b);
	*this = rgb.GetHSVColor();
}

void HSVColor::SetH(const int h) {
	m_h = h;
	while (m_h < 0) {
		m_h += 360;
	}
	m_h %= 360;
}

void HSVColor::SetS(const int s) {
	m_s = Math::Clamp(s, 0, 255);
}

void HSVColor::SetV(const int v) {
	m_v = Math::Clamp(v, 0, 255);
}

void HSVColor::SetColor(unsigned int colorcode) {
	RGBColor rgb(0, 0, 0);
	rgb.SetColor(colorcode);
	*this = rgb.GetHSVColor();
}

void HSVColor::ShiftR(const int value) {
	RGBColor rgb = GetRGBColor();
	rgb.ShiftR(value);
	*this = rgb.GetHSVColor();
}

void HSVColor::ShiftG(const int value) {
	RGBColor rgb = GetRGBColor();
	rgb.ShiftG(value);
	*this = rgb.GetHSVColor();
}

void HSVColor::ShiftB(const int value) {
	RGBColor rgb = GetRGBColor();
	rgb.ShiftB(value);
	*this = rgb.GetHSVColor();
}

void HSVColor::ShiftH(const int value) {
	m_h += value;
	while (m_h < 0) {
		m_h += 360;
	}
	m_h %= 360;
}

void HSVColor::ShiftS(const int value) {
	m_s = Math::Clamp(m_s + value, 0, 255);
}

void HSVColor::ShiftV(const int value) {
	m_v = Math::Clamp(m_v + value, 0, 255);
}
