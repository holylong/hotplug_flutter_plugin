import 'package:flutter/services.dart';
import 'package:flutter_test/flutter_test.dart';
import 'package:hotplug_flutter/hotplug_flutter.dart';

void main() {
  const MethodChannel channel = MethodChannel('hotplug_flutter');

  TestWidgetsFlutterBinding.ensureInitialized();

  setUp(() {
    channel.setMockMethodCallHandler((MethodCall methodCall) async {
      return '42';
    });
  });

  tearDown(() {
    channel.setMockMethodCallHandler(null);
  });

  test('getPlatformVersion', () async {
    expect(await HotplugFlutter.platformVersion, '42');
  });
}
