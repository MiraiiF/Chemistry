#pragma once
#include "General.h"
#include "Shader.h"
#define pi 3.1415926


std::vector<float> CriaEsfera(int nSetores, int nDivisoes, float r, std::vector<unsigned int> *indices);

void Desenha_Esfera(unsigned int VAOatom, std::vector<unsigned int> atomIndices);

std::vector<float> CriaCirculo(float r, int nSetores, float offset);

void Desenha_Circulo(unsigned int VAOcircle, int nSetores);

std::vector<float> Cria_CorpodeCilindro(int nSetores, float r, float h, std::vector<unsigned int> *indices);

void Desenha_CorpodeCilindro(unsigned int VAOcilinder, std::vector<unsigned int> indices);

void Desenha_Cilindro(unsigned int VAOcilinder, unsigned int VAOcircle1, unsigned int VAOcircle2, std::vector<unsigned int> indices);