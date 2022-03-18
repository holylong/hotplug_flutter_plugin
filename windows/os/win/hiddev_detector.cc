#include <hiddev_detector.h>
#include <Dbt.h>
#include <strsafe.h>

//#include <huiting_hid.h>
// #include <usbtracker.h>
#include <thread>
#include <util.h>

using namespace std;

//HuitingHidManager huitingHandler;
/////////////////////////////////////////////////////////////////////
//usb 拔插相关代码
bool InnerUsbDetector::createMessageNoWnd()
{
	m_hwnd = CreateWindowEx(0, m_className, L"", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,       // Parent window    
		NULL,       // Menu
		GetModuleHandle(NULL),  // Instance handle
		(LPVOID)this        // Additional application data
		);

	return m_hwnd != NULL;
}

InnerUsbDetector::InnerUsbDetector():
m_hwnd(NULL),
m_index(0),
m_className(L"hiddevwnd"),
m_handle(NULL), 
m_devFunction(NULL){

}

// void InnerUsbDetector::registerDeviceChandeCallback(DeviceChangeCallback callback, void* handle)
// {
// 	m_devCallback = callback;
// 	m_handle = handle;
// }

void InnerUsbDetector::InitialInst() {

}

void InnerUsbDetector::startService() {

}

void InnerUsbDetector::WideCharToString(std::string& szDst, wchar_t* wchar)
{
	wchar_t* wText = wchar;
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, NULL, 0, NULL, FALSE);
	char* psText; 
	psText = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, psText, dwNum, NULL, FALSE);
	szDst = psText;
	delete[]psText;
}

void InnerUsbDetector::UsbDeviceChange(WPARAM wParam, LPARAM lParam)
{
	if (DBT_DEVICEREMOVECOMPLETE == wParam)
	{
		PDEV_BROADCAST_HDR pHdr = (PDEV_BROADCAST_HDR)lParam;
		if (DBT_DEVTYP_DEVICEINTERFACE == pHdr->dbch_devicetype)
		{
			PDEV_BROADCAST_DEVICEINTERFACE pDevInf = (PDEV_BROADCAST_DEVICEINTERFACE)pHdr;
			string dbccName;
			WideCharToString(dbccName, pDevInf->dbcc_name);
			cout << "usb remove " << dbccName.c_str() << endl;
			if (dbccName.length() > 0 && m_devFunction != NULL)
			{
				vector<string> arrdevs;
				hotplug::split(dbccName, arrdevs, '#');
				if(arrdevs.size() > 2){
					string usb = arrdevs[1];
					hotplug::split(usb, arrdevs, '&');
					m_devFunction(0, arrdevs[0], arrdevs[1]);
				}
			}
		}
	}
	else if (DBT_DEVICEARRIVAL == wParam)
	{
		PDEV_BROADCAST_HDR pHdr = (PDEV_BROADCAST_HDR)lParam;
		if (DBT_DEVTYP_DEVICEINTERFACE == pHdr->dbch_devicetype)
		{
			PDEV_BROADCAST_DEVICEINTERFACE pDevInf = (PDEV_BROADCAST_DEVICEINTERFACE)pHdr;
			string dbccName;
			WideCharToString(dbccName, pDevInf->dbcc_name);
			cout << "usb arrival " << dbccName.c_str() << endl;
			if (dbccName.length() > 0 && m_devFunction != 0)
			{
				vector<string> arrdevs;
				hotplug::split(dbccName, arrdevs, '#');
				if(arrdevs.size() > 2){
					string usb = arrdevs[1];
					hotplug::split(usb, arrdevs, '&');
					m_devFunction(1, arrdevs[0], arrdevs[1]);
				}
			}
		}
	} 
}

void InnerUsbDetector::display()
{	
	cout << "in the usb detector" << endl;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static InnerUsbDetector* detector;// = (UsbDetector*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	switch (message)
	{
	case WM_CREATE:
		cout << "WM_CREATE" << endl;
		detector = (InnerUsbDetector*)(((LPCREATESTRUCT)lParam)->lpCreateParams);
		if (detector == NULL)cout << "detector is null" << endl;
		else {
			detector->display();
			cout << "detector is not null" << endl;
		}
		break;
#if 0
	case WM_PAINT:
		cout << "WM_PAINT" << endl;
		break;
	case WM_SIZE:
		cout << "WM_SIZE" << endl;
		break;
	case WM_DEVICECHANGE:
		cout << "WM_DEVICECHANGE" << endl;
		if (detector != NULL)
			detector->UsbDeviceChange(wParam, lParam);
		
		break;
#else
		
#endif
	}

	if (detector != NULL) {
		detector->SelfProc(hWnd, message, wParam, lParam);
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

void InnerUsbDetector::SelfProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DEVICECHANGE:
		//cout << "WM_DEVICECHANGE" << endl;
		UsbDeviceChange(wParam, lParam);
	default:
		break;
	}
}

ATOM InnerUsbDetector::selfRegisterClass()
{
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = WndProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = m_className;
	return RegisterClass(&wc);
}

void InnerUsbDetector::Idle() {
	MSG msg;
	while (int ret = GetMessage(&msg, m_hwnd, 0, 0))
	{
		cout << "msg:" << msg.message << endl;
		if (ret != -1) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

#if 1
bool InnerUsbDetector::registerDevice()
{
	const GUID GUID_DEVINTERFACE_LIST[] = {
		{ 0xA5DCBF10, 0x6530, 0x11D2, { 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED } },
		{ 0x53f56307, 0xb6bf, 0x11d0, { 0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b } },
		{ 0x4D1E55B2, 0xF16F, 0x11CF, { 0x88, 0xCB, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30 } }, /* HID */
		{ 0xad498944, 0x762f, 0x11d0, { 0x8d, 0xcb, 0x00, 0xc0, 0x4f, 0xc3, 0x35, 0x8c } },
		{ 0x4d1e55b2, 0xf16f, 0x11cf, { 0x88, 0xcb, 0x00, 0xc11, 0x11, 0x00, 0x00, 0x30 } } };
		//4d1e55b2-f16f-11cf-88cb-001111000030

	DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;
	ZeroMemory(&NotificationFilter, sizeof(NotificationFilter));
	NotificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
	NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;

	NotificationFilter.dbcc_classguid = GUID_DEVINTERFACE_LIST[0];
	HDEVNOTIFY hDevNotify = RegisterDeviceNotification(m_hwnd, &NotificationFilter, DEVICE_NOTIFY_WINDOW_HANDLE);
	if (!hDevNotify) {
		return false;
	}
	return true;
}
#endif

#if 0
/////////////////////////////////////////////////
int main(int argc, char* argv[])
{
	MyRegisterClass();
	CreateMessageOnlyWindow();
	RegisterDevice();

	//usb.start();

	//usb.usb_write_data_test();
	MSG msg;
	while (int ret = GetMessage(&msg, m_hwnd, 0, 0))
	{
		if (ret != -1) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return 0;
}
#else

#endif