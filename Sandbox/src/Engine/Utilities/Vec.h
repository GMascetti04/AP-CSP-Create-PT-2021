#pragma once
template<typename T>
struct Vec1 {
	T x;
};

template<typename T>
struct Vec2 {
	T x, y;

	Vec2() {}
	Vec2(float x_, float y_) : x(x_), y(y_) {}
};

template<typename T>
Vec2<T> operator + (const Vec2<T>& lhs, const Vec2<T>& rhs)
{
	return Vec2<T>(lhs.x+rhs.x, lhs.y+rhs.y);
}

template<typename T>
struct Vec3 {
	T x, y, z;
};

template<typename T>
struct Vec4 {
	T x, y, z, w;
};