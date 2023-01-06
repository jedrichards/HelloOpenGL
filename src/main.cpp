#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

static unsigned int CompileShader(unsigned int type,
                                  const std::string &source) {
  std::cout << "Compiling shader:" << std::endl;
  std::cout << source << std::endl;
  unsigned int id = glCreateShader(type);
  const char *src = source.c_str();
  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  // Validate shader
  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if (result == GL_FALSE) {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char message[length];
    glGetShaderInfoLog(id, length, &length, message);
    std::cout << "Failed to compile shader" << std::endl;
    std::cout << message << std::endl;
    glDeleteShader(id);
    return 0;
  }

  return id;
}

// Helper functions to create shaders from strings.
static unsigned int CreateShader(const std::string &vertexShader,
                                 const std::string &fragmentShader) {
  unsigned int program = glCreateProgram();
  unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
  unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glValidateProgram(program);
  glDeleteShader(vs);
  glDeleteShader(fs);
  return program;
}

int main(void) {
  GLFWwindow *window;

  if (!glfwInit()) {
    std::cout << "GLFW init failed" << std::endl;
    return -1;
  }

  // GLFW hints required to get newer OpenGL version on MacOS, just consider
  // this some voodoo
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

  window = glfwCreateWindow(640, 480, "Hello OpenGL", NULL, NULL);

  if (!window) {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (glewInit() != GLEW_OK) {
    std::cout << "GLEW init failed" << std::endl;
    return -1;
  }

  std::cout << "OpenGL Version:" << std::endl;
  std::cout << glGetString(GL_VERSION) << std::endl;

  // OpenGL is basically a state machine. We set state first, and then perform
  // draw operations. The result of the draw operations are contextual, and
  // depend on the state set beforehand - its an ugly low level API, most of the
  // the time you'll be interacting with it at a higher level of abtraction. The
  // state includes information about what data to use to draw, and how to
  // interpret it in shaders).

  // Setup some data to define what to draw, in this case a triangle defined
  // by three coordinates. Screen coordinates are defined as floats between
  // -1 and 1, with the middle of the screen being 0,0.

  float positions[6] = {-0.5f, -0.5f, 0.0f, 0.5f, 0.5f, -0.5f};

  // Pass data to OpenGL as a bound buffer. This is commonly called a vertex
  // buffer object, or vbo. In OpenGL the term "vertex" means effectively a
  // collection of related attributes, for example a position in
  // screen space, a color, a normal, a texture coordinate. And then the buffer
  // itself is one or more vertexes. In our example data above each vertex just
  // contains a position, and there are three of them in the buffer in order to
  // define a triangle.
  //
  // "Binding" the buffer causes the OpenGL state machine to "select" it -
  // in other words the next draw operations will use it

  unsigned int vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, positions, GL_STATIC_DRAW);

  // Setup the vertex array object, or vao. The significance of this step is
  // out of scope for now.

  unsigned int vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);

  // In the next line we describe the structure of our vertex attributes, so
  // the data is usable in shaders. The data passed to the GPU is just a flat
  // array so we have to tell OpenGL how to pick values out of it. We have just
  // one attribute, a position, so there's just one call to
  // glVertexAttribPointer. The first param `0` says we're describing the first
  // (in our case only) vertext attribute. The next param `2` says it's
  // comprised from 2 components the x and the y values. The next param
  // `GL_FLOAT` sets its data type. The next param `sizeof(float) * 2` is the
  // "stride", which is its size in bytes.
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

  // Shaders are programs that we define as strings, and send to be executed
  // on the GPU rather than the CPU

  // Vertex shader is run once for every vertex
  std::string vertexShader = "#version 330 core\n"
                             "\n"
                             "layout(location = 0) in vec4 position;\n"
                             "\n"
                             "void main()\n"
                             "{\n"
                             "  gl_Position = position;\n"
                             "}\n";

  // Fragment shader is run once for every pixel, in this case we're just
  // painting each pixel inside the triangle red
  std::string fragmentShader = "#version 330 core\n"
                               "\n"
                               "layout(location = 0) out vec4 color;\n"
                               "\n"
                               "void main()\n"
                               "{\n"
                               "  color = vec4(1.0, 0.0, 0.0, 1.0);\n"
                               "}\n";

  unsigned int shader = CreateShader(vertexShader, fragmentShader);
  glUseProgram(shader);

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}