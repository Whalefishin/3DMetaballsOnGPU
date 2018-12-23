#include "mypanelopengl.h"
#include <cmath>
#include <iostream>
#include <ctime>
#include <cstdlib>

using cs40::Sphere;
using cs40::Square;
using cs40::Circle;
using cs40::vec4;
using cs40::vec3;
using std::cout;
using std::endl;

struct GRIDCELL {
    //concentrations
    float val[8];
    //points
    vec3 p[8];
};

MyPanelOpenGL::MyPanelOpenGL(QWidget *parent)
    : QOpenGLWidget(parent), m_angles(0, 15., 0.) {
  /* Listen for keypress events */
  setFocusPolicy(Qt::StrongFocus);
  for (int i = 0; i < 2; i++) {
    m_shaderPrograms[i] = nullptr;
    m_vertexShaders[i] = nullptr;
    m_fragmentShaders[i] = nullptr;
  }
  m_square = nullptr;
  m_drawSphere = false;
  m_polymode = 2;
  m_cull = false;
  m_curr_prog = 0;
  m_timer = nullptr;
  yaw_angle = 2.0;

  W = 30;
  H = 30;
  L = 30;

  for (int i=0;i<W;i++){
    vector<float> toPush;
    concentrations.push_back(toPush);
    for (int j=0;j<H;j++){
      concentrations[i].push_back(0);
    }
  }

  for (int i=0;i<W;i++){
    vector<vector<float>> toPush;
    concentrations3D.push_back(toPush);
    for (int j=0;j<H;j++){
        vector<float> toPush_2;
        concentrations3D[i].push_back(toPush_2);
        for (int k=0;k<L;k++){
            concentrations3D[i][j].push_back(0);
        }
    }
  }

}

MyPanelOpenGL::~MyPanelOpenGL() {
  m_shaderPrograms[m_curr_prog]->release();

  // delete m_sphere;
  // m_sphere = nullptr;
  // delete m_square;
  // m_square = nullptr;
  for (int i=0;i<num_circles;i++){
    delete m_circles[i];
    m_circles[i] = nullptr;
  }

  if (m_timer) {
  delete m_timer;
  m_timer = nullptr;
  }
  destroyShaders(0);
  destroyShaders(1);
}

void MyPanelOpenGL::initializeGL() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  setCulling();
  updatePolyMode(m_polymode);

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  createShaders(0, "vshader.glsl", "fshader.glsl");
  createShaders(1, "vfraglight.glsl", "ffraglight.glsl");

  // m_sphere = new Sphere(m_shaderPrograms[m_curr_prog], 0.5, 8, 8);
  // m_sphere->setColor(0., 0., 0.7);
  // m_square = new Square(m_shaderPrograms[m_curr_prog], 1.);


  //the resolution for our simulation
  //tessellate2D(2,2);

  tessellate3D(2,2,2);


  //create an array of circles
  num_circles = 10;
  num_spheres = 10;
//  m_circles = new Circle*[num_circles];

  std::srand(std::time(nullptr)); // use current time as seed for random generator

  for (int i=0;i<num_circles;i++){
    Circle* circle;
    m_circles.push_back(circle);
  }

  for (int i=0;i<num_circles;i++){
    float v_x = random(-0.01,0.01); //movement in the x-direction
    float v_y = random(-0.01,0.01); //movement in the y-direction
    //randomly distribute some circles to start
    m_circles[i] = new Circle(m_shaderPrograms[m_curr_prog], vec3(random(-0.9,0.9),random(-0.9,0.9),0), random(0.05,0.2), v_x,v_y,0);

    //m_circles[i] = new Circle(m_shaderPrograms[m_curr_prog], vec3(0.2*i,0.2*i,0), 0.1, v_x,v_y);
    m_circles[i]->setColor(0., 0., 0.7);
  }

  //init spheres
    for (int i=0;i<num_spheres;i++){
      Circle* sphere;
      m_spheres.push_back(sphere);
    }
    for (int i=0;i<num_spheres;i++){
      float v_x = random(-0.01,0.01); //movement in the x-direction
      float v_y = random(-0.01,0.01); //movement in the y-direction
      float v_z = random(-0.01,0.01); //movement in the z-direction
      //randomly distribute some circles to start
      m_spheres[i] = new Circle(m_shaderPrograms[m_curr_prog], vec3(random(-0.75,0.75),random(-0.75,0.75),random(-0.75,0.75)), random(0.1,0.25), v_x,v_y, v_z);

      //m_circles[i] = new Circle(m_shaderPrograms[m_curr_prog], vec3(0.2*i,0.2*i,0), 0.1, v_x,v_y);
      m_spheres[i]->setColor(0., 0., 0.7);
    }

  //m_projection.perspective(40, 1, 1, 8);
  m_projection.perspective(40, 1, 1.5, 4.5);

  vec3 eye(0, 0, 3);
  vec3 up(0, 1, 0);
  vec3 at(0, 0, 0);

  camera.lookAt(eye,at,up);
  camera.setCurrPos(eye);
  camera.setCurrUp(up);

  //m_camera.lookAt(vec3(0, 0, 3), vec3(0, 0, 0), vec3(0, 1., 0.));
  m_model.setToIdentity();

  m_timer = new QTimer(this);
  connect(m_timer, SIGNAL(timeout()), this, SLOT(updateTime()));
  m_timer->start(10);

}

void MyPanelOpenGL::resizeGL(int w, int h) { glViewport(0, 0, w, h); }

void MyPanelOpenGL::paintGL() {
  /* clear both color and depth buffer */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if (!m_shaderPrograms[m_curr_prog]) {
    return;
  }
  //bind uniforms
  m_shaderPrograms[m_curr_prog]->bind();
  mat4 mview = camera.matrix() * m_model;
  m_shaderPrograms[m_curr_prog]->setUniformValue("projection", m_projection);
  m_shaderPrograms[m_curr_prog]->setUniformValue("camera", camera.matrix());
  m_shaderPrograms[m_curr_prog]->setUniformValue("model", m_model);
  m_shaderPrograms[m_curr_prog]->setUniformValue("modelView", mview);
  m_shaderPrograms[m_curr_prog]->setUniformValue("normalMatrix",
                                                 mview.normalMatrix());
  m_shaderPrograms[m_curr_prog]->setUniformValue(
      "lightPos", vec4(1.0, 0, 2, 1.)); // in world coordinates

  /*2D
  //update the location of the circles
  // updateCircles();
  // updateConcentration2D();
  // //setConcentrationToFull2D();
  // drawMetaBalls2D();
  */

  /*3D
  */
  updateSpheres();
  updateConcentration3D();
  drawMetaBalls3D();
  camera.yaw(yaw_angle);

  glFlush();
}

void MyPanelOpenGL::updateTime() {
  update();
}

void MyPanelOpenGL::drawSquare(float yangle) {
  m_model.setToIdentity();
  m_model.rotate(yangle, vec3(0, 1, 0));
  mat4 mview = camera.matrix() * m_model;
  m_shaderPrograms[m_curr_prog]->setUniformValue("model", m_model);
  m_shaderPrograms[m_curr_prog]->setUniformValue("modelView", mview);
  m_shaderPrograms[m_curr_prog]->setUniformValue("normalMatrix",
                                                 mview.normalMatrix());
  m_square->draw(m_shaderPrograms[m_curr_prog]);
  m_model.setToIdentity();
}

void MyPanelOpenGL::updateCircles(){
  for (int i=0;i<num_circles;i++){
    vec3 center = m_circles[i]->center;
    float r = m_circles[i]->radius;
    vec3 color = m_circles[i]->getColor();

    //move the circles
    m_circles[i]->center = vec3(center.x() + m_circles[i]->v_x ,center.y()+m_circles[i]->v_y,0);
    //bounce off boundaries
    if (center.x() + r >=1){
      m_circles[i]->v_x = random(-0.01,0);
    }
    else if (center.x()-r <= -1){
      m_circles[i]->v_x = random(0,0.01);
    }
    else if (center.y() + r >=1){
      m_circles[i]->v_y = random(-0.01,0);
    }
    else if (center.y() -r <=-1){
      m_circles[i]->v_y = random(0,0.01);
    }
    m_circles[i]->setColor(color);
  }
}

void MyPanelOpenGL::updateSpheres(){
    for (int i=0;i<num_spheres;i++){
      vec3 center = m_spheres[i]->center;
      float r = m_spheres[i]->radius;
      vec3 color = m_spheres[i]->getColor();

      //move the circles
      m_spheres[i]->center = vec3(center.x() + m_spheres[i]->v_x ,
        center.y()+m_spheres[i]->v_y,center.z()+m_spheres[i]->v_z);
      //bounce off boundaries
      if (center.x() + r >=0.75){
        m_spheres[i]->v_x = random(-0.01,0);
      }
      else if (center.x()-r <= -0.75){
        m_spheres[i]->v_x = random(0,0.01);
      }
      else if (center.y() + r >=0.75){
        m_spheres[i]->v_y = random(-0.01,0);
      }
      else if (center.y() -r <=-0.75){
        m_spheres[i]->v_y = random(0,0.01);
      }
      else if (center.z() +r >=0.75){
        m_spheres[i]->v_z = random(-0.01,0);
      }
      else if (center.z() -r <=-0.75){
        m_spheres[i]->v_z = random(0,0.01);
      }
      m_spheres[i]->setColor(color);
    }
}


void MyPanelOpenGL::drawCircle(float yangle) {
  m_model.setToIdentity();
  m_model.rotate(yangle, vec3(0, 1, 0));
  mat4 mview = camera.matrix() * m_model;
  m_shaderPrograms[m_curr_prog]->setUniformValue("model", m_model);
  m_shaderPrograms[m_curr_prog]->setUniformValue("modelView", mview);
  m_shaderPrograms[m_curr_prog]->setUniformValue("normalMatrix",
                                                 mview.normalMatrix());
  for (int i=0;i<num_circles;i++){
    m_circles[i]->draw(m_shaderPrograms[m_curr_prog]);
  }
  m_model.setToIdentity();
}

void MyPanelOpenGL::tessellate3D(float width, float height, float length){
    float width_step = width/(W-1);
    float height_step = height/(H-1);
    float length_step = length/(L-1);
    for (int i=0;i<W;i++){
      width_points.push_back(i*width_step-1); //-1 to change [0,2] to [-1,1]
    //  cout <<"width: " << i*width_step-1 << endl;
    }
    for (int i=0;i<H;i++){
      height_points.push_back(i*height_step-1); //-1 to change [0,2] to [-1,1]
    }
    for (int i=0;i<L;i++){
      length_points.push_back(i*length_step-1); //-1 to change [0,2] to [-1,1]
    }
    for (int i=0;i<=W-2;i++){
      vector<vector<cs40::Cube>> toPush;
      grid_3D.push_back(toPush);
      for (int j=0;j<=H-2;j++){
          vector<cs40::Cube> toPush;
          grid_3D[i].push_back(toPush);
          for (size_t k = 0; k <= L-2; k++) {
              //construct the cube
              cs40::Cube cube(m_shaderPrograms[m_curr_prog],
                vec3(width_points[i],height_points[j+1],length_points[k+1]),
                vec3(width_points[i],height_points[j],length_points[k+1]),
                vec3(width_points[i+1],height_points[j],length_points[k+1]),
                vec3(width_points[i+1],height_points[j+1],length_points[k+1]),
                vec3(width_points[i],height_points[j+1],length_points[k]),
                vec3(width_points[i],height_points[j],length_points[k]),
                vec3(width_points[i+1],height_points[j],length_points[k]),
                vec3(width_points[i+1],height_points[j+1],length_points[k]));
              //add the square
              grid_3D[i][j].push_back(cube);
          }
      }
    }

}

void MyPanelOpenGL::tessellate2D(float width, float height){
  float width_step = width/(W-1);
  float height_step = height/(H-1);
  for (int i=0;i<W;i++){
    width_points.push_back(i*width_step-1); //-1 to change [0,2] to [-1,1]
  //  cout <<"width: " << i*width_step-1 << endl;
  }
  for (int i=0;i<H;i++){
    height_points.push_back(i*height_step-1); //-1 to change [0,2] to [-1,1]
  }

  for (int i=0;i<=W-2;i++){
    vector<cs40::Square> toPush;
    grid.push_back(toPush);
    for (int j=0;j<=H-2;j++){
      //construct the square
      cs40::Square square(m_shaderPrograms[m_curr_prog], vec3(width_points[i],height_points[j],0),
       vec3(width_points[i],height_points[j+1],0), vec3(width_points[i+1],height_points[j+1],0),
       vec3(width_points[i+1],height_points[j],0));
       square.center = vec3((width_points[i] + width_points[i+1])/2, (height_points[j] + height_points[j+1])/2, 0);
      //add the square
      grid[i].push_back(square);
    }
  }

}


void MyPanelOpenGL::drawMetaBalls2D(){
  for (int i=0;i<W-1;i++){
    for (int j=0;j<H-1;j++){
      grid[i][j].getPolygonToDraw();
      grid[i][j].draw(m_shaderPrograms[m_curr_prog]);
    }
  }
}

void MyPanelOpenGL::drawMetaBalls3D(){
    for (int i=0;i<W-1;i++){
      for (int j=0;j<H-1;j++){
          for (int k=0;k<L-1;k++){
              grid_3D[i][j][k].polygonise(1.0);
              grid_3D[i][j][k].draw(m_shaderPrograms[m_curr_prog]);
          }
      }
    }

}

void MyPanelOpenGL::updateConcentration2D(){
  for (int i=0;i<W;i++){
    for (int j=0;j<H;j++){
      concentrations[i][j] = calculateConcentration2D(i,j);
    }
  }
  for (int i=0;i<=W-2;i++){
    for (int j=0;j<=H-2;j++){
        grid[i][j].concentrations[0] = concentrations[i][j];
        grid[i][j].concentrations[1] = concentrations[i][j+1];
        grid[i][j].concentrations[2] = concentrations[i+1][j+1];
        grid[i][j].concentrations[3] = concentrations[i+1][j];
    }
  }
}

void MyPanelOpenGL::updateConcentration3D(){
  for (int i=0;i<W;i++){
    for (int j=0;j<H;j++){
        for(int k=0;k<L;k++){
            concentrations3D[i][j][k] = calculateConcentration3D(i,j,k);
        }
    }
  }
  for (int i=0;i<=W-2;i++){
    for (int j=0;j<=H-2;j++){
        for (int k=0;k<=L-2;k++){
            grid_3D[i][j][k].concentrations3D[0] = concentrations3D[i][j+1][k+1];
            grid_3D[i][j][k].concentrations3D[1] = concentrations3D[i][j][k+1];
            grid_3D[i][j][k].concentrations3D[2] = concentrations3D[i+1][j][k+1];
            grid_3D[i][j][k].concentrations3D[3] = concentrations3D[i+1][j+1][k+1];
            grid_3D[i][j][k].concentrations3D[4] = concentrations3D[i][j+1][k];
            grid_3D[i][j][k].concentrations3D[5] = concentrations3D[i][j][k];
            grid_3D[i][j][k].concentrations3D[6] = concentrations3D[i+1][j][k];
            grid_3D[i][j][k].concentrations3D[7] = concentrations3D[i+1][j+1][k];
        }
    }
  }
}

float MyPanelOpenGL::calculateConcentration2D(int i, int j){

  float ret = 0;

  for (int k=0;k<m_circles.size();k++){
    cs40::Circle* circle = m_circles[k];
    vec3 center = circle->center;
    float radius = circle->radius;
    float x = width_points[i];
    float y = height_points[j];
    ret += radius*radius/((x-center.x())*(x-center.x()) + (y-center.y())*(y-center.y()));
  }
  return ret;
}

float MyPanelOpenGL::calculateConcentration3D(int i, int j, int l){

  float ret = 0;

  for (int k=0;k<m_spheres.size();k++){
    cs40::Circle* sphere = m_spheres[k];
    vec3 center = sphere->center;
    float radius = sphere->radius;
    float x = width_points[i];
    float y = height_points[j];
    float z = length_points[l];
    ret += radius*radius/((x-center.x())*(x-center.x()) + (y-center.y())*(y-center.y())
     + (z-center.z())*(z-center.z()));
  }
  return ret;
}

void MyPanelOpenGL::setConcentrationToFull2D(){
  for (int i=0;i<W;i++){
    for (int j=0;j<H;j++){
      concentrations[i][j] = 1.0;
    }
  }

  // //testing for one square
  // concentrations[0][0] = 1.2;
  // concentrations[0][1] = 1.2;
  // concentrations[1][1] = 0;
  // concentrations[1][0] = 0;

  for (int i=0;i<=W-2;i++){
    for (int j=0;j<=H-2;j++){
      grid[i][j].concentrations[0] = concentrations[i][j];
      grid[i][j].concentrations[1] = concentrations[i][j+1];
      grid[i][j].concentrations[2] = concentrations[i+1][j+1];
      grid[i][j].concentrations[3] = concentrations[i+1][j];
    }
  }
}

void MyPanelOpenGL::setConcentrationToFull3D(){
  // for (int i=0;i<W;i++){
  //   for (int j=0;j<H;j++){
  //       for (int k=0;k<L;k++){
  //           concentrations3D[i][j][k] = 1.0;
  //       }
  //   }
  // }

  //testing for one square
  concentrations3D[0][1][1] = 1.2;
  concentrations3D[0][0][1] = 1.2;
  concentrations3D[1][0][1] = 1.2;
  concentrations3D[1][1][1] = 0;
  concentrations3D[0][1][0] = 0;
  concentrations3D[0][0][0] = 0;
  concentrations3D[1][0][0] = 0;
  concentrations3D[1][1][0] = 0;

  for (int i=0;i<=W-2;i++){
    for (int j=0;j<=H-2;j++){
        for (int k=0;k<=L-2;k++){
            grid_3D[i][j][k].concentrations3D[0] = concentrations3D[i][j+1][k+1];
            grid_3D[i][j][k].concentrations3D[1] = concentrations3D[i][j][k+1];
            grid_3D[i][j][k].concentrations3D[2] = concentrations3D[i+1][j][k+1];
            grid_3D[i][j][k].concentrations3D[3] = concentrations3D[i+1][j+1][k+1];
            grid_3D[i][j][k].concentrations3D[4] = concentrations3D[i][j+1][k];
            grid_3D[i][j][k].concentrations3D[5] = concentrations3D[i][j][k];
            grid_3D[i][j][k].concentrations3D[6] = concentrations3D[i+1][j][k];
            grid_3D[i][j][k].concentrations3D[7] = concentrations3D[i+1][j+1][k];
        }
    }
  }
}


float MyPanelOpenGL::random(float min, float max){
    if (min > max){
      qDebug() << "Rand range error: min > max";
      return 0;
    }
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = max - min;
    float r = random * diff;
    return min + r;
}

void MyPanelOpenGL::keyPressEvent(QKeyEvent *event) {
  qreal step = 1;
  /*Enable strong Focus on GL Widget to process key events*/
  switch (event->key()) {
  case Qt::Key_X:
    if (event->text() == "x") {
      updateAngles(0, step);
    } else {
      updateAngles(0, -step);
    }
    break;
  case Qt::Key_Y:
    if (event->text() == "y") {
      updateAngles(1, step);
    } else {
      updateAngles(1, -step);
    }
    break;
  case Qt::Key_Z:
    if (event->text() == "z") {
      updateAngles(2, step);
    } else {
      updateAngles(2, -step);
    }
    break;
  //yaw right
  case Qt::Key_Q:
      camera.yaw(yaw_angle);
  break;
  case Qt::Key_C:
    m_cull = !m_cull;
    setCulling();
    break;
  case Qt::Key_P:
    m_polymode = (m_polymode + 1) % 3;
    updatePolyMode(m_polymode);
    break;
  case Qt::Key_S:
    m_drawSphere = !m_drawSphere;
    break;
  case Qt::Key_L:
    m_curr_prog = (m_curr_prog + 1) % 2;
    break;
  default:
    QWidget::keyPressEvent(event); /* pass to base class */
  }
  update();
}

void MyPanelOpenGL::updateAngles(int idx, qreal amt) {
  if (idx == 0) {
    m_angles.setX(wrap(m_angles.x() + amt));
  } else if (idx == 1) {
    m_angles.setY(wrap(m_angles.y() + amt));
  } else if (idx == 2) {
    m_angles.setZ(wrap(m_angles.z() + amt));
  }
}

qreal MyPanelOpenGL::wrap(qreal amt) {
  if (amt > 360.) {
    return amt - 360.;
  } else if (amt < 0.) {
    return amt + 360.;
  }
  return amt;
}

void MyPanelOpenGL::setCulling() {
  makeCurrent();
  if (m_cull) {
    glEnable(GL_CULL_FACE);
  } else {
    glDisable(GL_CULL_FACE);
  }
}

void MyPanelOpenGL::updatePolyMode(int val) {
  makeCurrent();
  GLenum mode = GL_NONE;
  if (val == 0) {
    mode = GL_POINT;
  } else if (val == 1) {
    mode = GL_LINE;
  } else if (val == 2) {
    mode = GL_FILL;
  }

  if (mode != GL_NONE) {
    glPolygonMode(GL_FRONT_AND_BACK, mode);
  }
  // glPolygonMode(GL_BACK,GL_POINT);
}

void MyPanelOpenGL::createShaders(int i, QString vertName, QString fragName) {

  cout << "building shader " << i << endl;
  destroyShaders(i); // get rid of any old shaders
  m_vertexShaders[i] = new QOpenGLShader(QOpenGLShader::Vertex);
  if (!m_vertexShaders[i]->compileSourceFile(vertName)) {
    qWarning() << m_vertexShaders[i]->log();
  }

  m_fragmentShaders[i] = new QOpenGLShader(QOpenGLShader::Fragment);
  if (!m_fragmentShaders[i]->compileSourceFile(fragName)) {
    qWarning() << m_fragmentShaders[i]->log();
  }

  m_shaderPrograms[i] = new QOpenGLShaderProgram();
  m_shaderPrograms[i]->addShader(m_vertexShaders[i]);
  m_shaderPrograms[i]->addShader(m_fragmentShaders[i]);

  if (!m_shaderPrograms[i]->link()) {
    qWarning() << m_shaderPrograms[i]->log() << endl;
  }
}

void MyPanelOpenGL::destroyShaders(int i) {
  delete m_vertexShaders[i];
  m_vertexShaders[i] = nullptr;
  delete m_fragmentShaders[i];
  m_fragmentShaders[i] = nullptr;

  if (m_shaderPrograms[i]) {
    m_shaderPrograms[i]->release();
    delete m_shaderPrograms[i];
    m_shaderPrograms[i] = nullptr;
  }
}
