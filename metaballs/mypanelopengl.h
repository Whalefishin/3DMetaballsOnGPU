#pragma once

#include "common/sphere.h"
#include "common/square.h"
#include "common/circle.h"
#include "common/cube.h"
#include "camera.h"
#include <QMatrix4x4>
#include <QtOpenGL>
#include <QtWidgets>
#include <utility>

class MyPanelOpenGL : public QOpenGLWidget {
  typedef QVector4D point4;
  typedef QVector4D color4;
  typedef QVector3D vec3;
  typedef QMatrix4x4 mat4;
  typedef QVector2D vec2;

  Q_OBJECT

protected:
  void initializeGL();
  void resizeGL(int w, int h);
  void paintGL();
  void keyPressEvent(QKeyEvent *event);

public:
  explicit MyPanelOpenGL(QWidget *parent = 0);
  virtual ~MyPanelOpenGL();

private:
  /* simple test shapes */
  cs40::Sphere *m_sphere;
  cs40::Square *m_square;
  vector<cs40::Circle*> m_circles; //an array of points to circles
  vector<cs40::Circle*> m_spheres;

  vector<vector<cs40::Square>> grid;
  vector<vector<vector<cs40::Cube>>> grid_3D;
  int W;
  int H;
  int L;
  vector<vector<float>> concentrations;
  vector<vector<vector<float>>> concentrations3D;
  vector<float> width_points;
  vector<float> height_points;
  vector<float> length_points;

  //number of circles/spheres to create.
  int num_circles;
  int num_spheres;

  int m_polymode;
  bool m_drawSphere;
  bool m_cull;

  QTimer *m_timer; /* event timer */

  vec3 m_angles; /* Euler angles for rotation */

  mat4 m_model;
  //mat4 m_camera;
  mat4 m_projection;
  cs40::Camera camera;

  float yaw_angle;

  /* Shaders and program */
  QOpenGLShader *m_vertexShaders[2];
  QOpenGLShader *m_fragmentShaders[2];
  QOpenGLShaderProgram *m_shaderPrograms[2];
  int m_curr_prog; // current program ID

  /* draw square originally in z=0 plane, rotated about y-axis
   * by yangle (in degrees) */
  void drawSquare(float yangle);
  void drawCircle(float yangle);

  //update the location of the circles
  void updateCircles();

  //update the location of the spheres
  void updateSpheres();

  //uniform random float between min and max
  float random(float min, float max);

  /* update Euler angle at index idx by amt
   * idx=0,1,2 -> x,y,z */
  void updateAngles(int idx, qreal amt);

  /* wrap a angle value to the range 0..360*/
  qreal wrap(qreal amt);

  /* update Polygon draw mode based
   * 0 : point
   * 1 : line
   * 2 : polygon */
  void updatePolyMode(int val);

  void setCulling();

  void createShaders(int i, QString vertName, QString fragName);
  void destroyShaders(int i);

  void tessellate2D(float width, float height); //W is the # of dicrete step applied to width
  //get all the face to draw based on the grid and calculated concentrations

  void tessellate3D(float width, float height, float length);
  void updateConcentration2D();
  void updateConcentration3D();
  void setConcentrationToFull2D();
  void setConcentrationToFull3D();
  float calculateConcentration2D(int i,int j);
  float calculateConcentration3D(int i,int j, int l);
  void drawMetaBalls2D();
  void drawMetaBalls3D();

signals:

public slots:
  /* trigger update of GL window */
  void updateTime();
};
