#include "sphere.h"
#include <iostream>

using namespace cs40;

Sphere::Sphere(QOpenGLShaderProgram *const prog, float radius, int slices,
               int stacks)
    : Drawable(prog), m_radius(radius), m_slices(slices), m_stacks(stacks) {

  m_stripsize = (slices + 1) * 2;
  int nverts = m_stripsize * (m_stacks - 2) + 2 * (slices + 2);
  vec3 *vertices = new vec3[nverts];
  vec3 *normals = new vec3[nverts];
  vec3 *tangents = new vec3[nverts];
  vec2 *texCoords = new vec2[nverts];
  double latstep = M_PI / m_stacks;
  double longstep = 2. * M_PI / m_slices;
  double texXstep = 1. / m_slices;
  double texYstep = 1. / m_stacks;

  double lat0 = -M_PI / 2. + latstep;
  double z0 = m_radius * sin(lat0);
  double zcos0 = m_radius * cos(lat0);
  double texY = texYstep;

  double lng = M_PI;
  double texX = 0;
  double coslong = cos(lng);
  double sinlong = sin(lng);
  int idx = 0;

  /* mid lat strips */
  for (int i = 1; i < m_stacks - 1; i++) {
    double lat1 = lat0 + latstep;
    double z1 = m_radius * sin(lat1);
    double zcos1 = m_radius * cos(lat1);

    lng = -M_PI;
    texX = 0;

    for (int j = 0; j <= m_slices; j++) {
      coslong = cos(lng);
      sinlong = sin(lng);
      vertices[idx] = vec3(zcos1 * coslong, zcos1 * sinlong, z1);
      normals[idx] = vec3(cos(lat1) * coslong, cos(lat1) * sinlong, sin(lat1));
      tangents[idx] = vec3(-sinlong, coslong, 0.);
      texCoords[idx++] = vec2(texX, texY + texYstep);
      vertices[idx] = vec3(zcos0 * coslong, zcos0 * sinlong, z0);
      normals[idx] = vec3(cos(lat0) * coslong, cos(lat0) * sinlong, sin(lat0));
      tangents[idx] = vec3(-sinlong, coslong, 0.);
      texCoords[idx++] = vec2(texX, texY);
      lng += longstep;
      texX += texXstep;
    }
    // swap lat1, z1, zcos1 up
    lat0 = lat1;
    z0 = z1;
    zcos0 = zcos1;
    texY += texYstep;
  }

  /* draw polar caps as fans */

  /* north pole */
  vertices[idx] = vec3(0, 0, m_radius);
  normals[idx] = vec3(0, 0., 1.);
  tangents[idx] = vec3(1., 0., 0.);
  texCoords[idx++] = vec2(0.5, 1);
  lat0 = M_PI / 2 - latstep;
  lng = -M_PI;
  z0 = m_radius * sin(lat0);
  texX = 0;
  texY = 1 - texYstep;
  zcos0 = m_radius * cos(lat0);
  for (int i = 0; i <= m_slices; i++) {
    coslong = cos(lng);
    sinlong = sin(lng);
    vertices[idx] = vec3(zcos0 * coslong, zcos0 * sinlong, z0);
    normals[idx] = vec3(cos(lat0) * coslong, cos(lat0) * sinlong, sin(lat0));
    tangents[idx] = vec3(-sinlong, coslong, 0.);
    texCoords[idx++] = vec2(texX, texY);
    lng += longstep;
    texX += texXstep;
  }

  /* south pole */
  vertices[idx] = vec3(0, 0, -m_radius);
  normals[idx] = vec3(0, 0., -1.);
  tangents[idx] = vec3(-1, 0., 0.);
  texCoords[idx++] = vec2(0.5, 0);
  lat0 = -M_PI / 2 + latstep;
  lng = M_PI; /* Q: why M_PI and not -M_PI */
  texX = 1;
  texY = texYstep;
  z0 = m_radius * sin(lat0);
  zcos0 = m_radius * cos(lat0);
  for (int i = 0; i <= m_slices; i++) {
    coslong = cos(lng);
    sinlong = sin(lng);
    vertices[idx] = vec3(zcos0 * coslong, zcos0 * sinlong, z0);
    normals[idx] = vec3(cos(lat0) * coslong, cos(lat0) * sinlong, sin(lat0));
    tangents[idx] = vec3(-sinlong, coslong, 0);
    texCoords[idx++] = vec2(texX, texY);
    lng -= longstep; /* Q: why -= ?*/
    texX -= texXstep;
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

void Sphere::draw(QOpenGLShaderProgram* prog) {
  bind();
  if(!prog){prog=m_prog;}
  prog->bind();
  prog->setUniformValue("vColor", m_color);
  prog->setUniformValue("vSColor", m_spec_color);
  for (int i = 0; i < m_stacks - 2; i++) {
    glDrawArrays(GL_TRIANGLE_STRIP, i * m_stripsize, m_stripsize);
  }
  int offset = (m_stacks - 2) * m_stripsize;
  int fansize = m_slices + 2;
  glDrawArrays(GL_TRIANGLE_FAN, offset, fansize);
  glDrawArrays(GL_TRIANGLE_FAN, offset + fansize, fansize);
  release();
}
