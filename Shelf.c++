// Shelf.c++

#include "Shelf.h"

Shelf::Shelf(ShaderIF* sIF, cryph::AffPoint _origin, float _l, float _d, float _h) : shaderIF(sIF)
{
	xmin = origin.x;
	xmax = origin.x+_l;
	ymin = origin.y;
	ymax = origin.y+_d;
	zmin = origin.z;
	zmax = origin.z+_h;

	l = _l;
	h = _h;
	d = _d;
	origin = _origin;

	blocks[0] = new Block(sIF, origin.x, origin.y, origin.z, l, d, h);
}

Shelf::~Shelf()
{
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void Shelf::getMCBoundingBox(double* xyzLimits) const
{
	xyzLimits[0] = xmin;
	xyzLimits[1] = xmax;
	xyzLimits[2] = ymin;
	xyzLimits[3] = ymax;
	xyzLimits[4] = zmin;
	xyzLimits[5] = zmax;
}

void Shelf::render()
{
	int savedPgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &savedPgm);
	glUseProgram(shaderIF->getShaderPgmID());

	cryph::Matrix4x4 mc_ec, ec_lds;
	getMatrices(mc_ec, ec_lds);
	float m[16];
	glUniformMatrix4fv(shaderIF->ppuLoc("mc_ec"), 1, false, mc_ec.extractColMajor(m));
	glUniformMatrix4fv(shaderIF->ppuLoc("ec_lds"), 1, false, mc_ec.extractColMajor(m));


	renderShelf();

	// 5. Reestablish previous shader program
	// ...
}

void Shelf::renderShelf()
{
	for (int i = 0; i < sizeof(blocks); i++)
	{
		blocks[i]->render();
	}
}
