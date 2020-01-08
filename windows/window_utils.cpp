// Copyright 2019 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include "window_utils.h"

#include <windows.h>
#include <iostream>

#include <VersionHelpers.h>
#include <flutter/method_channel.h>
#include <flutter/plugin_registrar.h>
#include <flutter/standard_method_codec.h>
#include <memory>
#include <sstream>
#include <unordered_map> 

namespace {

class WindowUtils : public flutter::Plugin {
 public:
  static void RegisterWithRegistrar(flutter::PluginRegistrar *registrar);

  // Creates a plugin that communicates on the given channel.
  WindowUtils(
      std::unique_ptr<flutter::MethodChannel<flutter::EncodableValue>> channel);

  virtual ~WindowUtils();

 private:
  // Called when a method is called on |channel_|;
  void HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

  // The MethodChannel used for communication with the Flutter engine.
  std::unique_ptr<flutter::MethodChannel<flutter::EncodableValue>> channel_;
};

// static
void WindowUtils::RegisterWithRegistrar(flutter::PluginRegistrar *registrar) {
  auto channel =
      std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
          registrar->messenger(), "window_utils",
          &flutter::StandardMethodCodec::GetInstance());
  auto *channel_pointer = channel.get();

  auto plugin = std::make_unique<WindowUtils>(std::move(channel));

  channel_pointer->SetMethodCallHandler(
      [plugin_pointer = plugin.get()](const auto &call, auto result) {
        plugin_pointer->HandleMethodCall(call, std::move(result));
      });

  registrar->AddPlugin(std::move(plugin));
}

WindowUtils::WindowUtils(
    std::unique_ptr<flutter::MethodChannel<flutter::EncodableValue>> channel)
    : channel_(std::move(channel)) {}

WindowUtils::~WindowUtils(){};

void WindowUtils::HandleMethodCall(
    const flutter::MethodCall<flutter::EncodableValue> &method_call,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  std::string method = method_call.method_name();
  printf("Method is %s \n" , method.c_str());
  if (method.compare("hideTitleBar") == 0) {
    HWND hWnd = GetActiveWindow();
    SetMenu(hWnd, NULL);
    // SetWindowLong(hWnd,GWL_STYLE,WS_EX_LAYERED);
    flutter::EncodableValue response(true);
    result->Success(&response);
  } else if (method.compare("getScreenSize") == 0) {
    flutter::EncodableMap umap;
    HWND hWnd = GetDesktopWindow();
    RECT rect;
    if(GetWindowRect(hWnd, &rect))
    {
      double width = rect.right;
      double height = rect.bottom;
      umap[flutter::EncodableValue("width")] = flutter::EncodableValue(width);
      umap[flutter::EncodableValue("height")] = flutter::EncodableValue(height);
    }
    flutter::EncodableValue response(umap);
    result->Success(&response);
  } else if (method.compare("getWindowSize") == 0) {
    flutter::EncodableMap umap;
    HWND hWnd = GetActiveWindow();
    RECT rect;
    if(GetWindowRect(hWnd, &rect))
    {
      double width = rect.right - rect.left;
      double height = rect.bottom - rect.top;
      umap[flutter::EncodableValue("width")] = flutter::EncodableValue(width);
      umap[flutter::EncodableValue("height")] = flutter::EncodableValue(height);
    }
    flutter::EncodableValue response(umap);
    result->Success(&response);
  } else if (method.compare("getWindowOffset") == 0) {
    flutter::EncodableMap umap;
    HWND hWnd = GetActiveWindow();
    RECT rect;
    if(GetWindowRect(hWnd, &rect))
    {
      double offsetX = rect.left;
      double offsetY = rect.top;
      umap[flutter::EncodableValue("offsetX")] = flutter::EncodableValue(offsetX);
      umap[flutter::EncodableValue("offsetY")] = flutter::EncodableValue(offsetY);
    }
    flutter::EncodableValue response(umap);
    result->Success(&response);
  } else {
    result->NotImplemented();
  }
}

}  // namespace

void WindowUtilsRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  // The plugin registrar owns the plugin, registered callbacks, etc., so must
  // remain valid for the life of the application.
  static auto *plugin_registrar = new flutter::PluginRegistrar(registrar);

  WindowUtils::RegisterWithRegistrar(plugin_registrar);
}
