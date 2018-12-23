#include "polygon.h"
#include <iostream>

using namespace cs40;
using std::cout;
using std::endl;

Polygon::Polygon(QOpenGLShaderProgram *const prog, float width)
    : Drawable(prog), m_width(width) {
  vec3 *vertices = new vec3[4];
  vec2 *texCoords = new vec2[4];
  vec3 *normals = new vec3[4];
  vec3 *tangents = new vec3[4];
  float d = width / 2;
  vertices[0] = vec3(-d, -d, 0);
  texCoords[0] = vec2(0, 0);
  normals[0] = vec3(0, 0, 1.);
  tangents[0] = vec3(1., 0, 0.);
  vertices[1] = vec3(d, -d, 0);
  texCoords[1] = vec2(1, 0);
  normals[1] = vec3(0, 0, 1.);
  tangents[1] = vec3(1., 0, 0.);
  vertices[2] = vec3(-d, d, 0);
  texCoords[2] = vec2(0, 1);
  normals[2] = vec3(0, 0, 1.);
  tangents[2] = vec3(1., 0, 0.);
  vertices[3] = vec3(d, d, 0);
  texCoords[3] = vec2(1, 1);
  normals[3] = vec3(0, 0, 1.);
  tangents[3] = vec3(1., 0, 0.);

  if (initVBO()) {
    writeData(4, vertices, texCoords, normals, tangents);
  }

  delete[] vertices;
  vertices = nullptr;
  delete[] texCoords;
  texCoords = nullptr;
  delete[] normals;
  normals = nullptr;
  delete[] tangents;
  tangents = nullptr;
}

void Polygon::draw(QOpenGLShaderProgram* prog) {
  bind();
  if(!prog){prog=m_prog;}
  prog->bind();
  prog->setUniformValue("vColor", m_color);
  prog->setUniformValue("vSColor", m_spec_color);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  release();
}
