#pragma once

#include "drawable.h"

namespace cs40 {

class Sphere : public Drawable {
public:
  /* construct a sphere centered at origin with given radius
   * by dividing it into vertical slices and horizontal stacks */
  Sphere(QOpenGLShaderProgram *const prog, float radius, int slices,
         int stacks);

  ~Sphere() { /* do nothing */
  }

  /* draw the sphere */
  void draw(QOpenGLShaderProgram* prog);

protected:
  float m_radius;
  int m_slices;
  int m_stacks;
  int m_stripsize; // num vertices per horizontal strip;
};

} // namespace cs40
