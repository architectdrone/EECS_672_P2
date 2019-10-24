// ExtendedController.c++

#include "ExtendedController.h"
#include "ModelView.h"

ExtendedController::ExtendedController(const std::string& name, int rcFlags):
	GLFWController(name, rcFlags)
{
}

void ExtendedController::handleMouseMotion(int x, int y)
{
	float rotation_factor = .20;

	int dx = x-screenBaseX;
	int dy = y-screenBaseY;
	screenBaseX = x;
	screenBaseY = y;

	int dimensions[4];
	glGetIntegerv(GL_VIEWPORT, dimensions);

	if (mouseMotionIsTranslate)
	{
		float dx_lds = dx/((dimensions[2]/2));
		float dy_lds = dy/((dimensions[3]/2));
		ModelView::addToGlobalPan(dx_lds, dy_lds, 0);
	}
	else if (mouseMotionIsRotate)
	{
		float rot_angle_x = dy*rotation_factor;
		float rot_angle_y = dx*rotation_factor;
		ModelView::addToGlobalRotationDegrees(rot_angle_x, rot_angle_y, 0);
	}

	redraw();
}
