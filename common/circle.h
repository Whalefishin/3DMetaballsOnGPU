#pragma once

#include "drawable.h"

namespace cs40 {

class Circle : public Drawable {

public:
  /* Construct a new Circle with given center, radius, numer of triangles
   * Internal Circle should be oriented CCW, regardless of
   * order of corners
   * Shader program is assumed to have the following variables
   *   vec2 vPosition            - VBO geometry
   *   uniform vec2 displacement - an amount to shift each vertex
   *   uniform vec3 color        - color of object */
  Circle(QOpenGLShaderProgram *const prog, const vec3 &p1, const double r,  float v_x,float v_y, float v_z);

  /* Construct a new Circle from an existing Circle */
  Circle(const Circle *const other);

  // No destructor necessary
  ~Circle();

  // Draw the Circle using openGL
  void draw(QOpenGLShaderProgram* prog);

  /* return true if shape contains point pt, false otherwise*/
  bool contains(const vec2 &pt) const;

  //updates coordinates of shape after having been moved
  void updateCoordinates(vec2 displacement);

  vec3 center;
  float radius;
  float v_x;
  float v_y;
  float v_z;

private:
  // Endpoints of Circle in ccw order
  vec3* m_pts;
  int num_triangle;

  /* don't implement this */
  Circle(const Circle &) = delete;
  // prevent shape1=shape2 assignment
  Circle &operator=(const Circle &) = delete;

}; // class Circle

} // namespace cs40
