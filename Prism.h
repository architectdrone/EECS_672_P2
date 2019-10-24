// Prism.h

#ifndef Prism_H
#define Prism_H

#include "ModelView.h"
#include "ShaderIF.h"

class Prism : public ModelView
{
public:
	// As before: you will likely want to add parameters to the constructor
	Prism(ShaderIF* sIF, cryph::AffPoint _origin, float _w, float _d, float _h);
	virtual ~Prism();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();
	void renderPrism();
	void definePrism();
private:
	ShaderIF* sIF;
	GLuint vao[1];
	GLuint vbo[1];

	vec3 c1;
	vec3 c2;
	cryph::AffPoint origin;
	float w;
  float d;
	float h;
	ShaderIF* shaderIF;
	float ka[3], kd[3];
};

#endif
