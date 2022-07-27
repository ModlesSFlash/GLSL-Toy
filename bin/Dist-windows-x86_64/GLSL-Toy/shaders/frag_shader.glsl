#version 450

out vec4 outColor;

uniform float uTime;

void main()
{
    vec3 color;
    float len;
    float startTime = uTime;
    for(int i=0; i<3; i++)
    {
        vec2 normCoord = gl_FragCoord.xy / vec2(640.f, 480.f);
        vec2 uv        = normCoord;
        normCoord     -= 0.5;
        startTime     += 0.07;
        normCoord.x   *= 640.f / 480.f;
        len            = length(normCoord);
        uv            += normCoord / len * sin(startTime) * abs(sin(len * 9.0 - 2.0 * startTime));
        color[i]       = 0.01 / length(mod(uv, 1.0) - 0.5);
    }
    outColor=vec4(color/len,uTime);
}