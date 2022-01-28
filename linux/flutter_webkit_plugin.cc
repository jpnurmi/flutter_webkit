#include "include/flutter_webkit/flutter_webkit_plugin.h"

#include <flutter_linux/flutter_linux.h>
#include <gtk/gtk.h>
#include <webkit2/webkit2.h>

#define FLUTTER_WEBKIT_PLUGIN(obj)                                     \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), flutter_webkit_plugin_get_type(), \
                              FlutterWebkitPlugin))

struct _FlutterWebkitPlugin {
  GObject parent_instance;
};

G_DEFINE_TYPE(FlutterWebkitPlugin, flutter_webkit_plugin, g_object_get_type())

// Called when a method call is received from Flutter.
static void flutter_webkit_plugin_handle_method_call(
    GtkWidget* window, FlMethodCall* method_call) {
  g_autoptr(FlMethodResponse) response = nullptr;

  const gchar* method = fl_method_call_get_name(method_call);
  FlValue* args = fl_method_call_get_args(method_call);

  if (strcmp(method, "openUrl") == 0) {
    GtkWidget* popup = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_transient_for(GTK_WINDOW(popup), GTK_WINDOW(window));

    GtkWidget* scrollview = gtk_scrolled_window_new(nullptr, nullptr);
    GtkWidget* webview = webkit_web_view_new();
    gtk_container_add(GTK_CONTAINER(scrollview), webview);
    gtk_container_add(GTK_CONTAINER(popup), scrollview);

    const gchar* url = fl_value_get_string(args);
    webkit_web_view_load_uri(WEBKIT_WEB_VIEW(webview), url);

    gtk_window_set_default_size(GTK_WINDOW(popup), 800, 600);
    gtk_widget_show_all(popup);

    response = FL_METHOD_RESPONSE(fl_method_success_response_new(nullptr));
  } else {
    response = FL_METHOD_RESPONSE(fl_method_not_implemented_response_new());
  }

  fl_method_call_respond(method_call, response, nullptr);
}

static void flutter_webkit_plugin_dispose(GObject* object) {
  G_OBJECT_CLASS(flutter_webkit_plugin_parent_class)->dispose(object);
}

static void flutter_webkit_plugin_class_init(FlutterWebkitPluginClass* klass) {
  G_OBJECT_CLASS(klass)->dispose = flutter_webkit_plugin_dispose;
}

static void flutter_webkit_plugin_init(FlutterWebkitPlugin* self) {}

static void method_call_cb(FlMethodChannel* channel, FlMethodCall* method_call,
                           gpointer user_data) {
  GtkWidget* window = GTK_WIDGET(user_data);
  flutter_webkit_plugin_handle_method_call(window, method_call);
}

void flutter_webkit_plugin_register_with_registrar(
    FlPluginRegistrar* registrar) {
  FlutterWebkitPlugin* plugin = FLUTTER_WEBKIT_PLUGIN(
      g_object_new(flutter_webkit_plugin_get_type(), nullptr));

  FlView* view = fl_plugin_registrar_get_view(registrar);
  GtkWidget* window = gtk_widget_get_toplevel(GTK_WIDGET(view));

  g_autoptr(FlStandardMethodCodec) codec = fl_standard_method_codec_new();
  g_autoptr(FlMethodChannel) channel =
      fl_method_channel_new(fl_plugin_registrar_get_messenger(registrar),
                            "flutter_webkit", FL_METHOD_CODEC(codec));
  fl_method_channel_set_method_call_handler(channel, method_call_cb, window,
                                            nullptr);

  g_object_unref(plugin);
}
