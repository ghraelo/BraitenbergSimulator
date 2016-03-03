#include "ScissorCmd.h"
#include "OpenGLRenderer.h"
ScissorCmd::ScissorCmd()
{
}

void ScissorCmd::Execute(OpenGLRenderer & renderer)
{
	renderer.ExecuteScissorCmd(*this);
}
