// RoboSubAIApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//                    
//
//  Author: Dillon Wall
//  dillon.wall@oit.edu

#include <iostream>
#include "../RoboSubAI/RoboSubAILib/AI.h"

int main()
{
    std::cout << "RoboSub AI v2.07\n";

    AI* ai = AI::GetInstance();

    ai->Init("../TensorFlow Graph/detect.tflite");
    ai->ProcessImage("test1.jpg");
    ai->Finalize();

    return 0;
}