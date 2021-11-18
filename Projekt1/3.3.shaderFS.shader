#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float mnoznik;

void main()
{
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 1.0f - mnoznik);    //*vec4(ourColor.x * mnoznik, ourColor.y * mnoznik, ourColor.z * mnoznik, 1.0f);
}