# hotplug_flutter

cross platform hotplug flutter plugin project.

## Getting Started
```
  Future<void> initUsbEvent() async{
    // String devChangeMessage;
    try{
      HotplugFlutter.registerCallback((String msg){
        setState(() {
          _devChangeMessage = msg;
        });
      });
    }on Exception{
      _devChangeMessage = "error";
    }
  }
```
