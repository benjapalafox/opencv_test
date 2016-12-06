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

#define BOX_UNDEFINED     0
#define BOX_FIRST_CORNER  1
#define BOX_DEFINED       2
#define BOX_MIN_SIZE      24

using namespace std;
using namespace cv;

typedef struct
{
  char state;
  int x;
  int y;
  int width;
  int height;
  int max_x;
  int max_y;
} box_t;

static void selectBox(int event, int x, int y, int flags, void *userdata);

int main(int argc, char **argv)
{
  box_t box;
  unsigned long nsec;
  unsigned long nsec_expected;
  struct timespec t_start;
  struct timespec t_end;
  VideoCapture cap;
  Mat frame;
  Mat gray;
  
  cap.open(0);
  
  if(!cap.isOpened())
  {
    return -1;
  }
  
  nsec_expected = 1000000000 / cap.get(CAP_PROP_FPS);
  box.max_x = cap.get(CAP_PROP_FRAME_WIDTH);
  box.max_y = cap.get(CAP_PROP_FRAME_HEIGHT);
  cout << "width: " << cap.get(CAP_PROP_FRAME_WIDTH) << " heigh: " << cap.get(CAP_PROP_FRAME_HEIGHT) << " fps: " << cap.get(CAP_PROP_FPS) << endl;
  
  box.state = BOX_UNDEFINED;
  
  namedWindow("Webcam", 0);
  setMouseCallback("Webcam", selectBox, &box);
  for(;;)
  {
    timespec_get(&t_start, TIME_UTC);
    /*------------------------------------------------------------------------------------------*/
    cap >> frame; //new frame
    imshow("Webcam", frame);
    
    if(box.state == BOX_DEFINED) // box set
    {
      cout << "(x1, y1) = (" << box.x << ", " << box.y << ")" << " (x2, y2) = (" << box.x + box.width << ", " << box.y + box.height << ")" << endl;
      box.state = BOX_UNDEFINED;
    }
    
    
    
    /*------------------------------------------------------------------------------------------*/
    timespec_get(&t_end, TIME_UTC);
    
    nsec = (t_end.tv_sec - t_start.tv_sec) * 1000000000 + t_end.tv_nsec - t_start.tv_nsec;
    
    //cout << "Time per frame ratio: " << (double) nsec / (double) nsec_expected << ". Less than 1 is better." << endl;
    
    if(waitKey(30) == 'q')
    {
      break;
    }
  }
  
  cap.release();
  
  return 0;
}

static void selectBox(int event, int x, int y, int flags, void *userdata)
{
  int tmp;
  box_t *box = (box_t *) userdata;
  
  if((event == EVENT_LBUTTONDOWN) && (box->state == BOX_UNDEFINED))
  {
    box->x = x;
    box->y = y;
    box->state = BOX_FIRST_CORNER;
  }
  else if((event == EVENT_LBUTTONUP) && (box->state == BOX_FIRST_CORNER))
  {
    if((x != box->x) && (y != box->y))
    {
      if((((x + BOX_MIN_SIZE ) <= box->x) || ((box->x + BOX_MIN_SIZE) <= x)) && (((y + BOX_MIN_SIZE ) <= box->y) || ((box->y + BOX_MIN_SIZE) <= y)))
      {
        if(x < box->x)
        {
          tmp = box->x;
          box->x = x;
          box->width = tmp - x;
        }
        else
        {
          box->width = x - box->x;
        }
        
        if(y < box->y)
        {
          tmp = box->y;
          box->y = y;
          box->width = tmp - y;
        }
        else
        {
          box->height = y - box->y;
        }
        box->state = BOX_DEFINED;
      }
      else
      {
        cout << "Box too small" << endl;
        box->state = BOX_UNDEFINED;
      }
    }
    else
    {
      cout << "There is no box" << endl;
      box->state = BOX_UNDEFINED;
    }
  }
}






















