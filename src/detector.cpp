#include "qr_detector/detector.h"

#include <cv_bridge/cv_bridge.h>

namespace qr_detector {

  Detector::Detector() : scanner_()
  {
    scanner_.set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 1);
  }

  Tags Detector::detect(const cv::Mat& image, size_t timeout)
  {
    cv::Mat grayImg;
    cv::cvtColor(image, grayImg, CV_BGR2GRAY);

    const auto width = image.cols;
    const auto height = image.rows;

    zbar::Image img(width, height, "Y800", grayImg.data, width * height);
    scanner_.scan(img);

    Tags tags;
    for (auto s = img.symbol_begin(); s != img.symbol_end(); ++s)
    {
      Tag tag;
      std::string message = formatMessage(s->get_data());
      tag.message = message;

      for(int i = 0; i < s->get_location_size(); i++) {
        tag.polygon.push_back(cv::Point(s->get_location_x(i), s->get_location_y(i)));
      }
      tags.push_back(tag);
    }

    return tags;
  }

  std::string Detector::formatMessage(const std::string& message) {
    std::string filteredString;
    for (char c : message) {
      if (std::isalnum(c) || std::isspace(c)) {
          filteredString += c;
      }
    }
    transform(filteredString.begin(), filteredString.end(), filteredString.begin(), ::tolower);
    return filteredString;
  }

}
