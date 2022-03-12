#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <iostream>

#include "lab_m1/Tema1/object2D.h"
#include "lab_m1/Tema1/transform2D.h"
#include "math.h"
#include <cmath>
#include <ctime>
using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}

void Tema1::Init()
{
    //Initialise camera
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 0));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    //Initialise health and score
    health = 100;
    score = 0;

    //Initialise player angle
    angleToMouse = 0;

    //Initialise bullet
    angle = 0;
    shoot = false;
    bulletX = 0;
    bulletY = 0;
    speed = 1000;
    randomFillPercent = 20;

    //Initialise Map
    mapWidth = 30;
    mapHeight = 30;
    GenerateMap(mapWidth, mapHeight);
    
    //Add Meshes
    playerDimension = 40;
    glm::vec3 cornerplayer = glm::vec3(-playerDimension / 2, -playerDimension / 2, -playerDimension / 2);

    wallDimension = 50;
    glm::vec3 cornerwall = glm::vec3(-wallDimension / 2, -wallDimension / 2, -wallDimension / 2);

    bulletDimension = 20;
    glm::vec3 cornerbullet = glm::vec3(-bulletDimension / 2, -bulletDimension / 2, -bulletDimension / 2);

    Mesh* square1 = object2Dtema1::CreatePlayer("player", cornerplayer, playerDimension, glm::vec3(1), true);
    AddMeshToList(square1);
    Mesh* square2 = object2Dtema1::CreateSquare("wall", cornerwall, wallDimension, glm::vec3(0 , 0.6f, 0), true);
    AddMeshToList(square2);
    Mesh* square3 = object2Dtema1::CreateEnemy("enemy", cornerplayer, playerDimension/2, glm::vec3(1, 0, 0), true);
    AddMeshToList(square3);
    Mesh* square4 = object2Dtema1::CreateSquare("bullet", cornerbullet, bulletDimension, glm::vec3(0), true);
    AddMeshToList(square4);
    Mesh* square5 = object2Dtema1::CreateSquare("healthbar", cornerwall, wallDimension, glm::vec3(1,0,0), true);
    AddMeshToList(square5);
    Mesh* square6 = object2Dtema1::CreateSquare("healthbarsquare", cornerwall, wallDimension, glm::vec3(1, 0, 0), false);
    AddMeshToList(square6);
    Mesh* square7 = object2Dtema1::CreateSquare("healthpack", cornerwall, wallDimension, glm::vec3(1), true);
    AddMeshToList(square7);

    //Initialise player
    playerX = wallDimension+playerDimension/2;
    playerY = wallDimension+playerDimension / 2;

    //Initialise enemy
    enemyX = (mapWidth - 2)*wallDimension;
    enemyY = (mapHeight - 2)*wallDimension;
    randomspeed = rand() % 300 + 100;
}

void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.3f, 0.3f, 0.3f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema1::Update(float deltaTimeSeconds)
{
    //DISPLAY STATS
    //cout << "FPS - SCOR - HEALTH: " << (int)(1 / deltaTimeSeconds) << " - " << score << " - " << health / 100 << "\r";
    

    //SET CAMERA
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(playerX-resolution.x/2, playerY-resolution.y/2, 250));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    //SET HEALTHBAR

    //wireframe
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2Dtema1::Translate(playerX, playerY + resolution.y / 2 - 50);
    modelMatrix *= transform2Dtema1::Scale(100 / 4, 1);
    RenderMesh2D(meshes["healthbarsquare"], shaders["VertexColor"], modelMatrix);

    //actual healthbar
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2Dtema1::Translate(playerX, playerY + resolution.y / 2 - 50);
    modelMatrix *= transform2Dtema1::Scale(health / 4, 1);
    RenderMesh2D(meshes["healthbar"], shaders["VertexColor"], modelMatrix);

    //DISPLAY MAP
    RenderMap(mapWidth, mapHeight);

    //SHOOT
    if (shoot == true) {
        modelMatrix = glm::mat3(1);
        bulletX += cos(angle) * speed * deltaTimeSeconds;
        bulletY += sin(angle) * speed * deltaTimeSeconds;
        modelMatrix *= transform2Dtema1::Translate(bulletX, bulletY);
        modelMatrix *= transform2Dtema1::Rotate(angle);
        RenderMesh2D(meshes["bullet"], shaders["VertexColor"], modelMatrix);
    }
    
    else if (shoot == false) {
        modelMatrix = glm::mat3(1);
        bulletX = playerX;
        bulletY = playerY;
        modelMatrix *= transform2Dtema1::Translate(bulletX, bulletY);
        modelMatrix *= transform2Dtema1::Rotate(angleToMouse);
        RenderMesh2D(meshes["bullet"], shaders["VertexColor"], modelMatrix);
    }

    //COORDONATE PENTRU O VIITOARE IMPLEMENTARE DE PATHFINDING POSIBILA
    int enemyGridX = (int)(enemyX / (wallDimension));
    int enemyGridY = (int)(enemyY / (wallDimension));

    int bulletGridX = (int)(bulletX / (wallDimension));
    int bulletGridY = (int)(bulletY / (wallDimension));

    //CALCUL UNGHI PLAYER INAMIC
    float unghideatac = atan2(playerY - enemyY, playerX - enemyX);
    srand(unsigned(time(0)));

    //COLIZIUNE PROIECTIL PERETI
    if (isCollidedBullet()) {
        shoot = false;
        bulletX = playerX;
        bulletY = playerY;
    }
    
    //COLIZIUNE PROIECTIL INAMIC
    if ((bulletX >= enemyX - playerDimension / 2 && bulletY >= enemyY - playerDimension / 2) && (bulletX <= enemyX + playerDimension / 2 && bulletY <= enemyY + playerDimension / 2)) {
        enemyX = (rand() % (mapWidth - 2) + 1) * wallDimension;
        enemyY = (rand() % (mapWidth - 2) + 1) * wallDimension;
        shoot = false;
        bulletX = playerX;
        bulletY = playerY;
        score += 10;
        cout << "SCOR:" << score << "\n";
        randomspeed = rand() % 300 + 100;
    }

    //COLIZIUNE PLAYER INAMIC
    if ((playerX >= enemyX - playerDimension && playerY >= enemyY - playerDimension ) && (playerX <= enemyX + playerDimension && playerY <= enemyY + playerDimension)) {
        enemyX = (rand() % (mapWidth - 15) + 1) * wallDimension;
        enemyY = (rand() % (mapWidth - 15) + 1) * wallDimension;
        if (health > 0) {
            health -= 10;
        } 
        cout << "HEALTH:" << health << "\n";
        randomspeed = rand() % 300+100;
    }
    

    //DRAW ENEMY
    enemyX += randomspeed * cos(unghideatac) * deltaTimeSeconds;
    enemyY += randomspeed * sin(unghideatac) * deltaTimeSeconds;
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2Dtema1::Translate(enemyX, enemyY);
    modelMatrix *= transform2Dtema1::Rotate(unghideatac);
    RenderMesh2D(meshes["enemy"], shaders["VertexColor"], modelMatrix);

    //DRAW PLAYER
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2Dtema1::Translate(playerX,playerY);
    modelMatrix *= transform2Dtema1::Rotate(angleToMouse);
    RenderMesh2D(meshes["player"], shaders["VertexColor"], modelMatrix);
    
    
    
}


void Tema1::FrameEnd()
{

}

//GENERARE HARTA RANDOM
void Tema1::GenerateMap(int width, int height) 
{
    srand(unsigned(time(0)));
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            if (x == 0 || x == width - 1 || y == 0 || y == height - 1) {
                mapArray[x][y] = 1;
            }
            else {
                mapArray[x][y] = ((rand() % 100) + 1 < randomFillPercent) ? 1 : 0;
            }
        }
    }

    for (int i = 0; i < 5; i++) {
        SmoothMap();
    }

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            if (x == 0 || x == width - 1 || y == 0 || y == height - 1) {
                mapArray[x][y] = 1;
            }
        }
    }
    for (int i = 0; i < 5; i++) {
        mapArray[rand() % (width-2)+1][rand() % (height-2)+1] = 2;
    }
    

    
}

//CELLULAR AUTOMATA
void Tema1::SmoothMap() {
    for (int x = 0; x < mapWidth; x++) {
        for (int y = 0; y < mapHeight; y++) {
            int neighbourWallTiles = SurroundingWalls(x, y);
            if (neighbourWallTiles < 4) {
                mapArray[x][y] = 1;
            }
            else if (neighbourWallTiles > 5) {
                mapArray[x][y] = 0;
            }
        }
    }
}
int Tema1::SurroundingWalls(int X, int Y) {
    int wallcount = 0;
    for (int neighbourX = X-3; neighbourX < X+3; neighbourX++) {
        for (int neighbourY = Y-3; neighbourY < Y+3; neighbourY++) {
            if (neighbourX >= 0 && neighbourX < mapWidth && neighbourY >= 0 && neighbourY < mapHeight) {
                if (neighbourX != X || neighbourY != Y) {
                    wallcount += mapArray[neighbourX][neighbourY];
                }
            }
            else {
                wallcount++;
            }
        }
    }
    return wallcount;
}

//RANDARE HARTA
void Tema1::RenderMap(int width, int height)
{
    glm::ivec2 resolution = window->GetResolution();
    for (int x=0; x < width; x++) {
        for (int y=0; y < height; y++) {
            //if (mapArray[x][y]) { 
            
            //OPTIMIZARE RANDARE HARTA DIMENSIUNI MARI
            if (mapArray[x][y]==1 && x * wallDimension <resolution.x/2+playerX && y * wallDimension <resolution.y/2+playerY) {
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2Dtema1::Translate(x*wallDimension, y*wallDimension);
                RenderMesh2D(meshes["wall"], shaders["VertexColor"], modelMatrix);
            }
            if (mapArray[x][y] == 2) {
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2Dtema1::Translate(x * wallDimension, y * wallDimension);
                RenderMesh2D(meshes["healthpack"], shaders["VertexColor"], modelMatrix);
            }
        }
    }
}

/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */

bool Tema1::isCollided() {
    bool isCollided = false;
    for (int x = 0; x < mapWidth; x++) {
        for (int y = 0; y < mapHeight; y++) {
            if ((x == mapWidth - 1 || y == mapHeight - 1)&& mapArray[x][y] == 0) {
                if ((playerX > x * wallDimension - wallDimension / 2 - playerDimension / 2 && playerY > y * wallDimension - wallDimension / 2 - playerDimension / 2) && (playerX < x * wallDimension + wallDimension / 2 + playerDimension / 2 && playerY < y * wallDimension + wallDimension / 2 + playerDimension / 2)) {
                    return true;
                }
            }
            if (mapArray[x][y]==1) {
                if ((playerX > x * wallDimension - wallDimension / 2 - playerDimension / 2 && playerY > y * wallDimension - wallDimension / 2 - playerDimension / 2) && (playerX < x * wallDimension + wallDimension / 2 + playerDimension / 2 && playerY < y * wallDimension + wallDimension / 2 + playerDimension / 2)) {
                    return true;
                }
            }
            if (mapArray[x][y] == 2) {
                if ((playerX > x * wallDimension - wallDimension / 2 - playerDimension / 2 && playerY > y * wallDimension - wallDimension / 2 - playerDimension / 2) && (playerX < x * wallDimension + wallDimension / 2 + playerDimension / 2 && playerY < y * wallDimension + wallDimension / 2 + playerDimension / 2)) {
                    mapArray[x][y] = 0;
                    
                    if (health < 100) {
                        health += 10;
                        cout << "HEALTH: " << health << "\n";
                    }
                }
            }
            
        }
    }
    return false;
}

bool Tema1::isCollidedBullet() {
    bool isCollided = false;
    for (int x = 0; x < mapWidth; x++) {
        for (int y = 0; y < mapHeight; y++) {
            if (mapArray[x][y]) {
                if ((bulletX > x * wallDimension - wallDimension / 2 - bulletDimension/2 && bulletY > y * wallDimension - wallDimension / 2 - bulletDimension / 2) && (bulletX < x * wallDimension + wallDimension / 2 + bulletDimension / 2 && bulletY < y * wallDimension + wallDimension / 2 + bulletDimension / 2)) {
                    return true;
                }
            }

        }
    }
    return false;
}
void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    


    //CONTROLUL PLAYER-ULUI
    if (window->KeyHold(GLFW_KEY_W)) {
        playerY += 250 * deltaTime;
        if (isCollided()) {
            playerY -= 250 * deltaTime;
        }
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        playerY -= 250 * deltaTime;
        if (isCollided()) {
            playerY += 250 * deltaTime;
        }
    }
    if (window->KeyHold(GLFW_KEY_D)) {
        playerX += 250 * deltaTime;
        if (isCollided()) {
            playerX -= 250 * deltaTime;
        }
    }
    if (window->KeyHold(GLFW_KEY_A)) {
        playerX -= 250 * deltaTime;
        if (isCollided()) {
            playerX += 250 * deltaTime;
        }
    }
    
    
}


void Tema1::OnKeyPress(int key, int mods)
{
    
    if (window->KeyHold(GLFW_KEY_Z)) {
        //health += 10;
        //cout << health << '\n';
        GenerateMap(mapWidth, mapHeight);
    }
    /*
    if (window->KeyHold(GLFW_KEY_X)) {
        health -= 10;
        //cout << health << '\n';
    }
    */
    // Add key press event
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
 
    //CALCULUL UNGHIULUI CATRE CURSOR
    glm::ivec2 resolution = window->GetResolution();
    angleToMouse = -atan2(mouseY-resolution.y/2, mouseX-resolution.x/2);
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (!shoot) {
        angle = angleToMouse;
    }
    shoot = true;
    //cout << angleToMouse << '\n';
    //cout << "\a";
    //GenerateMap(mapWidth, mapHeight);
    // Add mouse button press event
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
