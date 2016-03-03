
#include "DrawCmd.h"
#include "OpenGLRenderer.h"

DrawCmd::DrawCmd()
{
}

void DrawCmd::Execute(OpenGLRenderer& renderer)
{
	renderer.ExecuteDrawCmd(*this);
}
