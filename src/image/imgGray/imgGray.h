#pragma once

#include <imgStruct.h>

void helloWord();

// Funções de criação e liberação
ImageGray *create_image_gray(int largura, int altura);
void free_image_gray(ImageGray *image);
ImageGray *read_image_gray_from_file(const char *filename);

// Operações para ImageGray
ImageGray *flip_vertical_gray(ImageGray *image);
ImageGray *flip_horizontal_gray(ImageGray *image);
ImageGray *transpose_gray(const ImageGray *image);
// Manipulação por pixel para ImageGray

// funçoes auxiliares para filtro clahe_gray

void calcular_histograma(int fim_x, int inicio_x, int fim_y, int inicio_y, int *histograma, const ImageGray *img, int largura);
void limite_Histograma(int *histograma, float limite);
void calcular_destribuicao(const int *histograma, int num_blocos, int total_pixels, int *cdf);
void equalizar_regiao(PixelGray *regiao, int width, int height, const float *cfd);
void Processar_bloco(int inicio_x, int fim_x, int inicio_y, int fim_y, const ImageGray *imagem, int largura, int altura, int *histograma, int num_bins, int limite_corte, int *cdf);
int interpolar_bilinear(int cdf11, int cdf12, int cdf21, int cdf22, float dx, float dy);
ImageGray *clahe_gray(const ImageGray *image, int tile_width, int tile_height);

// funçoes auxiliares para filtro median_blur_gray
int getPixel(const ImageGray *image, int x, int y);
void setPixel(ImageGray *image, int x, int y, int valor);
int ValorMedio(int *valores, int kernel_size);
ImageGray *median_blur_gray(const ImageGray *image, int kernel_size);

ImageGray *add90_rotation_gray(const ImageGray *image);
ImageGray *neq90_rotation_gray(const ImageGray *image);