#include <uiRGB.h>
#include <uiHistorico.h>
#include <imgRGB.h>
#include <uiMain.h>
#include <gtk/gtk.h>

GtkWidget *imageWidgetRGB;
GtkWidget *navBox;

GtkWidget *forwardButton;
GtkWidget *backButton;
GtkWidget *flipHorizontalButtonRGB;
GtkWidget *flipVerticalButtonRGB;
GtkWidget *transposeButtonRGB;
GtkWidget *claheButtonRGB;
GtkWidget *medianBlurButtonRGB;

GtkWidget *rotateBoxRGBAdd90;
GtkWidget *labelContRotateAdd90RGB;
GtkWidget *rotateBoxRGBneq90;
GtkWidget *labelContRotateNeq90RGB;

void setup_ui_rgb(GtkWidget *stack)
{
  iniciarHistoricoRGB();

  // RGB Image Handling
  GtkWidget *mainBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_stack_add_titled(GTK_STACK(stack), mainBox, "Manipulação RGB", "Manipulação RGB");

  GtkWidget *imageBoxRGB = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_box_pack_start(GTK_BOX(mainBox), imageBoxRGB, FALSE, FALSE, 10);

  GdkPixbuf *pixbufRGB = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, historicoRGBAtual->imgRGB->dim.largura, historicoRGBAtual->imgRGB->dim.altura);
  set_img_to_pixbuf_rgb(pixbufRGB);

  imageWidgetRGB = gtk_image_new_from_pixbuf(pixbufRGB);
  gtk_box_pack_start(GTK_BOX(imageBoxRGB), imageWidgetRGB, FALSE, FALSE, 0);

  // Botões no menuBox
  GtkWidget *menuBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_box_pack_start(GTK_BOX(mainBox), menuBox, TRUE, TRUE, 10);

  rotateBoxRGBAdd90 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_box_pack_start(GTK_BOX(menuBox), rotateBoxRGBAdd90, TRUE, TRUE, 5);

  GtkWidget *rotate90ButtonRGB = gtk_button_new_with_label("Rotate 90º");
  g_signal_connect(rotate90ButtonRGB, "clicked", G_CALLBACK(on_add90_rotation_rgb_clicked), NULL);
  gtk_box_pack_start(GTK_BOX(rotateBoxRGBAdd90), rotate90ButtonRGB, TRUE, TRUE, 5);

  labelContRotateAdd90RGB = gtk_label_new("0");
  gtk_box_pack_start(GTK_BOX(rotateBoxRGBAdd90), labelContRotateAdd90RGB, TRUE, TRUE, 5);

  rotateBoxRGBneq90 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_box_pack_start(GTK_BOX(menuBox), rotateBoxRGBneq90, TRUE, TRUE, 5);

  GtkWidget *rotate_90ButtonRGB = gtk_button_new_with_label("Rotate -90º");
  g_signal_connect(rotate_90ButtonRGB, "clicked", G_CALLBACK(on_neq90_rotation_rgb_clicked), NULL);
  gtk_box_pack_start(GTK_BOX(rotateBoxRGBneq90), rotate_90ButtonRGB, TRUE, TRUE, 5);

  labelContRotateNeq90RGB = gtk_label_new("0");
  gtk_box_pack_start(GTK_BOX(rotateBoxRGBneq90), labelContRotateNeq90RGB, TRUE, TRUE, 5);

  flipHorizontalButtonRGB = gtk_button_new_with_label("Flip Horizontal");
  g_signal_connect(flipHorizontalButtonRGB, "clicked", G_CALLBACK(on_flip_horizontal_rgb_button_clicked), NULL);
  gtk_box_pack_start(GTK_BOX(menuBox), flipHorizontalButtonRGB, TRUE, TRUE, 5);

  flipVerticalButtonRGB = gtk_button_new_with_label("Flip Vertical");
  g_signal_connect(flipVerticalButtonRGB, "clicked", G_CALLBACK(on_flip_vertical_rgb_button_clicked), NULL);
  gtk_box_pack_start(GTK_BOX(menuBox), flipVerticalButtonRGB, TRUE, TRUE, 5);

  transposeButtonRGB = gtk_button_new_with_label("Transpose");
  g_signal_connect(transposeButtonRGB, "clicked", G_CALLBACK(on_transpose_rgb_button_clicked), NULL);
  gtk_box_pack_start(GTK_BOX(menuBox), transposeButtonRGB, TRUE, TRUE, 5);

  claheButtonRGB = gtk_toggle_button_new_with_label("filtro CLAHE");
  g_signal_connect(claheButtonRGB, "clicked", G_CALLBACK(on_filter_clahe_rgb_button_clicked), NULL);
  gtk_box_pack_start(GTK_BOX(menuBox), claheButtonRGB, TRUE, TRUE, 5);

  medianBlurButtonRGB = gtk_toggle_button_new_with_label("filtro Median Blur");
  g_signal_connect(medianBlurButtonRGB, "clicked", G_CALLBACK(on_filter_median_rgb_button_clicked), NULL);
  gtk_box_pack_start(GTK_BOX(menuBox), medianBlurButtonRGB, TRUE, TRUE, 5);

  GtkWidget *randomButton = gtk_button_new_with_label("---Imagem Aleatória---");
  g_signal_connect(randomButton, "clicked", G_CALLBACK(on_random_button_clicked), NULL);
  gtk_box_pack_start(GTK_BOX(menuBox), randomButton, TRUE, TRUE, 5);

  navBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_box_pack_start(GTK_BOX(menuBox), navBox, TRUE, TRUE, 5);

  backButton = gtk_button_new_with_label("<");
  g_signal_connect(backButton, "clicked", G_CALLBACK(on_back_button_clicked), navBox);
  gtk_box_pack_start(GTK_BOX(navBox), backButton, TRUE, TRUE, 5);

  GtkWidget *aboutButton = gtk_button_new_with_label("❤️");
  g_signal_connect(aboutButton, "clicked", G_CALLBACK(on_about_button_clicked), aboutDialog);
  gtk_box_pack_start(GTK_BOX(navBox), aboutButton, TRUE, TRUE, 5);

  forwardButton = gtk_button_new_with_label(">");
  g_signal_connect(forwardButton, "clicked", G_CALLBACK(on_forward_button_clicked), navBox);
  gtk_box_pack_start(GTK_BOX(navBox), forwardButton, TRUE, TRUE, 5);

  verificarBotoes();
}

static void on_random_button_clicked(GtkWidget *widget, gpointer data)
{
  (void)widget;
  (void)data;

  if(rand() % 2)
    on_add90_rotation_rgb_clicked(NULL, NULL);

  if(rand() % 2)
    on_neq90_rotation_rgb_clicked(NULL, NULL);

  if(rand() % 2)
    on_flip_horizontal_rgb_button_clicked(NULL, NULL);

  if(rand() % 2)
    on_flip_vertical_rgb_button_clicked(NULL, NULL);

  if(rand() % 2)
    on_transpose_rgb_button_clicked(NULL, NULL);
  
  if(rand() % 2)
    on_filter_clahe_rgb_button_clicked(NULL, NULL);
  
  if(rand() % 2)
    on_filter_median_rgb_button_clicked(NULL, NULL);

  atualizarImagem();
  verificarBotoes();
}

static void atualizarToggleBotaoRGB(GtkWidget *button, gboolean estado, GCallback callback)
{
  gulong handler_id = g_signal_handler_find(G_OBJECT(button), G_SIGNAL_MATCH_FUNC, 0, 0, NULL, callback, NULL);
  if (handler_id)
    g_signal_handler_block(G_OBJECT(button), handler_id);

  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), estado);

  if (handler_id)
    g_signal_handler_unblock(G_OBJECT(button), handler_id);
}

static void verificarBotoes()
{
  atualizarToggleBotaoRGB(claheButtonRGB, historicoRGBAtual->buttonStatus.clahe, G_CALLBACK(on_filter_clahe_rgb_button_clicked));
  atualizarToggleBotaoRGB(medianBlurButtonRGB, historicoRGBAtual->buttonStatus.median_blur, G_CALLBACK(on_filter_median_rgb_button_clicked));

  // atualizar labels de rotação de acordo com o historico
  gtk_label_set_text(GTK_LABEL(labelContRotateAdd90RGB), g_strdup_printf("%d", historicoRGBAtual->buttonStatus.qtdFlipAdd90));
  gtk_label_set_text(GTK_LABEL(labelContRotateNeq90RGB), g_strdup_printf("%d", historicoRGBAtual->buttonStatus.qtdFlipNeq90));

  // Ajustar a sensibilidade dos outros botões
  gtk_widget_set_sensitive(forwardButton, historicoRGBAtual->next != NULL);
  gtk_widget_set_sensitive(backButton, historicoRGBAtual->prev != NULL);
}

static void set_img_to_pixbuf_rgb(GdkPixbuf *pixbuf)
{
  guchar *pixels = gdk_pixbuf_get_pixels(pixbuf);

  for (int i = 0; i < historicoRGBAtual->imgRGB->dim.altura; i++)
    for (int j = 0; j < historicoRGBAtual->imgRGB->dim.largura; j++)
    {
      int index = (i * historicoRGBAtual->imgRGB->dim.largura + j) * 3;
      pixels[index] = historicoRGBAtual->imgRGB->pixels[i * historicoRGBAtual->imgRGB->dim.largura + j].red;
      pixels[index + 1] = historicoRGBAtual->imgRGB->pixels[i * historicoRGBAtual->imgRGB->dim.largura + j].green;
      pixels[index + 2] = historicoRGBAtual->imgRGB->pixels[i * historicoRGBAtual->imgRGB->dim.largura + j].blue;
    }
}

static void atualizarImagem()
{
  GdkPixbuf *pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, historicoRGBAtual->imgRGB->dim.largura, historicoRGBAtual->imgRGB->dim.altura);
  set_img_to_pixbuf_rgb(pixbuf);

  gtk_image_set_from_pixbuf(GTK_IMAGE(imageWidgetRGB), pixbuf);
  gtk_widget_show(imageWidgetRGB);
}

static void on_flip_horizontal_rgb_button_clicked(GtkWidget *widget, gpointer data)
{
  (void)widget;
  (void)data;

  ImageRGB *newImgRGB = flip_horizontal_rgb(historicoRGBAtual->imgRGB);
  adicionarHistoricoRGB(newImgRGB, FLIP_HORIZONTAL);

  atualizarImagem();
  verificarBotoes();
}

static void on_flip_vertical_rgb_button_clicked(GtkWidget *widget, gpointer data)
{
  (void)widget;
  (void)data;

  ImageRGB *newImgRGB = flip_vertical_rgb(historicoRGBAtual->imgRGB);
  adicionarHistoricoRGB(newImgRGB, FLIP_VERTICAL);

  atualizarImagem();
  verificarBotoes();
}

static void on_transpose_rgb_button_clicked(GtkWidget *widget, gpointer data)
{
  (void)widget;
  (void)data;

  ImageRGB *newImgRGB = transpose_rgb(historicoRGBAtual->imgRGB);
  adicionarHistoricoRGB(newImgRGB, TRANSPOSE);

  atualizarImagem();
  verificarBotoes();
}

static void on_filter_clahe_rgb_button_clicked(GtkWidget *widget, gpointer data)
{
  (void)widget;
  (void)data;

  if(historicoRGBAtual->buttonStatus.clahe)
  {
    ImageRGB *newImgRGB = refrashHistoricFuncRGB(CLAHE);
    adicionarHistoricoRGB(newImgRGB, CLAHE);
  }
  else
  {
    ImageRGB *newImgRGB = clahe_rgb(historicoRGBAtual->imgRGB, 256, 256);
    adicionarHistoricoRGB(newImgRGB, CLAHE);
  }

  atualizarImagem();
  verificarBotoes();
}

static void on_filter_median_rgb_button_clicked(GtkWidget *widget, gpointer data)
{
  (void)widget;
  (void)data;

  if(historicoRGBAtual->buttonStatus.median_blur)
  {
    ImageRGB *newImgRGB = refrashHistoricFuncRGB(MEDIAN_BLUR);
    adicionarHistoricoRGB(newImgRGB, MEDIAN_BLUR);
  }
  else
  {
    ImageRGB *newImgRGB = median_blur_rgb(historicoRGBAtual->imgRGB, 3);
    adicionarHistoricoRGB(newImgRGB, MEDIAN_BLUR);
  }

  atualizarImagem();
  verificarBotoes();
}

static void on_add90_rotation_rgb_clicked(GtkWidget *widget, gpointer data)
{
  (void)widget;
  (void)data;

  ImageRGB *newImgRGB = add90_rotation_RGB(historicoRGBAtual->imgRGB);
  adicionarHistoricoRGB(newImgRGB, FLIP_ADD90);

  atualizarImagem();
  verificarBotoes();
}

static void on_neq90_rotation_rgb_clicked(GtkWidget *widget, gpointer data)
{
  (void)widget;
  (void)data;

  ImageRGB *newImgRGB = neq90_rotation_RGB(historicoRGBAtual->imgRGB);
  adicionarHistoricoRGB(newImgRGB, FLIP_NEQ90);

  atualizarImagem();
  verificarBotoes();
}

static void on_back_button_clicked(GtkWidget *widget, gpointer data)
{
  (void)widget;
  (void)data;

  VoltarHistoricoRGB();

  atualizarImagem();
  verificarBotoes();
}

static void on_forward_button_clicked(GtkWidget *widget, gpointer data)
{
  (void)widget;
  (void)data;

  SeguirHistoricoRGB();

  atualizarImagem();
  verificarBotoes();
}
