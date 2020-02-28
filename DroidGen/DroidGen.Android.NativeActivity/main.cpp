/*
* Copyright (C) 2010 The Android Open Source Project
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
*/

//#include "glew.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"

#include <string>
#include <fstream>

#include "Terrain.h"
#include "PerformanceTool.h"
#include "PerlinNoise.h"
#include "RandomWalk.h"
#include "ObjectGenerator.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "AndroidProject1.NativeActivity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "AndroidProject1.NativeActivity", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "AndroidProject1.NativeActivity", __VA_ARGS__))



/**
* Our saved state data.
*/
struct saved_state {
	float angle;
	int32_t x;
	int32_t y;
};

/**
* Shared state for our app.
*/
struct engine {
	struct android_app* app;

	ASensorManager* sensorManager;
	const ASensor* accelerometerSensor;
	ASensorEventQueue* sensorEventQueue;

	int animating;
	EGLDisplay display;
	EGLSurface surface;
	EGLContext context;
	int32_t width;
	int32_t height;
	struct saved_state state;
};
const int tileCount = 511;
std::vector<Terrain> terrains;
Terrain water;

PerformanceTool performance;
PerlinNoise perlin;
RandomWalk walker;
ObjectGenerator generator;

std::string levelStatus;

int largeCount = 0;
int smallCount = 0;
int enemyCount = 0;
int perimeterLimit = 8;
int largeObjectData[45];
float waterHeight;

// For performance data
int perfTimes[100] = { 0 };
int generationCount = 0;

const int textureWidth = 512;
const int textureHeight = 512;

float yDisplacement[textureWidth * textureHeight];
int playableArea[textureWidth * textureHeight];

/* Write to CSV */
/* Used to output test data */
void printCSV(int times[100])
{
	std::ofstream ofs("/sdcard/PerformanceData.csv");
	if (ofs != NULL)
	{
		// for each row
		for (int i = 0; i < 100; i++) 
		{
			ofs << times[i] << ',';
		}
		ofs.close();
	}
}
/* Generate heightmap data using a Perlin style noise generator */
/* Perform random walk algorithm to shape playable area */
void CreateMap()
{
	// Update level status
	levelStatus = "Generating terrain...";

	// Generate land features
	perlin.GeneratePerlin(textureWidth, textureHeight, yDisplacement);

	// Update level status
	levelStatus = "Generating playing area...";

	// Generate playable area
	walker.Walk(playableArea);
}
/* Combine the two generated maps */
void CombineMap()
{
	// Update level status
	levelStatus = "Combining maps...";

	// Get average height
	float averageHeight = 0.0f;
	int counter = 0;

	for (int j = 0; j < textureHeight; j++)
	{
		for (int i = 0; i < textureWidth; i++)
		{
			averageHeight += yDisplacement[j * textureWidth + i];
			counter++;
		}
	}

	// Divide sum of heights by count
	averageHeight /= (float)counter;

	// Drop by 10%
	averageHeight *= 0.9f;

	for (int j = 0; j < textureHeight; j++)
	{
		for (int i = 0; i < textureWidth; i++)
		{
			// Combine the maps
			if (playableArea[j * textureWidth + i] == 0)
			{
				yDisplacement[j * textureWidth + i] = averageHeight;
			}
		}
	}

	// Calculate water level
	waterHeight = averageHeight * 0.4;
}
/* Generate multiple types of object into the world space */
void GenerateObjects()
{
	// Update level status
	levelStatus = "Generating objects...";

	// Pseudo-random number of large objects
	largeCount = rand() % 5 + 10;

	// Pseudo-random number of large objects
	smallCount = rand() % 16 + 20;

	// Pseudo-random number of enemy locations
	enemyCount = rand() % 11 + 5;

	// Generator creates the location data
	generator.GenerateObjects(playableArea, yDisplacement, textureWidth, textureHeight, largeCount, smallCount, 
		enemyCount, perimeterLimit, largeObjectData);

	// Place objects using generated data

}
/* Height/gradient based texture application */
/* Not currently implemented */
void Texturer()
{

}
/* Generate "water" at the determined height */
/* Not currently implemented */
void WaterTable()
{
	wat/er.EditHeight(waterHeight);
}
void ApplyHeights()
{
	for (int j = 0; j < tileCount; j++)
	{
		for (int i = 0; i < tileCount; i++)
		{
			float aDisp = yDisplacement[j * textureWidth + i]; 
			float bDisp = yDisplacement[j * textureWidth + (i + 1)];
			float cDisp = yDisplacement[(j + 1) * textureWidth + (i + 1)];
			float dDisp = yDisplacement[(j + 1) * textureWidth + i];

			terrains[j * tileCount + i].EditHeights(aDisp, bDisp, cDisp, dDisp);
		}
	}
}
/* Place Objects */
/* Used to place representations of spawned objects */
/* Using the generated coordinates */
void PlaceObjects()
{
	po[]
}
/* Initialize the generation algorithms */
void GenerateLevel()
{
	performance.StartTimer();

	CreateMap();
	CombineMap();
	GenerateObjects();
	ApplyHeights();
	WaterTable();
	PlaceObjects();

	performance.EndTimer();

	if (generationCount < 100)
	{
		perfTimes[generationCount] = performance.GetSeconds();
		generationCount++;
	}
	else
	{
		int cheese = 250;
	}

	// Update level status
	levelStatus = "Generation complete...";
}

/**
* Initialize an EGL context for the current display.
*/
static int engine_init_display(struct engine* engine) {
	// initialize OpenGL ES and EGL

	/*
	* Here specify the attributes of the desired configuration.
	* Below, we select an EGLConfig with at least 8 bits per color
	* component compatible with on-screen windows
	*/
	const EGLint attribs[] = {
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_BLUE_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_RED_SIZE, 8,
		EGL_NONE
	};
	EGLint w, h, format;
	EGLint numConfigs;
	EGLConfig config;
	EGLSurface surface;
	EGLContext context;

	EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

	eglInitialize(display, 0, 0);

	/* Here, the application chooses the configuration it desires. In this
	* sample, we have a very simplified selection process, where we pick
	* the first EGLConfig that matches our criteria */
	eglChooseConfig(display, attribs, &config, 1, &numConfigs);

	/* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
	* guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
	* As soon as we picked a EGLConfig, we can safely reconfigure the
	* ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
	eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

	ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);

	surface = eglCreateWindowSurface(display, config, engine->app->window, NULL);
	context = eglCreateContext(display, config, NULL, NULL);

	if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
		LOGW("Unable to eglMakeCurrent");
		return -1;
	}

	eglQuerySurface(display, surface, EGL_WIDTH, &w);
	eglQuerySurface(display, surface, EGL_HEIGHT, &h);

	engine->display = display;
	engine->context = context;
	engine->surface = surface;
	engine->width = w;
	engine->height = h;
	engine->state.angle = 0;

	// Initialize GL state.
	glDisable(GL_DITHER);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
	glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, w, h);
	GLfloat ratio = (GLfloat)w / h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustumf(-ratio, ratio, -1, 1, 1, 10);

	return 0;
}

/**
* Just the current frame in the display.
*/

static void engine_draw_frame(struct engine* engine) {
	if (engine->display == NULL) {
		// No display.
		return;
	}

	// Prepare
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < tileCount * tileCount; i++)
	{
		int terrainList = terrains.size();
		terrains[i].Draw();
	}
	water.Draw();

	eglSwapBuffers(engine->display, engine->surface);
}


/**
* Tear down the EGL context currently associated with the display.
*/
static void engine_term_display(struct engine* engine) {
	if (engine->display != EGL_NO_DISPLAY) {
		eglMakeCurrent(engine->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		if (engine->context != EGL_NO_CONTEXT) {
			eglDestroyContext(engine->display, engine->context);
		}
		if (engine->surface != EGL_NO_SURFACE) {
			eglDestroySurface(engine->display, engine->surface);
		}
		eglTerminate(engine->display);
	}
	engine->animating = 0;
	engine->display = EGL_NO_DISPLAY;
	engine->context = EGL_NO_CONTEXT;
	engine->surface = EGL_NO_SURFACE;

	for (int i = 0; i < tileCount* tileCount; i++)
		terrains[i].TearDownGL();

	water.TearDownGL();
}

/**
* Process the next input event.
*/
static int32_t engine_handle_input(struct android_app* app, AInputEvent* event) {
	struct engine* engine = (struct engine*)app->userData;
	if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) 
	{
		engine->state.x = AMotionEvent_getX(event, 0);
		engine->state.y = AMotionEvent_getY(event, 0);

		int32_t AMotionEvent_getAction(const AInputEvent *motion_event);

		if (engine->state.x < 200 && engine->state.y < 200)
		{

		}
		LOGI("Touch and drag registered");

		return 1;
	}
	return 0;
}

void DrawGUI()
{
	//ImGui::Text("FPS:");
}



/**
* Process the next main command.
*/
static void engine_handle_cmd(struct android_app* app, int32_t cmd) {
	struct engine* engine = (struct engine*)app->userData;
	switch (cmd) {
	case APP_CMD_SAVE_STATE:
		// The system has asked us to save our current state.  Do so.
		engine->app->savedState = malloc(sizeof(struct saved_state));
		*((struct saved_state*)engine->app->savedState) = engine->state;
		engine->app->savedStateSize = sizeof(struct saved_state);
		break;
	case APP_CMD_INIT_WINDOW:
		// The window is being shown, get it ready.
		if (engine->app->window != NULL) {
			engine_init_display(engine);
			engine_draw_frame(engine);
		}
		break;
	case APP_CMD_TERM_WINDOW:
		// The window is being hidden or closed, clean it up.
		engine_term_display(engine);
		break;
	case APP_CMD_GAINED_FOCUS:
		// When our app gains focus, we start monitoring the accelerometer.
		if (engine->accelerometerSensor != NULL) {
			ASensorEventQueue_enableSensor(engine->sensorEventQueue,
				engine->accelerometerSensor);
			// We'd like to get 60 events per second (in us).
			ASensorEventQueue_setEventRate(engine->sensorEventQueue,
				engine->accelerometerSensor, (1000L / 60) * 1000);
		}
		break;
	case APP_CMD_LOST_FOCUS:
		// When our app loses focus, we stop monitoring the accelerometer.
		// This is to avoid consuming battery while not being used.
		if (engine->accelerometerSensor != NULL) {
			ASensorEventQueue_disableSensor(engine->sensorEventQueue,
				engine->accelerometerSensor);
		}
		// Also stop animating.
		engine->animating = 0;
		engine_draw_frame(engine);
		break;
	}
}

/**
* This is the main entry point of a native application that is using
* android_native_app_glue.  It runs in its own thread, with its own
* event loop for receiving input events and doing other things.
*/
void android_main(struct android_app* state) {
	struct engine engine;

	memset(&engine, 0, sizeof(engine));
	state->userData = &engine;
	state->onAppCmd = engine_handle_cmd;
	state->onInputEvent = engine_handle_input;
	engine.app = state;

	// Prepare to monitor accelerometer
	engine.sensorManager = ASensorManager_getInstance();
	engine.accelerometerSensor = ASensorManager_getDefaultSensor(engine.sensorManager,
		ASENSOR_TYPE_ACCELEROMETER);
	engine.sensorEventQueue = ASensorManager_createEventQueue(engine.sensorManager,
		state->looper, LOOPER_ID_USER, NULL, NULL);

	if (state->savedState != NULL) {
		// We are starting with a previous saved state; restore from it.
		engine.state = *(struct saved_state*)state->savedState;
	}

	for (int j = 0; j < tileCount; j++)
	{
		for (int i = 0; i < tileCount; i++)
		{
			Terrain* tile = new Terrain(tileCount, i, j);
			terrains.push_back(*tile);
		}
	}

	water.SetPosition(1, 0, 0);
	
	engine.animating = 1;

	// loop waiting for stuff to do.

	while (1) {
		// Read all pending events.
		int ident;
		int events;
		struct android_poll_source* source;

		// If not animating, we will block forever waiting for events.
		// If animating, we loop until all events are read, then continue
		// to draw the next frame of animation.
		while ((ident = ALooper_pollAll(engine.animating ? 0 : -1, NULL, &events,
			(void**)&source)) >= 0) {

			// Process this event.
			if (source != NULL) {
				source->process(state, source);
			}

			// If a sensor has data, process it now.
			if (ident == LOOPER_ID_USER) {
				if (engine.accelerometerSensor != NULL) {
					ASensorEvent event;
					while (ASensorEventQueue_getEvents(engine.sensorEventQueue,
						&event, 1) > 0) {
						LOGI("accelerometer: x=%f y=%f z=%f",
							event.acceleration.x, event.acceleration.y,
							event.acceleration.z);
					}
				}
			}

			// Check if we are exiting.
			if (state->destroyRequested != 0) {
				engine_term_display(&engine);
				return;
			}
		}

		if (engine.animating) {
			// Done with events; draw next animation frame.
			for (int i = 0; i < tileCount * tileCount; i++)
				terrains[i].Update();

			water.Update();
			//if (engine.state.angle > 1) {
			//	engine.state.angle = 0;
			//}

			// Drawing is throttled to the screen update rate, so there
			// is no need to do timing here.
			engine_draw_frame(&engine);

			GenerateLevel();
		}
	}
}
