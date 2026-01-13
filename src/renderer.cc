#include "renderer.h"
#include "asset.h"
#include "compute.h"
#include <cmath>
#include <fmt/base.h>

Renderer::Renderer(double width, double height)
    : width(width), height(height), main_tex(width, height), v_rot(0.f),
      v_translate({.x = 0.f, .y = 0.f, .z = 0.f}) {}
Renderer::~Renderer() {}

void Renderer::apply_asset(const Asset &asset, Texture2D &tex) {
  for (const vec2 &v : asset.data()) {
    if (compute::boundary_check(v, width, height)) {
      tex.draw_dot(v.x, v.y);
    }
  }
}

void Renderer::setup() {
  main_tex.generate_gradient();

  assets.emplace_back(geometry::create_rectangle(10, 10, 200, 20));
  assets.emplace_back(geometry::create_circle(width/2, height/2, 20));
}

void Renderer::update_physics(double dt) {}

void Renderer::update() {
  /* Draw assets into scene */
  main_tex.clear({0, 0, 0, 255});
  for (const Asset &asset : assets) {
    apply_asset(asset, main_tex);
    main_tex.draw_dot(asset.get_axis().x, asset.get_axis().y, {255, 0, 0, 255});
  }

  /* Update the scene's projection */
  main_tex.apply_projection(v_rot, v_translate);
  v_rot += 0.01f;

  // TEST: update asset translations
  assets[1].rotate(0.01);

  static size_t _si = 0;
  const float _dxy = 1.013;
  if (_si < 200) {
    assets[1].scale({_dxy, _dxy});
    _si++;
  }

  static size_t tick = 0;
  v_translate.z = 1.f + ( (std::sin(tick / 100.f) + 1.f) / 2.f );
  v_translate.y = std::sin(tick / 10.f) * 10.f;
  v_translate.x = std::cos(tick / 10.f) * 10.f;
  tick++;
}

void Renderer::draw() {
  /* Prepare texture (scene) to be drawn */
  main_tex.update();

  /* Pass "software rendered" scene to be drawn by OpenGL */
  glClearColor(255.f, 255.f, 255.f, 255.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glTranslatef(0.f, 0.f, 0.f);
  glScalef(1.f, 1.f, 0.f);

  glBindTexture(GL_TEXTURE_2D, main_tex.get_texture_id());
  glColor3f(1.0f, 1.0f, 1.0f);
  glEnable(GL_TEXTURE_2D);

  glBegin(GL_QUADS);
  {
    /* Bottom-Left (0, 0) */
    glTexCoord2f(0.f, 0.f);
    glVertex3f(0.f, 0.f, 0.f);

    /* Top-Left (0, HEIGHT) */
    glTexCoord2f(0.f, 1.f);
    glVertex3f(0.f, (float)height, 0.f);

    /* Top-Right (WIDTH, HEIGHT) */
    glTexCoord2f(1.f, 1.f);
    glVertex3f((float)width, (float)height, 0.f);

    /* Bottom-Right (WIDTH, 0) */
    glTexCoord2f(1.f, 0.f);
    glVertex3f((float)width, 0.f, 0.f);
  }
  glEnd();

  glBindTexture(GL_TEXTURE_2D, 0);
  glDisable(GL_TEXTURE_2D);
}

