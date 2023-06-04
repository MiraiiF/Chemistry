#include "../Libraries/Desenhos.h"

std::vector<float> CriaEsfera(int nSetores, int nDivisoes, float r, std::vector<unsigned int> *indices){
    const GLfloat TWO_PI = pi * 2;
    const GLfloat HALF_PI = pi/2;

    GLfloat deltaPhi = pi / nDivisoes;
    GLfloat deltaTheta = TWO_PI / nSetores;

    std::vector<float> array;

    std::vector<float> pontos;
    std::vector<float> normais;
    std::vector<float> textura;

    float x, y , z;
    float nx, ny, nz;
    float s, t;
    float inversa = 1.0f/r;
    
    for(int i = 0; i <= nDivisoes; i++){
        GLfloat phi = HALF_PI - i * deltaPhi;
        GLfloat temp = r * cos(phi);
        z = r * sin(phi);

        unsigned int ini = i * (nSetores + 1);
        unsigned int fim = ini + nSetores + 1;
        for(int j = 0; j <= nSetores; ++j, ++ini, ++fim){
            GLfloat theta = j * deltaTheta;

            x = temp * cos(theta);
            y = temp * sin(theta);
            pontos.push_back(x);pontos.push_back(y);pontos.push_back(z);
            
            nx = x * inversa;
            ny = y * inversa;
            nz = z * inversa;
            normais.push_back(nx);normais.push_back(ny);normais.push_back(nz);

            s = (float)j/nSetores;
            t = (float)i/nSetores;

            textura.push_back(s);textura.push_back(t);

            if((j != nSetores) && (i != nDivisoes)){
                if(i != 0){
                    indices->push_back(ini);
                    indices->push_back(fim);
                    indices->push_back(ini + 1);
                }

                if(i != (nDivisoes - 1)){
                    indices->push_back(ini + 1);
                    indices->push_back(fim);
                    indices->push_back(fim + 1);
                }
            }
        }
    }

    for(int i = 0; i < pontos.size()/3; i++){
        array.push_back(pontos[3*i]);array.push_back(pontos[(3*i) + 1]);array.push_back(pontos[(3*i) + 2]);
        array.push_back(normais[3*i]);array.push_back(normais[(3*i) + 1]);array.push_back(normais[(3*i) + 2]);
        array.push_back(textura[2*i]);array.push_back(textura[(2*i) + 1]);
    }

    return array;
}