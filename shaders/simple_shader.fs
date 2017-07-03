#version 300 es

precision highp float;

in vec4 normal_EC;
in vec2 uv;

out vec4 color;

void main() {
    float zFar = 100.0;
    float zNear = 0.1;
    //color = vec4(vec3(1.0 - (2.0 * zNear) /
                      //(zFar + zNear - gl_FragCoord.z * (zFar - zNear))), 1.0);
    //color = vec4(vec3(dot(normalize(normal_EC), normalize(vec4(5)))), 1.0);
    color = vec4(uv, 1.0, 1.0);
}
