#version 330 core

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform float renderWidth;
uniform float renderHeight;
uniform float blurStrength; // Blur strength

out vec4 finalColor;

void main()
{
    vec3 color = vec3(0.0);

    // 5 samples
    float w[5] = float[](0.204164, 0.304005, 0.093913, 0.304005, 0.204164);
    float offset[5] = float[]( -2.0, -1.0, 0.0, 1.0, 2.0 );

    for (int i = 0; i < 5; i++)
    {
        float xOffset = offset[i] * blurStrength / renderWidth;
        color += texture(texture0, fragTexCoord + vec2(xOffset, 0.0)).rgb * w[i];
    }

    finalColor = vec4(color, 1.0);
}
