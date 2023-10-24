#include "burger.h"
#include <cstring>
#include <stdio.h>
using namespace burger; // i've always wanted to write that

static const char *burger_cstr;

// here lies the world's slowest portable base64 decoder
void decode_base64(const char *cstr, std::vector<uint8_t> &out)
{
  int len = (int)strlen(cstr);
  if (len < 2)
    return; // would have to think too hard about trivial inputs
  out.resize(len * 3 / 4); // deal with padding bytes later
  uint8_t base64_map[256] = {0};
  for (int i = 'A'; i <= 'Z'; i++)
    base64_map[i] = i - 'A';
  for (int i = 'a'; i <= 'z'; i++)
    base64_map[i] = i - 'a' + 26;
  for (int i = '0'; i <= '9'; i++)
    base64_map[i] = i - '0' + 52;
  base64_map['+'] = 62;
  base64_map['/'] = 63;
  int ridx = 0, widx = 0;
  uint32_t block;
  for (ridx = 0; ridx < len; ridx += 4)
  {
    // we expand each group of 4 code bytes into 3 output bytes
    uint32_t block = 0;
    block = (base64_map[cstr[ridx  ]] << 18) |
            (base64_map[cstr[ridx+1]] << 12) |
            (base64_map[cstr[ridx+2]] <<  6) |
            (base64_map[cstr[ridx+3]] <<  0) ;
    out[widx++] = (block >> 16) & 0xff;
    out[widx++] = (block >>  8) & 0xff;
    out[widx++] = (block >>  0) & 0xff;
  }
  // fix padding now. (these branches are untested so they're probably wrong)
  if (cstr[len-1] == '=' && cstr[len-2] == '=')
  {
    // there were two padding bytes. remove the two last output bytes
    out.pop_back();
    out.pop_back();
  }
  else if (cstr[len-1] == '=')
  {
    // there was only one padding byte. remove the last output byte.
    out.pop_back();
  }
}

Burger::Burger()
{
  size_t burger_size = strlen(BURGER);
  std::vector<uint8_t> burger_png;
  burger_png.resize(burger_size);
  decode_base64(BURGER, burger_png);
  burger_template = cv::imdecode(burger_png, cv::IMREAD_COLOR);
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
  burger_buf = cv::Scalar(0,0,0);
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

