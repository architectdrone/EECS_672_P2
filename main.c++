// main.c++: Starter for EECS 672 Projects 2-4

#include "ExtendedController.h"
#include "Shelf.h"
#include "Book.h"
#include "Block.h"
#include <algorithm>    // std::max

void createScene(ExtendedController& c, ShaderIF* sIF)
{
	//All Units are in inches.
	cryph::AffPoint shelf_origin(0, 0, 0);
	float lx = 5.5;
	float ly = 1.5;
	float lz = 8;
	vec3 support_color1 = {0.6, 0.63, .63}; //Steel https://encycolorpedia.com/9aa3a3
	vec3 support_color2 = {0.38, 0.41, 0.41}; //Sigma Cool Blue https://encycolorpedia.com/616a6a
	vec3 block_color1 = {0.8, 0.733, 0.61}; //Pale Oak https://encycolorpedia.com/cebb9e
	vec3 block_color2 = {0.71, 0.6, 0.43}; //Above, but 25% darker https://encycolorpedia.com/b6996e
	c.addModel(new Book(sIF, shelf_origin, lx, ly, lz, support_color1, support_color2, block_color1, block_color2));
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
