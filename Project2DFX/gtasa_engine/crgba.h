#ifndef _CRGBA
#define _CRGBA

typedef unsigned char BYTE;

class CRGBA
{
public:
	BYTE r, g, b, a;

	inline CRGBA() {}

	inline CRGBA(const CRGBA& in)
		: r(in.r), g(in.g), b(in.b), a(in.a)
	{}

	inline CRGBA(const CRGBA& in, BYTE alpha)
		: r(in.r), g(in.g), b(in.b), a(alpha)
	{}

	inline CRGBA(BYTE red, BYTE green, BYTE blue, BYTE alpha = 255)
		: r(red), g(green), b(blue), a(alpha)
	{}

	template <typename T>
	friend CRGBA Blend(const CRGBA& One, T OneStrength, const CRGBA& Two, T TwoStrength)
		{	T	TotalStrength = OneStrength + TwoStrength;
			return CRGBA(	((One.r * OneStrength) + (Two.r * TwoStrength))/TotalStrength,
							((One.g * OneStrength) + (Two.g * TwoStrength))/TotalStrength,
							((One.b * OneStrength) + (Two.b * TwoStrength))/TotalStrength,
							((One.a * OneStrength) + (Two.a * TwoStrength))/TotalStrength); }

	template <typename T>
	friend CRGBA Blend(const CRGBA& One, T OneStrength, const CRGBA& Two, T TwoStrength, const CRGBA& Three, T ThreeStrength)
		{	T	TotalStrength = OneStrength + TwoStrength + ThreeStrength;
			return CRGBA(	((One.r * OneStrength) + (Two.r * TwoStrength) + (Three.r * ThreeStrength))/TotalStrength,
							((One.g * OneStrength) + (Two.g * TwoStrength) + (Three.g * ThreeStrength))/TotalStrength,
							((One.b * OneStrength) + (Two.b * TwoStrength) + (Three.b * ThreeStrength))/TotalStrength,
							((One.a * OneStrength) + (Two.a * TwoStrength) + (Three.a * ThreeStrength))/TotalStrength); }

	void	BaseColors__Constructor();
};

#endif // _CRGBA