#include "drawable.h"

using namespace cs40;

Drawable::Drawable(QOpenGLShaderProgram *const prog)
    : m_prog(prog), m_vao(nullptr), m_vbo(nullptr), m_color(1, 0, 0),
      m_spec_color(1, 1, 1) {}

Drawable::~Drawable() {
  cleanUp();
}

void Drawable::cleanUp(){
  if (m_vbo) {
    m_vbo->release();
    delete m_vbo;
    m_vbo = nullptr;
  }
  if (m_vao) {
    m_vao->release();
    delete m_vao;
    m_vao = nullptr;
  }
}

bool Drawable::initVBO() {
  m_vao = new QOpenGLVertexArrayObject();
  bool ok = m_vao->create();
  if (!ok) {
    return false;
  }
  m_vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
  ok = m_vbo->create();
  if (!ok) {
    return false;
  }
  m_vbo->setUsagePattern(QOpenGLBuffer::StaticDraw);
  return ok;
}

void Drawable::bind() {
  if (m_vao) {
    m_vao->bind();
  }
  if (m_prog) {
    m_prog->bind();
  }
}

void Drawable::release() {
  if (m_vao) {
    m_vao->release();
  }
}

void Drawable::writeData(int npts, const vec3 *verts, const vec2 *texcoords,
                         const vec3 *normals, const vec3 *tangents) {

  /* size of verts, normals, or tangent data */
  int chunksize = npts * sizeof(vec3);
  /* size of texture data  */
  int bitesize = npts * sizeof(vec2);
  int offset = 0;

  if (m_vbo) {
    bind();
    m_vbo->bind();
    m_vbo->allocate(3 * chunksize + bitesize);
    m_vbo->write(offset, verts, chunksize);
    m_prog->enableAttributeArray("vPosition");
    m_prog->setAttributeBuffer("vPosition", GL_FLOAT, offset, 3, 0);
    offset += chunksize;
    m_vbo->write(offset, texcoords, bitesize);
    m_prog->enableAttributeArray("vTexture");
    m_prog->setAttributeBuffer("vTexture", GL_FLOAT, offset, 2, 0);
    offset += bitesize;
    m_vbo->write(offset, normals, chunksize);
    m_prog->enableAttributeArray("vNormal");
    m_prog->setAttributeBuffer("vNormal", GL_FLOAT, offset, 3, 0);
    offset += chunksize;
    m_vbo->write(offset, tangents, chunksize);
    m_prog->enableAttributeArray("vTangent");
    m_prog->setAttributeBuffer("vTangent", GL_FLOAT, offset, 3, 0);
    m_vbo->release();
    release();
  }
}

bool Drawable::leftOf(QVector3D p1, QVector3D p2, QVector3D q) const {
    return (QVector3D::dotProduct(QVector3D::crossProduct((p2-p1), (q-p2)), QVector3D(0,0,1)) > 0);
}
