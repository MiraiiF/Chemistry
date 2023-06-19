#include "../Libraries/Molecule.hpp"

void Molecule::add_atom(Atom ready){
    stack.spheres.push_back(ready);
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
    stack.cylinders[0].phi = -90;stack.cylinders[0].theta = 45; stack.cylinders[0].pos = 0.5f*(H1.pos-O.pos);
    stack.cylinders[1].phi = 90;stack.cylinders[1].theta = 45; stack.cylinders[1].pos = 0.5f*(H2.pos-O.pos);
    stack.cylinders[0].height = 3.5f;stack.cylinders[1].height = 3.5f;
}

void Molecule::draw(Shader base, unsigned int VAOcylinder, unsigned int VAOcircle1, unsigned int VAOcircle2, std::vector<unsigned int> cylinderIndices, unsigned int VAOsphere, std::vector<unsigned int> sphereIndices){
    glm::mat4 model;
    base.setvec4("color", BondColor);
    for(int i = 0; i < stack.cylinders.size(); i++){
        model = glm::mat4(1.0f);
        model = glm::translate(model, stack.cylinders[i].pos);
        model = glm::rotate(model, glm::radians(stack.cylinders[i].phi), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(stack.cylinders[i].theta), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, stack.cylinders[i].height));
        base.setmat4("model", model);
        if (stack.cylinders[i].visible)
        {
            Desenha_Cilindro(VAOcylinder, VAOcircle1, VAOcircle2, cylinderIndices);
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