// main.c++: Starter for EECS 672 Projects 2-4

#include "ExtendedController.h"
#include "Shelf.h"
#include "Book.h"
#include "Cup.h"
#include "Block.h"
#include <algorithm>    // std::max
#include <stdlib.h>     /* srand, rand */
using namespace std;


float randomFloat()
{
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

float randomBetween(float lo, float hi)
{
	float range = hi - lo;
	float offset = range*randomFloat();
	return offset+lo;
}

float mutateNumber(float num, float max_dist)
{
	float randomized_distance = (static_cast<float>(rand())/(static_cast <float>(RAND_MAX/(max_dist*2))))-max_dist;
	return num+randomized_distance;
}

float* mutateColor(vec3 color, float max_dist)
{
	float* toReturn = new float[3];
	float minimum = 0;
	float maximum = 1;
	toReturn[0] = min(max(mutateNumber(color[0], max_dist), minimum), maximum);
	toReturn[1] = min(max(mutateNumber(color[1], max_dist), minimum), maximum);
	toReturn[2] = min(max(mutateNumber(color[2], max_dist), minimum), maximum);
	return toReturn;
}

float* randomColor()
{
	float* toReturn = new float[3];
	toReturn[0] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	toReturn[1] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	toReturn[2] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	return toReturn;
}

void fillShelf(ExtendedController& c, ShaderIF* sIF, cryph::AffPoint shelf_origin)
{
	//Shelf
	float shelf_lx = 7;
	float shelf_ly = 24;
	float shelf_lz = 1;
	vec3 support_color1 = {0.6, 0.63, .63}; //Steel https://encycolorpedia.com/9aa3a3
	vec3 support_color2 = {0.38, 0.41, 0.41}; //Sigma Cool Blue https://encycolorpedia.com/616a6a
	vec3 block_color1 = {0.8, 0.733, 0.61}; //Pale Oak https://encycolorpedia.com/cebb9e
	vec3 block_color2 = {0.71, 0.6, 0.43}; //Above, but 25% darker https://encycolorpedia.com/b6996e
	c.addModel(new Shelf(sIF, shelf_origin, shelf_lx, shelf_ly, shelf_lz, 0.4, mutateColor(support_color1, 0.1), mutateColor(support_color2, 0.1), mutateColor(block_color1, 0.1), mutateColor(block_color2,0.1) ));

	//Books
	cryph::AffPoint next_book_origin = shelf_origin;
	for (int i = 0; i < (rand()%10)+3; i++)
	{
		float book_lx = mutateNumber(5.5, 1.0);
		float book_ly = mutateNumber(1.5, 1.0);
		float book_lz = mutateNumber(8.0, 1.0);
		vec3 white = {1.0, 1.0, 1.0};
		c.addModel(new Book(sIF, next_book_origin, book_lx, book_ly, book_lz, randomColor(), white, randomColor(), randomColor()));
		next_book_origin = next_book_origin+cryph::AffVector(0, book_ly, 0);
	}

	cryph::AffPoint cup_origin = next_book_origin+cryph::AffVector(randomBetween(0, shelf_lx-next_book_origin.x), randomBetween(0, shelf_ly-next_book_origin.y), shelf_lz);
	float cup_lx = mutateNumber(1, 0.5);
	float cup_ly = mutateNumber(1, 0.5);
	float cup_lz = mutateNumber(1, 0.5);
	c.addModel(new Book(sIF, cup_origin, cup_lx, cup_ly, cup_lz, randomColor(), randomColor(), randomColor(), randomColor()));


}

void createScene(ExtendedController& c, ShaderIF* sIF)
{
	//All Units are in inches.
	cryph::AffPoint shelf_origin(0, 0, 0);
	// Good book sizes.
	float book_lx = 5.5;
	float book_ly = 1.5;
	float book_lz = 8;
	for (int i = 0; i < 4; i++)
	{
		fillShelf(c, sIF, shelf_origin);
		shelf_origin = shelf_origin + cryph::AffVector(0, 0, 18);
	}
	//c.addModel(new Book(sIF, shelf_origin, book_lx, book_ly, book_lz, support_color1, support_color2, block_color1, block_color2));
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

	ecZmin = -d-r-10;
	ecZmax = -d+r;
	ModelView::setECZminZmax(ecZmin, ecZmax);

	//Compute/set ecZpp
	ecZpp = ecZmax;
	ModelView::setProjectionPlaneZ(ecZpp);
}

int main(int argc, char* argv[])
{
	ExtendedController c("Owen's Shelves", MVC_USE_DEPTH_BIT);
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
