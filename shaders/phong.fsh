#version 410 core

// phong.fsh - a fragment shader that implements a Phong Lighting model.

in PVA
{
	vec3 ecPosition;  // "Q" in our lighting model pictures
	vec3 ecUnitNormal;// "nHat" in our lighting model pictures
} pvaIn;

// For lighing model:
uniform mat4 ec_lds; // so projection type and hence vHat can be determined

// Phong material properties (RGB reflectances);
uniform vec3 ka = vec3(0.8, 0.0, 0.0); // default: darkish red
uniform vec3 kd = vec3(0.8, 0.0, 0.0); // default: darkish red
// Lighting environment
// RGB strength of assumed ambient light:
uniform vec3 La = vec3(0.15, 0.15, 0.15);

// output color from the lighting model:
out vec4 fragmentColor;

vec4 evaluateLightingModel()
{
	vec3 liHat = vec3(0.0, 0.0, 1.0);

	vec3 vhat;
	if (ec_lds[3][3] == 0)
	{
		vhat = normalize(0-pvaIn.ecPosition);
	}
	else
	{
		double dx = (0-ec_lds[0][2])/(ec_lds[0][0]);
		double dy = (0-ec_lds[1][2])/(ec_lds[1][1]);
		vhat = normalize(vec3(dx, dy, 1));
	}

	bool correctSide = (dot(vhat, pvaIn.ecUnitNormal) > 0);
	vec3 correct_normal;
	if (correctSide)
	{
		correct_normal = pvaIn.ecUnitNormal;
	}
	else
	{
		correct_normal = -pvaIn.ecUnitNormal;
	}

	vec3 I = (ka*La) + (kd*(dot(liHat, correct_normal)));

	vec4 output;
	output[0] = I[0];
	output[1] = I[1];
	output[2] = I[2];
	output[3] = 1.0;
	return output;
}

void main ()
{
	fragmentColor = evaluateLightingModel();
}
