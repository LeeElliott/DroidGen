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


#include <string>
#include <fstream>

#include "Terrain.h"
#include "PerformanceTool.h"
#include "PerlinNoise.h"
#include "RandomWalk.h"
#include "ObjectGenerator.h"
#include "ObjectMarker.h"
#include "Water.h"

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
const int chunkCount = 34;
std::vector<Terrain> terrains;
std::vector<ObjectMarker> largeMarkers;
std::vector<ObjectMarker> smallMarkers;
std::vector<ObjectMarker> enemyMarkers;
Water water;

PerformanceTool performance;
PerlinNoise perlin;
RandomWalk walker;
ObjectGenerator generator;

std::string levelStatus;

int largeCount = 0;
int smallCount = 0;
int enemyCount = 0;
int perimeterLimit = 8;
int largeObjectData[60] = { 0 };
int smallObjectData[70] = { 0 };
int enemyData[30] = { 0 };
float averageHeight = 0.0f;
float waterHeight;
bool canGen = true;

// For performance data
int perfTimes[250] = { 0 };
int generationCount = 0;

// Camera variables
float cameraX = 0;
float cameraY = 0;
float cameraZ = 0;
float cameraRotation = 0;

const int textureWidth = 512;
const int textureHeight = 512;

float yDisplacement[textureWidth * textureHeight];
int playableArea[textureWidth * textureHeight];

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
/* Calculate the average terrain height */
/* Calculate the desired water height */
void CombineMap()
{
	// Update level status
	levelStatus = "Combining maps...";

	// Get average height
	
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
/* Generate coordinate data for multiple objevt types */
/* To populate the world space */
void GenerateObjects()
{
	// Update level status
	levelStatus = "Generating objects...";

	// Pseudo-random number of large objects
	largeCount = rand() % 5 + 5;

	// Pseudo-random number of large objects
	smallCount = rand() % 11 + 5;

	// Pseudo-random number of enemy locations
	enemyCount = rand() % 11 + 5;

	// Generator creates the location data
	generator.GenerateObjects(playableArea, yDisplacement, textureWidth, textureHeight, largeCount, smallCount, 
		enemyCount, perimeterLimit, largeObjectData, smallObjectData, enemyData, waterHeight);
}
/* Height/gradient based texture application */
/* Not currently implemented */
void Texturer()
{

}
/* Generate "water" at the determined height */
/* Minimal implementation */
void WaterTable()
{
	water.EditHeight(waterHeight);
}
void ApplyHeights()
{
	float heights[256];

	// Loop through the chunks
	for (int j = 0; j < chunkCount; j++)
	{
		for (int i = 0; i < chunkCount; i++)
		{

			int x = i * 15;	int y = j * 15;

			for (int l = 0; l < 16; l++)
			{
				for (int k = 0; k < 16; k++)
				{
					heights[l * 16 + k] = yDisplacement[(y + l) * textureWidth + (x + k)];
				}
			}

			terrains[j * chunkCount + i].EditHeights(heights, averageHeight);
		}
	}
}
/* Place Objects */
/* Used to place representations of spawned objects */
/* Using the generated coordinates */
void PlaceObjects()
{
	int currentElem = 0;
	for (int i = 0; i < 10; i++)
	{
		if (largeObjectData[currentElem] == 0)
		{
			largeMarkers[i].SetActive(false);
		}
		else
		{
			int sizeX = largeObjectData[currentElem]; int sizeY = 32; int sizeZ = largeObjectData[currentElem + 1];
			int posX = largeObjectData[currentElem + 2]; int posZ = largeObjectData[currentElem + 3];
			int posY = yDisplacement[posZ * textureWidth + posX];

			largeMarkers[i].SetPosition(0, textureWidth, sizeX, sizeY, sizeZ, posX, posY, posZ);
			largeMarkers[i].SetActive(true);
		}
		currentElem += 4;
	}

	currentElem = 0;
	for (int i = 0; i < 15; i++)
	{
		if (smallObjectData[currentElem] == 0)
		{
			smallMarkers[i].SetActive(false);
		}
		else
		{
			int posX = smallObjectData[currentElem]; int posZ = smallObjectData[currentElem + 1];
			int posY = yDisplacement[posZ * textureWidth + posX] + 5;

			smallMarkers[i].SetPosition(1, textureWidth, 8, 8, 8, posX, posY, posZ);
			smallMarkers[i].SetActive(true);
		}
		currentElem += 2;
	}

	currentElem = 0;
	for (int i = 0; i < 15; i++)
	{
		if (enemyData[currentElem] == 0)
		{
			enemyMarkers[i].SetActive(false);
		}
		else
		{
			int posX = enemyData[currentElem]; int posZ = enemyData[currentElem + 1];
			int posY = yDisplacement[posZ * textureWidth + posX] + 5;

			enemyMarkers[i].SetPosition(2, textureWidth, 8, 8, 8, posX, posY, posZ);
			enemyMarkers[i].SetActive(true);
		}
		currentElem += 2;
	}
}
/* Initialize the generation algorithms */
void GenerateLevel()
{
	// Reset temporary arrays
	memset(largeObjectData, 0, sizeof(largeObjectData));
	memset(smallObjectData, 0, sizeof(smallObjectData));
	memset(enemyData, 0, sizeof(enemyData));

	performance.StartTimer();

	CreateMap();
	CombineMap();
	GenerateObjects();
	ApplyHeights();
	WaterTable();
	PlaceObjects();
	// Texturer();

	performance.EndTimer();

	if (generationCount < 250)
	{
		perfTimes[generationCount] = performance.GetSeconds();
		generationCount++;
	}
	else
	{
		generationCount = 900;
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
	glDisable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);


	glViewport(0, 0, w, h);
	GLfloat ratio = (GLfloat)w / h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glTranslatef(cameraX, cameraY, cameraZ);
	glFrustumf(-ratio, ratio, -1, 1, 0.5, 20);

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

	for (int i = 0; i < chunkCount * chunkCount; i++)
	{
		terrains[i].Draw();
	}
	for (int i = 0; i < 10; i++)
	{
		if (largeMarkers[i].GetActive())
		{
			largeMarkers[i].Draw();
		}
	}
	for (int i = 0; i < 15; i++)
	{
		if (smallMarkers[i].GetActive())
		{
			smallMarkers[i].Draw();
		}
	}
	for (int i = 0; i < 15; i++)
	{
		if (enemyMarkers[i].GetActive())
		{
			enemyMarkers[i].Draw();
		}
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

	for (int i = 0; i < sizeof(terrains); i++)
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

		if (AMotionEvent_getAction(event) == AMOTION_EVENT_ACTION_DOWN && canGen)
		{
			GenerateLevel();
			canGen = false;
		}
		else if (AMotionEvent_getAction(event) == AMOTION_EVENT_ACTION_UP)
		{
			canGen = true;
		}
		
		return 1;
	}
	return 0;
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

	for (int j = 0; j < chunkCount; j++)
	{
		for (int i = 0; i < chunkCount; i++)
		{
			Terrain* tile = new Terrain(textureWidth, i, j);
			terrains.push_back(*tile);
		}
	}
	for (int i = 0; i < 10; i++)
	{
		ObjectMarker* marker = new ObjectMarker(0, textureWidth, 32, 16, 32, 112, 0, 112);
		marker->SetActive(false);
		largeMarkers.push_back(*marker);
	}
	for (int i = 0; i < 15; i++)
	{
		ObjectMarker* marker = new ObjectMarker(1, textureWidth, 8, 8, 8, 112, 0, 112);
		marker->SetActive(false);
		smallMarkers.push_back(*marker);
	}
	for (int i = 0; i < 15; i++)
	{
		ObjectMarker* marker = new ObjectMarker(2, textureWidth, 8, 8, 8, 112, 0, 112);
		marker->SetActive(false);
		enemyMarkers.push_back(*marker);
	}

	water.SetPosition(1, 0, 0);
	
	engine.animating = 1;

	// loop waiting for stuff to do.
	//GenerateLevel();

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

			cameraRotation += 0.05;
			
			// Call update functions
			for (int i = 0; i < chunkCount * chunkCount; i++)
				terrains[i].Update();
			for (int i = 0; i < 10; i++)
				largeMarkers[i].Update();
			for (int i = 0; i < 15; i++)
				smallMarkers[i].Update();
			for (int i = 0; i < 15; i++)
				enemyMarkers[i].Update();
			water.Update();

			// Drawing is throttled to the screen update rate, so there
			// is no need to do timing here.
			engine_draw_frame(&engine);
		}
	}
}
