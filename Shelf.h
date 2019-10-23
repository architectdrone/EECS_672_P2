// Shelf.h

#ifndef Shelf_H
#define Shelf_H

#include "ModelView.h"
#include "ShaderIF.h"

class Shelf : public ModelView
{
public:
	// As before: you will likely want to add parameters to the constructor
	Shelf(ShaderIF* sIF);
	virtual ~Shelf();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();
	void renderXxx();
private:
	ShaderIF* shaderIF;
	float ka[3], kd[3];
};

#endif
