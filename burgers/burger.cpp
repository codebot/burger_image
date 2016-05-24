#include "burger.h"
#include "b64/decode.h"
#include <cstring>
#include <stdio.h>
using namespace burger; // i've always wanted to write that

static const char *burger_cstr;

Burger::Burger()
{
  base64::decoder D;
  size_t burger_size = strlen(BURGER);
  std::vector<char> burger_png;
  burger_png.resize(burger_size);
  D.decode(BURGER, strlen(BURGER), burger_png.data());
  burger_template = cv::imdecode(burger_png, CV_LOAD_IMAGE_COLOR);
  srand(time(NULL));
}

cv::Mat &Burger::render_burger(int width, int height)
{
  if (burger_buf.size().width != width || burger_buf.size().height != height)
  {
    int num_burgers = rand() % 10 + 2;
    x.resize(num_burgers);
    y.resize(num_burgers);
    x_inc.resize(num_burgers);
    y_inc.resize(num_burgers);
    for (int b = 0; b < num_burgers; b++)
    {
      x[b] = rand() % (width  - burger_template.size().width - 1);
      y[b] = rand() % (height - burger_template.size().height - 1);
      x_inc[b] = rand() % 3 + 1;
      y_inc[b] = rand() % 3 + 1;
    }
    burger_buf = cv::Mat(height, width, CV_8UC3);
    printf("creating %dx%d buffer\n", width, height);
  }
  burger_buf = cvScalar(0,0,0);
  for (int b = 0; b < (int)x.size(); b++)
  {
    burger_template.copyTo(burger_buf(cv::Rect(x[b], y[b], burger_template.size().height, burger_template.size().width)));
    x[b] += x_inc[b];
    y[b] += y_inc[b];
    // bounce as needed
    if (x[b] < 0 || x[b] > width - burger_template.size().width - 1)
    {
      x_inc[b] *= -1;
      x[b] += 2 * x_inc[b];
    }
    if (y[b] < 0 || y[b] > height - burger_template.size().height - 1)
    {
      y_inc[b] *= -1;
      y[b] += 2 * y_inc[b];
    }
  }
  return burger_buf;
}

