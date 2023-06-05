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

void Desenha_Esfera(unsigned int VAOatom, std::vector<unsigned int> atomIndices){
    glBindVertexArray(VAOatom);
	glDrawElements(GL_TRIANGLES, (unsigned int)atomIndices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

std::vector<float> CriaCirculo(float r, int nSetores, float offset){
    float deltaTheta = (2*pi)/nSetores;

    std::vector<float> array;

    std::vector<float> pontos;
    std::vector<float> textura;

    pontos.push_back(0.0f);pontos.push_back(0.0f);pontos.push_back(offset);
    textura.push_back(0.5f);textura.push_back(0.5f);

    float theta = 0;

    for(int i = 0; i <= nSetores; i++){
        float x = sin(theta);
        float y = cos(theta);
        pontos.push_back(x*r);pontos.push_back(y*r);pontos.push_back(offset);

        float s = (x+1)/2, t = (y+1)/2;
        textura.push_back(s);textura.push_back(t);
        theta += deltaTheta;
    }

    for(int i = 0; i < pontos.size()/3; i++){
        array.push_back(pontos[3*i]);array.push_back(pontos[(3*i) + 1]);array.push_back(pontos[(3*i) + 2]);
        array.push_back(0.0f);array.push_back(0.0f);array.push_back(1.0f);
        array.push_back(textura[2*i]);array.push_back(textura[(2*i) + 1]);
    }

    return array;
}

void Desenha_Circulo(unsigned int VAOcircle, int nSetores){
    glBindVertexArray(VAOcircle);
    glDrawArrays(GL_TRIANGLE_FAN, 0, nSetores + 1);
    glBindVertexArray(0);
}

std::vector<float> Cria_CorpodeCilindro(int nSetores, float r, float h, std::vector<unsigned int> *indices){
    float deltaTheta = (2*pi)/nSetores;

    float HALF_HEIGHT = h/2;

    std::vector<float> array;

    std::vector<float> pontos;
    std::vector<float> normais;
    std::vector<float> textura;

    float theta = 0;

    for(int i = 0; i <= nSetores; i++){
        float x = sin(theta);
        float y = cos(theta);
        pontos.push_back(x*r);pontos.push_back(y*r);pontos.push_back(HALF_HEIGHT);
        normais.push_back(x);normais.push_back(y);normais.push_back(0.0f);
        pontos.push_back(x*r);pontos.push_back(y*r);pontos.push_back(-HALF_HEIGHT);
        normais.push_back(x);normais.push_back(y);normais.push_back(0.0f);

        float s = (x+1)/2;
        textura.push_back(s);textura.push_back(1.0f);
        textura.push_back(s);textura.push_back(0.0f);

        if(i != 0){
            indices->push_back(2*(i-1));
            indices->push_back((2*(i-1)) + 1);
            indices->push_back(2*i);
            indices->push_back(2*i);
            indices->push_back((2*(i-1)) + 1);
            indices->push_back((2*i) + 1);
        }

        theta += deltaTheta;
    }

    

    for(int i = 0; i < pontos.size()/3; i++){
        array.push_back(pontos[3*i]);array.push_back(pontos[(3*i) + 1]);array.push_back(pontos[(3*i) + 2]);
        array.push_back(normais[3*i]);array.push_back(normais[(3*i) + 1]);array.push_back(normais[(3*i) + 2]);
        array.push_back(textura[2*i]);array.push_back(textura[(2*i) + 1]);
    }

    return array;
}

void Desenha_CorpodeCilindro(unsigned int VAOcilinder, std::vector<unsigned int> indices){
    glBindVertexArray(VAOcilinder);
	glDrawElements(GL_TRIANGLES, (unsigned int)indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Desenha_Cilindro(unsigned int VAOcilinder, unsigned int VAOcircle1, unsigned int VAOcircle2, std::vector<unsigned int> indices){
    Desenha_CorpodeCilindro(VAOcilinder, indices);
    Desenha_Circulo(VAOcircle1, indices.size()/6 + 1); 
    Desenha_Circulo(VAOcircle2, indices.size()/6 + 1); 
}