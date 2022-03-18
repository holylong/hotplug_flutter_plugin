#include "include/hotplug_flutter/hotplug_flutter_plugin.h"

// This must be included before many other Windows headers.
#include <windows.h>

// For getPlatformVersion; remove unless needed for your plugin implementation.
#include <VersionHelpers.h>

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <map>
#include <memory>
#include <sstream>
#include <usb_tracer.h>

namespace {

class HotplugFlutterPlugin : public flutter::Plugin {
 public:
  static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

  HotplugFlutterPlugin(flutter::PluginRegistrarWindows *registrar, std::unique_ptr<flutter::MethodChannel<>> channel);

  //HotplugFlutterPlugin();

  virtual ~HotplugFlutterPlugin();

  void OnDevChangeCallback(int, std::string, std::string);

 private:
  // Called when a method is called on this plugin's channel from Dart.
  void HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

  std::unique_ptr<flutter::MethodChannel<>> _channel;
  flutter::PluginRegistrarWindows *_registrar;

  UsbTracer* _tracer;
};

void HotplugFlutterPlugin::OnDevChangeCallback(int type, std::string vid, std::string pid)
{
  if(type == 1)
  _channel->InvokeMethod("devChange", std::make_unique<flutter::EncodableValue>("attach-" + vid + "-" + pid));
  else if(type == 0){
    _channel->InvokeMethod("devChange", std::make_unique<flutter::EncodableValue>("detach-" + vid + "-" + pid));
  }
}

// static
void HotplugFlutterPlugin::RegisterWithRegistrar(
    flutter::PluginRegistrarWindows *registrar) {
  auto channel =
      std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
          registrar->messenger(), "hotplug_flutter",
          &flutter::StandardMethodCodec::GetInstance());

  // auto plugin = std::make_unique<HotplugFlutterPlugin>();

    //warning
  auto *channel_pointer = channel.get();
  auto plugin = std::make_unique<HotplugFlutterPlugin>(registrar, std::move(channel));

  channel_pointer->SetMethodCallHandler(
      [plugin_pointer = plugin.get()](const auto &call, auto result) {
        plugin_pointer->HandleMethodCall(call, std::move(result));
      });

  registrar->AddPlugin(std::move(plugin));
}

// HotplugFlutterPlugin::HotplugFlutterPlugin() {}

HotplugFlutterPlugin::HotplugFlutterPlugin(flutter::PluginRegistrarWindows *registrar,
    std::unique_ptr<flutter::MethodChannel<>> channel):_registrar(registrar), _channel(std::move(channel)) {
      _tracer = UsbTracer::createTracer();
      _tracer->registerCallback(std::bind(&HotplugFlutterPlugin::OnDevChangeCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
      _tracer->Init();
    }

HotplugFlutterPlugin::~HotplugFlutterPlugin() {
  if(_tracer != NULL){
    _tracer->exitTracer();
    delete _tracer;
    _tracer = NULL;
  }
}

void HotplugFlutterPlugin::HandleMethodCall(
    const flutter::MethodCall<flutter::EncodableValue> &method_call,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  if (method_call.method_name().compare("getPlatformVersion") == 0) {
    std::ostringstream version_stream;
    version_stream << "Windows ";
    if (IsWindows10OrGreater()) {
      version_stream << "10+";
    } else if (IsWindows8OrGreater()) {
      version_stream << "8";
    } else if (IsWindows7OrGreater()) {
      version_stream << "7";
    }
    result->Success(flutter::EncodableValue(version_stream.str()));
  } else {
    result->NotImplemented();
  }
}

}  // namespace

void HotplugFlutterPluginRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  HotplugFlutterPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
