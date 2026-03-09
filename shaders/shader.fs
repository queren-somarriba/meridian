#version 330 core

float random(vec2 p)
{
	vec3 p3  = fract(vec3(p.xyx) * 0.1031);
	p3 += dot(p3, p3.yzx + 33.33);

	return fract((p3.x + p3.y) * p3.z);
}

float noise(vec2 st)
{
	vec2 i = floor(st);
	vec2 f = fract(st);
	float a = random(i);
	float b = random(i + vec2(1.0, 0.0));
	float c = random(i + vec2(0.0, 1.0));
	float d = random(i + vec2(1.0, 1.0));
	vec2 u = f * f * (3.0 - 2.0 * f);

	return mix(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
}

float fbm(vec2 st)
{
	float value = 0.0;
	float amplitude = 0.5;

	for (int i = 0; i < 5; i++)
	{
		value += amplitude * noise(st);
		st *= 2.0;
		amplitude *= 0.5;
	}

	return value;
}

uniform float u_angle;
in vec2 v_uv;
out vec4 FragColor;

void main()
{
	vec3 colorA = vec3(254.0 / 255.0, 254.0 / 255.0, 244.0 / 255.0);
	vec3 colorB = vec3(110.0 / 255.0, 110.0 / 255.0, 110.0 / 255.0);
	float mixFactor = sin((v_uv.x + v_uv.y) * 3.0 + u_angle) * 0.5 + 0.5;
	vec3 finalColor = mix(colorA, colorB, mixFactor);
	float rawFbm = fbm(v_uv * 500.0);
	float paperGrain = mix(0.85, 1.15, rawFbm);
	FragColor = vec4(finalColor * paperGrain, 1.0);
}