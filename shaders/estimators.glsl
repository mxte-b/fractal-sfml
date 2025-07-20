float mandelbulDE(in vec3 p, out vec3 trap)
{
    vec3 z = p;
    float dr = 1.0;
    float r = 0.;
    trap = vec3(1e20);

    for (int i = 0; i < 10; i++)
    {
        trap = min(trap, dot(z, z));

        r = length(z);
        if (r > 4.)
        break;

        float theta = acos(z.z / r) * power;
        float phi = atan(z.y, z.x) * power;

        dr = pow(r, power - 1.) * power * dr + 1.;
        z = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta)) * pow(r, power) + p;
    }
    return 0.5 * log(r) * r / dr;
}

float kleinianDE(vec3 p, inout float currentIter){
    float dr = 1.0;
    vec3 trap = vec3(0,0,0);
    float dist = 2.;
    float dist2 = 1e10;
    p+=vec3(1,1,0);
    for (int i = 0; i < 10; i++){
        p = -1. + 2. * fract(0.5*p+0.5);
        float r2 = dot(p, p);
        if (length(p) > 2.) break;
        float k = 1.5 / r2;

        p *= k;
        dr *= k;
        dist = max(dist, length(p-trap));
        dist2 = min(dist2, length(p-trap));
    }
    currentIter = mix(dist * 6.5, clamp(dist2 ,0. ,10.), 0.5);
    return 0.25 * abs(p.y) / dr;
}

float mandelboxDE(vec3 p, inout float currentIter){
    float minr2 = 0.5;
    float fixedr2 = 1.;
    vec3 z = p;
    float dr = 1.0;
    vec3 trap = vec3(0,0,0);
    float dist = 2.;
    float dist2 = 1e10;
    for (int i = 0; i < 20; i++){
        z = clamp(z, -u_foldlimit, u_foldlimit) * 2.0 - z; //Box fold

        float r2 = dot(z,z);
        if (r2 > 10000.) break;
        if (length(z) < minr2 || r2 < fixedr2){
            float temp = fixedr2 / r2;
            z *= temp;
            dr *= temp;
        }
        z = u_scale * z + p;
        dr = dr * abs(u_scale) + 1.;
        dist = max(dist, length(p-trap));
        dist2 = min(dist2, length(p-trap));
    }
    currentIter = mix(dist, clamp(dist2,0. ,10.), 0.5);
    float r = length(z);
    return r / abs(dr);
}