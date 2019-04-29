#include <iostream>
#include <vector>
#include <string>
#include "Ri.h"
#include "glm_ostream.hpp"
#include "shaders.hpp"

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

    RtColor red={1,0,0};
    RtColor green={0,1,0};
    RtColor blue={0,0,1};

    RiFrameBegin(0);
        /* set the perspective transformation */
        float fov = 45.0;
        RiProjection(RI_PERSPECTIVE, "fov", &fov);
        //RiProjection(RI_ORTHOGRAPHIC);

        RiWorldBegin();
            RiColor(blue);
            RiSurface(nullptr);
            RiTransformBegin();
                RiTranslate(0, 0, 5.0);
                RiRotate(-175, 0, 1, 0);
                RiRotate(110, 1, 0, 0);
                RiSphere(1, -1, 1, 360);
            RiTransformEnd();
//
//            RiColor(red);
//            RiTransformBegin();
//                RiTranslate(0, 0, 8.0);
//                RiRotate(-175, 0, 1, 0);
//                RiRotate(110, 1, 0, 0);
//                RiSphere(0.5, -1, 1, 360);
//            RiTransformEnd();
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

    return 0;
}