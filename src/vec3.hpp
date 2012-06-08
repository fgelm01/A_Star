#ifndef VEC3_HPP
#define	VEC3_HPP

#include <cmath>
#include <iostream>

template <typename T>
class vec3;

typedef vec3<float> vec3f;
typedef vec3<double> vec3d;


template <typename T>
class vec3{
public:
	vec3(T nx=0, T ny=0, T nz=0);
	template <typename U>
	vec3(const vec3<U>& other);
	template <typename U>
	vec3& operator=(const vec3<U>& other);

	T abs() const;
	vec3 norm() const;
	template <typename U>
	T dot(const vec3<U>& other) const;
	template <typename U>
	vec3 cross(const vec3<U>& other) const;

	T x, y, z;
};

template <typename T>
std::ostream& operator<<(std::ostream& out, const vec3<T>& v);
template <typename T>
std::istream& operator>>(std::istream& in, vec3<T>& v);

template <typename T, typename U>
vec3<T>& operator+=(vec3<T>& l, const vec3<U>& r);
template <typename T, typename U>
vec3<T>& operator-=(vec3<T>& l, const vec3<U>& r);
template <typename T>
vec3<T> operator-(const vec3<T>& r);
template <typename T, typename U>
vec3<T>& operator*=(vec3<T>& l, const vec3<U>& r);
template <typename T, typename U>
vec3<T>& operator/=(vec3<T>& l, const vec3<U>& r);
template <typename T, typename U>
vec3<T>& operator*=(vec3<T>& l, U r);
template <typename T, typename U>
vec3<T>& operator/=(vec3<T>& l, U r);

template <typename T, typename U>
vec3<T> operator+(vec3<T> l, const vec3<U>& r);
template <typename T, typename U>
vec3<T> operator-(vec3<T> l, const vec3<U>& r);
template <typename T, typename U>
vec3<T> operator*(vec3<T> l, const vec3<U>& r);
template <typename T, typename U>
vec3<T> operator/(vec3<T> l, const vec3<U>& r);
template <typename T, typename U>
vec3<T> operator*(vec3<T> l, U r);
template <typename T, typename U>
vec3<T> operator/(vec3<T> l, U r);
template <typename T, typename U>
vec3<T> operator*(U l, vec3<T> r);

template <typename T>
vec3<T>::vec3(T nx, T ny, T nz) : x(nx), y(ny), z(nz) {}
template <typename T> template <typename U>
vec3<T>::vec3(const vec3<U>& other) : x(other.x), y(other.y), z(other.z) {}
template <typename T> template <typename U>
vec3<T>& vec3<T>::operator=(const vec3<U>& other){
	x=other.x;
	y=other.y;
	z=other.z;
}

template <typename T>
T vec3<T>::abs() const{
	return sqrt(x*x + y*y + z*z);
}
template <typename T>
vec3<T> vec3<T>::norm() const{
	return *this/abs();
}
template <typename T> template <typename U>
T vec3<T>::dot(const vec3<U>& other) const{
	return x*other.x + y*other.y + z*other.z;
}
template <typename T> template <typename U>
vec3<T> vec3<T>::cross(const vec3<U>& other) const{
	return vec3(
			y*other.z - z*other.y,
			z*other.x - x*other.z,
			x*other.y - y*other.x
			);
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const vec3<T>& v){
	return out << v.x << ' ' << v.y << ' ' << v.z << ' ';
}
template <typename T>
std::istream& operator>>(std::istream& in, vec3<T>& v){
	return in >> v.x >> v.y >> v.z;
}

template <typename T, typename U>
vec3<T>& operator+=(vec3<T>& l, const vec3<U>& r){
	l.x += r.x;
	l.y += r.y;
	l.z += r.z;
	return l;
}
template <typename T, typename U>
vec3<T>& operator-=(vec3<T>& l, const vec3<U>& r){
	return l+= -r;
}
template <typename T>
vec3<T> operator-(const vec3<T>& r){
	return vec3<T>(-r.x, -r.y, -r.z);
}
template <typename T, typename U>
vec3<T>& operator*=(vec3<T>& l, const vec3<U>& r){
	l.x *= r.x;
	l.y *= r.y;
	l.z *= r.z;
	return l;
}
template <typename T, typename U>
vec3<T>& operator/=(vec3<T>& l, const vec3<U>& r){
	l.x /= r.x;
	l.y /= r.y;
	l.z /= r.z;
	return l;
}
template <typename T, typename U>
vec3<T>& operator*=(vec3<T>& l, U r){
	return l*=vec3<U>(r,r,r);
}
template <typename T, typename U>
vec3<T>& operator/=(vec3<T>& l, U r){
	return l/=vec3<U>(r,r,r);
}

template <typename T, typename U>
vec3<T> operator+(vec3<T> l, const vec3<U>& r){
	return l+=r;
}
template <typename T, typename U>
vec3<T> operator-(vec3<T> l, const vec3<U>& r){
	return l-=r;
}
template <typename T, typename U>
vec3<T> operator*(vec3<T> l, const vec3<U>& r){
	return l*=r;
}
template <typename T, typename U>
vec3<T> operator/(vec3<T> l, const vec3<U>& r){
	return l/=r;
}
template <typename T, typename U>
vec3<T> operator*(vec3<T> l, U r){
	return l*=r;
}
template <typename T, typename U>
vec3<T> operator/(vec3<T> l, U r){
	return l/=r;
}
template <typename T, typename U>
vec3<T> operator*(U l, vec3<T> r){
	return r*=l;
}



#endif	/* VEC3_HPP */

