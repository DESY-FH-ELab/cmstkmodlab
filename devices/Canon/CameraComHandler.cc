#include "CameraComHandler.h"

/**
  \brief Constructs a new communication handler for a digital camera.
  Communication with a digital camera is handled via libgphoto2.
  \arg modelName Full camera model name (e.g. "Canon EOS 550D")
  \arg port Name of the port the camera is connected to (e.g. "usb:001")
  */
CameraComHandler::CameraComHandler(const char* modelName,
                                   const char* port)
  : context_(NULL),
    camera_(NULL)
{
  // TODO error checking
  // TODO set context callback functions

  context_ = gp_context_new();

  // require given model name
  //  CameraAbilities abilities_;
  CameraAbilitiesList* list;
  gp_abilities_list_new(&list);
  // Load list of known devices, look for camera, get and set abilities
  gp_abilities_list_load(list, context_);
  gp_abilities_list_get_abilities(list,
                                  gp_abilities_list_lookup_model(list, modelName),
                                  &abilities_);
  gp_abilities_list_free(list);

  // Require it to be on the requested port
  //  GPPortInfo info_;
  GPPortInfoList* infoList;
  // Load list of known ports, look for camera, get and set abilities
  gp_port_info_list_new(&infoList);
  gp_port_info_list_load(infoList);
  gp_port_info_list_get_info(infoList,
                             gp_port_info_list_lookup_path(infoList, port),
                             &info_);
  gp_port_info_list_free(infoList);

  /*
    Create camera now, otherwise an invalid conversion from Camera* const*
    to Camera** arises in initialize.
   */
  gp_camera_new(&camera_);
}

/// Closes the communication and frees the associated objects.
CameraComHandler::~CameraComHandler() {

  gp_camera_exit(camera_, context_);
  gp_camera_free(camera_);
  //  delete context_;
}

/// Whether communication with the camera is possible.
bool CameraComHandler::initialize() {

  //  gp_camera_new(&camera_);

  // Set the retrieved (supported) requirements
  gp_camera_set_abilities(camera_, abilities_);
  gp_camera_set_port_info(camera_, info_);

  // Initialize camera with above options
  int error = gp_camera_init(camera_, context_);
  bool success;
  if ( error < GP_OK ) {
    gp_camera_exit(camera_, context_);
    gp_camera_free(camera_);
    camera_ = NULL;
    success = false;
  } else {
    // Test if really connected (by asking for information)
    CameraText text;
    error = gp_camera_get_summary(camera_, &text, context_);
    success = (error == GP_OK);
  }

  //  if (camera_ != NULL) {
  //    gp_camera_get_abilities(camera_, &abilities);
  //    std::cout << "[CameraComHandler] Connected to device "
  //              << abilities.model
  //              << std::endl;
  //  }

  return success;
}

/**
  \brief Returns a list of allowed options for the given setting name.
  \arg name Name of the setting for which the options should be retrieved.
  \return List of allowed options.
 */
std::vector<std::string> CameraComHandler::readConfigOptions(const char* name) const {

  // FIXME Error checking!

  // Getting camera config
  CameraWidget* window = NULL;
  CameraWidget* child = NULL;

  gp_camera_get_config(camera_, &window, context_);
  gp_widget_get_child_by_name(window, name, &child);

  //  TODO if ( type != GP_WIDGET_RADIO)
  //  CameraWidgetType type;
  //  gp_widget_get_type(child, &type);
  //  if ( type != GP_WIDGET_RADIO )
  //    std::cout << "radio" << std::endl;

  // Create vector of available options, using acquired size
  int count = gp_widget_count_choices(child);
  // TODO check for < 0; i.e. errors
  std::vector<std::string> options(count);

  char const* value = NULL;

  for ( unsigned int i = 0; i < count; ++i ) {
    gp_widget_get_choice(child, i, &value);
    std::string copy(value);
    options[i] = copy;
  }

  gp_widget_free(window);

  return options;
}

/**
  \brief Reads the current setting for given setting name.
  \arg name Field name for which the value should be retrieved.
  \return The current value of the setting.
  \see CameraComHandler::readConfigOptions
  \see CameraComHandler::writeConfigValue
 */
std::string CameraComHandler::readConfigValue(const char *name) const {

  CameraWidget* window;
  CameraWidget* child;

  // Retrieves the camera configuration
  gp_camera_get_config(camera_, &window, context_);
  gp_widget_get_child_by_name(window, name, &child);

  CameraWidgetType type;
  gp_widget_get_type(child, &type);

  //  if ( type == GP_WIDGET_RADIO )
  //    std::cout << "Type is radio" << std::endl;

  /*
    Please pass
    (char*) for GP_WIDGET_MENU, GP_WIDGET_TEXT,
    (float) for GP_WIDGET_RANGE,
    (int) for GP_WIDGET_DATE, GP_WIDGET_TOGGLE, GP_WIDGET_RADIO, and
    (CameraWidgetCallback) for GP_WIDGET_BUTTON.
   */
  char* value;
  gp_widget_get_value(child, &value);

  // Copy the value here, otherwise problems might occur
  std::string copy(value);

  gp_widget_free(window);

  return copy;
}

/**
  \brief Sets the current value of the wanted setting.
  Only the options in the list returned by readConfigOptions will result in
  predictable behaviour. Otherwise the driver might stick to the old value,
  or look for the best approximation.
  \arg name Name of the setting.
  \arg value New value for the setting.
  \return true if setting was changed, false otherwise.
  \see CameraComHandler::readConfigOptions
  \see CameraComHandler::writeConfigValue
 */
bool CameraComHandler::writeConfigValue(const char *name, const char *value) {

  // FIXME full length error checking
  CameraWidget* window;
  CameraWidget* child;

  // Retrieves the camera configuration
  gp_camera_get_config(camera_, &window, context_);
  gp_widget_get_child_by_name(window, name, &child);

  // Updates with the new value
  //  std::cout << value << std::endl;
  gp_widget_set_value(child, value);
  int error = gp_camera_set_config(camera_, window, context_);

  gp_widget_free(window);

  return (error == GP_OK);
}

/**
  \brief Instructs the camera to take a picture, downloads it and writes it in
  the file given by filedescriptor.
  In order to do this, the file needs to have at least write permissions for the
  current user. After acquisition the file will be deleted from the camera.
  \arg filedescriptor File descriptor of a file that is writable by the current
  user.
  \return true if picture was acquired and saved, false otherwise.
 */
bool CameraComHandler::acquireAndDownloadPicture(int filedescriptor) const {

  // FIXME decent error checking
  CameraFile *file;
  gp_file_new_from_fd(&file, filedescriptor);

  // Take and store the picture
  CameraFilePath path;
  int error = gp_camera_capture(camera_, GP_CAPTURE_IMAGE, &path, context_);
  gp_camera_file_get(camera_,
                     path.folder,
                     path.name,
                     GP_FILE_TYPE_NORMAL,
                     file,
                     context_);

  // Remove from camera
  gp_camera_file_delete(camera_, path.folder, path.name, context_);

  gp_file_free(file);

  return (error == GP_OK);
}

/**
  \brief Instructs the camera to take a preview picture, downloads it and writes
  it in the file given by filename. In order to do this, the file needs to have
  at least write permissions for the current user.
  \arg filename name of a file that is writable by the current user.
  \return true if picture was acquired and saved, false otherwise.
 */
bool CameraComHandler::acquirePreview(const char* filename) const {

  // FIXME decent error checking
  CameraFile *file;
  gp_file_new(&file);

  // Take and store the picture
  int error = gp_camera_capture_preview(camera_, file, context_);
  error = gp_file_save(file, filename);
  
  gp_file_unref(file);
  
  return (error == GP_OK);
}

/**
  \brief Switches camera into preview mode.
  \return true if operation was successful, false otherwise.
 */
bool CameraComHandler::startPreviewMode() const {

  // FIXME decent error checking
  CameraFile *file;
  gp_file_new(&file);

  // Take and store the picture
  int error = gp_camera_capture_preview(camera_, file, context_);

  gp_file_unref(file);
  
  return (error == GP_OK);
}

/**
  \brief Switches off camera preview mode.
  \return true if operation was successful, false otherwise.
 */
bool CameraComHandler::stopPreviewMode() const {

  int error = gp_camera_exit(camera_, context_);
  error = gp_camera_init(camera_, context_);

  return (error == GP_OK);
}
