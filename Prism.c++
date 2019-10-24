// Prism.c++

#include "Prism.h"
#include <math.h>

typedef float vec3[3];

Prism::Prism(ShaderIF* sIF, cryph::AffPoint _origin, float _w, float _d, float _h) : shaderIF(sIF)
{
	origin = _origin;
	w = _w;
	d = _d;
	h = _h;
	kd[0] = 0.0; kd[1] = 1.0; kd[2] = 0.0;
	definePrism();
}

Prism::~Prism()
{
	glDeleteBuffers(1, vbo);
	glDeleteVertexArrays(1, vao);
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void Prism::getMCBoundingBox(double* xyzLimits) const
{
	xyzLimits[0] = 0; // xmin  Give real values!
	xyzLimits[1] = d;  // xmax
	xyzLimits[2] = 0; // ymin
	xyzLimits[3] = w; // ymax
	xyzLimits[4] = 0-h; // zmin
	xyzLimits[5] = 0; // zmax
}

void Prism::render()
{
	GLint pgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
	glUseProgram(shaderIF->getShaderPgmID());

	cryph::Matrix4x4 mc_ec, ec_lds;
	getMatrices(mc_ec, ec_lds);
	float mat[16];
	glUniformMatrix4fv(shaderIF->ppuLoc("mc_ec"), 1, false, mc_ec.extractColMajor(mat));
	glUniformMatrix4fv(shaderIF->ppuLoc("ec_lds"), 1, false, ec_lds.extractColMajor(mat));

	renderPrism();

	glUseProgram(pgm);
}

void Prism::definePrism()
{
	cryph::AffPoint points[6];
	points[0] = origin;
	points[1] = origin+cryph::AffPoint(w, 0, 0);
	points[2] = origin+cryph::AffPoint(0, d, 0);
	points[3] = origin+cryph::AffPoint(w, d, 0);
	points[4] = origin+cryph::AffPoint(0, 0, 0-h);
	points[5] = origin+cryph::AffPoint(0, d, 0-h);

	//Store Aff Points into a vec array
	vec3 vertices[6];
	for (int i = 0; i < 6; i++)
	{
		vertices[i][0] = points[0].x;
		vertices[i][0] = points[0].y;
		vertices[i][0] = points[0].z;
	}

	//Send these points to the GPU
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);

	glGenBuffers(1, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 6*sizeof(vec3), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(shaderIF->pvaLoc("mcPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(shaderIF->pvaLoc("mcPosition"));
}

void Prism::renderPrism()
{
	glBindVertexArray(vao[0]);
	glUniform3fv(shaderIF->ppuLoc("kd"), 1, kd);

	GLuint verts0[4] = {0,1,2,3};
	GLuint verts1[3] = {0,1,4};
	GLuint verts2[3] = {2,3,5};
	GLuint verts3[4] = {0,2,4,5};
	GLuint verts4[4] = {1,3,4,5};

	glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), 0.0, 0.0, 1.0);
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, verts0);
	glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), 0.0, -1.0, 0.0);
	glDrawElements(GL_TRIANGLE_STRIP, 3, GL_UNSIGNED_INT, verts1);
	glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), 0.0, 1.0, 0.0);
	glDrawElements(GL_TRIANGLE_STRIP, 3, GL_UNSIGNED_INT, verts2);
	glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), -1.0, 0.0, 0.0);
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, verts3);
	glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), (0-h)/sqrt((h*h)+(w*w)), 0.0, (0-w)/sqrt((h*h)+(w*w)));
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, verts4);
}
