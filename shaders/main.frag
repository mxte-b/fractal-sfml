uniform vec2 iResolution;
uniform vec3 camPosition;
uniform mat3 camRotationMatrix;
uniform float fov;

uniform float power;
uniform float iTime;
uniform float epsilon;
uniform float maxDistance;
uniform int iterations;

uniform sampler2D lastFrame;
uniform float blendFactor;
uniform bool accumulate;

uniform vec2 jitter;

float distanceEstimator(vec3 p, inout float currentIter){
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

void main()
{
    vec2 texUv = gl_FragCoord.xy / iResolution;
    vec2 uv;
    if (accumulate) {
        uv = (texUv + jitter) * 2.0 - 1.0;
    }
    else {
        uv = texUv * 2.0 - 1.0;
    }
    uv.x *= iResolution.x / iResolution.y;

    vec3 rayOrigin = camPosition;
    vec3 rayDirection = normalize(camRotationMatrix * vec3(uv.xy * tan(fov / 2), 1));

    int i = 0;
    vec3 position = vec3(0, 0, 0);
    float distance = 0.0;
    vec3 trapColor;
    float inside = 0.0;
    for (; i < iterations; ++i)
    {
        position = rayOrigin + rayDirection * distance;

//        position = mod(position, vec3(4.0)) - vec3(2.0);

        float currentDistance = distanceEstimator(position, trapColor);

        distance += currentDistance;

        // Inside SDF
        if (currentDistance < epsilon)
        {
            inside = 1.0;
            break;
        }

        // Outside SDF
        if (distance > maxDistance)
        {
            break;
        }
    }
    float rel = float(i) / iterations;
    vec3 luminance = vec3(rel, rel, rel);

    vec3 color = luminance;
//    if (inside == 1.0) {
//        color = trapColor * (1.0 - luminance);
//    }

    // Temporal Accumulation
    if (accumulate) {
        vec3 lastColor = texture(lastFrame, texUv).rgb;
        color = mix(color, lastColor, blendFactor);
    }

//    vec3 color = rayDirection * 0.5 + 0.5;
    gl_FragColor = vec4(color, 1.0);
}
