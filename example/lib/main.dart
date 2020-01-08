import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';

import 'package:window_utils/window_utils.dart';

void main() {
  if (!kIsWeb && debugDefaultTargetPlatformOverride == null) {
    debugDefaultTargetPlatformOverride = TargetPlatform.android;
  }
  runApp(MyApp());
}

class MyApp extends StatefulWidget {
  @override
  _MyAppState createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  @override
  void initState() {
    WidgetsBinding.instance.addPostFrameCallback(
      (_) => WindowUtils.hideTitleBar(),
    );
    super.initState();
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      debugShowCheckedModeBanner: false,
      theme: ThemeData.light(),
      darkTheme: ThemeData.dark(),
      themeMode: ThemeMode.system,
      home: Scaffold(
        appBar: PreferredSize(
          preferredSize: Size.fromHeight(kToolbarHeight),
          child: Stack(
            children: <Widget>[
              Positioned.fill(
                child: GestureDetector(
                  onTapDown: (_) {
                    WindowUtils.startDrag();
                  },
                  onDoubleTap: () {
                    WindowUtils.windowTitleDoubleTap();
                  },
                  child: Material(
                    elevation: 4.0,
                    color: Theme.of(context).primaryColor,
                  ),
                ),
              ),
              Positioned.fill(
                child: IgnorePointer(
                  child: Center(
                    child: Text(
                      'Window Utils Example',
                      style: TextStyle(
                        color: Colors.white,
                        fontSize: 20.0,
                      ),
                    ),
                  ),
                ),
              ),
              Positioned(
                bottom: 0,
                top: 0,
                right: 0,
                child: Center(
                  child: Row(
                    children: <Widget>[
                      IconButton(
                        color: Colors.white,
                        icon: Icon(Icons.info_outline),
                        onPressed: () {
                          WindowUtils.getWindowSize()
                              .then((val) => print('Window: $val'));
                          WindowUtils.getScreenSize()
                              .then((val) => print('Screen: $val'));
                          WindowUtils.getWindowOffset()
                              .then((val) => print('Offset: $val'));
                        },
                      ),
                    ],
                  ),
                ),
              ),
            ],
          ),
        ),
        body: Container(
          child: ListView(
            children: <Widget>[
              ListTile(
                title: Text("Increase Window Size"),
                trailing: IconButton(
                  icon: Icon(Icons.desktop_windows),
                  onPressed: () {
                    WindowUtils.getWindowSize().then((val) {
                      WindowUtils.setSize(
                        Size(val.width + 20, val.height + 20),
                      );
                    });
                  },
                ),
              ),
              ListTile(
                title: Text("Move Window Position"),
                trailing: IconButton(
                  icon: Icon(Icons.drag_handle),
                  onPressed: () {
                    WindowUtils.getWindowOffset().then((val) {
                      WindowUtils.setPosition(
                        Offset(val.dx + 20, val.dy + 20),
                      );
                    });
                  },
                ),
              ),
            ],
          ),
        ),
      ),
    );
  }
}
