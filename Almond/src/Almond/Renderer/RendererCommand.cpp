#include "ampch.h"
#include "RendererCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Almond {

	RendererAPI* RendererCommand::s_RendererAPI = new OpenGLRendererAPI();

}