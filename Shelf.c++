// Shelf.c++

#include "Shelf.h"

Shelf::Shelf(ShaderIF* sIF, cryph::AffPoint _origin, float lx, float ly, float lz, float support_thickness, vec3 support_color1, vec3 support_color2, vec3 block_color1, vec3 block_color2) : shaderIF(sIF)
{


	l = lx;
	h = ly;
	d = lz;
	origin = _origin;

	float support_lz = lx;
	float support_lx = lx;
	float support_ly = support_thickness;
	cryph::AffPoint support1_origin = origin;
	cryph::AffPoint support2_origin = origin+cryph::AffPoint(0, ly-support_thickness, 0);

	xmin = origin.x;
	xmax = origin.x+lx;
	ymin = origin.y;
	ymax = origin.y+ly;
	zmin = origin.z-support_lz;
	zmax = origin.z+lz;

	blocks[0] = new Block(sIF, origin.x, origin.y, origin.z, lx, ly, lz, block_color1, block_color2);
	prisms[0] = new Prism(sIF, support1_origin, support_lx, support_ly, support_lz, support_color1, support_color2);
	prisms[1] = new Prism(sIF, support2_origin, support_lx, support_ly, support_lz, support_color1, support_color2);
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

	glUseProgram(savedPgm);
}

void Shelf::renderShelf()
{
	for (int i = 0; i < 1; i++)
	{
		blocks[i]->render();

	}
	for (int x = 0; x < 2; x++)
	{
		prisms[x]->render();
	}
}
