//#include "../EOS550D.h"

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

#include <gphoto2/gphoto2.h>

using namespace std;

static void
ctx_error_func (GPContext *context, const char *format, va_list args, void *data)
{
        fprintf  (stderr, "\n");
        fprintf  (stderr, "*** Contexterror ***              \n");
        vfprintf (stderr, format, args);
        fprintf  (stderr, "\n");
        fflush   (stderr);
}

static void
ctx_status_func (GPContext *context, const char *format, va_list args, void *data)
{
        vfprintf (stderr, format, args);
        fprintf  (stderr, "\n");
        fflush   (stderr);
}

void check(const char* str, int e) {
  cout << "*** " << str << " ***" << endl;
  if (e < GP_OK) throw e;
}

void resolveChildren(CameraWidget* parent, const char* root) {

  int childcount = gp_widget_count_children(parent);

  char const* val;
  gp_widget_get_name(parent, &val);

  char path[200];
  strcpy(path, root);

  strcat( strcat(path, "/"), val);

  for (int i=0; i<childcount; ++i) {
    CameraWidget* child;
    gp_widget_get_child(parent, i, &child);
    resolveChildren(child, path);
  }

  if (childcount == 0)
    cout << path << endl;

}

void getWithShutterspeed(Camera* cam, GPContext* context, const char* speed, int i) {

  // Getting camera config
  CameraWidget* window = NULL;
  check("get config", gp_camera_get_config(cam, &window, context));

//    resolveChildren(window, "");

  CameraWidget* child;
  check(
      "finding child"
    , gp_widget_get_child_by_name(window, "whitebalance", &child)
  );

  char const* value;
//  int value;

  // TYPE == GP_TYPE_RADIO
//  CameraWidgetType type;
//  check("get type", gp_widget_get_type(child, &type));
//  cout << type << endl;

//  int choices = gp_widget_count_choices(child);

//  for (int i=0; i<choices; ++i) {
//    gp_widget_get_choice(child, i, &value);
//    cout << value << endl;
//  }

//  check("set value", gp_widget_set_value(child, speed));
//  check("set config", gp_camera_set_config(cam, window, context));

  // Prepare for saving file
//  char fullPath[L_tmpnam];
//  sprintf(name, "capt%4d.jpg", i);
//  tmpnam(fullPath);

  char fullPath[] = "/tmp/defo_XXXXXX";
//  strcat(fullPath, name);

//  cout << fullPath << endl;

  CameraFile *file;
  int fileDescriptor = mkstemp(fullPath);
//          fullPath
//        , O_CREAT|O_WRONLY // Create, write only
//        , S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH // chmod 600
//  );

  check("create file", gp_file_new_from_fd(&file, fileDescriptor));

//  check("open file", gp_file_open(file, fullPath));
//  check("set filename", gp_file_set_name(file, name));

  // Take and store the picture
  CameraFilePath path;
  check("capture", gp_camera_capture(cam, GP_CAPTURE_IMAGE, &path, context));

  check("download image", gp_camera_file_get(
            cam
          , path.folder
          , path.name
          , GP_FILE_TYPE_NORMAL
          , file
          , context
  ));

  close(fileDescriptor);

  gp_file_free(file);
  gp_widget_free(window);

}

int main() {

  Camera* cam;
  GPContext* context = gp_context_new();

  gp_context_set_error_func (context, ctx_error_func, NULL);
  gp_context_set_status_func (context, ctx_status_func, NULL);

  gp_camera_new(&cam);


//  CameraText text;

//  gp_camera_get_manual(cam, &text, context);
//  cout << "MANUAL\n" << text.text << endl;

//  gp_camera_get_about(cam, &text, context);
//  cout << "ABOUT\n" << text.text << endl;

//  gp_camera_get_summary(cam, &text, context);
//  cout << "SUMMARY\n" << text.text << endl;

  try {

    // require Canon EOS 550D
    CameraAbilities abilities;
    CameraAbilitiesList* list;
    gp_abilities_list_new(&list);
    gp_abilities_list_load(list, context);

    int pos = gp_abilities_list_lookup_model(list, "Canon EOS 550D");
    check("get abilities list", pos);
    check("get abilities", gp_abilities_list_get_abilities(list, pos, &abilities));
    check("setting abilities", gp_camera_set_abilities(cam, abilities));

    gp_abilities_list_free(list);

    GPPortInfo info;
    GPPortInfoList* infoList;
    gp_port_info_list_new(&infoList);
    gp_port_info_list_load(infoList);

    for (int i=0; i<gp_port_info_list_count(infoList); ++i) {
      gp_port_info_list_get_info(infoList, i, &info);
      cout << info.path << endl;
    }

    pos = gp_port_info_list_lookup_path(infoList, "usb:001");
    check("retrieving port info index", pos);
    check("get port info", gp_port_info_list_get_info(infoList, pos, &info));
    check("set port info", gp_camera_set_port_info(cam, info));

    check("init", gp_camera_init(cam, context));

    check("get abilities", gp_camera_get_abilities(cam, &abilities));
    cout << "name=" <<  abilities.model << endl;

    check("get port info", gp_camera_get_port_info(cam, &info));
    cout << "port=" << info.name << ',' << info.path << endl;

    getWithShutterspeed(cam, context, "1", 1);
//    getWithShutterspeed(cam, context, "0.5", 2);
//    getWithShutterspeed(cam, context, "1/10", 3);

  }
  catch (int e) {
    cout << "Error "<< e << ": " << gp_result_as_string(e) << endl;
  }

  gp_camera_exit(cam, context);

  return 0;

}
