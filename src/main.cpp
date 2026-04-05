#include "raylib.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include <vector>
#include "raymath.h"

#include "Random.h"
#include "Body.h"
#include "World.h"

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

// Moved to Body.cpp as Body::AddForce()
//void addForce(Body& body, Vector2 force)
//{
//	body.acceleration += force/body.mass;
//}

// Moved to Body.cpp as Body::Step()
//void ExplicitEuler(Body& body, float dt)
//{
//	body.position += body.velocity * dt;
//	body.velocity += body.acceleration * dt;
//	body.acceleration = { 0,0 };
//}

// Moved to Body.cpp as Body::Step() - we use this one (semi-implicit is more stable)
//void SemiImplicitEuler(Body& body, float dt)
//{ 	
//	body.velocity += body.acceleration * dt;
//	body.position += body.velocity * dt;
//	body.acceleration = { 0,0 };
//}

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

	World world;
	world.bodies.reserve(1000);

	// game loop
	while (!WindowShouldClose())		// run the loop until the user presses ESCAPE or presses the Close button on the window
	{
		float dt = GetFrameTime();

		if (IsMouseButtonPressed(0) ||
			(IsKeyDown(KEY_LEFT_CONTROL) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)))
		{
			Body body;
			body.position = GetMousePosition();
			body.acceleration = { 0,0 };
			body.size = GetRandomFloat(5.0f, 35.0f);
			body.restitution = GetRandomFloat(0.5f, 1.0f);
			body.mass = body.size * 10.0f;

			float angle = GetRandomFloat(2.0f * PI);
			body.velocity = {
				cosf(angle) * GetRandomFloat(50.0f, 350.0f),
				sinf(angle) * GetRandomFloat(50.0f, 350.0f)
			};

			// Moved to Random.h as GetRandomFloat(min, max)
			//Vector2 direction;
			//direction.x = cosf(angle);
			//direction.y = sinf(angle);

			body.color = Color{
				(unsigned char)GetRandomValue(100, 255),
				(unsigned char)GetRandomValue(0, 100),
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
			world.ApplyRadialForce(mousePos, 100.0f, -100000.0f);
		}

		world.Step(dt);

		// Moved to World::Step() - world handles integration now
		//for (auto& body : bodies) SemiImplicitEuler(body, dt);

		// Moved to World::Step() - world handles collisions now
		//for (auto& body : bodies)
		//{
		//	if (body.position.x + body.size > GetScreenWidth())
		//	{
		//		body.position.x = GetScreenWidth() - body.size;
		//		body.velocity.x *= -body.restitution;
		//	}
		//	if (body.position.x - body.size < 0)
		//	{
		//		body.position.x = body.size;
		//		body.velocity.x *= -body.restitution;
		//	}
		//	if (body.position.y + body.size > GetScreenHeight())
		//	{
		//		body.position.y = GetScreenHeight() - body.size;
		//		body.velocity.y *= -body.restitution;
		//	}
		//	if (body.position.y - body.size < 0)
		//	{
		//		body.position.y = body.size;
		//		body.velocity.y *= -body.restitution;
		//	}
		//}

		// drawing
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(BLACK);

		// draw some text using the default font
		DrawText("Hello Raylib", 200, 200, 20, WHITE);

		// draw our texture to the screen
		DrawTexture(wabbit, 400, 200, WHITE);

		// Moved to World::Draw() - world draws all bodies now
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

		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// cleanup
	// unload our texture so it can be cleaned up
	UnloadTexture(wabbit);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}