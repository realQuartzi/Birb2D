#include "Birb2D.hpp"
#include "Grid.hpp"

const static int top_bar_height = 20;
const static int side_panel_width = 200;

const static Birb::Vector2Int DEFAULT_LEVEL_SIZE = { 256, 256 };
const static float DEFAULT_LEVEL_SCALE = 16.0f;
const static float SCALE_TICK = 1;

static bool ApplicationRunning = true;

static Birb::ApplicationInfo appInfo("birb-editor");


void Quit()
{
	Birb::Debug::Log("Quitting...");
	ApplicationRunning = false;
}

int main(void)
{
	Birb::Debug::Log("Creating the window");
	Birb::Window window("Birb Editor", Birb::Vector2Int(1280, 720), 75, false);
	Birb::TimeStep timeStep;
	timeStep.Init(&window);

	/* --------- *
	 * Variables *
	 * --------- */

	/* Locate resources */
	appInfo.LocateResources();

	/* Fonts */
	Birb::Font titleFont(appInfo.ResLocation + "/fonts/freefont/FreeSansBold.ttf");

	/* Top bar */
	Birb::Scene top_bar;
	Birb::Rect top_bar_background(0, 0, window.dimensions.x, top_bar_height);
	top_bar_background.color = 0x2a2738;
	top_bar.AddObject(&top_bar_background);

	Birb::Entity titleText("Title text", Birb::Vector2Int(4, 4), Birb::EntityComponent::Text("Birb Editor", &titleFont, &Birb::Colors::White));
	titleText.renderingPriority = 1;
	top_bar.AddObject(&titleText);

	Birb::Entity exitButton("Exit button", Birb::Rect(window.dimensions.x - 18, 2, 16, 16));
	exitButton.rect.color = Birb::Colors::Red;
	exitButton.renderingPriority = 20;
	top_bar.AddObject(&exitButton);


	/* Side panel */
	Birb::Scene side_panel;
	side_panel.SetPosition({ 0, top_bar_height });
	Birb::Rect side_panel_background(0, 0, side_panel_width, window.dimensions.y - top_bar_height);
	side_panel_background.color = 0x3a3844;
	side_panel.AddObject(&side_panel_background);

	/* Level viewer window */
	Birb::Scene level_view;
	level_view.SetPosition({ side_panel_width, top_bar_height });
	Birb::Rect level_view_background(0, 0, window.dimensions.x - side_panel_width, window.dimensions.y - top_bar_height);
	level_view_background.renderingPriority = -10;
	level_view_background.color = 0x565566;
	level_view.AddObject(&level_view_background);

	Birb::Scene level_grid;
	level_grid.renderingPriority = 5;

	Birb::Scene level_scene;
	level_scene.renderingPriority = 1;

	bool pending_level_scene_update = false;
	bool pending_level_scene_position_update = false;
	bool pending_level_grid_update = false;

	Birb::Vector2Int grid_position_offset;
	bool mwheel_dragging = false;
	Birb::Vector2Int drag_start_pos;

	Birb::Entity coordinate_text("Mouse coordinate text", Birb::Vector2Int(5, 5), Birb::EntityComponent::Text("", &titleFont, &Birb::Colors::White));

	/* Set some default values, so that there's something to render before
	 * the user moves their cursor over the editor window */
	coordinate_text.rect.w = 1;
	coordinate_text.rect.h = 1;
	coordinate_text.rect.color = level_view_background.color;

	coordinate_text.textComponent.bgColor = &Birb::Colors::DarkGray;
	coordinate_text.renderingPriority = 11;
	level_view.AddObject(&coordinate_text);

	float current_scale = DEFAULT_LEVEL_SCALE;
	level_view.AddObject(&level_grid);
	level_view.AddObject(&level_scene);

	Birb::Level level(DEFAULT_LEVEL_SIZE);
	level.SetScale(DEFAULT_LEVEL_SCALE);

	/** Create the grid lines **/
	std::vector<Birb::Line> horizontal_lines;
	std::vector<Birb::Line> vertical_lines;

	GenerateGridLines(window, &level_grid, level_view, grid_position_offset, DEFAULT_LEVEL_SCALE, DEFAULT_LEVEL_SIZE, horizontal_lines, vertical_lines);

	Birb::Debug::Log("Starting the game loop");

	Birb::UI ui;

	exitButton.clickComponent = Birb::EntityComponent::Click(Quit);
	ui.AddButton(&exitButton);

	Birb::Vector2Int last_cursor_pos;

	while (ApplicationRunning)
	{
		timeStep.Start();
		while (timeStep.Running())
		{
			/* Handle input stuff */
			while (window.PollEvents())
			{
				window.EventTick(window.event, &ApplicationRunning);
				ui.PollButtons(window);

				/* Scroll event */
				if (window.event.type == SDL_MOUSEWHEEL)
				{
					if (window.event.wheel.y > 0) /* Scroll up */
						current_scale += SCALE_TICK * current_scale * 0.2f;
					else if(window.event.wheel.y < 0) /* Scroll down */
						current_scale -= SCALE_TICK * current_scale * 0.2f;
					else
						continue;

					/* Clamp the scale value */
					current_scale = Birb::Math::Clamp(current_scale, 5.0f, 100.0f);


					/* Re-generate the grid lines */
					pending_level_grid_update = true;

					/* Update the level scene scale */
					level.SetScale(current_scale);
					pending_level_scene_update = true;
				}

				/* Mouse button events */
				if (window.isMouseDown())
				{
					switch (window.event.button.button)
					{
						/* Start dragging */
						case (SDL_BUTTON_MIDDLE):
							drag_start_pos = window.CursorPosition();
							mwheel_dragging = true;
							break;

						/* Mouse click */
						case (SDL_BUTTON_LEFT):
						{
							Birb::Vector2Int tile_pos = ClickToIndex(level_grid, DEFAULT_LEVEL_SIZE, window.CursorPosition(), current_scale);
							std::cout << "Tile pos: " << tile_pos << std::endl;

							/* Set the tile */
							if (tile_pos.x != -1 && tile_pos.y != -1)
							{
								if (level.GetTile(tile_pos).is_empty)
								{
									Birb::Level::Tile new_tile;
									new_tile.rect.color = 0xffffff;

									level.SetTile(tile_pos, new_tile);
								}
								else
								{
									Birb::Level::Tile tile = level.GetTile(tile_pos);
									tile.rect.color.ChangeIntensity(-10);
									level.SetTile(tile_pos, tile);
								}

								/* Update the level view */
								pending_level_scene_update = true;
							}

							break;
						}

						default:
							break;
					}
				}

				if (window.isMouseUp())
					if (window.event.button.button == SDL_BUTTON_MIDDLE)
						mwheel_dragging = false; /* Stop dragging */

				/* Update the offset if dragging */
				if (mwheel_dragging)
				{
					grid_position_offset = grid_position_offset - (drag_start_pos - window.CursorPosition());
					drag_start_pos = window.CursorPosition();

					/* Update the grid */
					pending_level_grid_update = true;

					/* Update the level view */
					pending_level_scene_position_update = true;
					//level_scene.SetPosition(level_grid.Position());
				}

				/* Keyboard events */
				if (window.isKeyDown())
				{
					switch (window.key_event())
					{
						case (Birb::Input::HOME):
							/* Reset the offset */
							grid_position_offset = {0, 0};
							pending_level_grid_update = true;
							pending_level_scene_position_update = true;
							break;

						case (Birb::Input::H):
							/* Toggle the grid */
							level_grid.Toggle();
							break;

						default:
							break;
					}
				}
			}

			timeStep.Step();
		}

		if (pending_level_grid_update)
		{
			GenerateGridLines(window, &level_grid, level_view, grid_position_offset, current_scale, DEFAULT_LEVEL_SIZE, horizontal_lines, vertical_lines);
			pending_level_grid_update = false;
		}

		if (pending_level_scene_update)
		{
			level_scene = level.ToScene();
			level_scene.SetPosition(level_grid.Position());
			pending_level_scene_update = false;
		}

		if (pending_level_scene_position_update)
		{
			level_scene.SetPosition(level_grid.Position());
			pending_level_scene_position_update = false;
		}

		if (window.CursorPosition() != last_cursor_pos)
		{
			Birb::Vector2Int current_tile_pos = ClickToIndex(level_grid, DEFAULT_LEVEL_SIZE, window.CursorPosition(), current_scale);
			coordinate_text.SetText(current_tile_pos.ToString());
			last_cursor_pos = window.CursorPosition();
		}

		window.Clear();
		/** Handle rendering **/

		/* Render scenes */
		level_view.Render();
		top_bar.Render();
		side_panel.Render();

		/** End of rendering **/
		window.Display();

		timeStep.End();
	}

	return 0;
}
