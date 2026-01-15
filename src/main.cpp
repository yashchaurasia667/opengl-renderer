#include <iostream>

#include <shader.h>
#include <renderer.h>
#include <utils.h>

Renderer ren("renderer window", 800, 600, "../resources/square.obj");
void gameLoop();

int main()
{
  ren.start(gameLoop);
  return 0;
}

void gameLoop()
{
  glCall(glClearColor(0.1f, 0.3f, 0.3f, 1.0f));
  glCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}