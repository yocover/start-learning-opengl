#ifndef PLANE_GROMETRY
#define PLANE_GROMETRY

#include <geometry/BufferGeometry.h>

using namespace std;

class SphereGeometry : public BufferGeometry
{
public:
  SphereGeometry(float radius = 1.0, float widthSegments = 8.0, float heightSegments = 6.0)
  {

    this->setupBuffers();
  }
};

#endif