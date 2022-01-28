import 'package:flutter/material.dart';
import 'package:flutter/services.dart';

const methodChannel = MethodChannel('flutter_webkit');

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('WebKitGTK'),
        ),
        body: Center(
          child: ElevatedButton(
            onPressed: () {
              methodChannel.invokeMethod('openUrl', 'https://webkitgtk.org/');
            },
            child: const Text('Open WebView'),
          ),
        ),
      ),
    );
  }
}
