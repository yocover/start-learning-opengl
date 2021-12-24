#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <map>

#include <tool/shader.h>
#include <tool/camera.h>
#include <geometry/BoxGeometry.h>
#include <geometry/PlaneGeometry.h>
#include <geometry/SphereGeometry.h>

#define STB_IMAGE_IMPLEMENTATION
#include <tool/stb_image.h>

#include <tool/gui.h>
#include <tool/mesh.h>
#include <tool/model.h>

#include <random>
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void processInput(GLFWwindow *window);
unsigned int loadTexture(char const *path);
unsigned int loadHdrTexture(char const *path);

// method
void drawMesh(BufferGeometry geometry);
void drawLightObject(Shader shader, BufferGeometry geometry, glm::vec3 position);

std::string Shader::dirName;

int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 600;
// int SCREEN_WIDTH = 1600;
// int SCREEN_HEIGHT = 1200;

// camera value
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// delta time
float deltaTime = 0.0f;
float lastTime = 0.0f;

float lastX = SCREEN_WIDTH / 2.0f; // 鼠标上一帧的位置
float lastY = SCREEN_HEIGHT / 2.0f;

Camera camera(glm::vec3(0.0, 0.0, 25.0));

using namespace std;

// 加速插值函数
float lerp(float a, float b, float f)
{
  return a + f * (b - a);
}

int main(int argc, char *argv[])
{
  Shader::dirName = argv[1];
  glfwInit();
  // 设置主要和次要版本
  const char *glsl_version = "#version 330";

  // 片段着色器将作用域每一个采样点（采用4倍抗锯齿，则每个像素有4个片段（四个采样点））
  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // 窗口对象
  GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // -----------------------
  // 创建imgui上下文
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  // 设置样式
  ImGui::StyleColorsDark();
  // 设置平台和渲染器
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  // -----------------------

  // 设置视口
  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  glEnable(GL_PROGRAM_POINT_SIZE);

  // 启用混合
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // 深度测试
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  //确保立方体正确采样，去除边缘接缝
  glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

  // 鼠标键盘事件
  // 1.注册窗口变化监听
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  // 2.鼠标事件
  glfwSetCursorPosCallback(window, mouse_callback);
  // 3.将鼠标隐藏
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  Shader sceneShader("./shader/scene_vert.glsl", "./shader/scene_frag.glsl");
  Shader lightObjShader("./shader/light_object_vert.glsl", "./shader/light_object_frag.glsl");
  Shader cubemapShader("./shader/cubemap_vert.glsl", "./shader/cubemap_frag.glsl");
  Shader envmapShader("./shader/envmap_vert.glsl", "./shader/envmap_frag.glsl");
  Shader irradianceShader("./shader/irradiance_vert.glsl", "./shader/irradiance_frag.glsl");

  Shader prefilterShader("./shader/prefilter_vert.glsl", "./shader/prefilter_frag.glsl");
  Shader brdfShader("./shader/brdf_vert.glsl", "./shader/brdf_frag.glsl");

  Shader testBrdfShader("./shader/test_brdf_vert.glsl", "./shader/test_brdf_frag.glsl");

  PlaneGeometry quadGeometry(2.0, 2.0);                // 屏幕四边形
  BoxGeometry boxGeometry(5.0, 5.0, 5.0);              // 盒子
  SphereGeometry pointLightGeometry(0.17, 64.0, 64.0); // 点光源位置显示
  SphereGeometry objectGeometry(1.0, 64.0, 64.0);      // 圆球

  // 点光源的位置
  vector<glm::vec3> lightPositions{
      glm::vec3(-10.0f, 10.0f, 10.0f),
      glm::vec3(10.0f, 10.0f, 10.0f),
      glm::vec3(-10.0f, -10.0f, 10.0f),
      glm::vec3(10.0f, -10.0f, 10.0f),
  };

  // 点光源颜色
  vector<glm::vec3> lightColors{
      glm::vec3(300.0f, 300.0f, 300.0f),
      glm::vec3(300.0f, 300.0f, 300.0f),
      glm::vec3(300.0f, 300.0f, 300.0f),
      glm::vec3(300.0f, 300.0f, 300.0f),
  };

  int nrRows = 7;
  int nrColumns = 7;
  float spacing = 2.5;

  sceneShader.use();
  sceneShader.setInt("irradianceMap", 0);
  sceneShader.setInt("prefilterMap", 1);
  sceneShader.setInt("brdfLUT", 2);

  sceneShader.setVec3("albedo", 0.0f, 0.5f, 0.0f);
  sceneShader.setFloat("ao", 1.0f);

  envmapShader.use();
  envmapShader.setInt("envMap", 0);

  // ---------------------
  unsigned int captureFBO;
  unsigned int captureRBO;
  glGenFramebuffers(1, &captureFBO);
  glGenRenderbuffers(1, &captureRBO);

  glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
  glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

  // 为六个面分配内存
  unsigned int envCubemap;
  glGenTextures(1, &envCubemap);
  glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
  for (unsigned int i = 0; i < 6; ++i)
  {
    // 使用16位浮点数存储
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  unsigned int hdrMap = loadHdrTexture("./static/texture/Alexs_Apt_2k.hdr");
  cubemapShader.use();
  cubemapShader.setInt("equireMap", 0);

  // 获取立方体贴图的每个面
  glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
  glm::vec3 lookEye = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::mat4 captureViews[] =
      {
          glm::lookAt(lookEye, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
          glm::lookAt(lookEye, glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
          glm::lookAt(lookEye, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
          glm::lookAt(lookEye, glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
          glm::lookAt(lookEye, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
          glm::lookAt(lookEye, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
      };

  // ---------------------

  // --------------------- 将hdr环境图转换为等效的立方体贴图
  cubemapShader.setMat4("projection", captureProjection);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, hdrMap);

  glViewport(0, 0, 512, 512);
  glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
  for (unsigned int i = 0; i < 6; i++)
  {
    cubemapShader.setMat4("view", captureViews[i]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawMesh(boxGeometry);
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  // ---------------------

  // ---------------------创建辐照度立方体贴图
  unsigned int irradianceMap;
  glGenTextures(1, &irradianceMap);
  glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
  for (unsigned int i = 0; i < 6; ++i)
  {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
  glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

  glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

  irradianceShader.use();
  irradianceShader.setInt("envMap", 0);
  irradianceShader.setMat4("projection", captureProjection);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

  glViewport(0, 0, 32, 32);
  glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);

  for (unsigned int i = 0; i < 6; ++i)
  {
    irradianceShader.setMat4("view", captureViews[i]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawMesh(boxGeometry);
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  // ---------------------

  // ---------------------创建预过滤立方体贴图
  unsigned int prefilterMap;
  glGenTextures(1, &prefilterMap);
  glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
  for (unsigned int i = 0; i < 6; ++i)
  {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // be sure to set minification filter to mip_linear
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
  // ---------------------

  // ---------------------使用蒙特卡洛积分创建预过滤器
  prefilterShader.use();
  prefilterShader.setInt("environmentMap", 0);
  prefilterShader.setMat4("projection", captureProjection);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

  glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
  unsigned int maxMipLevels = 5;
  for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
  {
    // 根据mip级别大小调整帧缓冲区大小
    unsigned int mipWidth = 128 * std::pow(0.5, mip);
    unsigned int mipHeight = 128 * std::pow(0.5, mip);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
    glViewport(0, 0, mipWidth, mipHeight);

    float roughness = (float)mip / (float)(maxMipLevels - 1);
    prefilterShader.setFloat("roughness", roughness);
    for (unsigned int i = 0; i < 6; ++i)
    {
      prefilterShader.setMat4("view", captureViews[i]);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap, mip);

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      drawMesh(boxGeometry);
    }
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  // ---------------------

  // ---------------------预处理BRDF
  unsigned int brdfLUTTexture;
  glGenTextures(1, &brdfLUTTexture);

  glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glBindFramebuffer(GL_FRAMEBUFFER, captureFBO); // 重新获取并配置帧缓冲对象，并使用BRDF着色器渲染屏幕四边形
  glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture, 0);

  glViewport(0, 0, 512, 512);
  brdfShader.use();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  drawMesh(quadGeometry); // 渲染屏幕四边形
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  // ---------------------

  // 恢复原来的窗口渲染尺寸
  int scrWidth, scrHeight;
  glfwGetFramebufferSize(window, &scrWidth, &scrHeight);
  glViewport(0, 0, scrWidth, scrHeight);

  while (!glfwWindowShouldClose(window))
  {
    processInput(window);

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastTime;
    lastTime = currentFrame;

    // 在标题中显示帧率信息
    // *************************************************************************
    int fps_value = (int)round(ImGui::GetIO().Framerate);
    int ms_value = (int)round(1000.0f / ImGui::GetIO().Framerate);

    std::string FPS = std::to_string(fps_value);
    std::string ms = std::to_string(ms_value);
    std::string newTitle = "LearnOpenGL - " + ms + " ms/frame " + FPS;
    glfwSetWindowTitle(window, newTitle.c_str());

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    // *************************************************************************

    glClearColor(25.0 / 255.0, 25.0 / 255.0, 25.0 / 255.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);

    float radius = 5.0f;
    float camX = sin(glfwGetTime() * 0.5) * radius;
    float camZ = cos(glfwGetTime() * 0.5) * radius;

    lightPositions[1].x = camX;
    lightPositions[1].y = camZ;

    sceneShader.use();
    // 绑定辐照图图以及预处理贴图和brdf贴图
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);

    for (unsigned int i = 0; i < lightPositions.size(); i++)
    {
      glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 15.0, 0.0, 0.0);
      newPos = lightPositions[i];
      sceneShader.setVec3("lightPositions[" + std::to_string(i) + "]", newPos);
      sceneShader.setVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);
    }

    sceneShader.setMat4("projection", projection);
    sceneShader.setMat4("view", view);
    sceneShader.setVec3("camPos", camera.Position);

    for (int row = 0; row < nrRows; ++row)
    {
      sceneShader.setFloat("metallic", (float)row / (float)nrRows);
      for (int col = 0; col < nrColumns; ++col)
      {

        sceneShader.setFloat("roughness", glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f));
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3((col - (nrColumns / 2)) * spacing, (row - (nrRows / 2)) * spacing, -1.0f));
        sceneShader.setMat4("model", model);

        // render sphere
        drawMesh(objectGeometry);
      }
    }

    // 直接采样hdr贴图
    // ----------------
    cubemapShader.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hdrMap);
    cubemapShader.setMat4("view", view);
    cubemapShader.setMat4("projection", projection);
    // drawMesh(boxGeometry);
    // ----------------

    // 使用处理之后的环境贴图
    // -------------------
    envmapShader.use();
    envmapShader.setMat4("view", view);
    envmapShader.setMat4("projection", projection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
    // glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap); // 显示生成的辐照度图
    // glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap); // 显示生成的预过滤图
    drawMesh(boxGeometry);
    // -------------------

    // 测试预计算 BRDF 纹理
    // testBrdfShader.use();
    // testBrdfShader.setInt("brdfTexture", 0);
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
    // drawMesh(quadGeometry);

    // 绘制灯光物体
    // --------------------------
    lightObjShader.use();
    lightObjShader.setMat4("view", view);
    lightObjShader.setMat4("projection", projection);

    for (unsigned int i = 0; i < lightPositions.size(); i++)
    {
      glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
      newPos = lightPositions[i];

      model = glm::mat4(1.0f);
      model = glm::translate(model, newPos);
      lightObjShader.setMat4("model", model);
      lightObjShader.setVec3("lightColor", lightColors[i]);
      drawMesh(pointLightGeometry);
    }
    // --------------------------

    // 渲染 gui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}

// 绘制物体
void drawMesh(BufferGeometry geometry)
{
  glBindVertexArray(geometry.VAO);
  glDrawElements(GL_TRIANGLES, geometry.indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

// 绘制灯光物体
void drawLightObject(Shader shader, BufferGeometry geometry, glm::vec3 position)
{
  glm::mat4 view = camera.GetViewMatrix();
  glm::mat4 projection = glm::mat4(1.0f);
  glm::mat4 model = glm::mat4(1.0f);
  projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

  // // 绘制灯光物体
  shader.use();
  shader.setMat4("view", view);
  shader.setMat4("projection", projection);

  model = glm::mat4(1.0f);
  model = glm::translate(model, position);

  shader.setMat4("model", model);
  shader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
  drawMesh(geometry);
}

// 窗口变动监听
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

// 键盘输入监听
void processInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, true);
  }

  // 相机按键控制
  // 相机移动
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
  {
    camera.ProcessKeyboard(FORWARD, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
  {
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
  {
    camera.ProcessKeyboard(LEFT, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
  {
    camera.ProcessKeyboard(RIGHT, deltaTime);
  }
}

// 鼠标移动监听
void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;

  lastX = xpos;
  lastY = ypos;

  camera.ProcessMouseMovement(xoffset, yoffset);
}

// 加载纹理贴图
unsigned int loadTexture(char const *path)
{
  unsigned int textureID;
  glGenTextures(1, &textureID);

  // 图像y轴翻转
  stbi_set_flip_vertically_on_load(true);
  int width, height, nrComponents;
  unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
  if (data)
  {
    GLenum format;
    if (nrComponents == 1)
      format = GL_RED;
    else if (nrComponents == 3)
      format = GL_RGB;
    else if (nrComponents == 4)
      format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
  }
  else
  {
    std::cout << "Texture failed to load at path: " << path << std::endl;
    stbi_image_free(data);
  }

  return textureID;
}

// 加载hdr贴图
unsigned int loadHdrTexture(char const *path)
{
  unsigned int textureID;
  glGenTextures(1, &textureID);

  // 图像y轴翻转
  stbi_set_flip_vertically_on_load(true);
  int width, height, nrComponents;
  float *data = stbi_loadf(path, &width, &height, &nrComponents, 0);
  if (data)
  {
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data); // note how we specify the texture's data value to be float

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
  }
  else
  {
    std::cout << "Failed to load HDR image." << std::endl;
    stbi_image_free(data);
  }

  return textureID;
}