// Cup.c++

#include "Cup.h"

Cup::Cup(ShaderIF* sIF, cryph::AffPoint origin, float lx, float ly, float lz, vec3 front_color, vec3 page_color, vec3 back_color, vec3 spine_color) : shaderIF(sIF)
{
	xmin = origin.x;
	xmax = origin.x+lx;
	ymin = origin.y;
	ymax = origin.y+ly;
	zmin = origin.z;
	zmax = origin.z+lz;

	float cup_thickness_ratio = 0.1;
	float s = lx; //Size (the cup is square. This is the size around the base.)
	float t = s*0.1; //Thickness
	float h = lz; //Height

	//Front
	cryph::AffPoint r1_origin = origin;
	float r1_lx = t;
	float r1_ly = s;
	float r1_lz = h;
	blocks[0] = new Block(sIF, r1_origin.x, r1_origin.y, r1_origin.z, r1_lx, r1_ly, r1_lz, front_color, front_color);

	//Left
	cryph::AffPoint r2_origin = origin+cryph::AffPoint(0, 0, 0);
	float r2_lx = s;
	float r2_ly = t;
	float r2_lz = h;
	blocks[1] = new Block(sIF, r2_origin.x, r2_origin.y, r2_origin.z, r2_lx, r2_ly, r2_lz, page_color, page_color);

	//Right
	cryph::AffPoint r3_origin = origin+cryph::AffPoint(0, s-t, 0);
	float r3_lx = s;
	float r3_ly = t;
	float r3_lz = h;
	blocks[2] = new Block(sIF, r3_origin.x, r3_origin.y, r3_origin.z, r3_lx, r3_ly, r3_lz, back_color, back_color);

	//Back
	cryph::AffPoint r4_origin = origin+cryph::AffPoint(s, 0, 0);
	float r4_lx = t;
	float r4_ly = s;
	float r4_lz = h;
	blocks[3] = new Block(sIF, r4_origin.x, r4_origin.y, r4_origin.z, r4_lx, r4_ly, r4_lz, spine_color, spine_color);

	//Bottom
	cryph::AffPoint r5_origin = origin+cryph::AffPoint(t, t, 0);
	float r5_lx = s-t;
	float r5_ly = s-t;
	float r5_lz = 0;
	blocks[4] = new Block(sIF, r5_origin.x, r5_origin.y, r5_origin.z, r5_lx, r5_ly, r5_lz, front_color, front_color);
}

Cup::~Cup()
{
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void Cup::getMCBoundingBox(double* xyzLimits) const
{
	xyzLimits[0] = xmin;
	xyzLimits[1] = xmax;
	xyzLimits[2] = ymin;
	xyzLimits[3] = ymax;
	xyzLimits[4] = zmin;
	xyzLimits[5] = zmax;
}

void Cup::render()
{
	int savedPgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &savedPgm);
	glUseProgram(shaderIF->getShaderPgmID());

	cryph::Matrix4x4 mc_ec, ec_lds;
	getMatrices(mc_ec, ec_lds);
	float m[16];
	glUniformMatrix4fv(shaderIF->ppuLoc("mc_ec"), 1, false, mc_ec.extractColMajor(m));
	glUniformMatrix4fv(shaderIF->ppuLoc("ec_lds"), 1, false, mc_ec.extractColMajor(m));

	renderCup();

	glUseProgram(savedPgm);
}

void Cup::renderCup()
{
	for (int i = 0; i < 5; i++)
	{
		blocks[i]->render();
	}
}
