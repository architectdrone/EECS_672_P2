// Shelf.c++

#include "Shelf.h"

Shelf::Shelf(ShaderIF* sIF, cryph::AffPoint _origin, float _sw, float _sd, float _sh, float _pw, float _pd, float _ph, float _d) : shaderIF(sIF)
{
	origin = _origin;
	sw = _sw;
	sd = _sd;
	sh = _sh;
	pw = _pw;
	pd = _pd;
	ph = _ph;
	d  = d;

	defineShelf();
	// DON'T FORGET TO SET INSTANCE VARIABLES, PERHAPS USING
	// SOME CONSTRUCTOR PARAMETERS
}

Shelf::~Shelf()
{
	glDeleteBuffers(1, vbo);
	glDeleteVertexArrays(1, vao);
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void Shelf::getMCBoundingBox(double* xyzLimits) const
{
	xyzLimits[0] = 0; // xmin
	xyzLimits[1] = sw;  // xmax
	xyzLimits[2] = 0; // ymin
	xyzLimits[3] = sd; // ymax
	xyzLimits[4] = 0-ph; // zmin
	xyzLimits[5] = sh; // zmax
}

void Shelf::render()
{
	//Save current and establish new current shader program
	GLint pgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
	glUseProgram(shaderIF->getShaderPgmID());

	//Establish "mc_ec" and "ec_lds" matrices
	cryph::Matrix4x4 mc_ec, ec_lds;
	ModelView::getMatrices(mc_ec, ec_lds);
	float m[16];
	glUniformMatrix4fv(shaderIF->ppuLoc("mc_ec"), 1, false, mc_ec.extractColMajor(m));
	glUniformMatrix4fv(shaderIF->ppuLoc("ec_lds"), 1, false, ec_lds.extractColMajor(m));

	renderShelf();

	//Reestablish previous shader program
	glUseProgram(pgm);
}

void Shelf::renderShelf()
{
	// 3. Set GLSL's "ka" and "kd" uniforms using this object's "ka" and "kd"
	//    instance variables
	// ...

	// 4. Establish any other attributes and make one or more calls to
	//    glDrawArrays and/or glDrawElements
	// ...
}

void Shelf::defineShelf()
{
	typedef float vec3[3];
	cryph::AffPoint vertex[20];
	//Shelf
	vertex[0] = origin+cryph::AffPoint(0, 0, 0);
	vertex[1] = origin+cryph::AffPoint(0, sw, 0);
	vertex[2] = origin+cryph::AffPoint(sd, 0, 0);
	vertex[3] = origin+cryph::AffPoint(sd, sw, 0);
	vertex[4] = origin+cryph::AffPoint(0, 0, sh);
	vertex[5] = origin+cryph::AffPoint(0, sw, sh);
	vertex[6] = origin+cryph::AffPoint(sd, 0, sh);
	vertex[7] = origin+cryph::AffPoint(sd, sw, sh);

	cryph::AffPoint p0 = cryph::AffPoint((sw/2)-(d/2), sd, 0);
	cryph::AffPoint p1 = cryph::AffPoint((sw/2)+(d/2)+pd, sd, 0);

	//Spoke 1
	vertex[8] = origin+ p0+cryph::AffPoint(0,  0,  0);
	vertex[9] = origin+ p0+cryph::AffPoint(pd, 0,  0);
	vertex[10] = origin+p0+cryph::AffPoint(0,  pw, 0);
	vertex[11] = origin+p0+cryph::AffPoint(pd, pw, 0);
	vertex[12] = origin+p0+cryph::AffPoint(0,  0,  ph);
	vertex[13] = origin+p0+cryph::AffPoint(pd, 0,  ph);

	//Spoke 2
	vertex[14] = origin+p1+cryph::AffPoint(0,  0,  0);
	vertex[15] = origin+p1+cryph::AffPoint(pd, 0,  0);
	vertex[16] = origin+p1+cryph::AffPoint(0,  pw, 0);
	vertex[17] = origin+p1+cryph::AffPoint(pd, pw, 0);
	vertex[18] = origin+p1+cryph::AffPoint(0,  0,  ph);
	vertex[19] = origin+p1+cryph::AffPoint(pd, 0,  ph);

	//Determine normals
	for (int i = 0; i < 16; i++)
	{
		int* face = faces[i];
		cryph::AffPoint point_0 = vertex[faces[i][0]];
		cryph::AffPoint point_1 = vertex[faces[i][1]];
		cryph::AffPoint point_2 = vertex[faces[i][2]];

		cryph::AffVector vector_0 = point_0-point_1;
		cryph::AffVector vector_1 = point_0-point_2;

		normal[i] = vector_0.cross(vector_1);
	}

	//Load cryph into bufferable object
	vec3 buffer_vertex[20];
	for (int i = 0; i < 20; i++)
	{
		buffer_vertex[i][0] = vertex[i].x;
		buffer_vertex[i][1] = vertex[i].y;
		buffer_vertex[i][2] = vertex[i].z;
	}

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);

	glGenBuffers(1, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 10*sizeof(vec3), buffer_vertex, GL_STATIC_DRAW);
	glVertexAttribPointer(shaderIF->pvaLoc("mcPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(shaderIF->pvaLoc("mcPosition"));

	glDisableVertexAttribArray(shaderIF->pvaLoc("mcNormal"));

}
