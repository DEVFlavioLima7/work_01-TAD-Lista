#pragma once

#include <gtk/gtk.h>
#include <imgStruct.h>

extern GtkWidget *imageWidgetRGB;

void setup_ui_rgb(GtkWidget *stack);
void set_img_to_pixbuf_rgb(GdkPixbuf *pixbuf);
static void verificarBotoes();
static void atualizarImagem();
static void on_flip_horizontal_rgb_button_clicked(GtkWidget *widget, gpointer data);
static void on_flip_vertical_rgb_button_clicked(GtkWidget *widget, gpointer data);
static void on_filter_median_rgb_button_clicked(GtkWidget *widget, gpointer data);
static void on_transpose_rgb_button_clicked(GtkWidget *widget, gpointer data);
static void on_filter_clahe_rgb_button_clicked(GtkWidget *widget, gpointer data);
static void on_neq90_rotation_rgb_clicked(GtkWidget *widget, gpointer data);
static void on_add90_rotation_rgb_clicked(GtkWidget *widget, gpointer data);
static void on_random_button_clicked(GtkWidget *widget, gpointer data);
static void on_back_button_clicked(GtkWidget *widget, gpointer data);
static void on_forward_button_clicked(GtkWidget *widget, gpointer data);

