#ifndef HID_DEV_DETECTOR_H_
#define HID_DEV_DETECTOR_H_
#include <iostream>
#include <Windows.h>
// #include <devcommon.h>
#include <functional>

class InnerUsbDetector {
public:
	typedef std::function<void(int, std::string, std::string)> DevChangeEventFunction;
	InnerUsbDetector();

	static void InitialInst();

	void startService();

	bool createMessageNoWnd();

	// void registerDeviceChandeCallback(DeviceChangeCallback, void*);

	bool registerDevice();

	void Idle();

	void display();


	ATOM selfRegisterClass();
	HWND getCurrentWnd() { return m_hwnd; }
	void SelfProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void UsbDeviceChange(WPARAM wParam, LPARAM lParam);


	void registerChangeFunc(const DevChangeEventFunction& callback) { m_devFunction = callback; }

	void WideCharToString(std::string& szDst, wchar_t* wchar);

private:

	HWND m_hwnd;

	int m_index;
	LPWSTR m_className;

	DevChangeEventFunction m_devFunction;

private:
	// DeviceChangeCallback m_devCallback;
	void* m_handle;
};

#endif //HID_DEV_DETECTOR_H_