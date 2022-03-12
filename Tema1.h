#pragma once

#include "components/simple_scene.h"
#include <vector>

namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

        void Init() override;
        
    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        void GenerateMap(int width, int height);
        void RenderMap(int width, int height);
        void SmoothMap();
        int SurroundingWalls(int X, int Y);
        bool isCollided();
        bool isCollidedBullet();
    protected:
        glm::mat3 modelMatrix;
        bool gone;
        int mapHeight;
        int mapWidth;
        int mapArray[3000][3000];
        int randomFillPercent;
        int health;
        int score;
        float randomspeed;
        float playerDimension;
        float wallDimension;
        float bulletDimension;
        float playerX;
        float playerY;
        float bulletX;
        float bulletY;
        float enemyX;
        float enemyY;
        float angleToMouse;
        float angle;
        float speed;
        bool shoot;


        // TODO(student): If you need any other class variables, define them here.

    };
}   // namespace m1