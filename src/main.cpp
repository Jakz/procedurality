#include "raylib.hpp"
#include "Vector2.hpp"
#include "Window.hpp"

#include <vector>

std::vector<Vector3> points;
std::vector<std::tuple<size_t, size_t, Color>> edges;

void prepare()
{
  
  
  for (int r = 0; r < 6; ++r)
  {
    size_t layerStart = points.size();
    
    float R_max = 1.0f;
    int num_layers = 6;
    float t = float(r) / num_layers;
    float radius = R_max * pow(t, 1.0f / 3.0f);

    float target_spacing = 0.1f;
    int parallels = std::max(1.0f, std::round(ceil(2 * PI * radius / target_spacing))); 
    int meridians = std::max(1.0f, std::round(ceil(2 * PI * radius / target_spacing)));

    for (int i = 0; i < parallels; i++)
    {
      for (int j = 0; j < meridians; j++)
      {
        float theta = 2.0f * PI * i / parallels;
        float phi = PI * j / meridians;

        float x = radius * sin(phi) * cos(theta);
        float y = radius * cos(phi);
        float z = radius * sin(phi) * sin(theta);

        points.push_back(Vector3{ x, y, z });

        if (i > 0 || j > 0)
        {
          if (i > 0)
            edges.emplace_back(std::make_tuple(layerStart + i * meridians + j, layerStart + (i - 1) * meridians + j, Color{ (uint8_t)(180 + 10 * r), 0, 0, 140 }));
          else
            edges.emplace_back(layerStart + i * meridians + j, layerStart + (parallels - 1) * meridians + j, Color{ (uint8_t)(180 + 10 * r), 0, 0, 140 });

          if (j > 0)
          {
            edges.emplace_back(layerStart + i * meridians + j, layerStart + i * meridians + (j - 1), Color{ (uint8_t)(180 + 10 * r), 0, 0, 140 });
          }
        }
      }
    }
  }


}

int main(int argc, char *argv[])
{
  SetConfigFlags(FLAG_MSAA_4X_HINT);

  raylib::Window bootstrap(1, 1, "Bootstrap");

  int monitor = GetCurrentMonitor();
  raylib::Vector2 screenSize = raylib::Vector2(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
  screenSize.x = std::min(screenSize.x * 0.666f, 1920.0f);
  screenSize.y = screenSize.x * 1.0f/(16.0f/10.0f);  // 16:9 aspect ratio

  bootstrap.Close();

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

  prepare();

  while (!WindowShouldClose())
  {
    UpdateCamera(&camera, CAMERA_ORBITAL);
    
    BeginDrawing();
    ClearBackground(Color{0, 12, 31});
   // DrawTexture(tex, 0, 0, WHITE);

    BeginMode3D(camera);
    //DrawModelWires(model, { 0, 0, 0 }, 1.0f, RED);

    for (const auto& p : points)
    {
      DrawSphereWires(p, 0.002f, 4, 4, RED);     
    }

    for (const auto& edge : edges)
    {
      const Vector3& p1 = points[std::get<0>(edge)];
      const Vector3& p2 = points[std::get<1>(edge)];
      DrawLine3D(p1, p2, std::get<2>(edge));
    }

    EndMode3D();

    EndDrawing();
  }
}