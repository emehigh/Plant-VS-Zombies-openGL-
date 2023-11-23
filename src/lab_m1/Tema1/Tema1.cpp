#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <iostream>

#include "lab_m1/Tema1/transform.h"
#include "lab_m1/Tema1/object.h"


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
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);


    lifes = 3;
    stars_in_bank = 5;

    float squareSide = 100;
    float rectangleWidth = 50;
    float rectangleHeight = 400;
    // TODO(student): Compute coordinates of a square's center, and store
    // then in the `cx` and `cy` class variables (see the header). Use
    // `corner` and `squareSide`. These two class variables will be used
    // in the `Update()` function. Think about it, why do you need them?

    // Initialize tx and ty (the translation steps)
    translateX = 0;
    translateY = 0;

    cx = corner.x + squareSide / 2;
    cy = corner.y + squareSide / 2;

    // Initialize sx and sy (the scale factors)
    scaleX = 1;
    scaleY = 1;

    // Initialize angularStep
    angularStep = 0;


    // 
    Mesh* spot_square[9];
    for (int i = 1; i < 10; i++) {
        spot_square[i-1] = object2DD::CreateSquare("spot_square" + to_string(i), corner, squareSide, glm::vec3(1, 0, 0), true);
        AddMeshToList(spot_square[i-1]);
    }

    Mesh* base_rectangle = object2DD::CreateRecangle("rectangle", corner, rectangleWidth, rectangleHeight, glm::vec3(0, 1, 0), true);
    AddMeshToList(base_rectangle);

    Mesh* canon_square[4];
    for (int i = 1; i < 5; i++) {
        canon_square[i - 1] = object2DD::CreateSquare("canon_square" + to_string(i), corner, squareSide, glm::vec3(0, 0, 1), false);
        AddMeshToList(canon_square[i - 1]);
    }
    Mesh* romb_port = object2DD::CreateDiamondWithCannon("romb1", corner, squareSide * 0.45, glm::vec3(1, 0.64, 0));
    Mesh* romb_albast = object2DD::CreateDiamondWithCannon("romb2", corner, squareSide * 0.45, glm::vec3(0, 0, 1));
    Mesh* romb_galb = object2DD::CreateDiamondWithCannon("romb3", corner, squareSide * 0.45, glm::vec3(1, 1, 0));
    Mesh* romb_mov = object2DD::CreateDiamondWithCannon("romb4", corner, squareSide * 0.45, glm::vec3(1, 0, 1));
    AddMeshToList(romb_port);
    AddMeshToList(romb_albast);
    AddMeshToList(romb_galb);
    AddMeshToList(romb_mov);

    Mesh* hexagon_port = object2DD::CreateHexagon("hexagon1", corner, squareSide/3, glm::vec3(1, 0.64, 0),true);
    Mesh* hexagon_albast = object2DD::CreateHexagon("hexagon2", corner, squareSide/3, glm::vec3(0, 0, 1),true);
    Mesh* hexagon_galb = object2DD::CreateHexagon("hexagon3", corner, squareSide/3, glm::vec3(1, 1, 0),true);
    Mesh* hexagon_mov = object2DD::CreateHexagon("hexagon4", corner, squareSide/3, glm::vec3(1, 0, 1),true);
    Mesh* hexagon_center = object2DD::CreateHexagon("hexagon", corner, squareSide/6, glm::vec3(0, 1, 1),true);
    AddMeshToList(hexagon_port);
    AddMeshToList(hexagon_albast);
    AddMeshToList(hexagon_galb);
    AddMeshToList(hexagon_mov);
    AddMeshToList(hexagon_center);


    Mesh* star_to_collect = object2DD::CreateStar("star_to_collect", corner, squareSide * 0.2, glm::vec3(1, 0.5, 0.5));
    AddMeshToList(star_to_collect);
    Mesh* star = object2DD::CreateStar("star", corner, squareSide * 0.1, glm::vec3(0.5, 0, 1));
    AddMeshToList(star);
    Mesh* star1 = object2DD::CreateStar("star1", corner, squareSide * 0.2, glm::vec3(1, 0.64, 0));
    AddMeshToList(star1);
    Mesh* star2 = object2DD::CreateStar("star2", corner, squareSide * 0.2, glm::vec3(0, 0, 1));
    AddMeshToList(star2);
    Mesh* star3 = object2DD::CreateStar("star3", corner, squareSide * 0.2, glm::vec3(1, 1, 0));
    AddMeshToList(star3);
    Mesh* star4 = object2DD::CreateStar("star4", corner, squareSide * 0.2, glm::vec3(1, 0, 1));
    AddMeshToList(star4);


    Mesh* life_square[3];
    for (int i = 1; i <= lifes; i++) {
        life_square[i-1] = object2DD::CreateSquare("life_square" + to_string(i), corner, squareSide * 2 / 3, glm::vec3(0, 1, 0), true);
        AddMeshToList(life_square[i-1]);
    }


    for (int i = 0; i < 9; i++) {
        guns[i][0] = 0;
        guns[i][1] = 0;
    }
    canon_selected = 0;
    elapsedTime = 0;
    randTime = rand() % (3) + 3.0;
    star_deleted = 0;
    gun_to_delete = -1;
    breaked = 0;
    canon_clicked = 0;
    randTimeStars = 1;
    elapsedTimeStars = 0;

}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();

    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema1::Update(float deltaTimeSeconds)
{
    if (lifes == 0) {
        Exit();
    }

    auto it = objects_to_shrink.begin();

    // micsoreaza cu fiecare frame hexagoanele care au fost lovit de 3 ori
    // sau armele care au fost lovite de un hexagon sau sterse in mod intentionat
    while (it != objects_to_shrink.end()) {
        if (it->scaleX > 0 || it->scaleY > 0) {
            if (it->type == "hexagon") {
                it->scaleX -= deltaTimeSeconds * 5;
                it->scaleY -= deltaTimeSeconds * 5;
                modelMatrix = glm::mat3(1);
                glm::mat3 translateToOrigin1 = transform2DD::Translate(0, 0);;
                glm::mat3 scaleMatrix = transform2DD::Scale(it->scaleX, it->scaleY);
                glm::mat3 translateBack1 = transform2DD::Translate(it->x, it->y);
                modelMatrix = translateBack1 * scaleMatrix * translateToOrigin1;
                RenderMesh2D(meshes["hexagon"], shaders["VertexColor"], modelMatrix);
                RenderMesh2D(meshes["hexagon" + to_string(it->color)], shaders["VertexColor"], modelMatrix);
                //++it;
                break;
			}
            if (it->type == "gun") {
                it->scaleX -= deltaTimeSeconds * 2.5;
                it->scaleY -= deltaTimeSeconds * 2.5;
                modelMatrix = glm::mat3(1);
                glm::mat3 translateToOrigin1 = transform2DD::Translate(0, 0);;
                glm::mat3 scaleMatrix = transform2DD::Scale(it->scaleX, it->scaleY);
                glm::mat3 translateBack1 = transform2DD::Translate(it->x, it->y);
                modelMatrix = translateBack1 * scaleMatrix * translateToOrigin1;

                RenderMesh2D(meshes["romb" + std::to_string(it->color)], shaders["VertexColor"], modelMatrix);
                ++it;
                
            }
		}
        else {
            it = objects_to_shrink.erase(it);
        }
        
    }

    // alege o linie random (1..3) pentru inamic
    // si o culoare random (1..4) pentru inamic
    if (elapsedTime > randTime) {
        printf("elapsed time: %.f\n", elapsedTime);
        randTime = rand() % (3) + 3.0;
        elapsedTime = 0;

        randLine = rand() % (3) + 1;
        if (randLine == 1) {
            enemyY = window->GetResolution().y - 340;
        }
        else if (randLine == 2) {
            enemyY = window->GetResolution().y - 490;
        }
        else {
            enemyY = window->GetResolution().y - 640;
        }

        // Generate three pairs of (X, Y) coordinates for stars
 
        randColor = rand() % (4) + 1;

       
        enemies.push_back({ (float)randColor,1280,(float)enemyY,3 });
    }
    // alege random un timp la care sa apara 3 stele
    if (elapsedTimeStars > randTimeStars) {
        randTimeStars = rand() % (10) + 5;

        // Generate three pairs of (X, Y) coordinates for stars
        for (int i = 0; i < 3; ++i) {
            int randX = rand() % 1281;
            int randY = rand() % 721;
            stars_to_collect.push_back({ randX, randY });
        }
        elapsedTimeStars = 0;
    }
    elapsedTime += deltaTimeSeconds;
    elapsedTimeStars += deltaTimeSeconds;




    // afiseaza stelele care trebuie colectate
    // si in cazul in care in pozitia lor se apasa click le adauga in banca
    for (int i = 0; i < stars_to_collect.size(); i++) {
        modelMatrix = glm::mat3(1);
        modelMatrix += transform2DD::Translate(stars_to_collect[i][0], stars_to_collect[i][1]);
        RenderMesh2D(meshes["star_to_collect"], shaders["VertexColor"], modelMatrix);
        if (curr_X > stars_to_collect[i][0] - 50 && curr_X < stars_to_collect[i][0] + 50 && curr_Y > stars_to_collect[i][1] - 50 && curr_Y < stars_to_collect[i][1] + 50
            && !canon_clicked) {
            stars_in_bank++;
            stars_to_collect.erase(stars_to_collect.begin() + i);
        }
    }
    
        

    
    

    // in functie de celula in care s a dat click
    // daca sunt suficiente in banca, randeaza cu fiecare frame un nou
    // romb te tipul selectat care 'urmareste' pozitia mouseului
    if (canon_selected == 1) {
        if (stars_in_bank >= 1) {
            modelMatrix = glm::mat3(1);
            modelMatrix += transform2DD::Translate(curr_X, curr_Y);
            RenderMesh2D(meshes["romb" + to_string(canon_selected)], shaders["VertexColor"], modelMatrix);
        }
        else {
            canon_selected = 0;
        }
    }

    if (canon_selected == 2 || canon_selected == 3) {
        if (stars_in_bank >= 2) {
            modelMatrix = glm::mat3(1);
            modelMatrix += transform2DD::Translate(curr_X, curr_Y);
            RenderMesh2D(meshes["romb" + to_string(canon_selected)], shaders["VertexColor"], modelMatrix);
        }
        else {
            canon_selected = 0;
        }
    }

    if (canon_selected == 4) {
        if (stars_in_bank >= 3) {
            modelMatrix = glm::mat3(1);
            modelMatrix += transform2DD::Translate(curr_X, curr_Y);
            RenderMesh2D(meshes["romb" + to_string(canon_selected)], shaders["VertexColor"], modelMatrix);
        }
        else {
            canon_selected = 0;
        }
    }
    // base 
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2DD::Translate(5, 30);
    RenderMesh2D(meshes["rectangle"], shaders["VertexColor"], modelMatrix);


    // placing spots
    modelMatrix = glm::mat3(1);
    glm::mat3 modelMatrix = glm::mat3(1);


    // pentru fiecare din cele 9 celule
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {

            int index = i * 3 + j + 1;
            float xTranslation = 80 + j * 150;
            float yTranslation = 30 + i * 150;

            // daca s a apasat click dreapta pe o celula
            // rombul respectiv este adaugat la structura 
            // care face shrink
            if (gun_to_delete == (index - 1) && !guns[index-1][3]) {
                object_to_shrink obj = {
                    xTranslation + 30,
                    yTranslation + 50,
                    (int)guns[index - 1][1],
                    "gun",
                    1,
                    1 };
                objects_to_shrink.push_back(obj);
                guns[index - 1][0] = 0;
                guns[index - 1][1] = 0;
                guns[index - 1][2] = 0;
                guns[index - 1][3] = 0;
                guns[index - 1][4] = 0;                
                breaked = 1;
                break;
            }

            

            angularStep += deltaTimeSeconds * 0.1;
           
            if ((int)guns[index - 1][0] == 1) {
                // if not charged

                if ((int)guns[index - 1][1] == 1 && (int)guns[index - 1][3] == 1) {
                    stars_in_bank -= 1;
                    // charged for stars
                    guns[index - 1][3] = 0;
                }
                // if not charged

                if ((int)guns[index - 1][1] == 2 && (int)guns[index - 1][3] == 1) {
                    stars_in_bank -= 2;
                    // charged for stars
                    guns[index - 1][3] = 0;
                }
                // if not charged

                if ((int)guns[index - 1][1] == 3 && (int)guns[index - 1][3] == 1) {
                    stars_in_bank -= 2;
                    // charged for stars
                    guns[index - 1][3] = 0;
                }
                // if not charged
                if ((int)guns[index - 1][1] == 4 && (int)guns[index - 1][3] == 1) {
                    stars_in_bank -= 3;
                    // charged for stars
                    guns[index - 1][3] = 0;
                }

                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2DD::Translate(xTranslation + 30, yTranslation + 50);
                RenderMesh2D(meshes["romb" + std::to_string((int)guns[index - 1][1])], shaders["VertexColor"], modelMatrix);
            }

            auto star_it = stars.begin();

            float baseIncrement = deltaTimeSeconds * 50;

            // pentru fiecare stea
            // cu fiecare frame, se muta toate stelele
            // la dreapta
            while (star_it != stars.end()) {
                modelMatrix = glm::mat3(1);
                glm::mat3 translateToOrigin = transform2DD::Translate(0,0); 
                glm::mat3 rotationMatrix = transform2DD::Rotate(-angularStep*10);

                star_it->at(0) += baseIncrement; 
                glm::mat3 translateBack = transform2DD::Translate(star_it->at(0), star_it->at(1));
                modelMatrix = translateBack * rotationMatrix * translateToOrigin;
                RenderMesh2D(meshes["star" + to_string((int)(star_it->at(2)))], shaders["VertexColor"], modelMatrix);

                auto it = enemies.begin();
             

                // daca o stea a lovit un inamic
                // pe aceeasi linie si de aceeasi culoare
                // decrementam hexagonului respectiv si stergem steaua
                while (it != enemies.end()) {
                    if ((int)star_it->at(0) + 50 > (int)it->at(1) && (int)star_it->at(2) == (int)it->at(0) && (int)star_it->at(1) == (int)it->at(2)
                        &&(int)star_it->at(3) < (int)it->at(1)) {
                        star_it = stars.erase(star_it);
                        star_deleted = 1;
                        it->at(3)--;
                        break;
                    }
                    ++it;
                }
                if (star_deleted) {
                    star_deleted = 0;
                    break;
                }
                // daca steaua a iesit din ecran si nu a lovit niciun inamic
                // o stergem
                if ((int)star_it->at(0) > 2000) {
                    star_it = stars.erase(star_it);
                }

                else {
                    ++star_it;
                }
                //////////////////////////////////////////////////////////
            }

            auto it = enemies.begin();

            // pentru fiecare inamic
            // cu fiecare frame
            // toate hexagoanele se muta spre stanga
            while (it != enemies.end()) {


                // daca a fost lovit de 3 stele
                if ((int)it->at(3) == 0) {
                    object_to_shrink obj = {
                        (int)it->at(1),
                        (int)it->at(2),
                        (int)it->at(0),
                        "hexagon",
                        1,
                        1};

                    objects_to_shrink.push_back(obj);
                    it = enemies.erase(it);
                    continue;
                }

                // daca a lovit un romb
                // stergem rombul respectiv si il adaugam la structura de shrink
                if (((int)it->at(1) < xTranslation + 100) && ((int)it->at(2) == yTranslation + 50) && ((int)(guns[index-1][1] != 0))
                    && ((int)it->at(1) > xTranslation - 30)) {

                    object_to_shrink obj = {
                        xTranslation + 30,
                        yTranslation + 50,
                        (int)guns[index - 1][1],
                        "gun",
                        1,
                        1 };
                    objects_to_shrink.push_back(obj);
                    guns[index - 1][0] = 0;
                    guns[index - 1][1] = 0;
                    guns[index - 1][2] = 0;
                    guns[index - 1][3] = 0;
                    guns[index - 1][4] = 0;
                    breaked = 1;
                    break;
                }
             
                // daca hexagonul e pe aceeasi linie cu rombul de aceeasi culoare
                // si exista arma de aceeasi culoare pe linia repsectiva
                // adaugam stele
                if (it->at(0) == (int)guns[index - 1][1] && it->at(2) == yTranslation + 50){


                    guns[index - 1][2] += deltaTimeSeconds;
                    // daca au trecut 2 secunde de la ultima stea
                    // adaugam alta
                    if ((int)guns[index - 1][2] > 2 && it->at(1) > xTranslation + 50) {
                        stars.push_back({ xTranslation + 80, yTranslation + 50, guns[index-1][1], xTranslation + 80});
                        guns[index - 1][2] = 0;        

                    }

                    //////////////////////////////// rotating hexagon
                    modelMatrix = glm::mat3(1);
                    glm::mat3 translateToOrigin = transform2DD::Translate(0, 0); 
                    glm::mat3 rotationMatrix = transform2DD::Rotate(angularStep);

                    it->at(1) -= deltaTimeSeconds * 10;
                    glm::mat3 translateBack = transform2DD::Translate(it->at(1), it->at(2));
                    modelMatrix = translateBack * rotationMatrix * translateToOrigin;

                    RenderMesh2D(meshes["hexagon"], shaders["VertexColor"], modelMatrix);
                    RenderMesh2D(meshes["hexagon" + to_string((int)it->at(0))], shaders["VertexColor"], modelMatrix);

                    modelMatrix *= transform2DD::Translate(50, 1);
                    //////////////////////////////////////////////////

                    ////////////////////////////////////////////////// rotating star projectile

              
                        
                }
                it->at(1) -= deltaTimeSeconds * 5;

                modelMatrix = glm::mat3(1);
                glm::mat3 translateToOrigin = transform2DD::Translate(0, 0); 
                glm::mat3 rotationMatrix = transform2DD::Rotate(angularStep);

                glm::mat3 translateBack = transform2DD::Translate(it->at(1), it->at(2)); 
                modelMatrix = translateBack * rotationMatrix * translateToOrigin;


                RenderMesh2D(meshes["hexagon"], shaders["VertexColor"], modelMatrix);
                RenderMesh2D(meshes["hexagon" + to_string((int)it->at(0))], shaders["VertexColor"], modelMatrix);
       
                // daca inamicul a ajust la "baza"
                // stergem inamicul si scadem "viata" jucatorului cu 1
                if (it->at(1) < 20) {
                    it = enemies.erase(it); // Erase the current enemy and get the iterator to the next element
                    lifes--;
                }
                else {
                    ++it; // Move to the next element
                }
            }

            if (breaked) {
                break;
            }


         
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2DD::Translate(xTranslation, yTranslation);
            RenderMesh2D(meshes["spot_square" + std::to_string(index)], shaders["VertexColor"], modelMatrix);


        }

        if (breaked) {
            gun_to_delete = -1;
            breaked = 0;
            break;
        }
    }



    // buying spots
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2DD::Translate(35, 500);
    RenderMesh2D(meshes["canon_square1"], shaders["VertexColor"], modelMatrix);
    modelMatrix *= transform2DD::Translate(180, 0);
    RenderMesh2D(meshes["canon_square2"], shaders["VertexColor"], modelMatrix);
    modelMatrix *= transform2DD::Translate(180, 0);
    RenderMesh2D(meshes["canon_square3"], shaders["VertexColor"], modelMatrix);
    modelMatrix *= transform2DD::Translate(180, 0);
    RenderMesh2D(meshes["canon_square4"], shaders["VertexColor"], modelMatrix);


    // and canons
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2DD::Translate(65, 550);
    RenderMesh2D(meshes["romb1"], shaders["VertexColor"], modelMatrix);


    modelMatrix *= transform2DD::Translate(180, 0);
    RenderMesh2D(meshes["romb2"], shaders["VertexColor"], modelMatrix);

    modelMatrix *= transform2DD::Translate(180, 0);
    RenderMesh2D(meshes["romb3"], shaders["VertexColor"], modelMatrix);

    modelMatrix *= transform2DD::Translate(180, 0);
    RenderMesh2D(meshes["romb4"], shaders["VertexColor"], modelMatrix);




    //canon stars
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2DD::Translate(50, 485);
    RenderMesh2D(meshes["star"], shaders["VertexColor"], modelMatrix);
    modelMatrix *= transform2DD::Translate(180, 0);
    RenderMesh2D(meshes["star"], shaders["VertexColor"], modelMatrix);
    modelMatrix *= transform2DD::Translate(30, 0);
    RenderMesh2D(meshes["star"], shaders["VertexColor"], modelMatrix);

    modelMatrix *= transform2DD::Translate(150, 0);
    RenderMesh2D(meshes["star"], shaders["VertexColor"], modelMatrix);
    modelMatrix *= transform2DD::Translate(30, 0);
    RenderMesh2D(meshes["star"], shaders["VertexColor"], modelMatrix);

    modelMatrix *= transform2DD::Translate(150, 0);
    RenderMesh2D(meshes["star"], shaders["VertexColor"], modelMatrix);
    modelMatrix *= transform2DD::Translate(30, 0);
    RenderMesh2D(meshes["star"], shaders["VertexColor"], modelMatrix);
    modelMatrix *= transform2DD::Translate(30, 0);
    RenderMesh2D(meshes["star"], shaders["VertexColor"], modelMatrix);


    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2DD::Translate(700, 500);

    for (int i = 0; i < lifes; i++) {
        modelMatrix *= transform2DD::Translate(100, 0);
        RenderMesh2D(meshes["life_square" + to_string(i+1)], shaders["VertexColor"], modelMatrix);
    }

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2DD::Translate(780, 480);
    for (int i = 0; i < stars_in_bank; i++) {
        modelMatrix *= transform2DD::Translate(40, 0);
        RenderMesh2D(meshes["star"], shaders["VertexColor"], modelMatrix);

    }

}


void Tema1::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    
    // daca s a ales o arma
    // tinem minte pozitia mouseului
    if (canon_selected) {
        curr_X = mouseX;
        curr_Y = window->GetResolution().y - mouseY;
    }
}

// 35 , 500
void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // pentru click stanga
    // in cazul in care s a dat click pe una din cele 4 celule
    // cu arme, se retine culoarea armei selectate
    if (button == 1) {
        curr_X = mouseX;
        curr_Y = window->GetResolution().y - mouseY;
        if (mouseX > 35 && mouseX < 135 && mouseY > 125 && mouseY < 225) {
            canon_selected = 1;
            canon_clicked = 1;
        }
        if (mouseX > 215 && mouseX < 315 && mouseY > 125 && mouseY < 225) {
            canon_selected = 2;
            canon_clicked = 1;
        }
        if (mouseX > 395 && mouseX < 495 && mouseY > 125 && mouseY < 225) {
            canon_selected = 3;
            canon_clicked = 1;
        }
        if (mouseX > 575 && mouseX < 675 && mouseY > 125 && mouseY < 225) {
            canon_selected = 4;
            canon_clicked = 1;
        }

    }
    // daca s a dat click dreapta
    // pe unul din cele 9 celule in care se pot afla arme
    // se retine indexul respectiv
    if (button == 2) {
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                int index = i * 3 + j;
                int xRangeStart = 80 + j * 150;
                int xRangeEnd = xRangeStart + 100;
                int yRangeStart = 690 - i * 150;
                int yRangeEnd = yRangeStart - 100;

                if (mouseX > xRangeStart && mouseX < xRangeEnd && mouseY < yRangeStart && mouseY > yRangeEnd) {
                    gun_to_delete = index;
                }
            }
        }
    }
    // Add mouse button press event
    //215 - 315
    //395 - 495 
    //575 - 675
}
//void Tema1::OnMouseHold() {
//}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // daca a fost 'dropped' o arma intr-o celula
    // se adauga la structura de arme o noua arma
    // la indexul respectiv (in functie de pozitia mouseului in care s a dat drop)
    // impreuna cu toate atributele ei (spot,culoare,timer sa traga, daca a fost platita etc.)
    if (canon_selected) {
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                int index = i * 3 + j;
                int xRangeStart = 80 + j * 150;
                int xRangeEnd = xRangeStart + 100;
                int yRangeStart = 690 - i * 150;
                int yRangeEnd = yRangeStart - 100;

                if (mouseX > xRangeStart && mouseX < xRangeEnd && mouseY < yRangeStart && mouseY > yRangeEnd) {
                    if (guns[index][0] != 1) {
                        guns[index][0] = 1;
                        guns[index][1] = canon_selected;
                        guns[index][2] = 0;
                        guns[index][3] = 1;
                        guns[index][4] = 1;
                    }
                }
            }
        }
    }

    
    canon_selected = 0;
    canon_clicked = 0;
    curr_X = mouseX;
    curr_Y = window->GetResolution().y - mouseY;

    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}

