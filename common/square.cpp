#include "square.h"
#include <iostream>

using namespace cs40;
using std::cout;
using std::endl;

Square::Square(QOpenGLShaderProgram *const prog, vec3 v1, vec3 v2, vec3 v3, vec3 v4)
    : Drawable(prog) {

  vertices[0] = v1;
  vertices[1] = v2;
  vertices[2] = v3;
  vertices[3] = v4;

  allocateVBO();


  // if (initVBO()) {
  //   writeData(4, vertices, texCoords, normals, tangents);
  // }
}

void Square::getPolygonToDraw(){
  vector<vec3> ret; //stores the vertices for the polygon to draw

  //push the saturated and interpolated points
  for (int i=0;i<4;i++){
    int index_1 = (i%4);
    int index_2 = (i+1)%4;
    vec3 p1 = vertices[index_1];
    if (concentrations[index_1] >=1){
      ret.push_back(p1);
    }
    if (concentrations[index_1] >=1 && concentrations[index_2] < 1){
      vec3 p = interpolate(index_1,index_2);
      ret.push_back(p);
    }
    else if ((concentrations[index_1] <1 && concentrations[index_2] >=1)){
      vec3 p = interpolate(index_2,index_1);
      ret.push_back(p);
    }
  }

  //push the start node again
  if (ret.size()!=0){
    ret.push_back(ret[0]);
  }


  //add in the vertices into the VBO
  if (initVBO()) {
    vec2 *texCoords = new vec2[ret.size()];
    vec3 *normals = new vec3[ret.size()];
    vec3 *tangents = new vec3[ret.size()];

    for (int i=0;i<ret.size();i++){
      texCoords[i] = vec2(0, 0);
      normals[i] = vec3(0, 0, 1.);
      tangents[i] = vec3(1., 0, 0.);
    }

    if (ret.size()!=0){
        //cout <<"here's the ret array" << endl;
    }

    vec3* ret_array = new vec3[ret.size()]; //manually convert the vector into an array
    for (int i=0;i<ret.size();i++){
    //  cout << "x: " << ret[i].x() <<  " y: " << ret[i].y() << " z: " << ret[i].z() << endl;
      ret_array[i] = ret[i];
    }

    //writeToVBO(ret.size(), ret_array, texCoords, normals, tangents);
    writeData(ret.size(), ret_array, texCoords, normals, tangents); //this version keeps allocating space to VBO

    num_vertices_polygon = ret.size();


    delete[] ret_array;
    delete[] texCoords;
    texCoords = nullptr;
    delete[] normals;
    normals = nullptr;
    delete[] tangents;
    tangents = nullptr;
  }
}

void Square::draw(QOpenGLShaderProgram* prog) {
  bind();
  if(!prog){prog=m_prog;}
  prog->bind();
  prog->setUniformValue("vColor", m_color);
  prog->setUniformValue("vSColor", m_spec_color);
  //cout <<"num_vertices_polygon: " << num_vertices_polygon <<  endl;
  glDrawArrays(GL_TRIANGLE_FAN, 0, num_vertices_polygon);
  release();
  cleanUp(); //so we don't create a billion VBOs
}

vec3 Square::interpolate(int index_1, int index_2 ){
  //assume that the first index is the lit vertex
  vec3 p1 = vertices[index_1];
  vec3 p2 = vertices[index_2];
  float c1 = concentrations[index_1];
  float c2 = concentrations[index_2];
  vec3 ret;
  ret.setZ(0);

  //check if the edge is vertical or horizontal
  if (p1.x() == p2.x()){
    //this means the edge is vertical
    ret.setX(p1.x());
    ret.setY(p2.y() + (p1.y()-p2.y())* (1-c2)/(c1-c2));
  }
  else{
    ret.setY(p1.y());
    ret.setX(p2.x() + (p1.x()-p2.x())* (1-c2)/(c1-c2));
  }

  return ret;
}

void Square::allocateVBO() {

  //assume a square creates a polygon of at most 6 vertices.
  //This gets us 24 for position, normal, tangent, and text coords
  int chunksize = 8 * sizeof(vec3);
  /* size of texture data  */
  int bitesize = 8 * sizeof(vec2);

  if (m_vbo) {
    bind();
    m_vbo->bind();
    m_vbo->allocate(3 * chunksize + bitesize);
    m_vbo->release();
    release();
  }
}

void Square::writeToVBO(int npts, const vec3 *verts, const vec2 *texcoords,
                         const vec3 *normals, const vec3 *tangents) {

  //assume a square creates a polygon of at most 6 vertices.
  int chunksize = 8 * sizeof(vec3);
  /* size of texture data  */
  int bitesize = 8 * sizeof(vec2);
  int offset = 0;

  if (m_vbo) {
    bind();
    m_vbo->bind();
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
