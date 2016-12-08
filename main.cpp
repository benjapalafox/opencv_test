/************************************* Include Files *********************************************/

//cv::cout
#include <iostream>

//Mat structure
#include <opencv2/core.hpp>

//cv::rectangle
#include <opencv2/imgproc.hpp>

//cv::imshow
//cv::waitKey
#include <opencv2/highgui.hpp>

//cv::<VideoCapture object>::open()
//cv::<VideoCapture object>::isOpen()
//cv::<VideoCapture object>::>> Mat obj
//cv::<VideoCapture object>::release()
#include <opencv2/videoio.hpp>  

/************************************* Constant Definitions **************************************/
#define BOX_UNACCEPTABLE  0
#define BOX_ACCEPTABLE    1

#define BOX_UNDEFINED     0
#define BOX_FIRST_CORNER  1
#define BOX_DEFINED       2
#define BOX_SELECTED      3

#define BOX_MIN_SIZE      24

using namespace std;
using namespace cv;

/************************************* Type Definitions ******************************************/
typedef struct
{
  char size;
  char state;
  int x;
  int y;
  int width;
  int height;
  Rect tld_box;
} box_t;

/************************************* Function Prototypes ***************************************/
static void selectBox(int event, int x, int y, int flags, void *userdata);

/************************************* Variable Definitions **************************************/
const Scalar WHITE = Scalar(255,255,255);
const Scalar PALETURQUOISE = Scalar(238,238,175);
const Scalar CYAN = Scalar(255,255,0);
const Scalar VIOLET = Scalar(238,130,238);
const Scalar MEDIUMSLATEBLUE = Scalar(238,104,123);
const Scalar DODGERBLUE = Scalar(255,144,30);
const Scalar MAGENTA = Scalar(255,0,255);
const Scalar DARKVIOLET = Scalar(211,0,148);
const Scalar BLUE = Scalar(255,0,0);
const Scalar KHAKI = Scalar(140,230,240);
const Scalar PALEGREEN = Scalar(152,251,152);
const Scalar SPRINGGREEN = Scalar(127,255,0);
const Scalar LIGHTCORAL = Scalar(128,128,240);
const Scalar GRAY = Scalar(128,128,128);
const Scalar TEAL = Scalar(128,128,0);
const Scalar DEEPPINK = Scalar(147,20,255);
const Scalar PURPLE = Scalar(128,0,128);
const Scalar NAVY = Scalar(128,0,0);
const Scalar YELLOW = Scalar(0,255,255);
const Scalar CHARTREUSE = Scalar(0,255,128);
const Scalar LIME = Scalar(0,255,0);
const Scalar DARKORANGE = Scalar(0,140,255);
const Scalar OLIVE = Scalar(0,128,128);
const Scalar GREEN = Scalar(0,128,0);
const Scalar RED = Scalar(0,0,255);
const Scalar MAROON = Scalar(0,0,128);
const Scalar BLACK = Scalar(0,0,0);

/************************************* Function Declarations *************************************/
int main(int argc, char **argv)
{
  box_t box;
  unsigned long nsec;
  unsigned long nsec_expected;
  unsigned long i;
  unsigned long j;
  struct timespec t_start;
  struct timespec t_end;
  VideoCapture cap;
  Mat frame;
  Mat gray;
  Mat subimage;
  
  cap.open(0);
  
  if(!cap.isOpened())
  {
    return -1;
  }
  
  nsec_expected = 1000000000 / cap.get(CAP_PROP_FPS);
  cout << "width: " << cap.get(CAP_PROP_FRAME_WIDTH) << " heigh: " << cap.get(CAP_PROP_FRAME_HEIGHT) << " fps: " << cap.get(CAP_PROP_FPS) << endl;
  
  box.state = BOX_UNDEFINED;
  
  namedWindow("Webcam", 0);
  setMouseCallback("Webcam", selectBox, &box);
  for(i = 0; i < 500; i++)
  {
    timespec_get(&t_start, TIME_UTC);
    /*------------------------------------------------------------------------------------------*/
    //Get frame
    cap >> frame;
    //Select box to track
    if((box.state == BOX_FIRST_CORNER) || (box.state == BOX_DEFINED))
    {
      if(box.size == BOX_UNACCEPTABLE)
      {
        rectangle(frame, box.tld_box, RED, 2, LINE_8, 0);
      }
      else
      {
        rectangle(frame, box.tld_box, GREEN, 2, LINE_8, 0);
      }
    }
    //Box selected, tracking
    else if(box.state == BOX_SELECTED)
    {
      cvtColor(frame, gray, COLOR_BGR2GRAY, 0);
      
      
      
      
      
      rectangle(frame, box.tld_box, BLUE, 2, LINE_8, 0);
      subimage = frame(box.tld_box);
    }
    if(box.state != BOX_SELECTED)
    imshow("Webcam", frame); //Last thing to do is show frame
    else
    imshow("Webcam", subimage);
    /*------------------------------------------------------------------------------------------*/
    
//    timespec_get(&t_end, TIME_UTC);
//    nsec = (t_end.tv_sec - t_start.tv_sec) * 1000000000 + t_end.tv_nsec - t_start.tv_nsec;
//    cout << (double) nsec / (double) nsec_expected << ",";
    
    if(waitKey(30) == 'q')
    {
      break;
    }
    
//    timespec_get(&t_end, TIME_UTC);
//    nsec = (t_end.tv_sec - t_start.tv_sec) * 1000000000 + t_end.tv_nsec - t_start.tv_nsec;
//    cout << (double) nsec / (double) nsec_expected << endl;
  }
  
  cap.release();
  
  return 0;
}

/*************************************************************************************************/
static void selectBox(int event, int x, int y, int flags, void *userdata)
{
  int tmp;
  box_t *box = (box_t *) userdata;
  Rect *box_rect = &box->tld_box;
  
  if((event == EVENT_MOUSEMOVE) && (box->state == BOX_FIRST_CORNER))
  {
    if((((x + BOX_MIN_SIZE ) <= box->x) || ((box->x + BOX_MIN_SIZE) <= x)) && (((y + BOX_MIN_SIZE ) <= box->y) || ((box->y + BOX_MIN_SIZE) <= y)))
    {
      box->size = BOX_ACCEPTABLE;
    }
    else
    {
      box->size = BOX_UNACCEPTABLE;
    }
    if(x < box->x)
    {
      box_rect->x = x;
      box_rect->width = box->x - x;
    }
    else
    {
      box_rect->x = box->x;
      box_rect->width = x - box->x;
    }
    
    if(y < box->y)
    {
      box_rect->y = y;
      box_rect->height = box->y - y;
    }
    else
    {
      box_rect->y = box->y;
      box_rect->height = y - box->y;
    }
  }
  else if((event == EVENT_MOUSEMOVE) && (box->state == BOX_DEFINED))
  {
    box_rect->x = box->x;
    box_rect->y = box->y;
    box_rect->width = box->width;
    box_rect->height = box->height;
  }
  
  if((event == EVENT_LBUTTONDOWN) && (box->state == BOX_UNDEFINED))
  {
    box->x = x;
    box->y = y;
    box->state = BOX_FIRST_CORNER;
  }
  else if((event == EVENT_LBUTTONUP) && (box->state == BOX_FIRST_CORNER))
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
        box->height = tmp - y;
      }
      else
      {
        box->height = y - box->y;
      }
      box->state = BOX_DEFINED;
    }
    else
    {
      box_rect->x = 0;
      box_rect->y = 0;
      box_rect->width = 0;
      box_rect->height = 0;
      box->state = BOX_UNDEFINED;
    }
  }
  else if((event == EVENT_LBUTTONDOWN) && (box->state == BOX_DEFINED))
  {
    if(((x > box->x) && (x < (box->x + box->width))) && ((y > box->y) && (y < (box->y + box->height))))
    {
      box->state = BOX_SELECTED;
    }
    else
    {
      box_rect->x = 0;
      box_rect->y = 0;
      box_rect->width = 0;
      box_rect->height = 0;
      box->state = BOX_UNDEFINED;
    }
  }
}






















