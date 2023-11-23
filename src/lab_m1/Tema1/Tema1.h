#pragma once

#include "components/simple_scene.h"
#include <thread> // Add this include for std::this_thread::sleep_for
#include <chrono>
#include <glm/glm.hpp>  // Include the necessary glm header

namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();
       // void shrink_objects(std::vector<struct object_to_shrink>& objects_to_shrink, float deltaTimeSeconds);

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


    protected:
        float cx, cy;
        glm::mat3 modelMatrix;
        float translateX, translateY;
        float scaleX, scaleY;
        float angularStep;
        int direction;
        int lifes;
        int stars_in_bank;
        int canon_selected;
        int curr_X;
        int curr_Y;
        float guns[9][5];
        //[0] - spot(0..8)
        //[1] - culoare
        //[3] - timp de tragere
        int randLine;
        float elapsedTime;
        int enemyY;
        float randTime;
        float randTimeStars;
        int randColor;
        float gunTimer;
        float hexXmovement;
        std::vector <std::vector <int>> stars_to_collect;

        // enemies[0] -> culoare
       // enemies[1] -> X
       // enemies[2] -> Y
       // enemies[3] -> lifes
        std::vector <std::vector <float>> enemies;


        std::vector <std::vector <float>> stars;
        int star_deleted;
        int gun_to_delete;
        typedef struct object_to_shrink {
            int x, y, color;
            std::string type;
            float scaleX, scaleY;
        } object_to_shrink;
        int breaked;
        int canon_clicked;
        float elapsedTimeStars;
        std::vector <object_to_shrink> objects_to_shrink;

    };
}   // namespace m1
