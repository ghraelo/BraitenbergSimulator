#pragma once

#include <Box2D\Box2D.h>
#include <vector>
#include <functional>

#include "Rectangle.h"
#include "IRenderable.h"
#include "Vehicle.h"
#include "Boundary.h"

class WrappingBoundary : public Boundary
{
public:
	WrappingBoundary(b2World* world, b2Vec2 topLeft, float width, float height, float influence);
	void Update(double time) override;
	void Render(NVGcontext* vg, Renderer& r) override;
};