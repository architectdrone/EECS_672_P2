// Cup.h

#ifndef Cup_H
#define Cup_H

#include "ModelView.h"
#include "ShaderIF.h"
#include "Block.h"
#include "Prism.h"
typedef float vec3[3];

class Cup : public ModelView
{
public:
	// As before: you will likely want to add parameters to the constructor
	Cup(ShaderIF* sIF, cryph::AffPoint origin, float lx, float ly, float lz, vec3 front_color, vec3 page_color, vec3 back_color, vec3 spine_color);
	virtual ~Cup();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();
	void renderCup();
private:
	float xmin, xmax, ymin, ymax, zmin, zmax;
	ShaderIF* shaderIF;
	float ka[3], kd[3];
	Block* blocks[4];
};

#endif
