uniform vec2 iResolution;

void main()
{
    vec2 uv = gl_FragCoord.xy / iResolution;
    gl_FragColor = vec4(uv, 0.0, 1.0);
}
