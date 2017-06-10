#version 430 core
out vec3 color;

void main() {
     //color = vec3(0, 1, 0);
    color = vec3(gl_FragCoord.x / 1920.0);
}
