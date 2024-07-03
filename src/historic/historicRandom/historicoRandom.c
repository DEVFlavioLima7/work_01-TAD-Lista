#include <gtk/gtk.h>
#include <stdlib.h>
#include "historico.h"
#include "historicoRandom.h"

ImgHistoricoRandomGray *historicoRandomGrayInicio = NULL;
ImgHistoricoRandomGray *historicoRandomGrayAtual = NULL;

ImgHistoricoRandomRGB *historicoRandomRGBInicio = NULL;
ImgHistoricoRandomRGB *historicoRandomRGBAtual = NULL;

GtkWidget *imageAtual = NULL;
GtkWidget *imageNext = NULL;

GtkWidget *buttonBox = NULL;
GtkWidget *window = NULL;
GtkWidget *mainBox = NULL;
GtkWidget *filterLabel = NULL;

void modFilterLabel(FuncUsed funcUsed)
{
  gtk_label_set_text(GTK_LABEL(filterLabel), getFuncUsedName(funcUsed));
}

char *getFuncUsedName(FuncUsed funcUsed)
{
  switch (funcUsed)
  {
  case FLIP_VERTICAL:
    return "Flip Vertical";
  case FLIP_HORIZONTAL:
    return "Flip Horizontal";
  case TRANSPOSE:
    return "Transpose";
  case CLAHE:
    return "CLAHE";
  case MEDIAN_BLUR:
    return "Median Blur";
  case FLIP_ADD90:
    return "Flip 90º";
  case FLIP_NEQ90:
    return "Flip -90º";
  default:
    return "None";
  }
}

void createRandomHistoricGray()
{
  ImgHistoricoRandomGray *aux = historicoRandomGrayInicio;
  while (aux->next != NULL)
    aux = aux->next;

  ImageGray *imgGray = aux->imgGray;

  int random = rand() % 7;
  switch (random)
  {
  case 0:
    addToHistoricRandomGray(flip_vertical_gray(imgGray), FLIP_VERTICAL);
    break;
  case 1:
    addToHistoricRandomGray(flip_horizontal_gray(imgGray), FLIP_HORIZONTAL);
    break;
  case 2:
    addToHistoricRandomGray(transpose_gray(imgGray), TRANSPOSE);
    break;
  case 3:
    addToHistoricRandomGray(clahe_gray(imgGray, 512, 512), CLAHE);
    break;
  case 4:
    addToHistoricRandomGray(median_blur_gray(imgGray, 9), MEDIAN_BLUR);
    break;
  case 5:
    addToHistoricRandomGray(add90_rotation_gray(imgGray), FLIP_ADD90);
    break;
  case 6:
    addToHistoricRandomGray(neq90_rotation_gray(imgGray), FLIP_NEQ90);
    break;
  default:
    break;
  }
}

void addToHistoricRandomGray(ImageGray *imgGray, FuncUsed funcUsed)
{
  ImgHistoricoRandomGray *new = (ImgHistoricoRandomGray *)malloc(sizeof(ImgHistoricoRandomGray));
  new->imgGray = imgGray;
  new->next = NULL;
  new->funcUsed = funcUsed;

  ImgHistoricoRandomGray *aux = historicoRandomGrayInicio;
  while (aux->next != NULL)
    aux = aux->next;

  aux->next = new;
}

void showRandomImageWindowGray()
{
  clearRandomHistoricGray();
  for (int i = 0; i < 10; i++)
    createRandomHistoricGray(historicoGrayAtual->imgGray);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Imagem Aleatória Gray");
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

  mainBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_container_add(GTK_CONTAINER(window), mainBox);

  imageAtual = gtk_image_new();
  gtk_box_pack_start(GTK_BOX(mainBox), imageAtual, TRUE, TRUE, 10);

  buttonBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_box_pack_start(GTK_BOX(mainBox), buttonBox, TRUE, TRUE, 10);

  GtkWidget *space = gtk_label_new(">>>>");
  gtk_box_pack_start(GTK_BOX(buttonBox), space, TRUE, TRUE, 5);

  GtkWidget *randomButton = gtk_button_new_with_label("Próxima Imagem");
  g_signal_connect(randomButton, "clicked", G_CALLBACK(next_image_clicked_gray), NULL);
  gtk_box_pack_start(GTK_BOX(buttonBox), randomButton, FALSE, TRUE, 5);

  filterLabel = gtk_label_new("Filtro Atual");
  gtk_box_pack_start(GTK_BOX(buttonBox), filterLabel, TRUE, TRUE, 5);

  imageNext = gtk_image_new();
  gtk_box_pack_start(GTK_BOX(mainBox), imageNext, TRUE, TRUE, 10);

  update_images_gray();

  gtk_widget_show_all(window);
}

void next_image_clicked_gray(GtkWidget *widget, gpointer data)
{
  (void)widget;
  (void)data;

  historicoRandomGrayAtual = historicoRandomGrayAtual->next;
  update_images_gray();
}

void update_images_gray()
{
  GdkPixbuf *pixbufAtual = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, historicoRandomGrayAtual->imgGray->dim.largura, historicoRandomGrayAtual->imgGray->dim.altura);
  set_img_to_pixbuf_gray_random(pixbufAtual, historicoRandomGrayAtual->imgGray);
  gtk_image_set_from_pixbuf(GTK_IMAGE(imageAtual), pixbufAtual);

  if (historicoRandomGrayAtual->next != NULL)
  {
    modFilterLabel(historicoRandomGrayAtual->next->funcUsed);
    GdkPixbuf *pixbufNext = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, historicoRandomGrayAtual->next->imgGray->dim.largura, historicoRandomGrayAtual->next->imgGray->dim.altura);
    set_img_to_pixbuf_gray_random(pixbufNext, historicoRandomGrayAtual->next->imgGray);
    gtk_image_set_from_pixbuf(GTK_IMAGE(imageNext), pixbufNext);
  }
  else
  {
    destroy_buttons();
  }
}

void set_img_to_pixbuf_gray_random(GdkPixbuf *pixbuf, ImageGray *imgGray)
{
  guchar *pixels = gdk_pixbuf_get_pixels(pixbuf);

  for (int i = 0; i < imgGray->dim.altura; i++)
    for (int j = 0; j < imgGray->dim.largura; j++)
    {
      int index = (i * imgGray->dim.largura + j) * 3;
      pixels[index] = imgGray->pixels[i * imgGray->dim.largura + j].value;
      pixels[index + 1] = imgGray->pixels[i * imgGray->dim.largura + j].value;
      pixels[index + 2] = imgGray->pixels[i * imgGray->dim.largura + j].value;
    }
}

void destroy_buttons()
{
  if (buttonBox != NULL)
  {
    gtk_container_remove(GTK_CONTAINER(mainBox), buttonBox);

    GList *children = gtk_container_get_children(GTK_CONTAINER(buttonBox));
    for (GList *iter = children; iter != NULL; iter = g_list_next(iter))
      gtk_widget_destroy(GTK_WIDGET(iter->data));

    g_list_free(children);

    buttonBox = NULL;
  }

  gtk_container_remove(GTK_CONTAINER(mainBox), imageNext);

  gtk_window_resize(GTK_WINDOW(window), 512, 512);

  gtk_widget_show_all(window);
}

void clearRandomHistoricGray()
{
  ImgHistoricoRandomGray *ant = historicoRandomGrayInicio;
  while (ant != NULL)
  {
    ImgHistoricoRandomGray *next = ant->next;
    free(ant->imgGray);
    free(ant);
    ant = next;
  }
  historicoRandomGrayInicio = (ImgHistoricoRandomGray *)malloc(sizeof(ImgHistoricoRandomGray));
  historicoRandomGrayInicio->imgGray = (ImageGray *)malloc(sizeof(ImageGray));
  historicoRandomGrayInicio->imgGray->dim.altura = historicoGrayAtual->imgGray->dim.altura;
  historicoRandomGrayInicio->imgGray->dim.largura = historicoGrayAtual->imgGray->dim.largura;
  historicoRandomGrayInicio->imgGray->pixels = (PixelGray *)malloc(historicoGrayAtual->imgGray->dim.altura * historicoGrayAtual->imgGray->dim.largura * sizeof(PixelGray));
  for (int i = 0; i < historicoGrayAtual->imgGray->dim.altura; i++)
    for (int j = 0; j < historicoGrayAtual->imgGray->dim.largura; j++)
      historicoRandomGrayInicio->imgGray->pixels[i * historicoGrayAtual->imgGray->dim.largura + j].value = historicoGrayAtual->imgGray->pixels[i * historicoGrayAtual->imgGray->dim.largura + j].value;

  historicoRandomGrayInicio->next = NULL;
  historicoRandomGrayInicio->funcUsed = NONE;

  historicoRandomGrayAtual = historicoRandomGrayInicio;
}

void createRandomHistoricRGB()
{
  ImgHistoricoRandomRGB *aux = historicoRandomRGBInicio;
  while (aux->next != NULL)
    aux = aux->next;

  ImageRGB *imgRGB = aux->imgRGB;

  int random = rand() % 7;
  switch (random)
  {
  case 0:
    addToHistoricRandomRGB(flip_vertical_rgb(imgRGB), FLIP_VERTICAL);
    break;
  case 1:
    addToHistoricRandomRGB(flip_horizontal_rgb(imgRGB), FLIP_HORIZONTAL);
    break;
  case 2:
    addToHistoricRandomRGB(transpose_rgb(imgRGB), TRANSPOSE);
    break;
  case 3:
    addToHistoricRandomRGB(clahe_rgb(imgRGB, 512, 512), CLAHE);
    break;
  case 4:
    addToHistoricRandomRGB(median_blur_rgb(imgRGB, 9), MEDIAN_BLUR);
    break;
  case 5:
    addToHistoricRandomRGB(add90_rotation_RGB(imgRGB), FLIP_ADD90);
    break;
  case 6:
    addToHistoricRandomRGB(neq90_rotation_RGB(imgRGB), FLIP_NEQ90);
    break;
  default:
    break;
  }
}

void addToHistoricRandomRGB(ImageRGB *imgRGB, FuncUsed funcUsed)
{
  ImgHistoricoRandomRGB *new = (ImgHistoricoRandomRGB *)malloc(sizeof(ImgHistoricoRandomRGB));
  new->imgRGB = imgRGB;
  new->next = NULL;
  new->funcUsed = funcUsed;

  ImgHistoricoRandomRGB *aux = historicoRandomRGBInicio;
  while (aux->next != NULL)
    aux = aux->next;

  aux->next = new;
}

void showRandomImageWindowRGB()
{
  clearRandomHistoricRGB();
  for (int i = 0; i < 10; i++)
    createRandomHistoricRGB(historicoRGBAtual->imgRGB);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Imagem Aleatória RGB");
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

  mainBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_container_add(GTK_CONTAINER(window), mainBox);

  imageAtual = gtk_image_new();
  gtk_box_pack_start(GTK_BOX(mainBox), imageAtual, TRUE, TRUE, 10);

  buttonBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_box_pack_start(GTK_BOX(mainBox), buttonBox, TRUE, TRUE, 10);

  GtkWidget *space = gtk_label_new(">>>>");
  gtk_box_pack_start(GTK_BOX(buttonBox), space, TRUE, TRUE, 5);

  GtkWidget *randomButton = gtk_button_new_with_label("Próxima Imagem");
  g_signal_connect(randomButton, "clicked", G_CALLBACK(next_image_clicked_rgb), NULL);
  gtk_box_pack_start(GTK_BOX(buttonBox), randomButton, FALSE, TRUE, 5);

  filterLabel = gtk_label_new("Filtro Atual");
  gtk_box_pack_start(GTK_BOX(buttonBox), filterLabel, TRUE, TRUE, 5);

  imageNext = gtk_image_new();
  gtk_box_pack_start(GTK_BOX(mainBox), imageNext, TRUE, TRUE, 10);

  update_images_rgb();
  gtk_widget_show_all(window);
}

void next_image_clicked_rgb(GtkWidget *widget, gpointer data)
{
  (void)widget;
  (void)data;

  historicoRandomRGBAtual = historicoRandomRGBAtual->next;
  update_images_rgb();
}

void update_images_rgb()
{
  if (historicoRandomRGBAtual != NULL)
  {
    GdkPixbuf *pixbufAtual = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, historicoRandomRGBAtual->imgRGB->dim.largura, historicoRandomRGBAtual->imgRGB->dim.altura);
    set_img_to_pixbuf_rgb_random(pixbufAtual, historicoRandomRGBAtual->imgRGB);
    gtk_image_set_from_pixbuf(GTK_IMAGE(imageAtual), pixbufAtual);

    if (historicoRandomRGBAtual->next != NULL)
    {
      modFilterLabel(historicoRandomRGBAtual->next->funcUsed);
      GdkPixbuf *pixbufNext = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, historicoRandomRGBAtual->next->imgRGB->dim.largura, historicoRandomRGBAtual->next->imgRGB->dim.altura);
      set_img_to_pixbuf_rgb_random(pixbufNext, historicoRandomRGBAtual->next->imgRGB);
      gtk_image_set_from_pixbuf(GTK_IMAGE(imageNext), pixbufNext);
    }
    else
    {
      destroy_buttons();
    }
  }
}

void set_img_to_pixbuf_rgb_random(GdkPixbuf *pixbuf, ImageRGB *imgRGB)
{
  guchar *pixels = gdk_pixbuf_get_pixels(pixbuf);

  for (int i = 0; i < imgRGB->dim.altura; i++)
    for (int j = 0; j < imgRGB->dim.largura; j++)
    {
      int index = (i * imgRGB->dim.largura + j) * 3;
      pixels[index] = imgRGB->pixels[i * imgRGB->dim.largura + j].red;
      pixels[index + 1] = imgRGB->pixels[i * imgRGB->dim.largura + j].green;
      pixels[index + 2] = imgRGB->pixels[i * imgRGB->dim.largura + j].blue;
    }
}

void clearRandomHistoricRGB()
{
  ImgHistoricoRandomRGB *ant = historicoRandomRGBInicio;
  while (ant != NULL)
  {
    ImgHistoricoRandomRGB *next = ant->next;
    free(ant->imgRGB);
    free(ant);
    ant = next;
  }
  historicoRandomRGBInicio = (ImgHistoricoRandomRGB *)malloc(sizeof(ImgHistoricoRandomRGB));
  historicoRandomRGBInicio->imgRGB = (ImageRGB *)malloc(sizeof(ImageRGB));
  historicoRandomRGBInicio->imgRGB->dim.altura = historicoRGBAtual->imgRGB->dim.altura;
  historicoRandomRGBInicio->imgRGB->dim.largura = historicoRGBAtual->imgRGB->dim.largura;
  historicoRandomRGBInicio->imgRGB->pixels = (PixelRGB *)malloc(historicoRGBAtual->imgRGB->dim.altura * historicoRGBAtual->imgRGB->dim.largura * sizeof(PixelRGB));
  for (int i = 0; i < historicoRGBAtual->imgRGB->dim.altura; i++)
    for (int j = 0; j < historicoRGBAtual->imgRGB->dim.largura; j++)
    {
      historicoRandomRGBInicio->imgRGB->pixels[i * historicoRGBAtual->imgRGB->dim.largura + j].red = historicoRGBAtual->imgRGB->pixels[i * historicoRGBAtual->imgRGB->dim.largura + j].red;
      historicoRandomRGBInicio->imgRGB->pixels[i * historicoRGBAtual->imgRGB->dim.largura + j].green = historicoRGBAtual->imgRGB->pixels[i * historicoRGBAtual->imgRGB->dim.largura + j].green;
      historicoRandomRGBInicio->imgRGB->pixels[i * historicoRGBAtual->imgRGB->dim.largura + j].blue = historicoRGBAtual->imgRGB->pixels[i * historicoRGBAtual->imgRGB->dim.largura + j].blue;
    }

  historicoRandomRGBInicio->next = NULL;
  historicoRandomRGBInicio->funcUsed = NONE;

  historicoRandomRGBAtual = historicoRandomRGBInicio;
}
