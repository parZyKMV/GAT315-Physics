#include "raylib.h"
#include "resource_dir.h"
#include <vector>
#include "raymath.h"
#include <string>

#include "Random.h"
#include "Body.h"
#include "World.h"
#include "world_camera.h"
#include "Integration.h"
#include "Effector.h"
#include "Spring.h"
#include "PointEffector.h"
#include "Gravitational_Effector.h"
#include "area_Effector.h"
#include "drag_Effector.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#define GUI_PHYSICS_IMPLEMENTATION
#pragma warning(push)
#pragma warning(disable: 4576)
#include "gui_physics.h"
#pragma warning(pop)

GuiPhysicsState state;

// Both functions must be declared before main so the compiler knows they exist
void AddBody(World& world, WorldCamera& camera);
void AddEffector(World& world, WorldCamera& camera);

int main()
{
	SetRandomSeed((unsigned int)GetTime());

	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(1280, 800, "Hello Raylib");

	// Get GUI state
	state = InitGuiPhysics();
	

	SearchAndSetResourceDir("resources");

	Texture wabbit = LoadTexture("wabbit_alpha.png");

	World world;
	WorldCamera world_camera(Vector2{ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f }, 500);
	world.SetBounds(world_camera.ScreenToWorld({ 0, (float)GetScreenHeight() }), world_camera.ScreenToWorld({ (float)GetScreenWidth(), 0 }));
	world.bodies.reserve(1000);


	Body* selectedBody = nullptr;
	Body* connectedBody = nullptr;

	float timeAccum = 0.0f;
	float fixedTimeStep = 1.0f / 60.0f;

	while (!WindowShouldClose())
	{
		float dt = fminf(GetFrameTime(), 0.1f);

		if (IsKeyPressed(KEY_SPACE)) state.SimulateActive = !state.SimulateActive;
		if (IsKeyPressed(KEY_TAB))   state.PhysicsPanelActive = !state.PhysicsPanelActive;

		// GUI panel occupies anchor02 + (0,0) to (304, 664) - matches InitGuiPhysics values
		bool mouseOverGui = state.PhysicsPanelActive &&
			CheckCollisionPointRec(GetMousePosition(), Rectangle{ 24, 40, 304, 664 });

		// gravity is a plain member variable, not a static method
		world.gravity = { 0, state.GravityValue };

		// Spawn body or effector only when NOT clicking over the GUI panel
		if (!mouseOverGui)
		{
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) ||
				(IsKeyDown(KEY_LEFT_CONTROL) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)))
			{
				if (IsKeyDown(KEY_LEFT_SHIFT))
					AddEffector(world,world_camera); // Hold Shift + Click to place an effector
				else
					AddBody(world,world_camera);     // Normal click spawns a body
			}

			if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
			{
				selectedBody = world.GetBodyInteract(GetMousePosition());
			}
			if (selectedBody)
			{
				if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && IsKeyDown(KEY_LEFT_CONTROL))
				{
					Vector2 position = world_camera.ScreenToWorld(GetMousePosition());
					Vector2 force = Spring::GetSpringForce(position, selectedBody->position, 1.0f, 1.0f);
					selectedBody->addForce(force);

					DrawLineV(position, selectedBody->position, YELLOW);
				}
			}
		}

		// Apply radial force before stepping so physics reacts this frame
		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
		{
			Vector2 mousePos = GetMousePosition();
			world.ApplyRadialForce(mousePos, 100.0f, 100000.0f);
		}

		// Fixed timestep accumulator - keeps physics stable regardless of frame rate
		if (state.SimulateActive)
		{
			timeAccum += dt;
			while (timeAccum > fixedTimeStep)
			{
				world.Step(fixedTimeStep);
				timeAccum -= fixedTimeStep;
			}
		}

		BeginDrawing();
		ClearBackground(BLACK);

		// Show FPS and body count
		std::string text = "FPS: " + std::to_string(GetFPS());
		DrawText(text.c_str(), 10, 10, 20, WHITE);

		std::string bodyCount = "Bodies: " + std::to_string(world.bodies.size());
		DrawText(bodyCount.c_str(), 10, 35, 20, WHITE);

		world_camera.Begin();
		world.Draw();
		world_camera.End();


		if (selectedBody)
		{
			DrawCircleLinesV(selectedBody->position, selectedBody->size + 5, YELLOW);
			 if (connectedBody)
			 {
				 DrawLineV(selectedBody->position, connectedBody->position, YELLOW);
			 }
		}

		GuiPhysics(&state);

		// Draw the repulsion radius while right click is held - inside BeginDrawing
		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
			DrawCircleLinesV(GetMousePosition(), 100.0f, RED);

		EndDrawing();
	}

	UnloadTexture(wabbit);
	CloseWindow();
	return 0;
}

void AddBody(World& world,WorldCamera& camera)
{
	Body body;

	body.type = (BodyType)state.BodyTypeActive;
	body.position = camera.ScreenToWorld(GetMousePosition());
	body.acceleration = { 0, 0 };
	body.size = state.BodySizeValue;
	body.restitution = state.BodyRestitutionValue;

	// Mass = size * multiplier so bigger bodies have stronger gravitational pull
	body.mass = body.size * state.BodyMassValue;
	body.inverseMass = (body.type == BodyType::Static) ? 0.0f : 1.0f / body.mass;

	body.gravityScale = 1.0f;
	body.damping = state.BodyDampingValue;

	float angle = GetRandomFloat(0.0f, 2.0f * PI);
	body.velocity = {
		cosf(angle) * GetRandomFloat(50.0f, 350.0f),
		sinf(angle) * GetRandomFloat(50.0f, 350.0f)
	};

	body.color = Color{
		(unsigned char)GetRandomValue(100, 255),
		(unsigned char)GetRandomValue(0,   100),
		(unsigned char)GetRandomValue(100, 255),
		255
	};

	world.AddBody(body);
}

void AddEffector(World& world,WorldCamera& camera)
{
	Effector* effector = nullptr;
	Vector2 mousePos = camera.ScreenToWorld(GetMousePosition());

	switch (state.EffectorTypeActive)
	{
	case 0: // Point Effector - pushes/pulls bodies radially from a point
		effector = new PointEffector(mousePos, state.EffectorSizeValue, state.EffectorForceValue);
		break;
	case 1: // Gravitational Effector - attracts bodies toward each other
		effector = new GravitationalEffector(mousePos, state.EffectorSizeValue, state.EffectorForceValue);
		break;
	case 2: // Area Effector - applies constant directional force inside a region
		effector = new AreaEffector(mousePos, state.EffectorSizeValue, state.EffectorForceValue, state.EffectorAngleValue);
		break;
	case 3: // Drag Effector - slows bodies down inside the region
		effector = new DragEffector(mousePos, state.EffectorSizeValue, state.EffectorForceValue);
		break;
	default:
		break;
	}

	if (effector != nullptr)
		world.AddEffector(effector);
}