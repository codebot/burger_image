#include "burger.h"
#include <opencv2/opencv.hpp>
using namespace cv;

int main(int argc, char** argv )
{
  burger::Burger b;
  namedWindow("Display Image", WINDOW_AUTOSIZE );

  while (true)
  {
    imshow("Display Image", b.render_burger(640, 480));
    waitKey(10);
  }

  return 0;  

}

