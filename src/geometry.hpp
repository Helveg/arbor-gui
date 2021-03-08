#pragma once

// needs to be at the top
#include <GL/gl3w.h>

#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>

#include <arbor/morph/morphology.hpp>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "id.hpp"
#include "view_state.hpp"
#include <utils.hpp>
#include <definition.hpp>

struct point {
  glm::vec3 position = {0.0f, 0.0f, 0.0f};
  glm::vec3 normal   = {0.0f, 0.0f, 0.0f};
  glm::vec3 id       = {0.0f, 0.0f, 0.0f};
};

glm::vec4 next_color();

struct renderable {
  size_t count = 0;
  size_t instances = 0;
  unsigned vao = 0;
  bool active = false;
  glm::vec4 color = next_color();
};

inline void destroy_renderables(std::vector<renderable>& rs) {
  for (auto& r: rs) {
    r.count = 0;
    r.instances = 0;
    glDeleteVertexArrays(1, &r.vao);
    r.active = false;
  }
}

struct picker {
  unsigned fbo = 0;
  unsigned post_fbo = 0;
  unsigned tex = 0;
};

struct object_id {
    size_t segment;
    size_t branch;
};

struct geometry {
  geometry();
  geometry(const arb::morphology&);

  void render(const view_state& view,
              const glm::vec2& size,
              const std::vector<renderable>&,
              const std::vector<renderable>&);

  renderable make_marker(const std::vector<glm::vec3>& points, glm::vec4 color);
  renderable make_region(const std::vector<arb::msegment>& segments, glm::vec4 color);

  std::optional<object_id> get_id_at(const glm::vec2& pos, const view_state&, const glm::vec2& size, const std::vector<renderable>&);

  void load_geometry(const arb::morphology&);

  glm::vec3 target = {0.0f, 0.0f, 0.0f};

  void make_fbo(int w, int h);
  std::vector<point>    vertices = {};
  std::vector<unsigned> indices  = {};
  std::unordered_map<size_t, size_t> id_to_index  = {}; // map segment id to cylinder index
  std::unordered_map<size_t, size_t> id_to_branch = {}; // map segment id to branch id

  unsigned fbo = 0;
  unsigned post_fbo = 0;
  unsigned tex = 0;
  unsigned vbo = 0;
  unsigned marker_vbo = 0;
  unsigned region_program = 0;
  unsigned object_program = 0;
  unsigned marker_program = 0;

  // Geometry
  size_t n_faces = 64;
  float rescale = -1;
  glm::vec3 root = {0.0f, 0.0f, 0.0f};

  // Viewport
  int width = -1;
  int height = -1;

  // Background
  glm::vec4 clear_color{214.0f/255, 214.0f/255, 214.0f/255, 1.00f};

  // Picker
  picker pick;
};
