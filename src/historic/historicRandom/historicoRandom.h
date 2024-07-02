#pragma once

#include <imgRGB.h>
#include <imgGray.h>
#include <historicStructs.h>

void setFilterLabel(FuncUsed funcUsed);
char* getFuncUsedName(FuncUsed funcUsed);

void createRandomHistoricGray();
void addToHistoricRandomGray(ImageGray *imgGray, FuncUsed funcUsed);
void showRandomImageWindowGray();
void next_image_clicked_gray(GtkWidget *widget, gpointer data);
void update_images_gray();
void set_img_to_pixbuf_gray_random(GdkPixbuf *pixbuf, ImageGray *imgGray);
void clearRandomHistoricGray();

void createRandomHistoricRGB();
void addToHistoricRandomRGB(ImageRGB *imgRGB, FuncUsed funcUsed);
void showRandomImageWindowRGB();
void next_image_clicked_rgb(GtkWidget *widget, gpointer data);
void update_images_rgb();
void set_img_to_pixbuf_rgb_random(GdkPixbuf *pixbuf, ImageRGB *imgRGB);
void clearRandomHistoricRGB();

void destroy_buttons();