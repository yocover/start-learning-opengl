// #include <glad/glad.h>
// #include <GLFW/glfw3.h>
// #include <cmath>
// #include <iostream>

// void framebuffer_size_callback(GLFWwindow *window, int width, int height);
// void processInput(GLFWwindow *window);

// const char *vertexShaderSource =
//     "#version 330 core\n"
//     "layout (location = 0) in vec3 aPos;\n"
//     "out vec4 vColor;\n"
//     "void main()\n"
//     "{\n"
//     "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);"
//     "vColor = vec4(0.0f, 0.5f, 0.1f, 1.0f);"
//     "gl_PointSize = 2.0f;"
//     "}\n";

// const char *fragmentShaderSource1 =
//     "#version 330 core\n"
//     "out vec4 FragColor;\n"
//     "in vec4 vColor;"
//     "void main()\n"
//     "{"
//     "vec4 color = vec4(1.0f, 0.5f, 0.2f, 1.0f);"
//     "FragColor = vColor;"
//     "}";

// const char *fragmentShaderSource2 =
//     "#version 330 core\n"
//     "out vec4 FragColor;\n"
//     "uniform vec4 ourColor;"
//     "void main()\n"
//     "{"
//     "FragColor = ourColor;"
//     "}";

// int main()
// {
//   glfwInit();
//   // 设置主要和次要版本
//   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

//   // 创建窗口对象
//   GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
//   if (window == NULL)
//   {
//     std::cout << "Failed to create GLFW window" << std::endl;
//     glfwTerminate();
//     return -1;
//   }
//   glfwMakeContextCurrent(window);

//   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//   {
//     std::cout << "Failed to initialize GLAD" << std::endl;
//     return -1;
//   }
//   // 设置视口
//   glViewport(0, 0, 800, 600);
//   glEnable(GL_PROGRAM_POINT_SIZE);

//   // 注册窗口变化监听
//   glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

//   int nrAttributes;
//   glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
//   std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

//   // 定义顶点数组
//   float vertices[] = {
//       // 第一个三角形
//       -0.5f,
//       0.5f,
//       0.0f,
//       -0.75,
//       -0.5,
//       0.0f,
//       -0.25,
//       -0.5,
//       0.0f,

//       // 第二个三角形
//       0.5f,
//       0.5f,
//       0.0f,
//       0.75,
//       -0.5,
//       0.0f,
//       0.25,
//       -0.5,
//       0.0f,
//   };

//   // 创建缓冲对象
//   unsigned int VBOS[2], VAOS[2];
//   glGenVertexArrays(2, VAOS);
//   glGenBuffers(2, VBOS);

//   // 绑定VAO缓冲对象
//   glBindVertexArray(VAOS[0]);
//   // 绑定VBO缓对象
//   glBindBuffer(GL_ARRAY_BUFFER, VBOS[0]);
//   // 填充VBO数据
//   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//   // 设置顶点属性指针
//   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
//   glEnableVertexAttribArray(0);

//   // 绑定VAO缓冲对象
//   glBindVertexArray(VAOS[1]);
//   // 绑定VBO缓对象
//   glBindBuffer(GL_ARRAY_BUFFER, VBOS[1]);
//   // 填充VBO数据
//   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//   // 设置顶点属性指针
//   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)(9 * sizeof(float)));
//   glEnableVertexAttribArray(0);

//   glBindVertexArray(0);

//   // 创建顶点和片段着色器
//   unsigned int vertexShader, fragmentShader1, fragmentShader2;
//   vertexShader = glCreateShader(GL_VERTEX_SHADER);
//   fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
//   fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);

//   // 附加着色器代码
//   glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
//   glCompileShader(vertexShader);

//   glShaderSource(fragmentShader1, 1, &fragmentShaderSource1, NULL);
//   glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
//   glCompileShader(fragmentShader1);
//   glCompileShader(fragmentShader2);

//   // 检测是否编译成功
//   int success;
//   char infoLog[512];
//   glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
//   if (!success)
//   {
//     glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
//     std::cout << "ERROR:SHADER::VERTEX::COMPILATION_FAILED\n"
//               << std::endl;
//   }

//   glGetShaderiv(fragmentShader1, GL_COMPILE_STATUS, &success);
//   if (!success)
//   {
//     glGetShaderInfoLog(fragmentShader1, 512, NULL, infoLog);
//     std::cout << "ERROR:SHADER::FRAGMENT::COMPILATION_FAILED\n"
//               << std::endl;
//   }

//   glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);
//   if (!success)
//   {
//     glGetShaderInfoLog(fragmentShader2, 512, NULL, infoLog);
//     std::cout << "ERROR:SHADER::FRAGMENT::COMPILATION_FAILED\n"
//               << std::endl;
//   }

//   // 创建程序对象
//   unsigned int shaderProgram1, shaderProgram2;
//   shaderProgram1 = glCreateProgram();
//   shaderProgram2 = glCreateProgram();

//   // 将着色器附加到程序对象上
//   glAttachShader(shaderProgram1, vertexShader);
//   glAttachShader(shaderProgram1, fragmentShader1);
//   glAttachShader(shaderProgram2, fragmentShader2);

//   // 链接
//   glLinkProgram(shaderProgram1);
//   glLinkProgram(shaderProgram2);

//   glGetProgramiv(shaderProgram1, GL_LINK_STATUS, &success);
//   if (!success)
//   {
//     glGetProgramInfoLog(shaderProgram1, 512, NULL, infoLog);
//     std::cout << "ERROR:SHADER_PROGRAM_LINK_FAILED" << std::endl;
//   }

//   glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
//   if (!success)
//   {
//     glGetProgramInfoLog(shaderProgram2, 512, NULL, infoLog);
//     std::cout << "ERROR:SHADER_PROGRAM_LINK_FAILED" << std::endl;
//   }

//   // 删除着色器
//   glDeleteShader(vertexShader);
//   glDeleteShader(fragmentShader1);
//   glDeleteShader(fragmentShader2);

//   // 设置线框绘制模式
//   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//   int ourColorLocation = glGetUniformLocation(shaderProgram2, "ourColor");

//   while (!glfwWindowShouldClose(window))
//   {
//     processInput(window);

//     // 渲染指令
//     // ...
//     glClearColor(25.0 / 255.0, 25.0 / 255.0, 25.0 / 255.0, 1.0);
//     glClear(GL_COLOR_BUFFER_BIT);

//     glUseProgram(shaderProgram1);
//     glBindVertexArray(VAOS[0]);
//     glDrawArrays(GL_POINTS, 0, 3);
//     glDrawArrays(GL_TRIANGLES, 0, 3);

//     glUseProgram(shaderProgram2);
//     glBindVertexArray(VAOS[1]);
//     glDrawArrays(GL_POINTS, 0, 3);
//     glDrawArrays(GL_TRIANGLES, 0, 3);

//     float timeValue = glfwGetTime();
//     float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
//     glUniform4f(ourColorLocation, 0.3f, greenValue, 0.2f, 1.0f);

//     glfwSwapBuffers(window);
//     glfwPollEvents();
//   }

//   glDeleteVertexArrays(2, VAOS);
//   glDeleteBuffers(2, VBOS);
//   glDeleteProgram(shaderProgram1);
//   glDeleteProgram(shaderProgram2);

//   glfwTerminate();
//   return 0;
// }

// void framebuffer_size_callback(GLFWwindow *window, int width, int height)
// {
//   glViewport(0, 0, width, height);
// }

// void processInput(GLFWwindow *window)
// {
//   if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//   {
//     glfwSetWindowShouldClose(window, true);
//   }
// }