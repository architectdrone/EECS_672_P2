// main.c++: Starter for EECS 672 Projects 2-4

#include "ExtendedController.h"
#include "Shelf.h"
#include "Block.h"

void createScene(ExtendedController& c, ShaderIF* sIF)
{
	cryph::AffPoint origin(0, 0, 0);
	float l = 5;
	float d = 1;
	float h = 1;

	c.addModel(new Shelf(sIF, origin, l, d, h));
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

	// TODO: Compute/set eye, center, up
	cryph::AffVector eye_offset(10, 10, 10);


	cryph::AffPoint center((overallBB[1]-overallBB[0])/2, (overallBB[3]-overallBB[2])/2, (overallBB[5]-overallBB[4])/2);
	cryph::AffPoint eye = center+eye_offset;
	cryph::AffVector up(0, 0, 1);
	ModelView::setEyeCenterUp(eye, center, up);

	// EC -> LDS:

	// Specify the initial projection type desired
	ModelView::setProjection(PERSPECTIVE);

	//Compute/set ecZmin, ecZmax (It is often useful to exaggerate these limits somewhat to prevent unwanted depth clipping.)
	double ecZmin, ecZmax;
	ecZmin = -5;
	ecZmax = -10;
	ModelView::setECZminZmax(ecZmin, ecZmax);

	//Compute/set ecZpp
	double ecZpp = -1;
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
