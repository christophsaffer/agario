#pragma once

namespace me {

template <typename Real> struct vector2 {
  Real x{};
  Real y{};
};

using vector2f = vector2<float>;
using vector2d = vector2<double>;

template <typename Real>
inline vector2<Real> operator+(vector2<Real> v, vector2<Real> w) noexcept {
  return {v.x + w.x, v.y + w.y};
}

template <typename Real>
inline vector2<Real> operator-(vector2<Real> v, vector2<Real> w) noexcept {
  return {v.x - w.x, v.y - w.y};
}

template <typename Real>
inline vector2<Real> operator*(Real s, vector2<Real> v) noexcept {
  return {s * v.x, s * v.y};
}

template <typename Real>
inline Real dot(vector2<Real> v, vector2<Real> w) noexcept {
  return v.x * w.x + v.y * w.y;
}

template <typename Real>
inline Real operator*(vector2<Real> v, vector2<Real> w) noexcept {
  return {v.x * w.x, v.y * w.y};
}

template <typename Real> inline Real squared_norm(vector2<Real> v) noexcept {
  return dot(v, v);
}

template <typename Real>
inline void operator+=(vector2<Real> &v, vector2<Real> w) noexcept {
  v.x += w.x;
  v.y += w.y;
}

} // namespace me
