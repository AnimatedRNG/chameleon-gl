#version 430 core
out vec3 color;

uniform sampler2D tex;
uniform vec4 viewport;

void main() {
     vec2 position = gl_FragCoord.xy / viewport.zw;
     position.y = 1.0 - position.y;
     color = texture(tex, position).rgb;
}