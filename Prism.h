// Prism.h

#ifndef Prism_H
#define Prism_H

#include "ModelView.h"
#include "ShaderIF.h"

typedef float vec3[3];

class Prism : public ModelView
{
public:
	// As before: you will likely want to add parameters to the constructor
	Prism(ShaderIF* sIF, cryph::AffPoint _origin, float _w, float _d, float _h, vec3 _c1, vec3 _c2);
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

	float c1[3];
	float c2[3];
	cryph::AffPoint origin;
	float w;
  float d;
	float h;
	ShaderIF* shaderIF;
	float ka[3], kd[3];
};

#endif
