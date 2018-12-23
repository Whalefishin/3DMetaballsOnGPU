#include "cylinder.h"
#include <iostream>

using namespace cs40;
using std::cout;
using std::endl;

Cylinder::Cylinder(QOpenGLShaderProgram *const prog, float radius, float height,
                   int slices, int stacks)
    : Drawable(prog), m_radius(radius), m_height(height), m_slices(slices),
      m_stacks(stacks) {

  m_stripsize = (slices + 1) * 2;
  int nverts = m_stripsize * (m_stacks) + 2 * (slices + 2);
  vec3 *vertices = new vec3[nverts];
  vec3 *normals = new vec3[nverts];
  vec2 *texCoords = new vec2[nverts];
  vec3 *tangents = new vec3[nverts];

  double ystep = m_height / m_stacks;
  double longstep = 2. * M_PI / m_slices;
  double texXstep = 1. / m_slices;
  double texYstep = 1. / m_stacks;

  double y0 = 0;
  double texY = 0;

  double lng = M_PI;
  double texX = 0;
  double coslong = cos(lng);
  double sinlong = sin(lng);
  int idx = 0;

  /* mid strips */
  for (int i = 0; i < m_stacks; i++) {

    double y1 = y0 + ystep;

    lng = -M_PI;
    texX = 0;

    for (int j = 0; j <= m_slices; j++) {
      coslong = m_radius * cos(lng);
      sinlong = m_radius * sin(lng);
      vertices[idx] = vec3(sinlong, y1, coslong);
      normals[idx] = vec3(sin(lng), 0, cos(lng));
      tangents[idx] = vec3(cos(lng), 0, sin(lng));
      texCoords[idx++] = vec2(texX, texY + texYstep);
      vertices[idx] = vec3(sinlong, y0, coslong);
      normals[idx] = vec3(sin(lng), 0, cos(lng));
      tangents[idx] = vec3(cos(lng), 0, sin(lng));
      texCoords[idx++] = vec2(texX, texY);
      lng += longstep;
      texX += texXstep;
    }
    // move up
    y0 = y1;
    texY += texYstep;
  }

  /* draw caps as fans */

  /* upper cap */
  vertices[idx] = vec3(0, m_height, 0);
  normals[idx] = vec3(0, 1., 0.);
  tangents[idx] = vec3(1, 0, 0);
  texCoords[idx++] = vec2(0.5, 1);
  lng = -M_PI;
  for (int i = 0; i <= m_slices; i++) {
    coslong = m_radius * cos(lng);
    sinlong = m_radius * sin(lng);
    vertices[idx] = vec3(sinlong, m_height, coslong);
    normals[idx] = vec3(0, 1., 0.);
    tangents[idx] = vec3(1, 0, 0);
    texCoords[idx++] = vec2(0.5, 1);
    lng += longstep;
  }

  /* lower cap */
  vertices[idx] = vec3(0, 0, 0);
  normals[idx] = vec3(0, -1., 0.);
  tangents[idx] = vec3(-1, 0, 0);
  texCoords[idx++] = vec2(0.5, 0);
  lng = M_PI;
  for (int i = 0; i <= m_slices; i++) {
    coslong = m_radius * cos(lng);
    sinlong = m_radius * sin(lng);
    vertices[idx] = vec3(sinlong, 0, coslong);
    normals[idx] = vec3(0, -1., 0.);
    tangents[idx] = vec3(-1, 0, 0);
    texCoords[idx++] = vec2(0.5, 0);
    lng -= longstep;
  }

  if (initVBO()) {
    writeData(nverts, vertices, texCoords, normals, tangents);
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

void Cylinder::draw(QOpenGLShaderProgram* prog) {
  bind();
  if(!prog){
    prog=m_prog;
  }
  prog->bind();
  prog->setUniformValue("vColor", m_color);
  prog->setUniformValue("vSColor", m_spec_color);
  for (int i = 0; i < m_stacks; i++) {
    glDrawArrays(GL_TRIANGLE_STRIP, i * m_stripsize, m_stripsize);
  }
  int offset = (m_stacks)*m_stripsize;
  int fansize = m_slices + 2;
  glDrawArrays(GL_TRIANGLE_FAN, offset, fansize);
  glDrawArrays(GL_TRIANGLE_FAN, offset + fansize, fansize);
  release();
}
