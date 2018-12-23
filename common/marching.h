#pragma once
#include <QtOpenGL>

/* THIS CODE IS FROM http://paulbourke.net/geometry/polygonise/ */

/* helpful debugging ostream operators */
std::ostream& operator<<(std::ostream& str, const QVector2D& vec);
std::ostream& operator<<(std::ostream& str, const QVector3D& vec);
std::ostream& operator<<(std::ostream& str, const QVector4D& vec);


namespace cs40{

    typedef QVector2D vec2;
    typedef QVector3D vec3;
    typedef QVector4D vec4;

    typedef struct {
      vec3 p[3];
    } TRIANGLE;

    typedef struct {
      vec3 p[8]; //  array of cell vertices
      double val[8]; // array of min distances (to center of nearest sphere)
    } GRIDCELL;

    int polygonise(GRIDCELL grid, double isolevel, TRIANGLE *triangles);
    vec3 vertexInterp(double isolevel, vec3 p1, vec3 p2, double valp1, double valp2);
    vec3 calcNormal(const vec3 &p);

} //namespace
