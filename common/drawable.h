#pragma once

#include <QtOpenGL>
#include <utility>
namespace cs40 {

typedef QVector2D vec2;
typedef QVector3D vec3;
typedef QVector4D vec4;

class Drawable {
public:
  /* store pointer to program */
  Drawable(QOpenGLShaderProgram *const prog);

  virtual ~Drawable();

  /* draw the shape using provided shader program  */
  virtual void draw(QOpenGLShaderProgram* prog=nullptr) = 0;

  /* Set color of shape */
  inline void setColor(const vec3 &color) { m_color = color; }
  inline void setColor(float r, float g, float b) { m_color = vec3(r, g, b); }
  /* Get current color of object */
  inline vec3 getColor() const { return m_color; }

  inline void setSpecularColor(const vec3 &color) { m_spec_color = color; }
  inline void setSpecularColor(float r, float g, float b) {
    m_spec_color = vec3(r, g, b);
  }
  /* Get current color of object */
  inline vec3 getSpecularColor() const { return m_spec_color; }

protected:
  /* Generate VAO/VBO, but do not bind/allocate.
   * Return true if generate was a success, false otherwise
   */
  bool initVBO();
  void cleanUp();

  /* bind VAO/program */
  void bind();

  //checks if point q is left of vector from p1 to p2
  bool leftOf(QVector3D p1, QVector3D p2, QVector3D q) const;

  /* release VAO/program */
  void release();

  void writeData(int npts, const vec3 *verts, const vec2 *texcoord,
                 const vec3 *normals, const vec3 *tangents);

  QOpenGLShaderProgram *const m_prog;
  QOpenGLVertexArrayObject *m_vao;
  QOpenGLBuffer *m_vbo;
  vec3 m_color;
  vec3 m_spec_color;
};

} // namespace cs40
