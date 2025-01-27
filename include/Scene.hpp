#pragma once

#include "STD.hpp"
#include "Vector.hpp"
#include "SceneObject.hpp"

namespace Birb
{
	class Scene : public SceneObject
	{
	public:
		Scene();
		Scene(const bool& isActive);
		void AddObject(SceneObject* obj);
		void AddObject(SceneObject** obj, int objCount);
		std::vector<SceneObject*> GetObjects() const;

		int ObjectCount() const;

		void Activate();
		void Deactivate();
		void Toggle();
		bool isActive() const;

		void SetPosition(const Vector2& newPosition);
		void Translate(const Vector2& delta);
		Vector2 Position() const;

		void Clear(); ///< Empty the scene
		void Render() const; ///< Render all objects in the scene if the scene is active

	private:
		void SortObjects(); ///< Sort objects according to their priority

		std::vector<SceneObject*> objects;
		bool active;

		Vector2 positionOffset;

		/* Variables for a scene that is a child of another scene */
		void RenderFunc() override; ///< Gets called when the scene is a child of another scene
		void SetPos(const Vector2& delta) override; ///< The same as Translate, used when the scene is a SceneObject
	};
}
