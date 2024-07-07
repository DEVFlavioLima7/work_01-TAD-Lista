#include <imgRGB.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//Aloca memória para a estrutura ImageRGB.
//Aloca memória para os pixels da imagem.
//Retorna o ponteiro para a nova imagem ou NULL em caso de falha.
ImageRGB *create_image_rgb(int largura, int altura)
{
    ImageRGB *image = (ImageRGB *)malloc(sizeof(ImageRGB));
    if (image == NULL)
    {
        return NULL;
    }
    image->dim.largura = largura;
    image->dim.altura = altura;

    image->pixels = (PixelRGB *)malloc(largura * altura * sizeof(PixelRGB));
    if (image->pixels == NULL)
    {
        free(image);
        return NULL;
    }
    return image;
}
//erifica se a imagem é NULL.
//Libera a memória dos pixels se não for NULL.
//Libera a estrutura da imagem.
void free_image_rgb(ImageRGB *image)
{
    if (image == NULL)
    {
        return;
    }
    else if (image->pixels != NULL)
    {
        free(image->pixels);
    }

    free(image);
}

//Cria uma nova imagem com as mesmas dimensões.
//Copia os pixels da imagem original para a nova imagem de forma invertida verticalmente.
ImageRGB *flip_vertical_rgb(const ImageRGB *image)
{
    if (image == NULL)
    {
        return NULL;
    }
    int largura = image->dim.largura;
    int altura = image->dim.altura;

    // to criando uma nova imagem e armazenando em nova_image.
    ImageRGB *nova_imageVertical = create_image_rgb(largura, altura);

    if (nova_imageVertical == NULL)
    {
        return NULL;
    }
    // aqui ira copiar os pixels da imagem original para a nova imagem e inverter ela verticalmente.
    for (int i = 0; i < altura; ++i)
    {
        for (int x = 0; x < largura; ++x)
        {
            nova_imageVertical->pixels[(altura - 1 - i) * largura + x] = image->pixels[i * largura + x];
        }
    }
    return nova_imageVertical;
}
//a mesma da vertical.
ImageRGB *flip_horizontal_rgb(const ImageRGB *image)
{
    if (image == NULL)
    {
        return NULL;
    }

    int largura = image->dim.largura;
    int altura = image->dim.altura;

    ImageRGB *nova_imagem_horizontal = create_image_rgb(largura, altura);

    if (nova_imagem_horizontal == NULL)
    {
        return NULL;
    }

    for (int i = 0; i < altura; ++i)
    {
        for (int y = 0; y < largura; ++y)
        {
            nova_imagem_horizontal->pixels[i * largura + (largura - 1 - y)] = image->pixels[i * largura + y];
        }
    }
    return nova_imagem_horizontal;
}

//Cria uma nova imagem que é a transposta da imagem original (troca linhas por colunas).
//Cria uma nova imagem com largura e altura invertidas.
//Copia os pixels da imagem original para a nova imagem de forma transposta.

ImageRGB *transpose_rgb(const ImageRGB *image){
    if(image==NULL){
        return NULL;

    }
    int largura=image->dim.largura;
    int altura=image->dim.altura;

    ImageRGB*transpose_image=create_image_rgb(altura,largura);

    if(transpose_image==NULL){
        return NULL;
    }

    for(int i=0;i<altura;++i){
        for(int y=0;y<largura;++y){
            transpose_image->pixels[y*altura+i]=image->pixels[i*largura+y];
        }
    }
    return  transpose_image;
}

// Cria uma nova imagem rotacionada 90 graus no sentido horário.
//Cria uma nova imagem com largura e altura invertidas.
//Copia os pixels da imagem original para a nova imagem de forma rotacionada 90 graus.

ImageRGB *add90_rotation_RGB(const ImageRGB *image)
{
    if(image==NULL){
        return NULL;
    }
    int largura =image->dim.largura;
    int altura=image->dim.altura;

    ImageRGB *newImg =create_image_rgb(altura,largura);
    if(newImg==NULL){
        return NULL;
    }
    for(int i=0;i<altura;++i){
        for(int y=0;y<largura;++y){
            newImg->pixels[y*altura + (altura-1-i)]=image->pixels[i*largura+y];
        }
    }

    return newImg;
}
//a mesma so que anti-horario
ImageRGB *neq90_rotation_RGB(const ImageRGB *image)
{
    if(image==NULL){
        return NULL;
    }
    int largura=image->dim.largura;
    int altura=image->dim.altura;

    ImageRGB *newImg = create_image_rgb(altura,largura);
    if(newImg==NULL){
        return NULL;
    }
    for(int i=0;i<altura;++i){
        for(int y=0;y<largura;++y){
            newImg->pixels[(largura-1-y)*altura+i]=image->pixels[i*largura+y];
        }
    }
    return newImg;
}
//Calcula o histograma de uma região da imagem para um canal de cor específico.
//Percorre a região especificada da imagem e incrementa o histograma com os valores dos pixels.

void calcula_histograma_rgb(int fim_x, int inicio_x, int fim_y, int inicio_y, int *histograma, const ImageRGB *img, int largura, char canal) {
    for (int y = inicio_y; y < fim_y; ++y) {
        for (int x = inicio_x; x < fim_x; ++x) {
            int valor_pixel;
            if (canal == 'r') {
                valor_pixel = img->pixels[y * largura + x].red;
            } else if (canal == 'g') {
                valor_pixel = img->pixels[y * largura + x].green;
            } else {
                valor_pixel = img->pixels[y * largura + x].blue;
            }
            histograma[valor_pixel]++;
        }
    }
}

void limite_histograma_rgb(int *histograma, int num_bins, int limite_corte) {
    int excesso = 0;

    for (int i = 0; i < num_bins; ++i) {
        if (histograma[i] > limite_corte) {
            excesso += histograma[i] - limite_corte;
            histograma[i] = limite_corte;
        }
    }

    int incremento = excesso / num_bins;
    int limite_superior = limite_corte - incremento;

    for (int i = 0; i < num_bins; ++i) {
        if (histograma[i] > limite_superior) {
            excesso += histograma[i] - limite_superior;
            histograma[i] = limite_superior;
        } else {
            histograma[i] += incremento;
            excesso -= incremento;
        }
    }

    for (int i = 0; i < num_bins && excesso > 0; ++i) {
        if (histograma[i] < limite_corte) {
            histograma[i]++;
            excesso--;
        }
    }
}

void calcular_distribuicao_rgb(const int *histograma, int num_bins, int total_pixels, int *cdf) {
    cdf[0] = histograma[0];

    for (int i = 1; i < num_bins; ++i) {
        cdf[i] = cdf[i - 1] + histograma[i];
    }

    for (int i = 0; i < num_bins; ++i) {
        cdf[i] = (int)(((float)cdf[i] / total_pixels) * 255.0f);
    }
}



ImageRGB *clahe_rgb(const ImageRGB *image, int tile_width, int tile_height)
{
   if (image == NULL || tile_width <= 0 || tile_height <= 0) {
    return NULL;
}

int largura = image->dim.largura;
int altura = image->dim.altura;
int total_pixels = largura * altura;
int limite_corte = (total_pixels / 256) * 2;

ImageRGB *resultado = create_image_rgb(largura, altura);
if (resultado == NULL) {
    return NULL;
}

int num_blocos_horizontal = (largura + tile_width - 1) / tile_width;
int num_blocos_vertical = (altura + tile_height - 1) / tile_height;

int num_bins = 256;
int ***histogramas_blocos = (int ***)malloc(3 * sizeof(int **));
int ***cdf_blocos = (int ***)malloc(3 * sizeof(int **));
for (int canal = 0; canal < 3; ++canal) {
    histogramas_blocos[canal] = (int **)malloc(num_blocos_horizontal * num_blocos_vertical * sizeof(int *));
    cdf_blocos[canal] = (int **)malloc(num_blocos_horizontal * num_blocos_vertical * sizeof(int *));
    for (int i = 0; i < num_blocos_horizontal * num_blocos_vertical; ++i) {
        histogramas_blocos[canal][i] = (int *)calloc(num_bins, sizeof(int));
        cdf_blocos[canal][i] = (int *)calloc(num_bins, sizeof(int));
    }
}

// Calcular histogramas e cdfs para cada bloco
for (int by = 0; by < num_blocos_vertical; ++by) {
    for (int bx = 0; bx < num_blocos_horizontal; ++bx) {
        int x_inicial = bx * tile_width;
        int y_inicial = by * tile_height;
        int x_final = (x_inicial + tile_width > largura) ? largura : x_inicial + tile_width;
        int y_final = (y_inicial + tile_height > altura) ? altura : y_inicial + tile_height;

        for (int canal = 0; canal < 3; ++canal) {
            int *hist = histogramas_blocos[canal][by * num_blocos_horizontal + bx];
            for (int i = 0; i < num_bins; i++) hist[i] = 0;

            // Calcular histogramas para cada canal
            calcula_histograma_rgb(x_final, x_inicial, y_final, y_inicial, hist, image, largura, 'r' + canal);

            limite_histograma_rgb(hist, num_bins, limite_corte);
            int regiao_pixels = (x_final - x_inicial) * (y_final - y_inicial);
            calcular_distribuicao_rgb(hist, num_bins, regiao_pixels, cdf_blocos[canal][by * num_blocos_horizontal + bx]);
        }
    }
}

// Aplicar CLAHE com interpolação
for (int y = 0; y < altura; ++y) {
    for (int x = 0; x < largura; ++x) {
        int bx = x / tile_width;
        int by = y / tile_height;

        int x1 = bx * tile_width;
        int y1 = by * tile_height;
        int x2 = (x1 + tile_width > largura) ? largura : x1 + tile_width;
        int y2 = (y1 + tile_height > altura) ? altura : y1 + tile_height;

        float dx = (float)(x - x1) / (x2 - x1);
        float dy = (float)(y - y1) / (y2 - y1);

        int novo_valor_red = 0, novo_valor_green = 0, novo_valor_blue = 0;

        for (int canal = 0; canal < 3; ++canal) {
            int valor_pixel;
            if (canal == 0) {
                valor_pixel = image->pixels[y * largura + x].red;
            } else if (canal == 1) {
                valor_pixel = image->pixels[y * largura + x].green;
            } else {
                valor_pixel = image->pixels[y * largura + x].blue;
            }

            // Interpolação entre os blocos
            int cdf1 = cdf_blocos[canal][by * num_blocos_horizontal + bx][valor_pixel];
            int cdf2 = (bx + 1 < num_blocos_horizontal) ? cdf_blocos[canal][by * num_blocos_horizontal + bx + 1][valor_pixel] : cdf1;
            int cdf3 = (by + 1 < num_blocos_vertical) ? cdf_blocos[canal][(by + 1) * num_blocos_horizontal + bx][valor_pixel] : cdf1;
            int cdf4 = (bx + 1 < num_blocos_horizontal && by + 1 < num_blocos_vertical) ? cdf_blocos[canal][(by + 1) * num_blocos_horizontal + bx + 1][valor_pixel] : cdf1;

            int novo_valor = (int)((1 - dx) * (1 - dy) * cdf1 + dx * (1 - dy) * cdf2 + (1 - dx) * dy * cdf3 + dx * dy * cdf4);

            // Verificação de valores
            if (novo_valor < 0) novo_valor = 0;
            if (novo_valor > 255) novo_valor = 255;

            if (canal == 0) {
                novo_valor_red = novo_valor;
            } else if (canal == 1) {
                novo_valor_green = novo_valor;
            } else {
                novo_valor_blue = novo_valor;
            }
        }

        // Atribuir novo valor ao resultado
        resultado->pixels[y * largura + x].red = novo_valor_red;
        resultado->pixels[y * largura + x].green = novo_valor_green;
        resultado->pixels[y * largura + x].blue = novo_valor_blue;
    }
}

// Liberar memória alocada para histogramas e CDFs
for (int canal = 0; canal < 3; ++canal) {
    for (int i = 0; i < num_blocos_horizontal * num_blocos_vertical; ++i) {
        free(histogramas_blocos[canal][i]);
        free(cdf_blocos[canal][i]);
    }
    free(histogramas_blocos[canal]);
    free(cdf_blocos[canal]);
}
free(histogramas_blocos);
free(cdf_blocos);

return resultado;
}

int compare_red(const void *a, const void *b)
{
    PixelRGB *pixel_a = (PixelRGB *)a;
    PixelRGB *pixel_b = (PixelRGB *)b;
    return pixel_a->red - pixel_b->red;
}

int compare_green(const void *a, const void *b)
{
    PixelRGB *pixel_a = (PixelRGB *)a;
    PixelRGB *pixel_b = (PixelRGB *)b;
    return pixel_a->green - pixel_b->green;
}

int compare_blue(const void *a, const void *b)
{
    PixelRGB *pixel_a = (PixelRGB *)a;
    PixelRGB *pixel_b = (PixelRGB *)b;
    return pixel_a->blue - pixel_b->blue;
}

ImageRGB *median_blur_rgb(const ImageRGB *image, int kernel_size)
{
    if (image == NULL || kernel_size <= 0 || kernel_size % 2 == 0) {
        return NULL;
    }

    int largura = image->dim.largura;
    int altura = image->dim.altura;

    ImageRGB *result = create_image_rgb(largura, altura);
    if (result == NULL) {
        return NULL;
    }

    int pad = kernel_size / 2;
    int window_size = kernel_size * kernel_size;
    PixelRGB *window = (PixelRGB *)malloc(window_size * sizeof(PixelRGB));

    if (window == NULL) {
        free_image_rgb(result);
        return NULL;
    }

    for (int y = 0; y < altura; ++y) {
        for (int x = 0; x < largura; ++x) {
            int count = 0;

            for (int ky = -pad; ky <= pad; ++ky) {
                for (int kx = -pad; kx <= pad; ++kx) {
                    int yy = y + ky;
                    int xx = x + kx;

                    if (yy >= 0 && yy < altura && xx >= 0 && xx < largura) {
                        window[count++] = image->pixels[yy * largura + xx];
                    }
                }
            }

            for (int c = 0; c < 3; ++c) {
                if (c == 0) {
                    qsort(window, count, sizeof(PixelRGB), compare_red);
                } else if (c == 1) {
                    qsort(window, count, sizeof(PixelRGB), compare_green);
                } else {
                    qsort(window, count, sizeof(PixelRGB), compare_blue);
                }

                int median_idx = count / 2;
                if (c == 0) {
                    result->pixels[y * largura + x].red = window[median_idx].red;
                } else if (c == 1) {
                    result->pixels[y * largura + x].green = window[median_idx].green;
                } else {
                    result->pixels[y * largura + x].blue = window[median_idx].blue;
                }
            }
        }
    }

    free(window);
    return result;
}




ImageRGB *read_imageRGB(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
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

    fscanf(file, "%d %d", &largura, &altura);

    ImageRGB *image = create_image_rgb(largura, altura);
    image->dim.altura = altura;
    image->dim.largura = largura;

    if (image == NULL)
    {
        fclose(file);
        return NULL;
    }

    for (int i = 0; i < largura * altura; i++)
    {
        fscanf(file, "%d %d %d", &image->pixels[i].red, &image->pixels[i].green, &image->pixels[i].blue);
        fgetc(file);
    }

    fclose(file);
    return image;
}

void mostra_imageRGB(const ImageRGB *image)
{
    if (image == NULL)
    {
        printf("imagem é NULL");
        return;
    }

    int largura = image->dim.largura;
    int altura = image->dim.altura;

    for (int i = 0; i < altura; ++i)
    {
        for (int y = 0; y < largura; ++y)
        {
            PixelRGB pixel = image->pixels[i * largura + y];
            printf("(%d,%d,%d)", pixel.red, pixel.green, pixel.blue);
        }
        printf("\n");
    }
}

