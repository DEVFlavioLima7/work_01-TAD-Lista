#include <historico.h>
#include <gtk/gtk.h>

ImgHistoricoRGB *historicoRGBInicio = NULL;
ImgHistoricoRGB *historicoRGBAtual = NULL;
ImgHistoricoGray *historicoGrayInicio = NULL;
ImgHistoricoGray *historicoGrayAtual = NULL;

void iniciarHistoricoRGB()
{
  historicoRGBInicio = (ImgHistoricoRGB *)malloc(sizeof(ImgHistoricoRGB));
  if (historicoRGBInicio == NULL)
  {
    fprintf(stderr, "Erro ao alocar memória para o histórico inicial.\n");
    return;
  }
  historicoRGBInicio->imgRGB = read_imageRGB("imgRGB.txt");
  if (historicoRGBInicio->imgRGB == NULL)
  {
    fprintf(stderr, "Erro ao ler a imagem inicial do arquivo.\n");
    free(historicoRGBInicio);
    historicoRGBInicio = NULL;
    return;
  }

  historicoRGBInicio->buttonStatus.flip_horizontal = 0;
  historicoRGBInicio->buttonStatus.flip_vertical = 0;
  historicoRGBInicio->buttonStatus.transpose = 0;
  historicoRGBInicio->buttonStatus.clahe = 0;
  historicoRGBInicio->buttonStatus.median_blur = 0;
  historicoRGBInicio->funcUsed = NONE;
  historicoRGBInicio->buttonStatus.qtdFlipAdd90 = 0;
  historicoRGBInicio->buttonStatus.qtdFlipNeq90 = 0;

  historicoRGBInicio->prev = NULL;
  historicoRGBInicio->next = NULL;
  historicoRGBAtual = historicoRGBInicio;
}

void removerValoresAFrenteRGB()
{
  if (historicoRGBAtual == NULL || historicoRGBAtual->next == NULL)
    return;

  ImgHistoricoRGB *aux = historicoRGBAtual->next;
  while (aux != NULL)
  {
    ImgHistoricoRGB *temp = aux;
    aux = aux->next;
    free(temp->imgRGB);
    free(temp);
  }
  historicoRGBAtual->next = NULL;
}

void adicionarHistoricoRGB(ImageRGB *newImgRGB, FuncUsed funcUsed)
{
  ImgHistoricoRGB *novaEntrada = (ImgHistoricoRGB *)malloc(sizeof(ImgHistoricoRGB));
  if (novaEntrada == NULL)
  {
    fprintf(stderr, "Erro ao alocar memória para nova entrada do histórico.\n");
    return;
  }

  novaEntrada->buttonStatus = historicoRGBAtual->buttonStatus;
  novaEntrada->imgRGB = newImgRGB;
  novaEntrada->prev = historicoRGBAtual;
  novaEntrada->funcUsed = funcUsed;
  novaEntrada->next = NULL;

  switch (funcUsed)
  {
    case FLIP_HORIZONTAL:
      novaEntrada->buttonStatus.flip_horizontal = !novaEntrada->buttonStatus.flip_horizontal;
      break;
    case FLIP_VERTICAL:
      novaEntrada->buttonStatus.flip_vertical = !novaEntrada->buttonStatus.flip_vertical;
      break;
    case TRANSPOSE:
      novaEntrada->buttonStatus.transpose = !novaEntrada->buttonStatus.transpose;
      break;
    case CLAHE:
      novaEntrada->buttonStatus.clahe = !novaEntrada->buttonStatus.clahe;
      break;
    case MEDIAN_BLUR:
      novaEntrada->buttonStatus.median_blur = !novaEntrada->buttonStatus.median_blur;
      break;
    case FLIP_ADD90:
      novaEntrada->buttonStatus.qtdFlipAdd90++;
      break;
    case FLIP_NEQ90:
      novaEntrada->buttonStatus.qtdFlipNeq90++;
      break;
    default:
      break;
  }

  if (historicoRGBInicio == NULL)
  {
    historicoRGBInicio = novaEntrada;
    historicoRGBAtual = novaEntrada;
    return;
  }

  removerValoresAFrenteRGB();
  historicoRGBAtual->next = novaEntrada;
  novaEntrada->prev = historicoRGBAtual;
  historicoRGBAtual = novaEntrada;
}

ImageGray *refrashHistoricFuncGray(FuncUsed funcUsed)
{
  if (historicoGrayAtual == NULL || historicoGrayInicio == NULL)
  {
    fprintf(stderr, "Erro: histórico atual ou inicial é nulo.\n");
    return NULL;
  }

  ImageGray *newImage = create_image_gray(historicoGrayInicio->imgGray->dim.altura, historicoGrayInicio->imgGray->dim.largura);
  if (newImage == NULL)
  {
    fprintf(stderr, "Erro ao alocar memória para nova imagem.\n");
    return NULL;
  }

  int usedClahe = 0;
  int usedMedianBlur = 0;

  newImage = read_image_gray_from_file("imgGray.txt");
  ImgHistoricoGray *aux = historicoGrayInicio;
  do
  {
      switch (aux->funcUsed)
      {
        case FLIP_HORIZONTAL:
          newImage = flip_horizontal_gray(newImage);
          break;
        case FLIP_VERTICAL:
          newImage = flip_vertical_gray(newImage);
          break;
        case TRANSPOSE:
          newImage = transpose_gray(newImage);
          break;
        case CLAHE:
          usedClahe = !usedClahe;
          break;
        case MEDIAN_BLUR:
          usedMedianBlur = !usedMedianBlur;
          break;
        case FLIP_ADD90:
          newImage = add90_rotation_gray(newImage);
          break;
        case FLIP_NEQ90:
          newImage = neq90_rotation_gray(newImage);
          break;
        default:
          break;
      }
    aux = aux->next;
  } while (aux != NULL);

  if (funcUsed == CLAHE)
    usedClahe = !usedClahe;
  
  if (funcUsed == MEDIAN_BLUR)
    usedMedianBlur = !usedMedianBlur;

  if (usedClahe)
    newImage = clahe_gray(newImage, 512, 512);

  if (usedMedianBlur)
    newImage = median_blur_gray(newImage, 9);
  
  return newImage;
}

ImageRGB *refrashHistoricFuncRGB(FuncUsed funcUsed)
{
  if (historicoRGBAtual == NULL || historicoRGBInicio == NULL)
  {
    fprintf(stderr, "Erro: histórico atual ou inicial é nulo.\n");
    return NULL;
  }

  ImageRGB *newImage = create_image_rgb(historicoRGBInicio->imgRGB->dim.altura, historicoRGBInicio->imgRGB->dim.largura);
  if (newImage == NULL)
  {
    fprintf(stderr, "Erro ao alocar memória para nova imagem.\n");
    return NULL;
  }

  int usedClahe = 0;
  int usedMedianBlur = 0;

  newImage = read_imageRGB("imgRGB.txt");
  ImgHistoricoRGB *aux = historicoRGBInicio;
  do
  {
    switch (aux->funcUsed)
    {
      case FLIP_HORIZONTAL:
        newImage = flip_horizontal_rgb(newImage);
        break;
      case FLIP_VERTICAL:
        newImage = flip_vertical_rgb(newImage);
        break;
      case TRANSPOSE:
        newImage = transpose_rgb(newImage);
        break;
      case CLAHE:
        usedClahe = !usedClahe;
        break;
      case MEDIAN_BLUR:
        usedMedianBlur = !usedMedianBlur;
        break;
      case FLIP_ADD90:
        newImage = add90_rotation_RGB(newImage);
        break;
      case FLIP_NEQ90:
        newImage = neq90_rotation_RGB(newImage);
        break;
      default:
        break;
    }
    aux = aux->next;
  } while (aux != NULL);

  if (funcUsed == CLAHE)
    usedClahe = !usedClahe;
  
  if (funcUsed == MEDIAN_BLUR)
    usedMedianBlur = !usedMedianBlur;

  if (usedClahe)
    newImage = clahe_rgb(newImage, 512, 512);

  if (usedMedianBlur)
    newImage = median_blur_rgb(newImage, 9);

  return newImage;
}

void SeguirHistoricoRGB()
{
  if (historicoRGBAtual != NULL && historicoRGBAtual->next != NULL)
    historicoRGBAtual = historicoRGBAtual->next;
}

void VoltarHistoricoRGB()
{
  if (historicoRGBAtual != NULL && historicoRGBAtual->prev != NULL)
    historicoRGBAtual = historicoRGBAtual->prev;
}

void iniciarHistoricoGray()
{
  historicoGrayInicio = (ImgHistoricoGray *)malloc(sizeof(ImgHistoricoGray));
  if (historicoGrayInicio == NULL)
  {
    fprintf(stderr, "Erro ao alocar memória para o histórico inicial.\n");
    return;
  }
  historicoGrayInicio->imgGray = read_image_gray_from_file("imgGray.txt");
  if (historicoGrayInicio->imgGray == NULL)
  {
    fprintf(stderr, "Erro ao ler a imagem inicial do arquivo.\n");
    free(historicoGrayInicio);
    historicoGrayInicio = NULL;
    return;
  }

  historicoGrayInicio->buttonStatus.flip_horizontal = 0;
  historicoGrayInicio->buttonStatus.flip_vertical = 0;
  historicoGrayInicio->buttonStatus.transpose = 0;
  historicoGrayInicio->buttonStatus.clahe = 0;
  historicoGrayInicio->buttonStatus.median_blur = 0;
  historicoGrayInicio->funcUsed = NONE;

  historicoGrayInicio->buttonStatus.qtdFlipAdd90 = 0;
  historicoGrayInicio->buttonStatus.qtdFlipNeq90 = 0;

  historicoGrayInicio->prev = NULL;
  historicoGrayInicio->next = NULL;
  historicoGrayAtual = historicoGrayInicio;
}

void removerValoresAFrenteGray()
{
  if (historicoGrayAtual == NULL || historicoGrayAtual->next == NULL)
    return;

  ImgHistoricoGray *aux = historicoGrayAtual->next;
  while (aux != NULL)
  {
    ImgHistoricoGray *temp = aux;
    aux = aux->next;
    free_image_gray(temp->imgGray);
    free(temp);
  }
  historicoGrayAtual->next = NULL;
}

void adicionarHistoricoGray(ImageGray *newImgGray, FuncUsed funcUsed)
{
  ImgHistoricoGray *novaEntrada = (ImgHistoricoGray *)malloc(sizeof(ImgHistoricoGray));
  if (novaEntrada == NULL)
  {
    fprintf(stderr, "Erro ao alocar memória para nova entrada do histórico.\n");
    return;
  }

  novaEntrada->buttonStatus = historicoGrayAtual->buttonStatus;
  novaEntrada->imgGray = newImgGray;
  novaEntrada->prev = historicoGrayAtual;
  novaEntrada->funcUsed = funcUsed;
  novaEntrada->next = NULL;

  switch (funcUsed)
  {
    case FLIP_HORIZONTAL:
      novaEntrada->buttonStatus.flip_horizontal = !novaEntrada->buttonStatus.flip_horizontal;
      break;
    case FLIP_VERTICAL:
      novaEntrada->buttonStatus.flip_vertical = !novaEntrada->buttonStatus.flip_vertical;
      break;
    case TRANSPOSE:
      novaEntrada->buttonStatus.transpose = !novaEntrada->buttonStatus.transpose;
      break;
    case CLAHE:
      novaEntrada->buttonStatus.clahe = !novaEntrada->buttonStatus.clahe;
      break;
    case MEDIAN_BLUR:
      novaEntrada->buttonStatus.median_blur = !novaEntrada->buttonStatus.median_blur;
      break;
    case FLIP_ADD90:
      novaEntrada->buttonStatus.qtdFlipAdd90++;
      break;
    case FLIP_NEQ90:
      novaEntrada->buttonStatus.qtdFlipNeq90++;
      break;
    default:
      break;
  }

  if (historicoGrayInicio == NULL)
  {
    historicoGrayInicio = novaEntrada;
    historicoGrayAtual = novaEntrada;
    return;
  }

  removerValoresAFrenteGray();
  historicoGrayAtual->next = novaEntrada;
  novaEntrada->prev = historicoGrayAtual;
  historicoGrayAtual = novaEntrada;
}

void SeguirHistoricoGray()
{
  if (historicoGrayAtual != NULL && historicoGrayAtual->next != NULL)
    historicoGrayAtual = historicoGrayAtual->next;
}

void VoltarHistoricoGray()
{
  if (historicoGrayAtual != NULL && historicoGrayAtual->prev != NULL)
    historicoGrayAtual = historicoGrayAtual->prev;
}

void removerHistoricoRGB()
{
  if (historicoRGBInicio == NULL)
    return;

  ImgHistoricoRGB *aux = historicoRGBInicio;
  while (aux != NULL)
  {
    ImgHistoricoRGB *temp = aux;
    aux = aux->next;
    free(temp->imgRGB);
    free(temp);
  }
  historicoRGBInicio = NULL;
  historicoRGBAtual = NULL;
}

void removerHistoricoGray()
{
  if (historicoGrayInicio == NULL)
    return;

  ImgHistoricoGray *aux = historicoGrayInicio;
  while (aux != NULL)
  {
    ImgHistoricoGray *temp = aux;
    aux = aux->next;
    free_image_gray(temp->imgGray);
    free(temp);
  }
  historicoGrayInicio = NULL;
  historicoGrayAtual = NULL;
}
