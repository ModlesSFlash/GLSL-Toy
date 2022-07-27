#version 450

out vec4 outColor;

uniform float uTime;

void main()
{
    // basic shit
    //vec2 fragCoord = gl_FragCoord.xy / vec2(640.f, 480.f);
    //fragCoord -= fragCoord;
	//outColor = vec4(fragCoord.xy, uTime/4., 1.0);
    
    // thing1
	// vec3 color;
	// float len;
    // float startTime = uTime;
	// for(int i=0; i<3; i++)
    // {
        // vec2 normCoord = gl_FragCoord.xy / vec2(640.f, 480.f);
		// vec2 uv        = normCoord;
		// normCoord     -= 0.5;
        // startTime     += 0.07;
        // normCoord.x   *= 640.f / 480.f;
		// len            = length(normCoord);
		// uv            += normCoord / len * sin(startTime) * abs(sin(len * 9.0 - 2.0 * startTime));
		// color[i]       = 0.01 / length(mod(uv, 1.0) - 0.5);
	// }
	// outColor=vec4(color/len,uTime);

    // thing2
    #define rotate(v) v *= mat2(C = cos(uTime), S = sin(uTime), -S, C)
	outColor -= outColor;
    float C,S,r,rt,r2,x,x1,x2,x3,n;
    vec4 p = vec4(gl_FragCoord.xy,0,1) / vec4(480.f, 480.f, 640.f, 480.f) - 0.5;
    vec4 d;
    
    // init ray
    p.x -= 0.18; 
    
    // camera rotations
    rotate(p.xz);
    rotate(p.yz);
    rotate(p.xy);
    
    // ray dir = ray0-vec3(0)
    d = p;                                 
    p.z += 5.0 * uTime;
   
    for (float i = 1.0; i > 0.1; i -= 0.01)  
    {
        // objects id + local frame
        vec4 u = floor(p / 8.0);
        vec4 t = mod(p, 8.0) - 4.0;
        vec4 ta;
        vec4 M;
        vec4 m; 

        //objects' rotation
        rotate(t.xy);
        rotate(t.xz);
        rotate(t.yz);
        
        // randomize ids
        u = fract(1234.0 * sin(78.0 * (u + u.yzxw)));
   
        r  = 1.2;
        ta = abs(t);
        M  = max(ta,  ta.yzxw);
        m  = min(M,   M.yzxw );
        x1 = max(ta.x,M.y    )-r;
        x2 = min(m.x, m.y    );
        x  = max(x1,  1.1-x2 );
        
        // cube frame
        rt = cos(3.0 * uTime + 10.0 * u.y + 23.0 * u.z);
        r2 = r * (0.5 + 0.4 * rt);
        n  = fract(u.x + u.y + u.z);
        
        // growing plate
        if      (n < 0.25)             
        {
            x3 = max(ta.x - r2, M.y - r);
        } 
        
        // growing bar
        else if (n < 0.5)              
        {
            x3 = max(ta.x - r,  M.y - r2);
        }          
        
        // moving plate
        else if (n < 0.75)             
        {                        
            ta.x = abs(t.x  - r * rt);  
            M    = max(ta,             ta.yzxw);      
            x3   = max(ta.x - r * 0.1, M.y - r);
        }
        
        // rotating plate
        else                           
        {
            rotate(t.xy);
            ta = abs(t);
            M  = max(ta,             ta.yzxw);
            x3 = max(ta.x - r * 0.1, M.y - r);      
        }
        
        x = min(x, x3);

        // hit !
        if(x<0.01)                     
        { 
            // color texture + black fog
            outColor = i * i * (1.0 + 0.2 * t); 
            break;
        }
        
        // march ray
        p -= d*x;
     }
}