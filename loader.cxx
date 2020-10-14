#include <vector>
#include <cstdio>
#include <gl3w/GL/gl3w.h>
#include <GLFW/glfw3.h>

typedef unsigned int uint;

////////////////////////////////////////////////////////////////////////////////
// Host code.

struct vert_features_t {
  // Each flag indicates the availability of a vertex attribute.
  // vattrib_position is always available.
  bool normal;      // vattrib_normal
  bool tangent;     // vattrib_tangent + vattrib_binormal
  bool texcoord0;   // vattrib_texcoord0
  bool texcoord1;   // vattrib_texcoord1
  bool joints0;     // vattrib_joints0 + vattrib_weights0
  bool joints1;     // vattrib_joints1 + vattrib_weights1
};

struct frag_features_t {
  // Incoming vertex properties.
  bool normal;
  bool tangents;

  // Available texture maps.
  bool normal_map;
  bool emissive_map;
  bool occlusion_map;
  
  // Material properties.
  bool metallicRoughness;
  bool anisotropy;
  bool ibl;
  bool point_lights;
};


void load_shader(GLuint shader, const char* module, const char* entry_point, 
  int num_spec, const uint* spec_indices, const uint* spec_values) {
  FILE* f = fopen(module, "r");
  fseek(f, 0, SEEK_END);
  long len = ftell(f);
  fseek(f, 0, SEEK_SET);

  std::vector<char> vec;
  vec.resize(len);
  fread(vec.data(), 1, len, f);
  fclose(f);

  glShaderBinary(1, &shader, GL_SHADER_BINARY_FORMAT_SPIR_V_ARB, 
    vec.data(), len);
  glSpecializeShader(shader, entry_point, num_spec, spec_indices, spec_values);
}
 
int main() {
  glfwInit();
  gl3wInit();
  
  glfwWindowHint(GLFW_DOUBLEBUFFER, 1);
  glfwWindowHint(GLFW_DEPTH_BITS, 24);
  glfwWindowHint(GLFW_STENCIL_BITS, 8);
  glfwWindowHint(GLFW_SAMPLES, 4); // HQ 4x multisample.
  glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

  GLFWwindow* window = glfwCreateWindow(640, 480, "bug", nullptr, nullptr);
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  // Load and compile the shaders.
  GLuint vs = glCreateShader(GL_VERTEX_SHADER);
  GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

  {
    uint indices[6] { 0, 1, 2, 3, 4, 5 };
    uint values[6]  { 1, 0, 1, 0, 0, 0 };
    #if LINK_SPV
      load_shader(vs, "link.spv", "_Z9vert_mainv", 6, indices, values);
    #else
      load_shader(vs, "vert.spv", "_Z9vert_mainv", 6, indices, values);
    #endif
  }

  {
    uint indices[9] { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
    uint values[9]  { 1, 0, 1, 1, 1, 1, 0, 1, 0 };
    #if LINK_SPV
      load_shader(vs, "link.spv", "_Z9frag_mainv", 6, indices, values);
    #else
      load_shader(vs, "frag.spv", "_Z9frag_mainv", 6, indices, values);
    #endif
  }

  printf("glSpecialized worked!\n");

  return 0;
}