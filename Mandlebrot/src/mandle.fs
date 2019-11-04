#version 400 core

/*
uniform vec2 resolution;
uniform vec2 center;
uniform float range;
*/

uniform dvec2 resolution;
uniform dvec2 center;
uniform double range;

in vec4 gl_FragCoord;

out vec4 FragColor;

#define MAX_ITER 4096

vec2 sqr(vec2 a)
{
	vec2 result;
	result.x = a.x * a.x - a.y * a.y;
	result.y = 2.0 * a.x * a.y;
	return result;
}

dvec2 sqr(dvec2 a)
{
	dvec2 result;
	result.x = a.x * a.x - a.y * a.y;
	result.y = 2.0 * a.x * a.y;
	return result;
}

float map(float value, float min1, float max1, float min2, float max2)
{
	return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}

double map(double value, double min1, double max1, double min2, double max2)
{
	return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}

vec2 f_c(vec2 z, vec2 c)
{
	return sqr(z) + c;
}

dvec2 f_c(dvec2 z, dvec2 c)
{
	return sqr(z) + c;
}

float to_scalar(vec2 a)
{
	return a.x * a.x + a.y * a.y;
}

double to_scalar(dvec2 a)
{
	return a.x * a.x + a.y * a.y;
}

void main()
{
	double off = range / 2.0;
	dvec2 z = dvec2(0.0);
	dvec2 c;
	
	c.x = map(double(gl_FragCoord.x), 0.0, resolution.x, center.x - off, center.x + off);
	c.y = map(double(gl_FragCoord.y), 0.0, resolution.y, center.y - off, center.y + off);
	int n;

	for (n = 0; n <= MAX_ITER; n++) {
		z = f_c(z, c);
		if (to_scalar(z) > 20.0)
			break;
	}

	vec4 out_color = vec4(1.0);
	out_color.b = map(float(n & 0xf), 0.0, 15.0, 0.0, 1.0);
	out_color.g = map(float((n >> 4) & 0xf), 0.0, 15.0, 0.0, 1.0);
	out_color.r = map(float((n >> 8) & 0xf), 0.0, 15.0, 0.0, 1.0);

	FragColor = out_color;
}

/*
void main()
{
	float off = range / 2.0;
	vec2 z = vec2(0);
	vec2 c;
	
	c.x = map(gl_FragCoord.x, 0.0, resolution.x, center.x - off, center.x + off);
	c.y = map(gl_FragCoord.y, 0.0, resolution.y, center.y - off, center.y + off);
	int n;

	for (n = 0; n <= MAX_ITER; n++) {
		z = f_c(z, c);
		if (to_scalar(z) > 50.0)
			break;
	}

	float r = map(float(n & 0xf), 0.0, 15.0, 0.0, 1.0);
	float g = map(float((n >> 4) & 0xf), 0.0, 15.0, 0.0, 1.0);
	float b = map(float((n >> 8) & 0xf), 0.0, 15.0, 0.0, 1.0);

	vec4 out_color = vec4(1.0);
	out_color.r = r;
	out_color.g = g;
	out_color.b = b;
	FragColor = out_color;
}
*/