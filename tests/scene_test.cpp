#include "doctest.h"
#include "Entity.hpp"
#include "Graphs.hpp"
#include "Scene.hpp"

namespace Birb
{
	TEST_CASE("Scenes")
	{
		Scene scene;

		Entity entityA("Test entity A");
		Entity entityB("Test entity B");
		entityB.renderingPriority = 5;

		Widgets::Graph graphs[2] = {
			Widgets::Graph(),
			Widgets::Graph(),
		};
		graphs[1].renderingPriority = 3;

		Polygon polygon({{0, 0}, {2, 0}, {1, 1}}, 3);
		Line line({0, 0}, {3, 3});
		Rect rect(0, 0, 10, 10);
		Circle circle(3, {0, 0});

		scene.AddObject(&entityA);
		scene.AddObject(&entityB);
		scene.AddObject(&graphs[0]);
		scene.AddObject(&graphs[1]);
		scene.AddObject(&polygon);
		scene.AddObject(&line);
		scene.AddObject(&rect);
		scene.AddObject(&circle);

		CHECK(scene.ObjectCount() == 8);

		std::vector<SceneObject*> objPointers = scene.GetObjects();
		CHECK(objPointers[objPointers.size() - 1] == &entityB);
		CHECK(objPointers[objPointers.size() - 2] == &graphs[1]);

		SUBCASE("Scene state changing")
		{
			scene.Deactivate();
			CHECK_FALSE(scene.isActive());
			CHECK_FALSE(entityA.sceneActive);
			CHECK_FALSE(entityB.sceneActive);
			CHECK_FALSE(polygon.sceneActive);
			CHECK_FALSE(line.sceneActive);

			scene.Activate();
			CHECK(scene.isActive());
			CHECK(entityA.sceneActive);
			CHECK(entityB.sceneActive);
			CHECK(polygon.sceneActive);
			CHECK(line.sceneActive);

			scene.Toggle();
			CHECK_FALSE(scene.isActive());
			CHECK_FALSE(entityA.sceneActive);
			CHECK_FALSE(entityB.sceneActive);
			CHECK_FALSE(polygon.sceneActive);
			CHECK_FALSE(line.sceneActive);
		}

		SUBCASE("Scene position change")
		{
			/* Set some default positions for scene objects */
			entityA.rect = {0, 150, 128, 72};
			entityB.rect = {50, 100, 128, 72};
			graphs[0].rect = {50, 100, 128, 72};
			graphs[1].rect = {50, 100, 128, 72};

			/* Set a specific position */
			scene.SetPosition(Vector2(10, 10));
			CHECK(scene.Position() == Vector2(10, 10));

			CHECK(entityA.rect == Rect(10, 160, 128, 72));
			CHECK(entityB.rect == Rect(60, 110, 128, 72));
			CHECK(graphs[0].rect == Rect(60, 110, 128, 72));
			CHECK(graphs[1].rect == Rect(60, 110, 128, 72));

			CHECK(polygon.points[0] == Vector2(10, 10));
			CHECK(polygon.points[1] == Vector2(12, 10));
			CHECK(polygon.points[2] == Vector2(11, 11));

			CHECK(line.pointA == Vector2(10, 10));
			CHECK(line.pointB == Vector2(13, 13));

			CHECK(rect == Rect(10, 10, 10, 10));
			CHECK(circle.pos == Vector2Int(10, 10));

			/* Move the scene to the other direction with a delta amount */
			scene.Translate(Vector2(-20, -20));
			CHECK(scene.Position() == Vector2(-10, -10));

			CHECK(entityA.rect == Rect(-10, 140, 128, 72));
			CHECK(entityB.rect == Rect(40, 90, 128, 72));
			CHECK(graphs[0].rect == Rect(40, 90, 128, 72));
			CHECK(graphs[1].rect == Rect(40, 90, 128, 72));

			CHECK(polygon.points[0] == Vector2(-10, -10));
			CHECK(polygon.points[1] == Vector2(-8, -10));
			CHECK(polygon.points[2] == Vector2(-9, -9));

			CHECK(line.pointA == Vector2(-10, -10));
			CHECK(line.pointB == Vector2(-7, -7));

			CHECK(rect == Rect(-10, -10, 10, 10));
			CHECK(circle.pos == Vector2Int(-10, -10));

			/* Reset scene position back to the original position */
			scene.SetPosition(Vector2(0, 0));
			CHECK(scene.Position() == Vector2(0, 0));
			CHECK(entityA.rect == Rect(0, 150, 128, 72));
			CHECK(entityB.rect == Rect(50, 100, 128, 72));
			CHECK(graphs[0].rect == Rect(50, 100, 128, 72));
			CHECK(graphs[1].rect == Rect(50, 100, 128, 72));

			CHECK(polygon.points[0] == Vector2(0, 0));
			CHECK(polygon.points[1] == Vector2(2, 0));
			CHECK(polygon.points[2] == Vector2(1, 1));

			CHECK(line.pointA == Vector2(0, 0));
			CHECK(line.pointB == Vector2(3, 3));

			CHECK(rect == Rect(0, 0, 10, 10));
			CHECK(circle.pos == Vector2Int(0, 0));
		}

		SUBCASE("Update the changed scene position for new objects")
		{
			entityA.rect = Rect(10, 10, 20, 20);
			scene.SetPosition(Vector2(10, 10));
			CHECK(entityA.rect == Rect(20, 20, 20, 20));

			/* Add a new objects after setting the position */
			Rect newRect(10, 10, 20, 20);
			scene.AddObject(&newRect);
			CHECK(newRect == Rect(20, 20, 20, 20));

			scene.Translate(Vector2(10, 10));
			Rect newRectB(10, 10, 30, 30);
			scene.AddObject(&newRectB);
			CHECK(newRect == Rect(30, 30, 20, 20));
			CHECK(newRectB == Rect(30, 30, 30, 30));
			CHECK(entityA.rect == Rect(30, 30, 20, 20));
		}

		SUBCASE("Reset the scene")
		{
			scene.Clear();
			CHECK(scene.ObjectCount() == 0);
		}
	}

	TEST_CASE("Scene that is inactive by default")
	{
		Scene sceneThatIsDisabled(false);
		CHECK(sceneThatIsDisabled.isActive() == false);
	}

	TEST_CASE("Scene sorting")
	{
		/* Scene sorting is already partially tested in the previous
		 * test cases, but this test case is meant to test that part
		 * a bit more thoroughly just to make sure that the sorting algo
		 * works as intended */
		Scene testScene;

		/* We'll be using empty scenes as testing objects */
		Scene sceneA;
		sceneA.renderingPriority = 4;
		testScene.AddObject(&sceneA);

		Scene sceneB;
		sceneB.renderingPriority = 10;
		testScene.AddObject(&sceneB);

		Scene sceneC;
		sceneC.renderingPriority = -2;
		testScene.AddObject(&sceneC);

		Scene sceneD;
		sceneD.renderingPriority = 0;
		testScene.AddObject(&sceneD);

		Scene sceneE;
		sceneE.renderingPriority = 2;
		testScene.AddObject(&sceneE);

		Scene sceneF;
		sceneF.renderingPriority = 2;
		testScene.AddObject(&sceneF);

		std::vector<SceneObject*> sortedList = testScene.GetObjects();
		CHECK(sortedList[0] == &sceneC);
		CHECK(sortedList[1] == &sceneD);
		CHECK(sortedList[2] == &sceneE);
		CHECK(sortedList[3] == &sceneF);
		CHECK(sortedList[4] == &sceneA);
		CHECK(sortedList[5] == &sceneB);
	}
}
