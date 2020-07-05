#ifndef VERTEX_JJ_H
#define VERTEX_JJ_H

#include <array>

namespace Jing
{
  template<typename T>
  struct Vertex : public std::array<T, 3>
  {
    Vertex(T a=0, T b=0, T c=0) : std::array<T, 3>{a, b, c} { }
    T norm() const
      {
        T n = this[0][0]*this[0][0] + this[0][1]*this[0][1] + this[0][2]*this[0][2];
        return sqrt(n);
      }
  };

  template<typename T>
  Vertex<T> operator+(const Vertex<T> &a, const Vertex<T> &b)
  {
    return {a[0]+b[0], a[1]+b[1], a[2]+b[2]};
  }

  template<typename T>
  Vertex<T> operator-(const Vertex<T> &a, const Vertex<T> &b)
  {
    return {a[0]-b[0], a[1]-b[1], a[2]-b[2]};
  }

  template<typename T>
  T operator*(const Vertex<T> &a, const Vertex<T> &b)
  {
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
  }

  template<typename T>
  Vertex<T> operator^(const Vertex<T> &a, const Vertex<T> &b)
  {
    return {a[1]*b[2] - a[2]*b[1],
        a[2]*b[0] - a[0]*b[2],
        a[0]*b[1] - a[1]*b[0]};
  }
}

#endif // VERTEX_JJ_H
