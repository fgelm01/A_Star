/* 
 * File:   vec2.hpp
 * Author: Filipp
 *
 * Created on April 21, 2012, 12:01 AM
 */

#ifndef VEC2_HPP
#define	VEC2_HPP

#include <cmath>
#include <iostream>

template <typename T>
class vec2;

typedef vec2<float> vec2f;
typedef vec2<double> vec2d;
typedef vec2<int> vec2i;

template <typename T>
class vec2{
public:
	vec2(T nx=0, T ny=0);
	template <typename U>
	vec2(const vec2<U>& other);
	template <typename U>
	vec2& operator=(const vec2<U>& other);

	T abs() const;
	vec2 norm() const;
	template <typename U>
	T dot(const vec2<U>& other) const;
	template <typename U>
	T cross(const vec2<U>& other) const;
	template <typename U>
	vec2 rotate(U angle) const;	//radians ccw
	template <typename U>
	vec2 rotate(const vec2<U>& cs) const;
	T to_angle() const;
	T x, y;
};

template <typename T>
std::ostream& operator<<(std::ostream& out, const vec2<T>& v);
template <typename T>
std::istream& operator>>(std::istream& in, vec2<T>& v);

template <typename T, typename U>
bool operator==(const vec2<T>& l, const vec2<U>& r);
template <typename T, typename U>
bool operator!=(const vec2<T>& l, const vec2<U>& r);

template <typename T, typename U>
vec2<T>& operator+=(vec2<T>& l, const vec2<U>& r);
template <typename T, typename U>
vec2<T>& operator-=(vec2<T>& l, const vec2<U>& r);
template <typename T>
vec2<T> operator-(const vec2<T>& r);
template <typename T, typename U>
vec2<T>& operator*=(vec2<T>& l, const vec2<U>& r);
template <typename T, typename U>
vec2<T>& operator/=(vec2<T>& l, const vec2<U>& r);
template <typename T, typename U>
vec2<T>& operator*=(vec2<T>& l, U r);
template <typename T, typename U>
vec2<T>& operator/=(vec2<T>& l, U r);

template <typename T, typename U>
vec2<T> operator+(vec2<T> l, const vec2<U>& r);
template <typename T, typename U>
vec2<T> operator-(vec2<T> l, const vec2<U>& r);
template <typename T, typename U>
vec2<T> operator*(vec2<T> l, const vec2<U>& r);
template <typename T, typename U>
vec2<T> operator/(vec2<T> l, const vec2<U>& r);
template <typename T, typename U>
vec2<T> operator*(vec2<T> l, U r);
template <typename T, typename U>
vec2<T> operator/(vec2<T> l, U r);
template <typename T, typename U>
vec2<T> operator*(U l, vec2<T> r);

template <typename T>
vec2<T>::vec2(T nx, T ny) : x(nx), y(ny) {}
template <typename T> template <typename U>
vec2<T>::vec2(const vec2<U>& other) : x(other.x), y(other.y) {}
template <typename T> template <typename U>
vec2<T>& vec2<T>::operator=(const vec2<U>& other){
	x=other.x;
	y=other.y;
}

template <typename T>
T vec2<T>::abs() const{
	return sqrt(x*x + y*y);
}
template <typename T>
vec2<T> vec2<T>::norm() const{
	return *this/abs();
}
template <typename T> template <typename U>
T vec2<T>::dot(const vec2<U>& other) const{
	return x*other.x + y*other.y;
}
template <typename T> template <typename U>
T vec2<T>::cross(const vec2<U>& other) const{
	return x*other.y - y*other.x;
}
template <typename T> template <typename U>
vec2<T> vec2<T>::rotate(U angle) const{
	return rotate(vec2<U>(cos(angle), sin(angle)));
}
template <typename T> template <typename U>
vec2<T> vec2<T>::rotate(const vec2<U>& cs) const{
	return vec2(
			x*cs.x-y*cs.y,
			x*cs.y+y*cs.x
			);
}
template <typename T>
T vec2<T>::to_angle() const{
	return atan2(y, x);
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const vec2<T>& v){
	return out << v.x << ' ' << v.y << ' ';
}
template <typename T>
std::istream& operator>>(std::istream& in, vec2<T>& v){
	return in >> v.x >> v.y;
}

template <typename T, typename U>
bool operator==(const vec2<T>& l, const vec2<U>& r){
	return l.x==r.x && l.y==r.y;
}
template <typename T, typename U>
bool operator!=(const vec2<T>& l, const vec2<U>& r){
	return l.x!=r.x || l.y!=r.y;
}

template <typename T, typename U>
vec2<T>& operator+=(vec2<T>& l, const vec2<U>& r){
	l.x += r.x;
	l.y += r.y;
	return l;
}
template <typename T, typename U>
vec2<T>& operator-=(vec2<T>& l, const vec2<U>& r){
	return l+= -r;
}
template <typename T>
vec2<T> operator-(const vec2<T>& r){
	return vec2<T>(-r.x, -r.y);
}
template <typename T, typename U>
vec2<T>& operator*=(vec2<T>& l, const vec2<U>& r){
	l.x *= r.x;
	l.y *= r.y;
	return l;
}
template <typename T, typename U>
vec2<T>& operator/=(vec2<T>& l, const vec2<U>& r){
	l.x /= r.x;
	l.y /= r.y;
	return l;
}
template <typename T, typename U>
vec2<T>& operator*=(vec2<T>& l, U r){
	return l*=vec2<U>(r,r);
}
template <typename T, typename U>
vec2<T>& operator/=(vec2<T>& l, U r){
	return l/=vec2<U>(r,r);
}

template <typename T, typename U>
vec2<T> operator+(vec2<T> l, const vec2<U>& r){
	return l+=r;
}
template <typename T, typename U>
vec2<T> operator-(vec2<T> l, const vec2<U>& r){
	return l-=r;
}
template <typename T, typename U>
vec2<T> operator*(vec2<T> l, const vec2<U>& r){
	return l*=r;
}
template <typename T, typename U>
vec2<T> operator/(vec2<T> l, const vec2<U>& r){
	return l/=r;
}
template <typename T, typename U>
vec2<T> operator*(vec2<T> l, U r){
	return l*=r;
}
template <typename T, typename U>
vec2<T> operator/(vec2<T> l, U r){
	return l/=r;
}
template <typename T, typename U>
vec2<T> operator*(U l, vec2<T> r){
	return r*=l;
}


#endif	/* VEC2_HPP */

