#version 430 core
out vec4 cores;
in vec3 fragpos;
in vec3 norm;

void main(){
    vec3 vetor = normalize(vec3(1.0, 1.0, 0.0));
    float var = (dot(vetor, norm) + 1)/2;
    cores = vec4(0.0, var, 0.0, 1.0);
}