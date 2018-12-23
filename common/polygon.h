#pragma once

#include "drawable.h"

namespace cs40 {

class Polygon : public Drawable {

public:
  /* construct a Polygon with given width
   * centered at origin, in z=0 plane*/
  Polygon(QOpenGLShaderProgram *const prog, float width);

  ~Polygon() { /* do nothing */
  }

  /* draw the Polygon*/
  void draw(QOpenGLShaderProgram* prog);

protected:
  float m_width;
};

} // namespace cs40
