#pragma once
#include "raylib.h"

// ############################
// #     Vector Operators     #
// ############################

constexpr inline Vector2 operator+=(Vector2& a, const Vector2& b)
{
	a.x += b.x;
	a.y += b.y;
	return a;
}

constexpr inline Vector2 operator-=(Vector2& a, const Vector2& b)
{
	a.x -= b.x;
	a.y -= b.y;
	return a;
}

constexpr inline Vector2 operator*=(Vector2& a, const Vector2& b)
{
	a.x *= b.x;
	a.y *= b.y;
	return a;
}

constexpr inline Vector2 operator*=(Vector2& a, float b)
{
	a.x *= b;
	a.y *= b;
	return a;
}

constexpr inline Vector2 operator/=(Vector2& a, const Vector2& b)
{
	a.x /= b.x;
	a.y /= b.y;
	return a;
}

constexpr inline Vector2 operator/=(Vector2& a, float b)
{
	a.x /= b;
	a.y /= b;
	return a;
}

constexpr inline Vector2 operator+(Vector2 a, const Vector2& b)
{
	a += b;
	return a;
}

constexpr inline Vector2 operator-(Vector2 a, const Vector2& b)
{
	a -= b;
	return a;
}

constexpr inline Vector2 operator*(Vector2 a, const Vector2& b)
{
	a *= b;
	return a;
}

constexpr inline Vector2 operator*(Vector2 a, float b)
{
	a *= b;
	return a;
}

constexpr inline Vector2 operator/(Vector2 a, const Vector2& b)
{
	a /= b;
	return a;
}

constexpr inline Vector2 operator/(Vector2 a, float b)
{
	a /= b;
	return a;
}