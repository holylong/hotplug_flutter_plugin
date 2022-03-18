#include <usb_tracer.h>
#include <thread>
#include <hiddev_detector.h>

using namespace std;

void IdleThread(void* obj){
  UsbTracer* idle = (UsbTracer*)obj;
  idle->createDetector();//必须在这里创建wnd否则，postmessage 发送不过来
  idle->Idle();
}

UsbTracer::UsbTracer():_callback(NULL){}

UsbTracer* UsbTracer::createTracer()
{
  return new UsbTracer;
}

void UsbTracer::Idle()
{
  _tracker->Idle();
}

TracerStatus UsbTracer::createDetector()
{
_tracker = new InnerUsbDetector();
	_tracker->selfRegisterClass();
	_tracker->createMessageNoWnd();
	_tracker->registerDevice();
  if(_callback == NULL){
    return TracerStatus_Callback_NotSet;
  }
  _tracker->registerChangeFunc(_callback);
  return TracerStatus_CreateDetectorOK;
}

TracerStatus UsbTracer::Init()
{
  _entry = thread(IdleThread, this);
  return TracerStatus_InitOK;
}

void UsbTracer::exitTracer()
{
  PostMessage(_tracker->getCurrentWnd(), WM_QUIT, NULL, NULL);
  _entry.join();
}
    