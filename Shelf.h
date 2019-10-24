// Shelf.h

#ifndef Shelf_H
#define Shelf_H

#include "ModelView.h"
#include "ShaderIF.h"
#include "Block.h"
#include "Prism.h"
typedef float vec3[3];

class Shelf : public ModelView
{
public:
	// As before: you will likely want to add parameters to the constructor
	Shelf(ShaderIF* sIF, cryph::AffPoint _origin, float lx, float ly, float lz, float support_thickness, vec3 support_color1, vec3 support_color2, vec3 block_color1, vec3 block_color2);
	virtual ~Shelf();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();
	void renderShelf();
private:
	float xmin, xmax, ymin, ymax, zmin, zmax;
	ShaderIF* shaderIF;
	float ka[3], kd[3];
	cryph::AffPoint origin;
	Block* blocks[1];
	Prism* prisms[2];
	float l, d, h;
};

#endif
