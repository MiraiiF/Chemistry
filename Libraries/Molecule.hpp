#include "Shader.h"
#include "Desenhos.h"

enum bondType{
    SIMPLE,
    DOUBLE,
    TRIPLE
};

struct Atom{
    glm::vec4 element = BondColor;
    bool visited = false;
    glm::vec3 pos;
    float scale = 1.0f;
    bool visible = true;
};

struct Bond{
    bondType type = SIMPLE;
    Atom *bound[2];
    bool visited = false;
    float phi = 0.0f;
    float theta = 0.0f;
    float height = 1.0f;
    glm::vec3 pos;
    bool visible = true;
};

struct DrawStack{
    std::vector<Bond> cylinders;
    std::vector<Atom> spheres;

    void empty(){
        cylinders.clear();
        spheres.clear();
    }
};

class Molecule{
public:
    DrawStack stack;
    /*float return_dimensions(glm::vec3 atom1, glm::vec3 atom2, float* phi, float*theta){
        glm::vec3 diff = atom2 - atom1;
        float height = glm::length(diff);
        diff = glm::normalize(diff);
        float theight = glm::sqrt((diff.y * diff.y) + (diff.z * diff.z));
    }*/
    float retrieve_phi(Atom main, Atom secondary);
    float retrieve_theta(Atom main, Atom secondary);
    void add_atom(Atom ready);
    void add_bond(unsigned int indice0, unsigned int indice1, bondType type);
    void water_template();
    void test_template();
    void draw(Shader base, unsigned int VAOcylinder, unsigned int VAOcircle1, unsigned int VAOcircle2, std::vector<unsigned int> cylinderIndices, unsigned int VAOsphere, std::vector<unsigned int> sphereIndices);
};