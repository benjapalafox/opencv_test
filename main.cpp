//cv::cout
#include <iostream>

//Mat structure
#include <opencv2/core.hpp>

//cv::imshow
//cv::waitKey
#include <opencv2/highgui.hpp>

//cv::<VideoCapture object>::open()
//cv::<VideoCapture object>::isOpen()
//cv::<VideoCapture object>::>> Mat obj
//cv::<VideoCapture object>::release()
#include <opencv2/videoio.hpp>  

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
  unsigned long nsec;
  unsigned long nsec_expected;
  struct timespec t_start;
  struct timespec t_end;
  VideoCapture cap;
  Mat frame;
  
  cap.open(0);
  
  if(!cap.isOpened())
  {
    return -1;
  }
  
  nsec_expected = 1000000000 / cap.get(CAP_PROP_FPS);
  cout << "width: " << cap.get(CAP_PROP_FRAME_WIDTH) << " heigh: " << cap.get(CAP_PROP_FRAME_HEIGHT) << " fps: " << cap.get(CAP_PROP_FPS) << endl;
  
  namedWindow("Video", 1);
  for(;;)
  {
    timespec_get(&t_start, TIME_UTC);
    /*------------------------------------------------------------------------------------------*/
    cap >> frame; //new frame
    imshow("Webcam", frame);
    
    /*------------------------------------------------------------------------------------------*/
    timespec_get(&t_end, TIME_UTC);
    
    nsec = (t_end.tv_sec - t_start.tv_sec) * 1000000000 + t_end.tv_nsec - t_start.tv_nsec;
    
    cout << "Time per frame ratio: " << (double) nsec / (double) nsec_expected << ". Less than 1 is better." << endl;
    
    if(waitKey(30) == 'q')
    {
      break;
    }
  }
  
  cap.release();
  
  return 0;
}
