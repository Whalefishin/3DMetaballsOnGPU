#pragma once

#include "drawable.h"

using std::vector;

namespace cs40 {

class Cube : public Drawable {

public:
  /* construct a Cube with given width
   * centered at origin, in z=0 plane*/
  Cube(QOpenGLShaderProgram *const prog, vec3 v1, vec3 v2, vec3 v3, vec3 v4,
      vec3 v5, vec3 v6, vec3 v7, vec3 v8); //in CCW order

  ~Cube() { /* do nothing */
  }

  /* draw the Cube*/
  void draw(QOpenGLShaderProgram* prog);


  vec3 vertices[8];
  float concentrations3D[8];
  vec3 center;

  int num_triangle;

  vec3 calcNormal(const vec3 &p, vec3 v2, vec3 v3) const;
  vec3 vertexInterp(double isolevel, vec3 p1, vec3 p2, double valp1, double valp2) ;
  void polygonise(double isolevel);


protected:
  float m_width;
};

} // namespace cs40
