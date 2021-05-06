#pragma once

#include <vector>
#include <string>

using std::vector;
using std::string;

class AbstractTask
{
public:
	AbstractTask();

	bool IsDone();
	virtual void Update(vector<double[4]> boundingBoxes, vector<string> classifications, vector<double> accuracies) = 0;

private:
	bool m_done;

};

