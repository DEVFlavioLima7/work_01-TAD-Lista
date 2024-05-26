#include <uiRGB.h>
#include <imgRGB.h>
#include <uiMain.h>

void setup_ui_rgb(GtkWidget *stack)
{
  // RGB Image Handling
  GtkWidget *imageBoxRGB = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_stack_add_titled(GTK_STACK(stack), imageBoxRGB, "Manipulação RGB", "Manipulação RGB");

  // GdkPixbuf *pixbufRGB = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, imgRGB->dim.largura, imgRGB->dim.altura);
  // set_img_to_pixbuf_rgb(imgRGB, pixbufRGB);

  // imageWidgetRGB = gtk_image_new_from_pixbuf(pixbufRGB);
  // gtk_box_pack_start(GTK_BOX(imageBoxRGB), imageWidgetRGB, TRUE, TRUE, 0);

  GtkWidget *buttonFlip = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3);
  gtk_box_pack_start(GTK_BOX(imageBoxRGB), buttonFlip, FALSE, FALSE, 2);

  // sub
  GtkWidget *flipHorizontalButtonRGB = gtk_toggle_button_new_with_label("Flip Horizontal");
  g_signal_connect(flipHorizontalButtonRGB, "toggled", G_CALLBACK(NULL), NULL);
  gtk_box_pack_start(GTK_BOX(buttonFlip), flipHorizontalButtonRGB, TRUE, TRUE, 0);

  GtkWidget *flipVerticalButtonRGB = gtk_toggle_button_new_with_label("Flip Vertical");
  g_signal_connect(flipVerticalButtonRGB, "clicked", G_CALLBACK(NULL), NULL);
  gtk_box_pack_start(GTK_BOX(buttonFlip), flipVerticalButtonRGB, TRUE, TRUE, 0);

  GtkWidget *transposeButtonRGB = gtk_toggle_button_new_with_label("Transpose");
  g_signal_connect(transposeButtonRGB, "clicked", G_CALLBACK(NULL), NULL);
  gtk_box_pack_start(GTK_BOX(buttonFlip), transposeButtonRGB, TRUE, TRUE, 0);

  GtkWidget *buttonBoxRGB = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_box_pack_start(GTK_BOX(imageBoxRGB), buttonBoxRGB, FALSE, FALSE, 2);

  GtkWidget *claheButtonRGB = gtk_toggle_button_new_with_label("filtro CLAHE");
  g_signal_connect(claheButtonRGB, "clicked", G_CALLBACK(NULL), NULL);
  gtk_box_pack_start(GTK_BOX(buttonBoxRGB), claheButtonRGB, TRUE, TRUE, 0);

  GtkWidget *medianBlurButtonRGB = gtk_toggle_button_new_with_label("filtro Median Blur RGB");
  g_signal_connect(medianBlurButtonRGB, "clicked", G_CALLBACK(NULL), NULL);
  gtk_box_pack_start(GTK_BOX(buttonBoxRGB), medianBlurButtonRGB, TRUE, TRUE, 0);
}

void set_img_to_pixbuf_rgb(ImageRGB *imgRGB, GdkPixbuf *pixbuf)
{
  guchar *pixels = gdk_pixbuf_get_pixels(pixbuf);

  for (int i = 0; i < imgRGB->dim.altura; i++)
  {
    for (int j = 0; j < imgRGB->dim.largura; j++)
    {
      int index = (i * imgRGB->dim.largura + j) * 3;
      pixels[index] = imgRGB->pixels[i * imgRGB->dim.largura + j].red;
      pixels[index + 1] = imgRGB->pixels[i * imgRGB->dim.largura + j].green;
      pixels[index + 2] = imgRGB->pixels[i * imgRGB->dim.largura + j].blue;
    }
  }
}

void on_flip_horizontal_rgb_button_clicked(GtkWidget *widget, gpointer data)
{
  (void)widget;
  (void)data;

  // imgRGB = flip_horizontal_rgb(imgRGB);

  GdkPixbuf *pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, imgRGB->dim.largura, imgRGB->dim.altura);
  set_img_to_pixbuf_rgb(imgRGB, pixbuf);

  gtk_image_set_from_pixbuf(GTK_IMAGE(imageWidgetRGB), pixbuf);
  gtk_widget_show(imageWidgetRGB);
}

void on_flip_vertical_rgb_button_clicked(GtkWidget *widget, gpointer data)
{
  (void)widget;
  (void)data;

  // imgRGB = flip_vertical_rgb(imgRGB);

  GdkPixbuf *pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, imgRGB->dim.largura, imgRGB->dim.altura);
  set_img_to_pixbuf_rgb(imgRGB, pixbuf);

  gtk_image_set_from_pixbuf(GTK_IMAGE(imageWidgetRGB), pixbuf);
  gtk_widget_show(imageWidgetRGB);
}
