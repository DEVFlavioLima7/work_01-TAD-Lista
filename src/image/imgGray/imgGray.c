#include <imgGray.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ImageGray *create_image_gray(int largura, int altura)
{
  ImageGray *img = malloc(sizeof(ImageGray));

  img->dim.altura = altura;
  img->dim.largura = largura;
  img->pixels = malloc((largura * altura) * sizeof(PixelGray));

  return img;
}
void free_image_gray(ImageGray *image)
{
  if (image)
  {
    free(image->pixels);
    free(image);
  }
}

ImageGray *read_image_gray_from_file(const char *filename)
{
  FILE *file = fopen(filename, "r");
  if (!file)
  {
    // botar sufixo builddir no filename
    char *suf = "builddir/";
    char *new_filename = malloc(strlen(filename) + strlen(suf) + 1);
    strcpy(new_filename, suf);
    strcat(new_filename, filename);
    file = fopen(new_filename, "r");
    if (!file)
    {
      fprintf(stderr, "Não foi possível abrir o arquivo %s\n", filename);
      return NULL;
    }
    free(new_filename);
  }

  int largura, altura;
  fscanf(file, "%d", &altura);
  fscanf(file, "%d", &largura);
  // Leia as dimensões da imagem

  ImageGray *image = create_image_gray(largura, altura);
  if (!image)
  {
    fclose(file);
    return NULL;
  }

  for (int i = 0; i < altura; i++)
  {
    for (int j = 0; j < largura; j++)
    {
      fscanf(file, "%d", &image->pixels[i * largura + j].value);
      fgetc(file);
    }
  }
  fclose(file);
  return image;
}

ImageGray *flip_vertical_gray(ImageGray *image)
{
  if (image == NULL)
  {
    return NULL;
  }
  int largura = image->dim.largura;
  int altura = image->dim.altura;

  // Criando uma nova imagem e armazenando em nova_image.
  ImageGray *nova_imageVertical = create_image_gray(largura, altura);

  if (nova_imageVertical == NULL)
  {
    return NULL;
  }
  // Os pixels da imagem gray original serão copiados na nova imagem, porem invertidos verticalmente
  for (int i = 0; i < altura; ++i)
  {
    for (int x = 0; x < largura; ++x)
    {
      nova_imageVertical->pixels[(altura - 1 - i) * largura + x] = image->pixels[i * largura + x];
    }
  }
  return nova_imageVertical;
}

ImageGray *flip_horizontal_gray(ImageGray *image)
{
 if (image == NULL)
  {
    return NULL;
  }

  int largura = image->dim.largura;
  int altura = image->dim.altura;

  ImageGray *imagem_horizontal = create_image_gray(largura, altura);

  if (imagem_horizontal == NULL)
  {
    return NULL;
  }
  // Os pixels da imagem gray original serão copiados na nova imagem, porem invertidos horizontalmente
  for (int i = 0; i < altura; ++i)
  {
    for (int y = 0; y < largura; ++y)
    {
      imagem_horizontal->pixels[i * largura + (largura - 1 - y)] = image->pixels[i * largura + y];
    }
  }

  return imagem_horizontal;
}

ImageGray *transpose_gray(const ImageGray *image)
{
  int largura = image->dim.largura;
  int altura = image->dim.altura;

  ImageGray *imagem_trasposta = create_image_gray(largura, altura);

  if (imagem_trasposta == NULL)
  {
    return NULL;
  }

  // Os pixels da imagem gray original serão copiados na nova imagem, porem traspostos, trocando
  // os valores das linhas pelos das colunas
  for (int i = 0; i < largura; i++)
  {
    for (int j = 0; j < altura; j++)
    {
      imagem_trasposta->pixels[j * altura + i].value = image->pixels[i * largura + j].value;
    }
  }
  return imagem_trasposta;
  
}

// Funcao para calcular os valores do histograma
int interpolar_bilinear(int cdf11, int cdf12, int cdf21, int cdf22, float dx, float dy)
{
  return (int)((1 - dx) * (1 - dy) * cdf11 +
               dx * (1 - dy) * cdf21 +
               (1 - dx) * dy * cdf12 +
               dx * dy * cdf22);
}

void calcular_histograma(int fim_x, int inicio_x, int fim_y, int inicio_y, int *histograma, const ImageGray *img, int largura)
{
  for (int y = inicio_y; y < fim_y; ++y)
  {
    for (int x = inicio_x; x < fim_x; ++x)
    {
      int valor_pixel = img->pixels[y * largura + x].value;
      histograma[valor_pixel]++;
    }
  }
}

void limite_histograma(int *histograma, int num_blocos, int limite_corte)
{
  int excesso = 0;
  for (int i = 0; i < num_blocos; ++i)
  {
    if (histograma[i] > limite_corte)
    {
      excesso += histograma[i] - limite_corte;
      histograma[i] = limite_corte;
    }
  }
  int incremento = excesso / num_blocos;
  int limite_superior = limite_corte - incremento;
<<<<<<< HEAD

  excesso = 0;
  // aqui o estograma sera ajustado e havera a redestribuicao do excesso
=======
  excesso = 0;
>>>>>>> refs/remotes/origin/main
  for (int i = 0; i < num_blocos; ++i)
  {
    if (histograma[i] > limite_superior)
    {
      excesso += histograma[i] - limite_superior;
      histograma[i] = limite_superior;
    }
    else
    {
      histograma[i] += incremento;
    }
  }
<<<<<<< HEAD

  // destribuicao do excesso que sobrou da primeira destricuicao
=======
>>>>>>> refs/remotes/origin/main
  for (int i = 0; i < num_blocos && excesso > 0; ++i)
  {
    if (histograma[i] < limite_corte)
    {
      histograma[i]++;
      excesso--;
    }
  }
}

void calcular_distribuicao(const int *histograma, int num_blocos, int total_pixels, int *cdf)
{
  cdf[0] = histograma[0];
  for (int i = 1; i < num_blocos; ++i)
  {
    cdf[i] = cdf[i - 1] + histograma[i];
  }
  for (int i = 0; i < num_blocos; ++i)
  {
    cdf[i] = (int)(((float)cdf[i] / total_pixels) * 255.0);
  }
}

void Processar_bloco(int inicio_x, int fim_x, int inicio_y, int fim_y, const ImageGray *imagem, int largura, int altura, int *histograma, int num_bins, int limite_corte, int *cdf)
{
  if (fim_x > largura)
    fim_x = largura;
  if (fim_y > altura)
    fim_y = altura;

  calcular_histograma(fim_x, inicio_x, fim_y, inicio_y, histograma, imagem, largura);
  limite_histograma(histograma, num_bins, limite_corte);

  int regiao_pixels = (fim_x - inicio_x) * (fim_y - inicio_y);
  calcular_distribuicao(histograma, num_bins, regiao_pixels, cdf);
}

ImageGray *clahe_gray(const ImageGray *imagem, int tile_width, int tile_height)
{
  int largura = imagem->dim.largura;
  int altura = imagem->dim.altura;
  int total_pixels = largura * altura;
  int limite_corte = (total_pixels / 256) * 2;
  ImageGray *resultado = create_image_gray(largura, altura);
  if (resultado == NULL)
    return NULL;

  int num_blocos_horizontal = (largura + tile_width - 1) / tile_width;
  int num_blocos_vertical = (altura + tile_height - 1) / tile_height;
  int num_bins = 256;
  int *histograma = (int *)malloc(num_bins * sizeof(int));
  int *cdf = (int *)malloc(num_bins * sizeof(int));

  if (!histograma || !cdf)
  {
    free(histograma);
    free(cdf);
    free_image_gray(resultado);
    return NULL;
  }

  // Agora, cdf_tiles é um vetor unidimensional
  int *cdf_tiles = (int *)malloc(num_blocos_vertical * num_blocos_horizontal * num_bins * sizeof(int));
  if (cdf_tiles == NULL)
  {
    free(histograma);
    free(cdf);
    free_image_gray(resultado);
    return NULL;
  }

  for (int id_vertical = 0; id_vertical < num_blocos_vertical; ++id_vertical)
  {
    for (int id_horizontal = 0; id_horizontal < num_blocos_horizontal; ++id_horizontal)
    {
      for (int i = 0; i < num_bins; i++)
        histograma[i] = 0;

      int inicio_x = id_horizontal * tile_width;
      int inicio_y = id_vertical * tile_height;
      int fim_x = inicio_x + tile_width;
      int fim_y = inicio_y + tile_height;

      Processar_bloco(inicio_x, fim_x, inicio_y, fim_y, imagem, largura, altura, histograma, num_bins, limite_corte, cdf);
      for (int i = 0; i < num_bins; ++i)
      {
        // Calcula o índice correto no vetor cdf_tiles
        cdf_tiles[(id_vertical * num_blocos_horizontal + id_horizontal) * num_bins + i] = cdf[i];
      }
    }
  }

  for (int y = 0; y < altura; ++y)
  {
    for (int x = 0; x < largura; ++x)
    {
      int bloco_x = x / tile_width;
      int bloco_y = y / tile_height;
      float dx = (float)(x % tile_width) / tile_width;
      float dy = (float)(y % tile_height) / tile_height;

      // Ajusta índices para evitar acessar fora dos limites
      int bloco_x_next;
      if (bloco_x + 1 < num_blocos_horizontal)
      {
        bloco_x_next = bloco_x + 1;
      }
      else
      {
        bloco_x_next = bloco_x;
      }

      int bloco_y_next;
      if (bloco_y + 1 < num_blocos_vertical)
      {
        bloco_y_next = bloco_y + 1;
      }
      else
      {
        bloco_y_next = bloco_y;
      }

      int cdf11 = cdf_tiles[(bloco_y * num_blocos_horizontal + bloco_x) * num_bins + imagem->pixels[y * largura + x].value];
      int cdf12 = cdf_tiles[(bloco_y_next * num_blocos_horizontal + bloco_x) * num_bins + imagem->pixels[y * largura + x].value];
      int cdf21 = cdf_tiles[(bloco_y * num_blocos_horizontal + bloco_x_next) * num_bins + imagem->pixels[y * largura + x].value];
      int cdf22 = cdf_tiles[(bloco_y_next * num_blocos_horizontal + bloco_x_next) * num_bins + imagem->pixels[y * largura + x].value];

      int novo_valor = interpolar_bilinear(cdf11, cdf12, cdf21, cdf22, dx, dy);

      if (novo_valor > 255)
      {
        resultado->pixels[y * largura + x].value = 255;
      }
      else if (novo_valor < 0)
      {
        resultado->pixels[y * largura + x].value = 0;
      }
      else
      {
        resultado->pixels[y * largura + x].value = novo_valor;
      }
    }
  }

  free(cdf_tiles);
  free(histograma);
  free(cdf);

  return resultado;
}

int getPixel(const ImageGray *image, int x, int y)
{
  int largura = image->dim.largura;
  int altura = image->dim.altura;

  if (x < 0)
  {
    x = 0;
  }
  else if (x >= largura)
  {
    x = largura - 1;
  }
  if (y < 0)
  {
    y = 0;
  }
  else if (y >= altura)
  {
    y = altura - 1;
  }

  return image->pixels[y * largura + x].value;
}

int ValorMedio(int *valores, int kernel_size)
{
  for (int i = 0; i < kernel_size * kernel_size - 1; i++)
  {
    for (int j = 0; j < kernel_size * kernel_size - i - 1; j++)
    {
      if (valores[j] > valores[j + 1])
      {
        int aux = valores[j];
        valores[j] = valores[j + 1];
        valores[j + 1] = aux;
      }
    }
  }
  return valores[(kernel_size * kernel_size) / 2];
}

void setPixel(ImageGray *image, int x, int y, int valor)
{
  int largura = image->dim.largura;
  int altura = image->dim.altura;

  if (x < 0)
  {
    x = 0;
  }
  else if (x >= largura)
  {
    x = largura - 1;
  }
  if (y < 0)
  {
    y = 0;
  }
  else if (y >= altura)
  {
    y = altura - 1;
  }

  image->pixels[y * largura + x].value = valor;
}

ImageGray *median_blur_gray(const ImageGray *image, int kernel_size)
{
  int largura = image->dim.largura;
  int altura = image->dim.altura;

  ImageGray *resultado = create_image_gray(largura, altura);
  if (resultado == NULL)
  {
    return NULL;
  }

  for (int y = 0; y < altura; y++)
  {
    for (int x = 0; x < largura; x++)
    {
      int metade_kernel = kernel_size / 2;
      int valores[kernel_size * kernel_size];

      // Preenche o kernel com os valores dos pixels vizinhos
      int indice_kernel = 0;
      for (int ky = 0; ky < kernel_size; ky++)
      {
        for (int kx = 0; kx < kernel_size; kx++)
        {
          int pixelx = x - metade_kernel + kx;
          int pixely = y - metade_kernel + ky;

          valores[indice_kernel++] = getPixel(image, pixelx, pixely);
        }
      }

      // Calcula o valor medio e atribui ao pixel na imagem resultado
      int valor_medio = ValorMedio(valores, kernel_size);
      setPixel(resultado, x, y, valor_medio);
    }
  }

  return resultado;
}

ImageGray *add90_rotation_gray(const ImageGray *image)
{
  int largura = image->dim.largura;
  int altura = image->dim.altura;

  ImageGray *resultado = create_image_gray(altura, largura);

  for (int i = 0; i < altura; i++)
  {
    for (int j = 0; j < largura; j++)
    {
      resultado->pixels[j * altura + (altura - 1 - i)].value = image->pixels[i * largura + j].value;
    }
  }

  return resultado;
}

ImageGray *neq90_rotation_gray(const ImageGray *image)
{
  int largura = image->dim.largura;
  int altura = image->dim.altura;

  ImageGray *resultado = create_image_gray(altura, largura);

  for (int i = 0; i < altura; i++)
  {
    for (int j = 0; j < largura; j++)
    {
      resultado->pixels[(largura - 1 - j) * altura + i].value = image->pixels[i * largura + j].value;
    }
  }

  return resultado;
}

void helloWord()
{
  printf("Hello, World!\n");
}
