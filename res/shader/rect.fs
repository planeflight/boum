#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

out vec4 color;

void main() {
    vec2 uv = fragTexCoord;
    color = vec4(fragColor);
    color.rgb += 0.7 * abs(uv.y - 0.5);
}
