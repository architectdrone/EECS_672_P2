// main.c++: Starter for EECS 672 Projects 2-4

#include "ExtendedController.h"
#include "Shelf.h"
#include "Block.h"
#include <algorithm>    // std::max

void createScene(ExtendedController& c, ShaderIF* sIF)
{
	cryph::AffPoint origin(0, 0, 0);
	float l = 5;
	float d = 1;
	float h = 1;
	vec3 c1 = {0.0, 0.5, 0.0};
	vec3 c2 = {0.0, 0.9, 0.0};
	c.addModel(new Prism(sIF,origin, l, d, h, c1, c2));
}

void set3DViewingInformation(double overallBB[])
{
	// IF we are using Viewing Strategy #1, THEN
	//     Notify class ModelView that we initially want to see the entire scene:
	//   ModelView::setMCRegionOfInterest(overallBB);
	// ELSE (Viewing Strategy #2)
	//     Modify all three of deltaX, deltaY, and deltaZ of the incoming overallBB
	//     to have the dimensions of the desired field of view, then invoke:
	//   ModelView::setMCRegionOfInterest(modified_overallBB);
	//   Tell the ModelView class that dynamic rotations are to be done about the eye.

	ModelView::setMCRegionOfInterest(overallBB);

	// MC -> EC:
	double r = std::max((overallBB[1]-overallBB[0])/2, std::max((overallBB[3]-overallBB[2])/2, (overallBB[5]-overallBB[4])/2));

	// TODO: Compute/set eye, center, up
	float d = 3*r;
	cryph::AffVector dir(1, 0, 0);

	cryph::AffPoint center((overallBB[1]-overallBB[0])/2, (overallBB[3]-overallBB[2])/2, (overallBB[5]-overallBB[4])/2);
	cryph::AffPoint eye = center+(d*dir);
	cryph::AffVector up(0, 0, 1);
	ModelView::setEyeCenterUp(eye, center, up);

	// EC -> LDS:

	// Specify the initial projection type desired
	ModelView::setProjection(PERSPECTIVE);

	//Compute/set ecZmin, ecZmax (It is often useful to exaggerate these limits somewhat to prevent unwanted depth clipping.)
	double ecZmin, ecZmax, ecXmin, ecXmax, ecYmin, ecYmax, ecZpp, ecProjDir;

	ecXmin = ecYmin = 0-r;
	ecXmax = ecXmin = r;

	ecZmin = -d-r;
	ecZmax = -d+r;
	ModelView::setECZminZmax(ecZmin, ecZmax);

	//Compute/set ecZpp
	ecZpp = ecZmax;
	ModelView::setProjectionPlaneZ(ecZpp);
}

int main(int argc, char* argv[])
{
	ExtendedController c("NEAT", MVC_USE_DEPTH_BIT);
	c.reportVersions(std::cout);

	ShaderIF* sIF = new ShaderIF("shaders/basic.vsh", "shaders/phong.fsh");

	createScene(c, sIF);

	glClearColor(1.0, 1.0, 1.0, 1.0);

	double xyz[6];
	c.getOverallMCBoundingBox(xyz);
	std::cout << "Bounding box: " << xyz[0] << " <= x <= " << xyz[1] << '\n';
	std::cout << "              " << xyz[2] << " <= y <= " << xyz[3] << '\n';
	std::cout << "              " << xyz[4] << " <= z <= " << xyz[5] << "\n\n";
	set3DViewingInformation(xyz);
	c.run();

	delete sIF;

	return 0;
}
