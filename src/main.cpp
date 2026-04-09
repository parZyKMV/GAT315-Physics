#include "raylib.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include <vector>
#include "raymath.h"
#include <string>

#include "Random.h"
#include "Body.h"
#include "World.h"
#include "Integration.h"
#include "Effector.h"
#include "PointEffector.h"
#include "Gravitational_Effector.h"

// Moved to Body.h and Body.cpp
//struct Body
//{
//	Vector2 position;
//	Vector2 velocity;
//	Vector2 acceleration;
//	float mass;
//	float size;	
//	float restitution;
//	Color color;
//};

// Moved to Random.h
//float getRandomFloat()
//{
//	return GetRandomValue(0, 1000) / 1000.0f;
//}

// Moved to Body.cpp as Body::addForce()
//void addForce(Body& body, Vector2 force)
//{
//	body.acceleration += force/body.mass;
//}

// Moved to Integration.h
//void ExplicitEuler(Body& body, float dt) { ... }

// Moved to Integration.h - we use this one (semi-implicit is more stable)
//void SemiImplicitEuler(Body& body, float dt) { ... }

// Moved to World.h
//Vector2 gravity{ 0, 100.0f };

int main()
{
	// Moved to World - world manages the list of bodies now
	//std::vector<Body> bodies;
	//bodies.reserve(1000);

	SetRandomSeed(GetTime());

	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(1280, 800, "Hello Raylib");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	Texture wabbit = LoadTexture("wabbit_alpha.png");

	//SetTargetFPS(10);

	World world;
	world.gravity = { 0, 0 }; // turn off world gravity so only gravitational attraction between bodies acts

	// Point effector in the center - pushes bodies away within its radius
	world.AddEffector(new PointEffector({ 400, 400 }, 200.0f, 50000.0f));

	// Gravitational effector - pulls every body toward every other body
	world.AddEffector(new GravitationalEffector(10000.0f));

	world.bodies.reserve(1000);

	float timeAccum = 0.0f;
	float fixedTimeStep = 1.0f / 60.0f; // Fixed physics rate for stability

	// game loop
	while (!WindowShouldClose())
	{
		float dt = GetFrameTime();

		if (IsMouseButtonPressed(0) ||
			(IsKeyDown(KEY_LEFT_CONTROL) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)))
		{
			Body body;

			// Default spawn is Dynamic. Hold ALT to spawn Kinematic bodies that ignore forces.
			body.type = (IsKeyDown(KEY_LEFT_ALT)) ? BodyType::Kinematic : BodyType::Dynamic;

			body.position = GetMousePosition();
			body.acceleration = { 0, 0 };
			body.size = GetRandomFloat(5.0f, 35.0f);
			body.restitution = GetRandomFloat(0.5f, 1.0f);

			// Mass = size so bigger bodies have stronger gravitational pull
			body.mass = body.size;
			body.inverseMass = (body.type == BodyType::Static) ? 0.0f : 1.0f / body.mass;

			body.gravityScale = 1.0f; // full gravity
			body.damping = 0.2f; // a little drag so bodies slow down over time

			float angle = GetRandomFloat(2.0f * PI);
			body.velocity = {
				cosf(angle) * GetRandomFloat(50.0f, 350.0f),
				sinf(angle) * GetRandomFloat(50.0f, 350.0f)
			};

			body.velocity *= 0.001f;
			// Moved to Random.h as GetRandomFloat(min, max)
			//Vector2 direction;
			//direction.x = cosf(angle);
			//direction.y = sinf(angle);

			body.color = Color{
				(unsigned char)GetRandomValue(100, 255),
				(unsigned char)GetRandomValue(0,   100),
				(unsigned char)GetRandomValue(100, 255),
				255
			};

			world.AddBody(body);
		}

		// Moved to World::Step() - world handles gravity and integration now
		//for (auto& body : bodies) body.acceleration = Vector2{ 0,0 };
		//for (auto& body : bodies) addForce(body, (gravity * 100.0f));

		// Apply radial force before stepping so physics reacts this frame
		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
		{
			Vector2 mousePos = GetMousePosition();
			world.ApplyRadialForce(mousePos, 100.0f, 100000.0f);
		}

		// Fixed timestep accumulator - keeps physics stable regardless of frame rate
		timeAccum += dt;
		while (timeAccum > fixedTimeStep)
		{
			world.Step(fixedTimeStep);
			timeAccum -= fixedTimeStep;
		}

		// Moved to World::Step() - world handles integration now
		//for (auto& body : bodies) SemiImplicitEuler(body, dt);

		// Moved to World::Step() - world handles collisions now
		//for (auto& body : bodies)
		//{
		//	if (body.position.x + body.size > GetScreenWidth()) ...
		//}

		// drawing
		BeginDrawing();

		ClearBackground(BLACK);

		// Show FPS so we can monitor performance
		std::string text = "FPS: ";
		text += std::to_string(GetFPS());
		DrawText(text.c_str(), 200, 200, 20, WHITE);

		//DrawTexture(wabbit, 400, 200, WHITE);

		// Moved to World::Draw() - world draws all bodies and effectors now
		//for (Body& body : bodies)
		//{
		//	DrawCircleV(body.position, body.size, body.color);
		//}
		world.Draw();

		// Draw the repulsion radius while right click is held - must be inside BeginDrawing()
		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
		{
			DrawCircleLinesV(GetMousePosition(), 100.0f, RED);
		}

		EndDrawing();
	}

	// cleanup
	UnloadTexture(wabbit);
	CloseWindow();
	return 0;
}