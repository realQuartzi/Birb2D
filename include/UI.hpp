#pragma once

#include <vector>
#include "Entity.hpp"
#include "Renderwindow.hpp"

namespace Birb
{
	class UI
	{
	public:
		UI();
		std::vector<Entity> Buttons;
		void AddButton(Entity buttonEntity); ///< Addin't an entity to this list makes it interactable as a button
		void PollButtons(Window window); ///< Check if the user is clicking on any buttons etc.
	};
}
