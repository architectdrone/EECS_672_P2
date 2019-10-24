// Book.c++

#include "Book.h"

Book::Book(ShaderIF* sIF, cryph::AffPoint origin, float lx, float ly, float lz, vec3 front_color, vec3 page_color, vec3 back_color, vec3 spine_color) : shaderIF(sIF)
{
	xmin = origin.x;
	xmax = origin.x+lx;
	ymin = origin.y;
	ymax = origin.y+ly;
	zmin = origin.z;
	zmax = origin.z+lz;

	float cover_thickness_ratio = 0.2; //Percent of the size in the y direction that is cover.
	float paper_size_ratio = 0.8; //Percentage of the cover that is the size of page
	float ct = (cover_thickness_ratio/2) * ly;
	float cl = lx;
	float ch = ly;
	float pt = (1-cover_thickness_ratio)*ly;
	float pl = paper_size_ratio *lx;
	float ph = paper_size_ratio *ly;

	//Front
	cryph::AffPoint r1_origin = origin;
	float r1_lx = cl;
	float r1_ly = ct;
	float r1_lz = ch;
	blocks[0] = new Block(sIF, r1_origin.x, r1_origin.y, r1_origin.z, r1_lx, r1_ly, r1_lz, front_color, front_color);

	//Pages
	cryph::AffPoint r2_origin = origin+cryph::AffPoint((cl-pl)/2, ct, (ch-ph)/2);
	float r2_lx = pl;
	float r2_ly = pt;
	float r2_lz = ph;
	blocks[1] = new Block(sIF, r2_origin.x, r2_origin.y, r2_origin.z, r2_lx, r2_ly, r2_lz, page_color, page_color);

	//Back
	cryph::AffPoint r3_origin = origin+cryph::AffPoint(0, ct+pt, 0);
	float r3_lx = cl;
	float r3_ly = ct;
	float r3_lz = ch;
	blocks[2] = new Block(sIF, r3_origin.x, r3_origin.y, r3_origin.z, r3_lx, r3_ly, r3_lz, back_color, back_color);

	//Spine
	cryph::AffPoint r4_origin = origin;
	float r4_lx = ct;
	float r4_ly = 2*ct+pt;
	float r4_lz = ch;
	blocks[3] = new Block(sIF, r4_origin.x, r4_origin.y, r4_origin.z, r4_lx, r4_ly, r4_lz, spine_color, spine_color);
}

Book::~Book()
{
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void Book::getMCBoundingBox(double* xyzLimits) const
{
	xyzLimits[0] = xmin;
	xyzLimits[1] = xmax;
	xyzLimits[2] = ymin;
	xyzLimits[3] = ymax;
	xyzLimits[4] = zmin;
	xyzLimits[5] = zmax;
}

void Book::render()
{
	int savedPgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &savedPgm);
	glUseProgram(shaderIF->getShaderPgmID());

	cryph::Matrix4x4 mc_ec, ec_lds;
	getMatrices(mc_ec, ec_lds);
	float m[16];
	glUniformMatrix4fv(shaderIF->ppuLoc("mc_ec"), 1, false, mc_ec.extractColMajor(m));
	glUniformMatrix4fv(shaderIF->ppuLoc("ec_lds"), 1, false, mc_ec.extractColMajor(m));

	renderBook();

	glUseProgram(savedPgm);
}

void Book::renderBook()
{
	for (int i = 0; i < 4; i++)
	{
		blocks[i]->render();
	}
}
