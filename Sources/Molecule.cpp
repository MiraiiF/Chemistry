#include "../Libraries/Molecule.hpp"
#include <cmath>

void Molecule::add_atom(Atom ready){
    stack.spheres.push_back(ready);
}

float Molecule::retrieve_phi(Atom main, Atom secondary){
    glm::vec3 diff = secondary.pos - main.pos;
    glm::vec2 cut = glm::normalize(glm::vec2(diff.z, diff.x));
    if(diff.z < 0){
        return -std::asin(cut.y) * 180/pi;
    }
    return std::asin(cut.y) * 180/pi;
}

float Molecule::retrieve_theta(Atom main, Atom secondary){
    glm::vec3 diff = glm::normalize(secondary.pos - main.pos);
    glm::vec2 cut = glm::vec2(diff.z, diff.x);
    float constant = glm::length(cut);
    printf("%f", glm::acos(constant)* 180/pi);
    if(diff.x > 0){
        return -glm::acos(constant)* 180/pi;
    }
    return glm::acos(constant)* 180/pi;
}

void Molecule::add_bond(unsigned int indice0, unsigned int indice1, bondType type){
    Bond ready;
    ready.type = type;
    ready.bound[0] = &stack.spheres[indice0];ready.bound[1] = &stack.spheres[indice1];
    stack.cylinders.push_back(ready);
}

void Molecule::water_template(){
    stack.empty();
    Atom O, H1, H2;
    O.element = OxygenColor; H1.element = HydrogenColor; H2.element = HydrogenColor;
    O.pos = glm::vec3(0.0f, 0.0f, 0.0f); O.scale = 2.0f;
    H1.pos = glm::vec3(-3.0f, -3.0f, 0.0f); H2.pos = glm::vec3(3.0f, -3.0f, 0.0f);
    add_atom(O);add_atom(H1);add_atom(H2);
    add_bond(0, 1, SIMPLE); add_bond(0, 2, SIMPLE);
    stack.cylinders[0].phi = -90;stack.cylinders[0].theta = 45; stack.cylinders[0].pos = 0.5f*(H1.pos+O.pos);
    stack.cylinders[1].phi = 90;stack.cylinders[1].theta = 45; stack.cylinders[1].pos = 0.5f*(H2.pos+O.pos);
    stack.cylinders[0].height = 3.5f;stack.cylinders[1].height = 3.5f;
}

void Molecule::test_template(){
    stack.empty();
    Atom O, H1, H2, H3;
    O.element = IodineColor; H1.element = BromineColor; H2.element = CarbonColor; H3.element = PhosphorusColor;
    O.pos = glm::vec3(0.0f, 8.0f, 0.0f); O.scale = 2.0f;
    H1.pos = glm::vec3(-3.0f, 7.0f, 3.0f); H2.pos = glm::vec3(3.0f, 5.0f, -3.0f); H3.pos = glm::vec3(5.0f, 9.0f, 4.0f);
    add_atom(O);add_atom(H1);add_atom(H2);add_atom(H3);
    add_bond(0, 1, TRIPLE); add_bond(0, 2, DOUBLE); add_bond(2, 3, SIMPLE);

    GLfloat phi1 = retrieve_phi(O, H1);
    GLfloat phi2 = retrieve_phi(O, H2);
    GLfloat phi3 = retrieve_phi(H2, H3);
    GLfloat theta1 = retrieve_theta(O, H1);
    GLfloat theta2 = retrieve_theta(O, H2);
    GLfloat theta3 = retrieve_theta(H2, H3);

    stack.cylinders[0].phi = phi1;stack.cylinders[0].theta = theta1; stack.cylinders[0].pos = 0.5f*(H1.pos+O.pos);
    stack.cylinders[1].phi = phi2;stack.cylinders[1].theta = theta2; stack.cylinders[1].pos = 0.5f*(H2.pos+O.pos);
    stack.cylinders[2].phi = phi3;stack.cylinders[2].theta = theta3; stack.cylinders[2].pos = 0.5f*(H3.pos+H2.pos);
    stack.cylinders[0].height = glm::length(H1.pos - O.pos);stack.cylinders[1].height = glm::length(H2.pos - O.pos);stack.cylinders[2].height = glm::length(H3.pos - H2.pos);
}

void Molecule::draw(Shader base, unsigned int VAOcylinder, unsigned int VAOcircle1, unsigned int VAOcircle2, std::vector<unsigned int> cylinderIndices, unsigned int VAOsphere, std::vector<unsigned int> sphereIndices){
    glm::mat4 model;
    for(int i = 0; i < stack.cylinders.size(); i++){
        stack.cylinders[i].visited = false;
    }
    base.setvec4("color", BondColor);
    for(int i = 0; i < stack.cylinders.size(); i++){
        glm::vec3 overalleffect = glm::vec3(0.0f, 0.0f, 0.0f);
        model = glm::mat4(1.0f);
        if(stack.cylinders[i].type == DOUBLE){
            glm::vec3 xeffect = glm::cos(stack.cylinders[i].phi) * glm::vec3(0.5f, 0.0f, 0.0f);
            glm::vec3 yeffect = glm::sin(stack.cylinders[i].phi) * glm::vec3(0.0f, 0.5f, 0.0f);
            overalleffect = xeffect + yeffect;
        }
        model = glm::translate(model, stack.cylinders[i].pos + overalleffect);
        model = glm::rotate(model, glm::radians(stack.cylinders[i].phi), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(stack.cylinders[i].theta), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, stack.cylinders[i].height));
        base.setmat4("model", model);
        if (stack.cylinders[i].visible)
        {
            Desenha_Cilindro(VAOcylinder, VAOcircle1, VAOcircle2, cylinderIndices);
            if(stack.cylinders[i].type == DOUBLE){
                model = glm::mat4(1.0f);
                model = glm::translate(model, stack.cylinders[i].pos - overalleffect);
                model = glm::rotate(model, glm::radians(stack.cylinders[i].phi), glm::vec3(0.0f, 1.0f, 0.0f));
                model = glm::rotate(model, glm::radians(stack.cylinders[i].theta), glm::vec3(1.0f, 0.0f, 0.0f));
                model = glm::scale(model, glm::vec3(1.0f, 1.0f, stack.cylinders[i].height));
                base.setmat4("model", model);
                Desenha_Cilindro(VAOcylinder, VAOcircle1, VAOcircle2, cylinderIndices);
                if(stack.cylinders[i].visited){
                    stack.cylinders[i].type = TRIPLE;
                }
            }
            else if(stack.cylinders[i].type == TRIPLE){
                stack.cylinders[i].type = DOUBLE;
                stack.cylinders[i].visited = true;
                i--;
            } 
        }
    }
    for(int i = 0; i < stack.spheres.size(); i++){
        model = glm::mat4(1.0f);
        model = glm::translate(model, stack.spheres[i].pos);
        model = glm::scale(model, glm::vec3(stack.spheres[i].scale));
        base.setmat4("model", model);
        base.setvec4("color", stack.spheres[i].element);
        if (stack.spheres[i].visible)
        {
            Desenha_Esfera(VAOsphere, sphereIndices);
        }
    }
}