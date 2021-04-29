#ifndef PLANE_GROMETRY
#define PLANE_GROMETRY

#include <geometry/BufferGeometry.h>

using namespace std;

class PlaneGeometry : public BufferGeometry
{
public:
  PlaneGeometry(float width = 1.0, float height = 1.0, float wSegment = 1.0, float hSegment = 1.0)
  {

    float width_half = width / 2.0f;
    float height_half = height / 2.0f;

    float gridX1 = wSegment + 1.0f;
    float gridY1 = hSegment + 1.0f;

    float segment_width = width / wSegment;
    float segment_height = height / hSegment;

    Vertex vertex;

    // generate Position Normal TexCoords
    for (int iy = 0; iy < gridY1; iy++)
    {

      float y = iy * segment_height - height_half;

      for (int ix = 0; ix < gridX1; ix++)
      {
        float x = ix * segment_width - height_half;
        vertex.Position = glm::vec3(x, -y, 0.0f);
        vertex.Normal = glm::vec3(0.0f, 0.0f, 1.0f);
        vertex.TexCoords = glm::vec2(ix / wSegment, 1.0f - (iy / hSegment));

        this->vertices.push_back(vertex);
      }
    }
    // generate indices
    for (int iy = 0; iy < hSegment; iy++)
    {
      for (int ix = 0; ix < wSegment; ix++)
      {
        float a = ix + gridX1 * iy;
        float b = ix + gridX1 * (iy + 1);
        float c = (ix + 1) + gridX1 * (iy + 1);
        float d = (ix + 1) + gridX1 * iy;
        this->indices.push_back(a);
        this->indices.push_back(b);
        this->indices.push_back(d);
        this->indices.push_back(b);
        this->indices.push_back(c);
        this->indices.push_back(d);
      }
    }

    this->setupBuffers();
  }
};

#endif