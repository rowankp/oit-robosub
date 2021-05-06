#include "pch.h"
#include "AI.h"
#include <exception>
#include <iostream>

using std::cout;

AI* AI::GetInstance()
{
	if (!instance)
		instance = new AI();

	return instance;
}

void AI::Init(string model_filename)
{	
	// Create model
	m_model = Model(model_filename);
    // Initialize the variables...
    m_model.init();
    // ... or restore from checkpoint
    //m_model.restore("train.ckpt")
}

void AI::Finalize()
{

}

// Path to image, .png
void AI::ProcessImage(string imageFileName)
{
    Tensor outNames1{ m_model, "num_detections" };
    Tensor outNames2{ m_model, "detection_scores" };
    Tensor outNames3{ m_model, "detection_boxes" };
    Tensor outNames4{ m_model, "detection_classes" };

    Tensor inpName{ m_model, "image_tensor" };


    // Read image
    cv::Mat img, inp;
    img = cv::imread(imageFileName, cv::IMREAD_COLOR);

    int rows = img.rows;
    int cols = img.cols;

    cv::resize(img, inp, cv::Size(300, 300));
    cv::cvtColor(inp, inp, cv::COLOR_BGR2RGB);

    // Put image in Tensor
    std::vector<uint8_t > img_data;
    img_data.assign(inp.data, inp.data + inp.total() * inp.channels());
    inpName.set_data(img_data, { 1, 300, 300, 3 });

    m_model.run(inpName, { &outNames1, &outNames2, &outNames3, &outNames4 });

    // Visualize detected bounding boxes.
    int num_detections = (int)outNames1.get_data<float>()[0];
    for (int i = 0; i < num_detections; i++) {
        int classId = (int)outNames4.get_data<float>()[i];
        float score = outNames2.get_data<float>()[i];
        auto bbox_data = outNames3.get_data<float>();
        std::vector<float> bbox = { bbox_data[i * 4], bbox_data[i * 4 + 1], bbox_data[i * 4 + 2], bbox_data[i * 4 + 3] };
        if (score > 0.3) {
            float x = bbox[1] * cols;
            float y = bbox[0] * rows;
            float right = bbox[3] * cols;
            float bottom = bbox[2] * rows;

            cv::rectangle(img, { (int)x, (int)y }, { (int)right, (int)bottom }, { 125, 255, 51 }, 2);
        }
    }

    cv::imshow("Image", img);
    cv::waitKey(0);
}
