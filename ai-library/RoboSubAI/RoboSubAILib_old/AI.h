#pragma once

/*
*	AI.h: This Singleton class manages the functions of the AI. It includes a queue of tasks to manage, 
*			and will handle updates and image processes which connects to the Python script
*			from TensorFlow.
*
*
*	Author: Dillon Wall
*	dillon.wall@oit.edu
*
*/

#include <vector>
#include <string>
#include "Model.h"
#include "Tensor.h"
#include <opencv2/opencv.hpp>
#include <algorithm>
#include <iterator>

using std::string;
using std::vector;

class AI
{
public:
	static AI* GetInstance();

	void Init(string model_filename);
	void Finalize();
	void ProcessImage(string imageFileName);
	void Update();
private:
	static AI* instance;

	int m_numDetections;
	vector<double[4]> m_boundingBoxes;
	vector<string> m_classifications;
	vector<double> m_accuracies;

	Model m_model;

	AI();
};

