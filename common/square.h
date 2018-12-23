#pragma once

#include "drawable.h"
#include "polygon.h"

using std::vector;

namespace cs40 {

class Square : public Drawable {

public:
  /* construct a square with given width
   * centered at origin, in z=0 plane*/
  Square(QOpenGLShaderProgram *const prog, vec3 v1, vec3 v2, vec3 v3, vec3 v4); //in CCW order

  ~Square() { /* do nothing */
  }

  /* draw the square*/
  void draw(QOpenGLShaderProgram* prog);

  //get all the polygons needed to be drawn in this square, add their info to the VBO
  void getPolygonToDraw();

  vec3 vertices[4];
  float concentrations[4];
  vec3 center;

  vec3 interpolate(int index_1, int index_2);
  int num_vertices_polygon;

  void writeToVBO(int npts, const vec3 *verts, const vec2 *texcoords,
                           const vec3 *normals, const vec3 *tangents);

  void allocateVBO();


protected:
  float m_width;
};

} // namespace cs40
