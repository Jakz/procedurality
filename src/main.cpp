#include "raylib.hpp"
#include "Vector2.hpp"
#include "Window.hpp"

int main(int argc, char *argv[])
{
  SetConfigFlags(FLAG_MSAA_4X_HINT);

  raylib::Vector2 screenSize = raylib::Vector2(GetMonitorWidth(0), GetMonitorHeight(0));
  screenSize *= 0.1f;

  printf("Screen size: %s\n", screenSize.ToString().c_str());

  raylib::Window window = raylib::Window(screenSize.x, screenSize.y, "Procedurality");
  
  Image img = GenImagePerlinNoise(256, 256, 0, 0, 10.0f);
  Texture2D tex = LoadTextureFromImage(img);

  SetTargetFPS(60);

  Mesh mesh = GenMeshTorus(1.0f, 0.25f, 16, 32);
  Model model = LoadModelFromMesh(mesh);

  Camera camera = { 0 };
  camera.position = { 4.0f, 2.0f, 1.0f };  // Camera position
  camera.target = { 0.0f, 0.0f, 0.0f };    // Look at the origin
  camera.up = { 0.0f, 1.0f, 0.0f };        // Up vector
  camera.fovy = 45.0f;
  camera.projection = CAMERA_PERSPECTIVE;

  while (!WindowShouldClose())
  {
    UpdateCamera(&camera, CAMERA_ORBITAL);
    
    BeginDrawing();
    ClearBackground(Color{0, 12, 31});
    DrawTexture(tex, 0, 0, WHITE);

    BeginMode3D(camera);
    DrawModelWires(model, { 0, 0, 0 }, 1.0f, RED);
    EndMode3D();

    EndDrawing();
  }
}