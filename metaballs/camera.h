#pragma once

#include <QMatrix4x4>
#include <QVector3D>
#include <QVector4D>

namespace cs40 {

class Camera {

  typedef QVector3D vec3;
  typedef QVector4D vec4;
  typedef QMatrix4x4 mat4;

public:
  /* Construct a new camera
   * with eye at origin
   * looking down -z axis
   * up direction in +y axis
   */
  Camera();

  /* Reproduce behavior of QMatrix4x4::lookAt,
     Store constructed matrix in m_lookat, but also
     return copy of newly constructed matrix */
  mat4 lookAt(const vec3 &eye, const vec3 &at, const vec3 &up);

  /* move camera right by amount amt. Scene should shift left
   * amt is in world units */
  void right(float amt);
  void up(float amt);      /* move camera up */
  void forward(float amt); /* move camera forward */

  /* helpers for those that don't want to be negative */
  inline void left(float amt) { right(-amt); }
  inline void down(float amt) { up(-amt); }
  inline void back(float amt) { forward(-amt); }

  /* rotate CCW about -z axis _of camera_
   * angle is in degrees */
  void roll(float angle);
  void pitch(float angle); /* rotate CCW about +x */
  void yaw(float angle);   /* CCW about +y */

  inline void setCurrPos(vec3 pos) { curr_position = pos;}

  inline vec3 getCurrPos(){ return curr_position;}

  /* return 4x4 matrix describing camera tranform
   * Should map world coordinates to eye/camera coordinates */
  const mat4 &matrix() const { return m_lookAt; }


  //toggle follow Neptune
  inline bool getFollowNeptune(){ return followNeptune; }

  //access currUp
  inline QVector3D getCurrUp() {
      QVector3D newCurrUp(currUp.x(), currUp.y(), currUp.z());
      return newCurrUp;
  }

  //get currUp
  inline void setCurrUp(QVector3D up) {
      currUp = up;
  }



  /* reset camera to default location
   * as in constructor */
  void reset();

private:
  mat4 m_lookAt;
  vec3 curr_position;
  QVector4D currUp;
  bool followNeptune = true;
};

} // namespace cs40
