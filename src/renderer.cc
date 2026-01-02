#include "renderer.h"
#include <fmt/base.h>

Renderer::Renderer(double width, double height) : width(width), height(height), main_tex(width, height) {}
Renderer::~Renderer() {}


void Renderer::setup() {
  main_tex.generate_gradient();
}

void Renderer::update_physics(double dt) {}

void Renderer::update() {
  // TEST:
  fmt::println("[dt={:.2f}] j={}", dt, j);
  for (size_t i = 0; i < main_tex.width; i++) {
    if (j > 0) {
      main_tex.at(i, j - 1).set(0, 0, 0, 255);
    }
    main_tex.at(i, j).set(255, 0, 0, 255);
  }
  j = (j + 1) % main_tex.height;

  main_tex.update();
}

void Renderer::draw() {
  glClearColor(255.f, 255.f, 255.f, 255.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glTranslatef(0.f, 0.f, 0.f);
  glScalef(1.f, 1.f, 0.f);

  glBindTexture(GL_TEXTURE_2D, main_tex.id);
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

