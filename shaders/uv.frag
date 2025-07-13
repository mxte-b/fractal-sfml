uniform vec2 iResolution;
uniform vec3 camPosition;
uniform mat3 camRotationMatrix;

uniform float power;
uniform float epsilon;
uniform float maxDistance;
uniform int iterations;

float distanceEstimator(vec3 p)
{
    vec3 z = p;
    float dr = 1.0;
    float r = 0.;

    for (int i = 0; i < 10; i++)
    {
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


void main()
{
    vec2 uv = gl_FragCoord.xy / iResolution * 2.0 - 1.0;
    uv.x *= iResolution.x / iResolution.y;

    vec3 rayOrigin = camPosition;
    vec3 rayDirection = normalize(camRotationMatrix * vec3(uv.xy, 1));

    int i = 0;
    vec3 position = vec3(0, 0, 0);
    float distance = 0.0;
    for (; i < iterations; ++i)
    {
        position = rayOrigin + rayDirection * distance;

        float currentDistance = distanceEstimator(position);

        distance += currentDistance;

        if (currentDistance < epsilon)
        {
            break;
        }

        if (distance > maxDistance)
        {
            break;
        }
    }
    float rel = float(i) / iterations;
    vec3 luminance = vec3(rel, rel, rel);

    vec3 color = rayDirection * 0.5 + 0.5;
    gl_FragColor = vec4(luminance, 1.0);
}
