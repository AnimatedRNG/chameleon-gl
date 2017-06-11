#version 430 core
out vec3 color;

uniform vec3 origin;
uniform mat4 inv;
uniform vec4 viewport;

uniform float NEAR;
//uniform float FAR;

float sdfSphere(in vec3 pos, in float radius) {
    return length(pos);
}

float sdfTriPrism(in vec3 p, in vec2 h) {
    vec3 q = abs(p);
    return max(q.z - h.y, max(q.x * 0.866025 + p.y * 0.5, -p.y) - h.x * 0.5);
}

vec3 opCheapBend(vec3 p) {
    float c = cos(0.1 * p.y);
    float s = sin(0.1 * p.y);
    mat2 m = mat2(c, -s, s, c);
    return vec3(m * p.xy, p.z);
}

void main() {
    float i = gl_FragCoord.x;
    float j = gl_FragCoord.y;
    vec4 world_space_vec = vec4((i - viewport[0]) / viewport[2],
                                ((viewport[3] - j) - viewport[1]) / viewport[3],
                                1,
                                1);
    world_space_vec = world_space_vec * 2.0 - 1.0;

    vec4 tmp = inv * world_space_vec;
    tmp /= tmp.w;
    tmp -= vec4(origin, 0);
    vec3 ray_vec = vec3(tmp);
    ray_vec = normalize(ray_vec);

    vec3 current_point = origin + ray_vec * NEAR;
    float radius = 0;
    for (int k = 0; k < 20; k++) {
        radius = sdfTriPrism(opCheapBend(current_point), vec2(8, 8));
        current_point += ray_vec * radius;
    }

    float distance = length(current_point - origin);

    if (radius < 0.1)
        color = vec3(dot(normalize(current_point), normalize(vec3(1, 1, 1))));
    else
        color = vec3(0);
}
