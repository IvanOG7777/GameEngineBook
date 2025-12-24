#version 330 core
layout(location = 0) in vec2 aPos;
uniform vec2 uResolution;
void main() {
    vec2 ndc = aPos / uResolution;
    ndc = ndc * 2.0 - 1.0;
    ndc.y = -ndc.y;
    gl_Position = vec4(ndc, 0.0, 1.0);
}
