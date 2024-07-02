#include <uiGray.h>
#include <imgGray.h>
#include <historico.h>
#include <historicoRandom.h>
#include <uiMain.h>

GtkWidget *imageWidgetGray;
GtkWidget *navBoxGray;
GtkWidget *rotateBoxAdd90;
GtkWidget *rotateBoxNeq90;
GtkWidget *counterLabelNeq90;
GtkWidget *counterLabelAdd90;

GtkWidget *forwardButtonGray;
GtkWidget *backButtonGray;

GtkWidget *flipHorizontalButtonGray;
GtkWidget *flipVerticalButtonGray;
GtkWidget *transposeButtonGray;
GtkWidget *claheButtonGray;
GtkWidget *medianBlurButtonGray;

static void atualizarToggleBotao(GtkWidget *button, gboolean estado, GCallback callback)
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
  atualizarToggleBotao(claheButtonGray, historicoGrayAtual->buttonStatus.clahe, G_CALLBACK(on_clahe_gray_button_clicked));
  atualizarToggleBotao(medianBlurButtonGray, historicoGrayAtual->buttonStatus.median_blur, G_CALLBACK(on_median_blur_gray_button_clicked));

  gtk_label_set_text(GTK_LABEL(counterLabelAdd90), g_strdup_printf("%d", historicoGrayAtual->buttonStatus.qtdFlipAdd90));
  gtk_label_set_text(GTK_LABEL(counterLabelNeq90), g_strdup_printf("%d", historicoGrayAtual->buttonStatus.qtdFlipNeq90));

  gtk_widget_set_sensitive(forwardButtonGray, historicoGrayAtual->next != NULL);
  gtk_widget_set_sensitive(backButtonGray, historicoGrayAtual->prev != NULL);
}

void set_img_to_pixbuf_gray(GdkPixbuf *pixbuf)
{
  guchar *pixels = gdk_pixbuf_get_pixels(pixbuf);

  for (int i = 0; i < historicoGrayAtual->imgGray->dim.altura; i++)
    for (int j = 0; j < historicoGrayAtual->imgGray->dim.largura; j++)
    {
      int index = (i * historicoGrayAtual->imgGray->dim.largura + j) * 3;
      pixels[index] = historicoGrayAtual->imgGray->pixels[i * historicoGrayAtual->imgGray->dim.largura + j].value;
      pixels[index + 1] = historicoGrayAtual->imgGray->pixels[i * historicoGrayAtual->imgGray->dim.largura + j].value;
      pixels[index + 2] = historicoGrayAtual->imgGray->pixels[i * historicoGrayAtual->imgGray->dim.largura + j].value;
    }
}

static void refresh_image_gray()
{
  GdkPixbuf *pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, historicoGrayAtual->imgGray->dim.largura, historicoGrayAtual->imgGray->dim.altura);
  set_img_to_pixbuf_gray(pixbuf);

  gtk_image_set_from_pixbuf(GTK_IMAGE(imageWidgetGray), pixbuf);
  gtk_widget_show(imageWidgetGray);
}

static void on_flip_horizontal_gray_button_clicked(GtkWidget *widget, gpointer data)
{
  (void)widget;
  (void)data;

  ImageGray *newImage = flip_horizontal_gray(historicoGrayAtual->imgGray);
  adicionarHistoricoGray(newImage, FLIP_HORIZONTAL);

  refresh_image_gray();
  verificarBotoes();
}

static void on_flip_vertical_gray_button_clicked(GtkWidget *widget, gpointer data)
{
  (void)widget;
  (void)data;

  ImageGray *newImage = flip_vertical_gray(historicoGrayAtual->imgGray);
  adicionarHistoricoGray(newImage, FLIP_VERTICAL);

  refresh_image_gray();
  verificarBotoes();
}

static void on_transpose_gray_button_clicked(GtkWidget *widget, gpointer data)
{
  (void)widget;
  (void)data;

  ImageGray *newImage = transpose_gray(historicoGrayAtual->imgGray);
  adicionarHistoricoGray(newImage, TRANSPOSE);

  refresh_image_gray();
  verificarBotoes();
}

static void on_clahe_gray_button_clicked(GtkWidget *widget, gpointer data)
{
  (void)widget;
  (void)data;

  ImageGray *newImage;

  if (historicoGrayAtual->buttonStatus.clahe)
  {
    newImage = refrashHistoricFuncGray(CLAHE);
    adicionarHistoricoGray(newImage, CLAHE);
  }

  else if (!historicoGrayAtual->buttonStatus.clahe)
  {
    newImage = clahe_gray(historicoGrayAtual->imgGray, 256, 256);
    adicionarHistoricoGray(newImage, CLAHE);
  }

  refresh_image_gray();
  verificarBotoes();
}

static void on_median_blur_gray_button_clicked(GtkWidget *widget, gpointer data)
{
  (void)widget;
  (void)data;

  if (historicoGrayAtual->buttonStatus.median_blur)
  {
    ImageGray *newImage = refrashHistoricFuncGray(MEDIAN_BLUR);
    adicionarHistoricoGray(newImage, MEDIAN_BLUR);
  }
  else if (!historicoGrayAtual->buttonStatus.median_blur)
  {
    ImageGray *newImage = median_blur_gray(historicoGrayAtual->imgGray, 9);
    adicionarHistoricoGray(newImage, MEDIAN_BLUR);
  }

  refresh_image_gray();
  verificarBotoes();
}

static void on_add90_rotation_gray_clicked(GtkWidget *widget, gpointer data)
{
  (void)widget;
  (void)data;

  ImageGray *newImage = add90_rotation_gray(historicoGrayAtual->imgGray);
  adicionarHistoricoGray(newImage, FLIP_ADD90);

  refresh_image_gray();
  verificarBotoes();
}

static void on_neq90_rotation_gray_clicked(GtkWidget *widget, gpointer data)
{
  (void)widget;
  (void)data;

  ImageGray *newImage = neq90_rotation_gray(historicoGrayAtual->imgGray);
  adicionarHistoricoGray(newImage, FLIP_NEQ90);

  refresh_image_gray();
  verificarBotoes();
}

static void on_back_button_clicked(GtkWidget *widget, gpointer data)
{
  (void)widget;
  (void)data;

  VoltarHistoricoGray();

  refresh_image_gray();
  verificarBotoes();
}

static void on_forward_button_clicked(GtkWidget *widget, gpointer data)
{
  (void)widget;
  (void)data;

  SeguirHistoricoGray();

  refresh_image_gray();
  verificarBotoes();
}

void on_random_image_gray_clicked(GtkWidget *widget, gpointer data)
{
  (void)widget;
  (void)data;

  showRandomImageWindowGray();
}

void setup_ui_Gray(GtkWidget *stack)
{
  iniciarHistoricoGray();

  // RGB Image Handling
  GtkWidget *mainBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_stack_add_titled(GTK_STACK(stack), mainBox, "Manipulação Gray", "Manipulação Gray");

  GtkWidget *imageBoxRGB = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_box_pack_start(GTK_BOX(mainBox), imageBoxRGB, TRUE, TRUE, 10);

  GdkPixbuf *pixbufGray = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, historicoRGBAtual->imgRGB->dim.largura, historicoRGBAtual->imgRGB->dim.altura);
  set_img_to_pixbuf_gray(pixbufGray);

  imageWidgetGray = gtk_image_new_from_pixbuf(pixbufGray);
  gtk_box_pack_start(GTK_BOX(imageBoxRGB), imageWidgetGray, TRUE, TRUE, 0);

  // Botões no menuBox
  GtkWidget *menuBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_box_pack_start(GTK_BOX(mainBox), menuBox, TRUE, TRUE, 10);

  rotateBoxAdd90 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_box_pack_start(GTK_BOX(menuBox), rotateBoxAdd90, TRUE, TRUE, 5);

  GtkWidget *rotate90ButtonGray = gtk_button_new_with_label("Rotate 90º");
  g_signal_connect(rotate90ButtonGray, "clicked", G_CALLBACK(on_add90_rotation_gray_clicked), NULL);
  gtk_box_pack_start(GTK_BOX(rotateBoxAdd90), rotate90ButtonGray, TRUE, TRUE, 5);

  counterLabelAdd90 = gtk_label_new("0");
  gtk_box_pack_start(GTK_BOX(rotateBoxAdd90), counterLabelAdd90, TRUE, TRUE, 5);

  rotateBoxNeq90 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_box_pack_start(GTK_BOX(menuBox), rotateBoxNeq90, TRUE, TRUE, 5);

  GtkWidget *rotate_90ButtonGray = gtk_button_new_with_label("Rotate -90º");
  g_signal_connect(rotate_90ButtonGray, "clicked", G_CALLBACK(on_neq90_rotation_gray_clicked), NULL);
  gtk_box_pack_start(GTK_BOX(rotateBoxNeq90), rotate_90ButtonGray, TRUE, TRUE, 5);

  counterLabelNeq90 = gtk_label_new("0");
  gtk_box_pack_start(GTK_BOX(rotateBoxNeq90), counterLabelNeq90, TRUE, TRUE, 5);

  flipHorizontalButtonGray = gtk_button_new_with_label("Flip Horizontal");
  g_signal_connect(flipHorizontalButtonGray, "clicked", G_CALLBACK(on_flip_horizontal_gray_button_clicked), NULL);
  gtk_box_pack_start(GTK_BOX(menuBox), flipHorizontalButtonGray, TRUE, TRUE, 5);

  flipVerticalButtonGray = gtk_button_new_with_label("Flip Vertical");
  g_signal_connect(flipVerticalButtonGray, "clicked", G_CALLBACK(on_flip_vertical_gray_button_clicked), NULL);
  gtk_box_pack_start(GTK_BOX(menuBox), flipVerticalButtonGray, TRUE, TRUE, 5);

  transposeButtonGray = gtk_button_new_with_label("Transpose");
  g_signal_connect(transposeButtonGray, "clicked", G_CALLBACK(on_transpose_gray_button_clicked), NULL);
  gtk_box_pack_start(GTK_BOX(menuBox), transposeButtonGray, TRUE, TRUE, 5);

  claheButtonGray = gtk_toggle_button_new_with_label("filtro CLAHE");
  g_signal_connect(claheButtonGray, "toggled", G_CALLBACK(on_clahe_gray_button_clicked), NULL);
  gtk_box_pack_start(GTK_BOX(menuBox), claheButtonGray, TRUE, TRUE, 5);

  medianBlurButtonGray = gtk_toggle_button_new_with_label("filtro Median Blur");
  g_signal_connect(medianBlurButtonGray, "toggled", G_CALLBACK(on_median_blur_gray_button_clicked), NULL);
  gtk_box_pack_start(GTK_BOX(menuBox), medianBlurButtonGray, TRUE, TRUE, 5);

  GtkWidget *randomButton = gtk_button_new_with_label("---Imagem Aleatória---");
  g_signal_connect(randomButton, "clicked", G_CALLBACK(on_random_image_gray_clicked), NULL);
  gtk_box_pack_start(GTK_BOX(menuBox), randomButton, TRUE, TRUE, 5);

  navBoxGray = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_box_pack_start(GTK_BOX(menuBox), navBoxGray, TRUE, TRUE, 5);

  backButtonGray = gtk_button_new_with_label("<");
  g_signal_connect(backButtonGray, "clicked", G_CALLBACK(on_back_button_clicked), NULL);
  gtk_box_pack_start(GTK_BOX(navBoxGray), backButtonGray, TRUE, TRUE, 5);

  GtkWidget *aboutButton = gtk_button_new_with_label("❤️");
  g_signal_connect(aboutButton, "clicked", G_CALLBACK(on_about_button_clicked), aboutDialog);
  gtk_box_pack_start(GTK_BOX(navBoxGray), aboutButton, TRUE, TRUE, 5);

  forwardButtonGray = gtk_button_new_with_label(">");
  g_signal_connect(forwardButtonGray, "clicked", G_CALLBACK(on_forward_button_clicked), NULL);
  gtk_box_pack_start(GTK_BOX(navBoxGray), forwardButtonGray, TRUE, TRUE, 5);

  verificarBotoes();
}
