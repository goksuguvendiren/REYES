#include <iostream>
#include <vector>
#include <string>
#include "Ri.h"
#include "glm_ostream.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

constexpr double MY_PI = 3.14;

void BasicScene()
{
    RiBegin(RtToken("guzu.png"));

    RiFormat(600, 600, 1.0);
    RiFrameAspectRatio(4.0/3.0);

    RiFrameBegin(0);
    /* set the perspective transformation */
    float fov = 45.0;
    RiProjection(RI_PERSPECTIVE, "fov", &fov);
    RiTranslate(0, 0, 5.0);

    RiWorldBegin();
    RiSphere(5.0, 5.0, 5.0, 2 * MY_PI);
    RiWorldEnd();
    RiFrameEnd();

    RiEnd();
}

void Earth()
{
    RiBegin(RI_NULL);
    //RiFormat(960, 720, 1.0);
    RiFormat(400, 300, 1.0);
    //RiFormat(200, 150, 1.0);
    RiDisplay("Earth.png", "", "");
    RiPixelSamples(2,2);

    RiFrameBegin(0);
        /* set the perspective transformation */
        float fov = 45.0;
        RiProjection(RI_PERSPECTIVE, "fov", &fov);
        //RiProjection(RI_ORTHOGRAPHIC);

        RiWorldBegin();
            RiTransformBegin();
                RiTranslate(0, 0, 5.0);
                RiRotate(-175, 0, 1, 0);
                RiRotate(110, 1, 0, 0);
                RiSphere(1, -1, 1, 360);
            RiTransformEnd();
        RiWorldEnd();

    RiFrameEnd();

    RiEnd();
}

void BasicScene1()
{
    RiBegin(RtToken("guzu.png"));

    RtPoint p1={30,0,10}; /* ball's initial position */
//    RtPoint p2={0,20,10}; /* ball's final position  */

    RiFormat(600, 600, 1.0);
    RiFrameAspectRatio(4.0/3.0);

    RiFrameBegin(0);
    /* set the perspective transformation */
        float fov = 45.0;
        RiProjection(RI_PERSPECTIVE, "fov", &fov);
        RiTranslate(0, -5, 60);
        RiRotate(-120, 1, 0, 0);
        RiRotate(25,0,0,1);

        RiWorldBegin();
            RiTransformBegin();
                RiTranslate(30, 0, 10);
                RiSphere(5, -5, 5, 360, RI_NULL);
            RiTransformEnd();
        RiWorldEnd();
    RiFrameEnd();

    RiEnd();
}

int main()
{
    // select which scene to render
//	SampleScene0();
//    BasicScene();
//    BasicScene1();
    Earth();


    // VIEW PROJECTION :

    // PERSPECTIVE PROJECTION :
    auto fov = 45.f;
    auto projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

    auto view = glm::lookAt(glm::vec3{0, 0, 0}, glm::vec3{0, 0, 1}, glm::vec3{0, 1, 0});

    // MODEL TRANSFORM :
    auto model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0, 0, 5.0));
    model = glm::rotate(model, -175.0f, glm::vec3(0, 1, 0));
    model = glm::rotate(model, 110.0f,  glm::vec3(1, 0, 0));
//    model = glm::translate(model, glm::vec3{0, 0, 5.0});
//    model = glm::rotate(model, glm::radians(-175.f), glm::vec3{0, 1, 0});
//    model = glm::rotate(model, glm::radians(110.f),  glm::vec3{1, 0, 0});

    auto MVP = projection * view * model;

    glm::vec3 point = {0, 0, 0};
    auto p = MVP * glm::vec4(point, 1.0f);
    p /= p.w;

    std::cout << p << '\n';

    glm::mat4 viewport_matrix = glm::mat4(1.0f);
    viewport_matrix[0][0] = 400 / 2.f;
    viewport_matrix[0][3] = 400 / 2.f;
    viewport_matrix[1][1] = 300 / 2.f;
    viewport_matrix[1][3] = 300 / 2.f;

    p = glm::transpose(viewport_matrix) * p;

    std::cout << viewport_matrix << '\n';
    std::cout << p << '\n';

    return 0;
}