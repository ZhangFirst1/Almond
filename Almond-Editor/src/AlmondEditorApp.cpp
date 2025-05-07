#include "ampch.h"
#include <Almond.h>
#include <Almond/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Almond {

	class AlmondEditor : public Application {
	public:
		AlmondEditor() 
			: Application("Almond Editor") {
			PushLayer(new EditorLayer());
		}

		~AlmondEditor() {

		}

	};

	Application* CreateApplication() {
		return new AlmondEditor();
	}
}