#include "circle.h"
#include <math.h>


using namespace cs40;

Circle::Circle(QOpenGLShaderProgram *const prog, const vec3 &p1, const double r, float v_x,float v_y, float v_z)
    : Drawable(prog){

  center = p1;
  radius = r;
  this->v_x = v_x;
  this->v_y = v_y;
  this->v_z = v_z;

  this->num_triangle = 100;

  m_pts = new vec3[num_triangle*3];

  vec3 *normals = new vec3[3*num_triangle];
  vec3 *tangents = new vec3[3*num_triangle];
  vec2 *texCoords = new vec2[3*num_triangle];

  //set to dummy values for now
  for (size_t i = 0; i < 3*num_triangle; i++) {
      normals[i] = vec3(0, 0, 1.);
      tangents[i] = vec3(1., 0, 0.);
      texCoords[i] = vec2(0, 0);
  }

  //generate theta
  int j=0;
  for (int i = num_triangle-1; i >=0; i--) {
      double angle2 = 2*M_PI * double(i+1) / num_triangle;
      double angle1 = 2*M_PI * double(i) / num_triangle;
      double x1 = p1.x() + r*cos(angle1);
      double y1 = p1.y()+ r*sin(angle1);
      double x2 = p1.x() + r*cos(angle2);
      double y2 = p1.y()+ r*sin(angle2);

      vec3 vertex1  = vec3(x1,y1,0);
      vec3 vertex2  = vec3(x2,y2,0);
      m_pts[j] = p1;
      m_pts[j+1] = vertex1;
      m_pts[j+2] = vertex2;
      j += 3;
  }

    //checking leftof - if points don't pass leftof, reorient
    bool flag = true;
    //check if all points are in ccw
    for(int i = 0; i < num_triangle*3; i+=3) {
        if (!leftOf(m_pts[i+1], m_pts[i+2], m_pts[i])){
            flag = false;
        }
    }

    if (flag == true){
        if (initVBO()) {
          writeData(3*num_triangle, m_pts, texCoords, normals, tangents);
      }
    }
    else{
        j=0;
        for (int i = 0; i <num_triangle; i++) {
            double angle1 = 2*M_PI * double(i) / num_triangle;
            double angle2 = 2*M_PI * double(i+1) / num_triangle;
            double x1 = p1.x() + r*cos(angle1);
            double y1 = p1.y()+ r*sin(angle1);
            double x2 = p1.x() + r*cos(angle2);
            double y2 = p1.y()+ r*sin(angle2);

            vec3 vertex1  = vec3(x1,y1,0);
            vec3 vertex2  = vec3(x2,y2,0);
            m_pts[j] = p1;
            m_pts[j+1] = vertex1;
            m_pts[j+2] = vertex2;
            j += 3;
        }
        if (initVBO()) {
          writeData(3*num_triangle, m_pts, texCoords, normals, tangents);
      }
        //m_offset = m_helper->addPoints(m_pts, 3*num_triangle);
    }

    //qDebug() << "Verify leftOf Circle" << flag;
    //There's no need to adjust the CCW, because the way
    //we draw the circle always guarantees CCW.
}

Circle::Circle(const Circle *const other) : Drawable(other->m_prog) {
  this->num_triangle = other->num_triangle;

  for (int i=0;i<3*num_triangle;i++){
      this->m_pts[i] = other->m_pts[i];
  }

  vec3 *normals = new vec3[3*num_triangle];
  vec3 *tangents = new vec3[3*num_triangle];
  vec2 *texCoords = new vec2[3*num_triangle];
  for (size_t i = 0; i < 3*num_triangle; i++) {
      normals[i] = vec3(0, 0, 1.);
      tangents[i] = vec3(1., 0, 0.);
      texCoords[i] = vec2(0, 1.);
  }

  writeData(3*num_triangle, m_pts, texCoords, normals, tangents);
  //m_offset = m_helper->addPoints(m_pts, 3*num_triangle);
}

Circle::~Circle(){
}

void Circle::draw(QOpenGLShaderProgram* prog) {
  bind(); /* bind VAO, progam, see Drawable::bind() */
  if(!prog){prog=m_prog;}
  prog->bind();
  //set *all* uniforms in shaders
  prog->setUniformValue("vSColor", m_spec_color);
  prog->setUniformValue("vColor", m_color);
  //make approriate glDrawArrays call
  for (int i = 0; i < num_triangle; i++) {
      glDrawArrays(GL_TRIANGLES, 3*i, 3); //uses current shader, etc, which are determined by bind()
  }

  //glFlush();
  release(); /* Drawable::release() */
}

bool Circle::contains(const vec2 &pt) const {
    bool flag = true;
    for(int i = 0; i < num_triangle*3; i+=3) {
        if (!leftOf(m_pts[i+1], m_pts[i+2],pt)){
            flag = false;
        }
    }
  return flag;
}
void Circle::updateCoordinates(vec2 displacement){
  for (int i=0;i<3*num_triangle;i++){
      m_pts[i]+=displacement;
  }
}
