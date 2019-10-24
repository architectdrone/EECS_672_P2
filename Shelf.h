// Shelf.h

#ifndef Shelf_H
#define Shelf_H

#include "ModelView.h"
#include "ShaderIF.h"
#include "Block.h"

class Shelf : public ModelView
{
public:
	// As before: you will likely want to add parameters to the constructor
	Shelf(ShaderIF* sIF, cryph::AffPoint _origin, float _l, float _d, float _h);
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
	float l, d, h;
};

#endif
