#pragma once

#include "drawable.h"

namespace cs40 {

class Cylinder : public Drawable {

public:
  /* construct a Cylinder centered at origin with given radius
   * and height (along +y axis)
   * by dividing it into vertical slices and horizontal stacks */
  Cylinder(QOpenGLShaderProgram *const prog, float radius, float height,
           int slices, int stacks);

  ~Cylinder(){/* do nothing */};

  /* draw the cylinder  */
  void draw(QOpenGLShaderProgram* prog);

protected:
  float m_radius;
  float m_height;
  int m_slices;
  int m_stacks;
  int m_stripsize; // num vertices per horizontal strip;
};

} // namespace cs40
