#pragma once

#include <imgRGB.h>
#include <imgGray.h>

typedef enum funcUsed
{
  NONE,
  FLIP_VERTICAL,
  FLIP_HORIZONTAL,
  TRANSPOSE,
  CLAHE,
  MEDIAN_BLUR,
  FLIP_ADD90,
  FLIP_NEQ90,
} FuncUsed;

typedef struct ButtonStatus
{
  int flip_vertical;
  int flip_horizontal;
  int transpose;
  int clahe;
  int median_blur;
  int qtdFlipAdd90;
  int qtdFlipNeq90;
} ButtonStatus;

typedef struct imgHistoricoRGB
{
  ImageRGB *imgRGB;
  struct imgHistoricoRGB *prev;
  struct imgHistoricoRGB *next;
  ButtonStatus buttonStatus;
  FuncUsed funcUsed;
} ImgHistoricoRGB;

typedef struct imgHistoricoGray
{
  ImageGray *imgGray;
  struct imgHistoricoGray *prev;
  struct imgHistoricoGray *next;
  ButtonStatus buttonStatus;
  FuncUsed funcUsed;
} ImgHistoricoGray;


typedef struct imgHistoricoRandomGray
{
  ImageGray *imgGray;
  struct imgHistoricoRandomGray *next;
  FuncUsed funcUsed;
} ImgHistoricoRandomGray;

typedef struct imgHistoricoRandomRGB
{
  ImageRGB *imgRGB;
  struct imgHistoricoRandomRGB *next;
  FuncUsed funcUsed;
} ImgHistoricoRandomRGB;
