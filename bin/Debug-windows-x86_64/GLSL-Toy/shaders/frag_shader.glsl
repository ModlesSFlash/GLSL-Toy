#version 450
out vec4 outColor;
uniform float uDeltaTime;

vec2 uResolution = vec2(1920., 1080.);

float t;
#define R_MULT 0.5
#define rotate(v)\
    v *= mat2(cos(t/R_MULT), sin(t/R_MULT),\
             -sin(t/R_MULT), cos(t/R_MULT))

#define USE_BPM

#ifdef USE_BPM
    #define BPM         141. // 162.835 // -14400.
    #define BEATS       4.
    #define BEAT_TIME   60. / BPM
    #define BAR_TIME    (BEAT_TIME * BEATS)
    #define CYCLE_MULT  (PI / BAR_TIME)
#else
    #define CYCLE_MULT 1.
#endif

#define PI      (acos(-1.))
#define TWO_PI  (2. * PI)

void main()
{
    #ifdef USE_BPM
        t = mod(uDeltaTime * CYCLE_MULT, TWO_PI);
    #else
        t = uDeltaTime;
    #endif

    // init
    vec2 uv_lb        = gl_FragCoord.xy / uResolution;  
    vec2 uv_cc        = uv_lb - .5;  
    uv_cc.x          *= uResolution.x / uResolution.y; 
    float len         = length(uv_cc) 
                        * pow(((((sin(t*8)+8.)/6.))), 0.4);
    uv_cc = rotate(uv_cc); 

    // do magic
    uv_lb            += uv_cc                           
                        *
                        sin(t)
                        *
                        abs(sin(len * 9. - t))
                        /
                        pow(len, 2. + 1.* ((sin(t) + 1.) / 2.));

    // convert magic to reasonable color values
    float col         = .01 / length(fract(uv_lb) - .5);

    // rhythm
    float v = uDeltaTime * CYCLE_MULT / PI / 4.;

    // colors!
    vec3 color;
    if (v < 1.)
    {
        col         -= .21 - pow(pow(0.3, 1/v), 1/v);
        color        = vec3(col + .07 + .07, col + .07, col) / len;
    }
    else color       = vec3(col + .03, col - .05, col - .05) / len;

    // output
    outColor=vec4(color, 1.);
    //outColor=vec4(color-color, 1.);
}

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
 