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
uniform float aperture;
uniform float focusDistance;

struct HitInfo {
    bool hit;
    vec3 position;
    vec3 trapColor;
    vec3 normal;
    float distance;
    int steps;
};

vec2 hash2(vec2 p) {
    // Large constants = better scrambling
    const float K1 = 0.3183099; // 1/π
    const float K2 = 0.3678794; // 1/e

    p = fract(p * vec2(K1, K2));
    p += dot(p, p.yx + 19.19); // scrambled feedback
    return fract(vec2(p.x * p.y * 95.233, p.x + p.y * 11.472));
}

vec2 sampleDisk(vec2 seed) {
    vec2 rnd = hash2(seed);
    float angle = rnd.x * 6.2831853;
    float radius = sqrt(rnd.y);
    return vec2(cos(angle), sin(angle)) * radius;
}

float distanceEstimator(in vec3 p, out vec3 trap)
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

vec3 surfaceNormal(vec3 p) {
    float h = epsilon;
    vec3 dummyTrap;
    return normalize(vec3(
        distanceEstimator(p + vec3(h, 0, 0), dummyTrap) - distanceEstimator(p - vec3(h, 0, 0), dummyTrap),
        distanceEstimator(p + vec3(0, h, 0), dummyTrap) - distanceEstimator(p - vec3(0, h, 0), dummyTrap),
        distanceEstimator(p + vec3(0, 0, h), dummyTrap) - distanceEstimator(p - vec3(0, 0, h), dummyTrap)
    ));
}


vec3 computeRayDirection(vec2 uv)
{
    vec2 ndc = uv * 2.0 - 1.0;
    ndc.x *= iResolution.x / iResolution.y;

    float lens = tan(fov * 0.5);
    vec3 dir = normalize(vec3(ndc * lens, 1.0));
    return camRotationMatrix * dir;
}

HitInfo raymarch(vec3 rayOrigin, vec3 rayDir) {
    HitInfo info;
    info.hit = false;
    info.trapColor = vec3(0, 0, 0);
    info.distance = 0.0;

    float totalDist = 0.0;
    vec3 p;
    float hit = 0.0;
    int i;

    for (i = 0; i < iterations; ++i)
    {
        p = rayOrigin + rayDir * info.distance;

//        p = mod(p, vec3(4.0)) - vec3(2.0);

        vec3 trap;
        float d = distanceEstimator(p, trap);

        info.distance += d;

        if (d < epsilon) {
            info.hit = true;
            info.position = p;
            info.trapColor = trap;
            info.normal = surfaceNormal(p);
            break;
        }
        if (info.distance > maxDistance) {
            break;
        }
    }

    info.steps = i;
    return info;
}

float shadowFactor(HitInfo info, vec3 lightPos, float w) {
    if (!info.hit) return 1.0;

    vec3 lightDir = normalize(lightPos - info.position);
    vec3 shadowOrigin = info.position + info.normal * epsilon;
    float maxDist = length(lightPos - info.position);
    float distance = 0;

    float factor = 1.0;

    for (int i = 0; i < 128; i++) {
        vec3 p = shadowOrigin + lightDir * distance;

        vec3 trap;
        float d = distanceEstimator(p, trap);

        factor = min(factor, d / (w * distance));

        distance += clamp(d, 0.005, 0.50);

        if (factor < -1.0 || distance > maxDist) break;
    }

    factor = max(factor, -0.6);

    return 0.25*(1.0+factor)*(1.0+factor)*(2.0-factor);
}

vec3 renderPixel(vec2 fragCoord)
{
    vec2 texUv = fragCoord / iResolution;
    // Jittered UV for accumulation
    vec2 jitteredUv = accumulate ? texUv + jitter : texUv;
    vec3 rayOrigin = camPosition;
    vec3 rayDir = computeRayDirection(jitteredUv);

    // Depth of Field
    if (accumulate) {
        vec3 focusPoint = rayOrigin + rayDir * focusDistance;
        vec2 lensSample = sampleDisk(fragCoord + iTime) * aperture;

        vec3 dofOffset = camRotationMatrix * vec3(lensSample, 0.0);

        rayOrigin += dofOffset;
        rayDir = normalize(focusPoint - rayOrigin);
    }

    HitInfo info = raymarch(rayOrigin, rayDir);

    // Get shadow
    float shadow = shadowFactor(info, vec3(100, 100, -10), 0.05);

    float shade = float(info.steps) / float(iterations);
    vec3 color = mix(info.normal, vec3(0.529, 0.808, 0.922), (info.hit ? 0.0 : 1.0));
    color *= shadow;

    // Temporal accumulation
    if (accumulate) {
        vec3 prevColor = texture(lastFrame, texUv).rgb;
        color = mix(color, prevColor, blendFactor);
    }

    return color;
}

void main()
{
    vec3 finalColor = renderPixel(gl_FragCoord.xy);
    gl_FragColor = vec4(finalColor, 1.0);
}