#ifndef _USB_TRACER_H_
#define _USB_TRACER_H_

#include <thread>
#include <functional>

typedef enum {
  TracerStatus_RegsiterCallbackOK = 0,
  TracerStatus_RegsiterCallbackFailed,
  TracerStatus_Callback_NotSet,
  TracerStatus_InitOK,
  TracerStatus_CreateDetectorOK,
}TracerStatus;

class InnerUsbDetector;

class UsbTracer{
  public:
    typedef std::function<void(int, std::string, std::string)> DevChangeEventFunction;
    static UsbTracer* createTracer();

    UsbTracer();

    TracerStatus Init();
    TracerStatus createDetector();
    TracerStatus registerCallback(DevChangeEventFunction callback){_callback = callback; return TracerStatus_RegsiterCallbackOK;}

    //void OnDevChangeCallback
  public:
    void exitTracer();
    void Idle();

  private:
    std::thread _entry;
    DevChangeEventFunction _callback;
    InnerUsbDetector* _tracker;
};

#endif //_USB_TRACER_H_