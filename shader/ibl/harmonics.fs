#version 330


/*
 * harmonics for realtime rendering:
 *  https://huailiang.github.io/blog/2019/harmonics/
 */


const float PI = 3.1415926535897932384626433832795;

uniform vec3 coef[16];

in vec3 normal;

out vec4 FragColor;


void main(void)
{
    float basis[16];
    
    float x = normal.x;
    float y = normal.y;
    float z = normal.z;
    float x2 = x*x;
    float y2 = y*y;
    float z2 = z*z;
    
    basis[0]  = 1.f / 2.f * sqrt(1.f / PI);
    basis[1]  = sqrt(3.f / (4.f*PI))*z;
    basis[2]  = sqrt(3.f / (4.f*PI))*y;
    basis[3]  = sqrt(3.f / (4.f*PI))*x;
    basis[4]  = 1.f / 2.f * sqrt(15.f / PI) * x * z;
    basis[5]  = 1.f / 2.f * sqrt(15.f / PI) * z * y;
    basis[6]  = 1.f / 4.f * sqrt(5.f / PI) * (-x*x - z*z + 2 * y*y);
    basis[7]  = 1.f / 2.f * sqrt(15.f / PI) * y * x;
    basis[8]  = 1.f / 4.f * sqrt(15.f / PI) * (x*x - z*z);
    basis[9]  = 1.f / 4.f * sqrt(35.f / (2.f*PI))*(3 * x2 - z2)*z;
    basis[10] = 1.f / 2.f * sqrt(105.f / PI)*x*z*y;
    basis[11] = 1.f / 4.f * sqrt(21.f / (2.f*PI))*z*(4 * y2 - x2 - z2);
    basis[12] = 1.f / 4.f * sqrt(7.f / PI)*y*(2 * y2 - 3 * x2 - 3 * z2);
    basis[13] = 1.f / 4.f * sqrt(21.f / (2.f*PI))*x*(4 * y2 - x2 - z2);
    basis[14] = 1.f / 4.f * sqrt(105.f / PI)*(x2 - z2)*y;
    basis[15] = 1.f / 4.f * sqrt(35.f / (2 * PI))*(x2 - 3 * z2)*x;
    
    vec3 c = vec3(0,0,0);
    for (int i = 0; i < 16; i++)
    {
        c += coef[i] * basis[i];
    }
    
    FragColor = vec4(c, 1);
}
