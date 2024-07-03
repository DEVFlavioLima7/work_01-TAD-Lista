#pragma once

#include <historicStructs.h>

extern ImgHistoricoRGB *historicoRGBInicio;
extern ImgHistoricoRGB *historicoRGBAtual;
extern ImgHistoricoGray *historicoGrayInicio;
extern ImgHistoricoGray *historicoGrayAtual;

void iniciarHistoricoRGB();
void removerValoresAFrenteRGB();
void adicionarHistoricoRGB(ImageRGB *newImgRGB, FuncUsed funcUsed);
void SeguirHistoricoRGB();
void VoltarHistoricoRGB();
void removerHistoricoRGB();

ImageRGB *refrashHistoricFuncRGB(FuncUsed funcUsed);

void iniciarHistoricoGray();
void removerValoresAFrenteGray();
void adicionarHistoricoGray(ImageGray *newImgGray, FuncUsed funcUsed);
void SeguirHistoricoGray();
void VoltarHistoricoGray();
void removerHistoricoGray();

ImageGray *refrashHistoricFuncGray(FuncUsed funcUsed);
