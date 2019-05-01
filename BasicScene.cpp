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
    RiTranslate(0, 0, 10.0);

    RiWorldBegin();
//    RiSphere(5.0, -5.0, 5.0, 360);
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
    RtColor gray={0.5,0.5,0.5};

    float var = 0;
    int key = 0;
    while (key != 27) {
        RiFrameBegin(0);
        /* set the perspective transformation */
        float fov = 45.0;
        RiProjection(RI_PERSPECTIVE, "fov", &fov);
        //RiProjection(RI_ORTHOGRAPHIC);

        RiWorldBegin();
            RiColor(gray);
            RiTransformBegin();
                RiSurface(PHONG);
                RiDisplacement(BUMPY);
                RiTranslate(0, 0, 5.0);
                RiRotate(-175, 0, 1, 0);
                RiRotate(110, 1, 0, 0);
                RiSphere(1, -1, 1, 360);
            RiTransformEnd();
            RiWorldEnd();
        RiFrameEnd();

        key = cv::waitKey(0);
        if (key == 'a')
        {
            var -= 1;
        }
        else if (key == 'd')
        {
            var += 1;
        }
    }
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

void Cone()
{
    RiBegin(RI_NULL);
    RiFormat(960, 720, 1.0);
//    RiFormat(400, 300, 1.0);
    RiDisplay("Tunnel.png", "", "");
    RiPixelSamples(2,2);

    RiFrameBegin(0);
            /* set the perspective transformation */
        float fov = 45.0;
        RiProjection(RI_PERSPECTIVE, "fov", &fov);
        //RiProjection(RI_ORTHOGRAPHIC);
        RiWorldBegin();
            RiTransformBegin();
                RtColor color = {1,0,0};
                RiColor(color);
                RiSurface(NORMAL);
                RiTranslate(0, -0.5, 7.0);
                RiRotate(60, 1, 0, 0);
                RiTorus(1, .25, 0, 360, 360);
            RiTransformEnd();
//            RiTransformBegin();
//                color[0] = 0; color[1] = 1;
//                RiSurface(NORMAL);
//                RiColor(color);
//                RiTranslate(0, 0, 8.0);
//                RiRotate(60, 1, 0, 0);
//                RiRotate(30, 0, 1, 0);
//                RiCylinder(1, -1, 1, 360);
//            RiTransformEnd();
            RiTransformBegin();
                color[1] = 0; color[2] = 1;
                RiColor(color);
//                RiSurface(PHONG);
                RiTranslate(0, 1, 9.0);
                RiRotate(60, 1, 0, 0);
                RiCylinder(1, -1, 1, 360);
            RiTransformEnd();
            RiTransformBegin();
                color[0] = 1; color[1] = .4; color[2] = .4;
                RiColor(color);
//                CHECK_SIZE_X = 40;
//                CHECK_SIZE_Y = 40;
//                RiSurface(PHONG);
                RiTranslate(0, -1, 8.5);
                RiRotate(-160, 1, 0, 0);
                RiRotate(30, 0, 1, 0);
                RiCone(2, 1, 360);
            RiTransformEnd();
//            RiTransformBegin();
//                CHECK_SIZE_X = 40;
//                CHECK_SIZE_Y = 40;
//                RiTranslate(0, 0, 7.0);
//                RiRotate(-160, 1, 0, 0);
//                RiRotate(30, 0, 1, 0);
//                RiCone(2, 1, 360);
//            RiTransformEnd();
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
//    Earth();
    Cone();

    return 0;
}
