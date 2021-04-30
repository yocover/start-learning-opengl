#ifndef SPHERE_GROMETRY
#define SPHERE_GROMETRY

#include <geometry/BufferGeometry.h>

using namespace std;

const float PI = glm::pi<float>();

class BoxGeometry : public BufferGeometry
{
public:
  float width;
  float height;
  float widthSegments;
  float heightSegments;
  float depthSegments;
  BoxGeometry(float width = 1.0f, float height = 1.0f, float depth = 1, float widthSegments = 1.0f, float heightSegments = 1.0f, float depthSegments = 1.0f)
  {

    widthSegments = glm::floor(widthSegments);
    heightSegments = glm::floor(heightSegments);
    depthSegments = glm::floor(depthSegments);

    this->width = width;
    this->height = height;
    this->widthSegments = widthSegments;
    this->heightSegments = heightSegments;
    this->depthSegments = depthSegments;

    /**
     * 三分量对应
     * vec3(u, v, w)
     * x y z | 0, 1, 2
     * z y x | 2, 1, 0
     * x z y | 0, 2, 1
     */
    this->buildPlane(2, 1, 0, -1, -1, depth, height, width, depthSegments, heightSegments, 0); // px
    this->buildPlane(2, 1, 0, 1, -1, depth, height, -width, depthSegments, heightSegments, 1); // nx

    this->buildPlane(0, 2, 1, 1, 1, width, depth, height, widthSegments, depthSegments, 2);   // py
    this->buildPlane(0, 2, 1, 1, -1, width, depth, -height, widthSegments, depthSegments, 3); // ny

    this->buildPlane(0, 1, 2, 1, -1, width, height, depth, widthSegments, heightSegments, 4);   // pz
    this->buildPlane(0, 1, 2, -1, -1, width, height, -depth, widthSegments, heightSegments, 5); // nz

    // // 生成 vertices normals uvs

    // vector<vector<int>> grid;

    // glm::vec3 position = glm::vec3(0.0f);
    // glm::vec3 normal = glm::vec3(0.0f);

    // Vertex vertex;

    // // 计算 vertices normals 和 uvs
    // for (unsigned int iy = 0; iy <= heightSegments; iy++)
    // {
    //   vector<int> verticesRow;

    //   float v = iy / heightSegments;

    //   float uOffset = 0;

    //   if (iy == 0 && thetaStart == 0)
    //   {
    //     uOffset = 0.5 / widthSegments;
    //   }
    //   else if (iy == heightSegments && thetaEnd == PI)
    //   {
    //     uOffset = -0.5 / widthSegments;
    //   }
    //   for (unsigned int ix = 0; ix <= widthSegments; ix++)
    //   {
    //     const float u = ix / widthSegments;

    //     // position
    //     position.x = -radius * glm::cos(phiStart + u * phiLength) * glm::sin(thetaStart + v * thetaLength);
    //     position.y = radius * glm::cos(thetaStart + v * thetaLength);
    //     position.z = radius * glm::sin(phiStart + u * phiLength) * glm::sin(thetaStart + v * thetaLength);

    //     vertex.Position = position;

    //     // normal
    //     normal = glm::normalize(position);
    //     vertex.Normal = normal;

    //     // uv
    //     vertex.TexCoords = glm::vec2(u + uOffset, 1 - v);

    //     this->vertices.push_back(vertex);

    //     verticesRow.push_back(index++);
    //   }
    //   grid.push_back(verticesRow);
    // }

    // // indices
    // for (unsigned int iy = 0; iy < heightSegments; iy++)
    // {
    //   for (unsigned ix = 0; ix < widthSegments; ix++)
    //   {
    //     unsigned int a = grid[iy][ix + 1];
    //     unsigned int b = grid[iy][ix];
    //     unsigned int c = grid[iy + 1][ix];
    //     unsigned int d = grid[iy + 1][ix + 1];

    //     if (iy != 0 || thetaStart > 0)
    //     {
    //       this->indices.push_back(a);
    //       this->indices.push_back(b);
    //       this->indices.push_back(d);
    //     }
    //     if (iy != heightSegments - 1 || thetaEnd < PI)
    //     {
    //       this->indices.push_back(b);
    //       this->indices.push_back(c);
    //       this->indices.push_back(d);
    //     }
    //   }
    // }

    this->setupBuffers();
  }

private:
  int numberOfVertices = 0;
  float groupStart = 0;

  /**
   * 三分量对应
   * vec3(u, v, w)
   * x y z | 0, 1, 2
   * z y x | 2, 1, 0
   * x z y | 0, 2, 1
   */
  void buildPlane(int u, int v, int w, float udir, float vdir, float width, float height, float depth, float gridX, float gridY, float materialIndex)
  {
    float segmentWidth = width / gridX;
    float segmentHeight = height / gridY;

    float widthHalf = width / 2.0f;
    float heightHalf = height / 2.0f;
    float depthHalf = depth / 2.0f;

    float gridX1 = gridX + 1.0f;
    float gridY1 = gridY + 1.0f;

    float vertexCounter = 0.0f;
    float groupCount = 0.0f;

    glm::vec3 vector = glm::vec3(0.0f, 0.0f, 0.0f);

    Vertex vertex;

    // 生成 顶点数据
    for (unsigned int iy = 0; iy < gridY1; iy++)
    {
      float y = iy * segmentHeight - heightHalf;
      for (unsigned int ix = 0; ix < gridX1; ix++)
      {
        float x = ix * segmentWidth - widthHalf;

        // position
        vector[u] = x * udir;
        vector[v] = y * vdir;
        vector[w] = depthHalf;
        vertex.Position = glm::vec3(vector.x, vector.y, vector.z);

        // normals
        vector[u] = 0;
        vector[v] = 0;
        vector[w] = depth > 0 ? 1 : -1;
        vertex.Normal = glm::vec3(vector.x, vector.y, vector.z);

        // uvs
        vertex.TexCoords = glm::vec2(ix / gridX, 1 - (iy / gridY));

        this->vertices.push_back(vertex);

        // counters
        vertexCounter += 1.0;
      }
    }

    // indices

    for (unsigned int iy = 0; iy < gridY; iy++)
    {
      for (unsigned int ix = 0; ix < gridX; ix++)
      {
        float a = numberOfVertices + ix + gridX1 * iy;
        float b = numberOfVertices + ix + gridX1 * (iy + 1);
        float c = numberOfVertices + (ix + 1) + gridX1 * (iy + 1);
        float d = numberOfVertices + (ix + 1) + gridX1 * iy;

        this->indices.push_back(a);
        this->indices.push_back(b);
        this->indices.push_back(d);

        this->indices.push_back(b);
        this->indices.push_back(c);
        this->indices.push_back(d);
      }
    }
    numberOfVertices += vertexCounter;
  }
};

#endif