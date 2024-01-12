#include "snapdecision/dnn.h"
#if 0
#include <onnxruntime_c_api.h>
#include <onnxruntime_cxx_api.h>

#include <QImage>
#include <QPixmap>
#include <QRectF>
#include <QVector>
#include <cstdint>
#include <functional>
#include <iostream>
#include <mutex>
#include <utility>
#include <vector>

using Coord = std::pair<float, float>;
using ImageMapping = std::function<Coord(Coord)>;

std::tuple<std::vector<unsigned char>, ImageMapping, int64_t, int64_t> processImage(const QPixmap& pixmap,
                                                                                    int targetWidth)
{
  // Step 1: Scale QPixmap
  double aspectRatio = static_cast<double>(pixmap.height()) / pixmap.width();
  int targetHeight = static_cast<int>(targetWidth * aspectRatio);
  QPixmap scaledPixmap = pixmap.scaled(targetWidth, targetHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);

  // Step 2: Convert QPixmap to std::vector<unsigned char>
  QImage scaledImage = scaledPixmap.toImage().convertToFormat(QImage::Format_RGB888);
  std::vector<unsigned char> pixelData;
  pixelData.reserve(scaledImage.sizeInBytes());

  for (int y = 0; y < scaledImage.height(); ++y)
  {
    for (int x = 0; x < scaledImage.width(); ++x)
    {
      QRgb pixel = scaledImage.pixel(x, y);
      pixelData.push_back(qRed(pixel));
      pixelData.push_back(qGreen(pixel));
      pixelData.push_back(qBlue(pixel));
    }
  }

  // Step 3: Create ImageMapping Function
  double scaleX = static_cast<double>(scaledImage.width()) / pixmap.width();
  double scaleY = static_cast<double>(scaledImage.height()) / pixmap.height();
  ImageMapping mappingFunction = [scaleX, scaleY](Coord scaledCoord) -> Coord
  {
    double originalX = scaledCoord.first / scaleX;
    double originalY = scaledCoord.second / scaleY;
    return { originalX, originalY };
  };

  return { pixelData, mappingFunction, targetWidth, targetHeight };
}

struct EfficientdetD0
{
  Ort::Env env;
  Ort::SessionOptions session_options;
  const wchar_t* model;
  Ort::Session session;
  Ort::MemoryInfo memory_info;

  EfficientdetD0()
    : env(Ort::Env(ORT_LOGGING_LEVEL_WARNING, "snap-decision"))
    , model(L"D:/programming/SnapDecision/efficientdet_d0.onnx")
    , session(Ort::Session(env, model, session_options))
    , memory_info(Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault))
  {
    // Get the number of input nodes
    size_t num_input_nodes = session.GetInputCount();
    for (std::size_t i = 0; i < num_input_nodes; i++)
    {
      // Get the name of the ith input node
      auto input_name = session.GetInputNameAllocated(i, Ort::AllocatorWithDefaultOptions());
      std::cout << i << "/" << num_input_nodes << " " << input_name.get() << std::endl;
    }

    // Get the number of output nodes
    size_t num_output_nodes = session.GetOutputCount();
    for (std::size_t i = 0; i < num_output_nodes; i++)
    {
      auto output_name = session.GetOutputNameAllocated(i, Ort::AllocatorWithDefaultOptions());
      std::cout << i << "/" << num_output_nodes << " " << output_name.get() << std::endl;
    }
  }

  QVector<QRectF> getObjectBoxes(const QPixmap& pixmap, float threshold)
  {
    const int64_t width = pixmap.width();
    const int64_t height = pixmap.height();

    auto [model_input, inv_func, target_width, target_height] = processImage(pixmap, 600);

    std::vector<int64_t> tensor_shape = { 1, target_height, target_width, 3 };

    // std::vector<unsigned char> model_input = pixmapToVector(pixmap);

    Ort::Value input_tensor = Ort::Value::CreateTensor<unsigned char>(
        memory_info, model_input.data(), model_input.size(), tensor_shape.data(), tensor_shape.size());

    const char* input_names[] = { "input_tensor" };
    const char* output_names[] = { "num_detections", "detection_boxes", "detection_classes", "detection_scores" };

    // const char* output_names[] = { "num_detections",           "detection_boxes",
    //                               "detection_classes",        "detection_scores",
    //                               "raw_detection_boxes",      "raw_detection_scores",
    //                               "detection_anchor_indices", "detection_multiclass_scores" };
    auto output_tensors =
        session.Run(Ort::RunOptions{ nullptr }, input_names, (const Ort::Value*)&input_tensor, 1, output_names, 4);

    auto* num_detections = output_tensors[0].GetTensorData<float>();
    std::size_t num_boxes = static_cast<std::size_t>(num_detections[0]);

    const float* detection_boxes = output_tensors[1].GetTensorData<float>();

    const float* detection_classes = output_tensors[2].GetTensorData<float>();

    const float* detection_scores = output_tensors[3].GetTensorData<float>();

    QVector<QRectF> boxes;

    const size_t num_coords = 4;  // 4 coordinates per box

    for (size_t i = 0; i < num_boxes; ++i)
    {
      float ymin = detection_boxes[i * num_coords + 0];
      float xmin = detection_boxes[i * num_coords + 1];
      float ymax = detection_boxes[i * num_coords + 2];
      float xmax = detection_boxes[i * num_coords + 3];

      std::tie(xmin, xmax) = inv_func({ xmin, xmax });
      std::tie(ymin, ymax) = inv_func({ ymin, ymax });

      // Convert to QRectF and scale to original image dimensions if necessary
      QRectF box(xmin * width, ymin * height, (xmax - xmin) * width, (ymax - ymin) * height);

      if (detection_scores[i] > threshold)
      {
        boxes.push_back(box);
      }

      std::cout << i << "/" << num_boxes << " " << detection_classes[i] << ":" << detection_scores[i] << std::endl;
    }

    return boxes;
  }
};

DNN::DNN()
{
}

DNN::~DNN()
{
}

std::function<void(double&)> DNN::initializeWorkerFunction()
{
  std::cout << "init wrkr function" << std::endl;
  auto shared_this = shared_from_this();
  return [shared_this](double& progress)
  {
    progress = 0;
    std::cout << "DNN init started" << std::endl;

    shared_this->setEfficientdetD0(std::make_unique<EfficientdetD0>());
    std::cout << "DNN init complete" << std::endl;
    progress = 1;
  };
}

void DNN::setEfficientdetD0(std::unique_ptr<EfficientdetD0>&& efficientdet_d0)
{
  std::lock_guard lock(mutex_);
  efficientdet_d0_ = std::move(efficientdet_d0);
}
#endif
