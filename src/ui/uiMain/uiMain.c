#include <uiMain.h>
#include <uiRGB.h>
#include <uiGray.h>
#include <historico.h>
#include <gtk/gtk.h>

GtkWidget *aboutDialog;

int start(int argc, char *argv[])
{
  gtk_init(&argc, &argv);

  GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  g_signal_connect(window, "destroy", G_CALLBACK(on_window_destroy), NULL);

  GtkSettings *settings = gtk_settings_get_default();
  g_object_set(settings, "gtk-application-prefer-dark-theme", TRUE, NULL);

  GtkWidget *stack = gtk_stack_new();
  GtkWidget *stackSwitcher = gtk_stack_switcher_new();
  gtk_stack_switcher_set_stack(GTK_STACK_SWITCHER(stackSwitcher), GTK_STACK(stack));

  GtkWidget *headerBar = gtk_header_bar_new();
  gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(headerBar), TRUE);
  gtk_window_set_titlebar(GTK_WINDOW(window), headerBar);

  gtk_header_bar_pack_start(GTK_HEADER_BAR(headerBar), stackSwitcher);

  GtkWidget *mainBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_box_pack_start(GTK_BOX(mainBox), stack, TRUE, TRUE, 1);

  gtk_container_add(GTK_CONTAINER(window), mainBox);

  // About Dialog
  aboutDialog = gtk_about_dialog_new();
  gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(aboutDialog), "Imagem RGB e Gray");
  gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(aboutDialog), "1.0");
  gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(aboutDialog), "Trabalho de Estrutura de Dados I");

  // Adicione a URL do site
  gtk_about_dialog_set_website_label(GTK_ABOUT_DIALOG(aboutDialog), "Github");
  gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(aboutDialog), "https://github.com/rayss4lves/work_01-TAD-Lista");

  // Corrigido: Adicionado uma vírgula no final do array de autores
  gtk_about_dialog_set_authors(GTK_ABOUT_DIALOG(aboutDialog), (const gchar *[]){"Kauã Henrique", "Flavio Lima", "Rayssa Alves", NULL});
  gtk_about_dialog_set_license_type(GTK_ABOUT_DIALOG(aboutDialog), GTK_LICENSE_GPL_3_0);
  gtk_about_dialog_set_wrap_license(GTK_ABOUT_DIALOG(aboutDialog), TRUE);
  gtk_about_dialog_set_logo_icon_name(GTK_ABOUT_DIALOG(aboutDialog), "emblem-favorite");

  setup_ui_rgb(stack);
  setup_ui_Gray(stack);

  gtk_widget_show_all(window);
  gtk_main();

  return 0;
}

void on_about_button_clicked(GtkWidget *widget, gpointer data)
{
  (void)widget;

  GtkWidget *aboutDialog = GTK_WIDGET(data);
  gtk_dialog_run(GTK_DIALOG(aboutDialog));
  gtk_widget_hide(aboutDialog);
}

void on_window_destroy(GtkWidget *widget, gpointer data)
{
  (void)widget;
  (void)data;

  removerHistoricoGray();
  removerHistoricoRGB();
  gtk_main_quit();
}
