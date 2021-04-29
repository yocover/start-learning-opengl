#ifndef BOX_GROMETRY
#define BOX_GROMETRY

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>

using namespace std;

class Box
{
public:
  vector<glm::vec3> vertices;

  // 构造函数
  Box(float height, float width, float depth)
  {
  }
};

#endif