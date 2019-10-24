// Shelf.h

#ifndef Shelf_H
#define Shelf_H

#include "ModelView.h"
#include "ShaderIF.h"

class Shelf : public ModelView
{
public:
	// As before: you will likely want to add parameters to the constructor
	Shelf(ShaderIF* sIF, cryph::AffPoint _origin, float _sw, float _sd, float _sh, float _pw, float _pd, float _ph, float _d);
	virtual ~Shelf();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();
	void renderXxx();
	void defineShelf();

private:
	ShaderIF* shaderIF;
	GLuint vao[1];
	GLuint vbo[1];

	cryph::AffPoint origin;
	float sw;
	float sd;
	float sh;
	float pw;
	float pd;
	float ph;
	float d;

	float ka[3], kd[3];
	int faces[16][4] = {{0, 1, 2, 3},  {4, 5, 6, 7},  {0, 1, 4, 5},  {1, 3, 5, 7},  {3, 2, 7, 6},
	 				 {2, 0, 6, 4},  {8, 9, 10,11}, {11,10,12,13}, {8, 9, 12,13}, {9, 11,12,-1},
	         {8, 10,13,-1}, {14,15,16,17}, {17,16,18,19}, {14,15,18,19}, {15,17,18,-1}, {14,16,19,-1}};;
	cryph::AffVector normal[16];
};

#endif
