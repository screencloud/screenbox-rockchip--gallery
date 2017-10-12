#ifndef GLOBAL_VALUE_H
#define GLOBAL_VALUE_H

#include <QStandardPaths>
#include <mainwindow.h>

class MainWindow;

extern MainWindow *mainWindow;

#ifdef DEVICE_EVB
const int font_size = 32;
const int font_size_big = 35;
// top part
const QString return_resource_str = ":/image/main/return_big.png";
const int return_icon_width = 212;
const int return_icon_height = 70;
const int top_icon_size = 60;
#else
const int font_size = 17;
const int font_size_big = 22;
// top part
const QString return_resource_str = ":/image/main/return.png";
const int return_icon_width = 115;
const int return_icon_height = 40;
const int top_icon_size = 40;
#endif

// gallery
const QString GALLERY_SEARCH_PATH = QStandardPaths::writableLocation(QStandardPaths::HomeLocation).append("/mnt");
const QString str_top_title = "imageGallery";
const QString str_gallery_empty_tip1= "All in the family";
const QString str_gallery_empty_tip2= "Take a picture & Photo saved on this device appear here.";
const QString str_button_edit_image= "EditMode";
const QString str_button_refresh_image= "Refresh";
const QString str_button_edit_image_cancel= "Cancel";
const QString str_button_delete_image= "Delete";
const QString str_image_and_preview= "★ Image and Preview";
const QString str_imge_have_seleted1= "Current has   ";
const QString str_imge_have_seleted2= "   images seleted";

const QString str_image_information= "Image Infomation";
const QString str_confirm= "Confirm";
const QString str_image_name= "Name";
const QString str_image_pattern= "Pattern";
const QString str_image_resolution= "Resolution";
const QString str_image_location= "Location";
const QString str_image_size= "Size";
const QString str_image_create_time= "CreateTime";
const QString str_resolution_tip = "(width×height)";
const QString str_question_Tip= "warm";
const QString str_question_delete_image= "Delete images?";
const QString str_button_delete= "Delete";
const QString str_button_cancel= "Cancel";

#endif
