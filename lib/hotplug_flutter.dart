
import 'dart:async';

import 'package:flutter/services.dart';

class HotplugFlutter {
  static const MethodChannel _channel = MethodChannel('hotplug_flutter');

  static Map<int, Function> _devChangeCallback = {};

  static Future<String?> get platformVersion async {
    final String? version = await _channel.invokeMethod('getPlatformVersion');
    return version;
  }

  static Future<void> methodCallHandler(MethodCall call) async{
    switch(call.method){
      case 'devChange':
        if(_devChangeCallback.length > 0){
            final callback = _devChangeCallback[0];
            if(callback != null)
            callback(call.arguments as String);
          }
          break;
       default:
          print('not method');
    }
  }

  static void registerCallback(Function cb){
    _channel.setMethodCallHandler(methodCallHandler);
    _devChangeCallback[0] = cb;
  }
}
