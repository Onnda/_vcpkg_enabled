#include "Application.h"
#include "Globals.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"

Application::Application() : debug(false)
{
	window = new ModuleWindow();
	input = new ModuleInput();
	scene_intro = new ModuleSceneIntro();
	renderer3D = new ModuleRenderer3D();
	camera = new ModuleCamera3D();
	ui = new ModuleUI();
	audio = new ModuleAudio();

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(audio);
	AddModule(ui);

	
	// Scenes
	AddModule(scene_intro);

	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application()
{
	std::list<Module*>::reverse_iterator item;

	for (item = list_modules.rbegin(); item != list_modules.rend(); ++item)
	{
		RELEASE(*item);
	}
}

bool Application::Init()
{
	bool ret = true;
	App = this;

	char* buffer = nullptr;

	if (buffer != nullptr)
	{
		std::list<Module*>::iterator item;

		RELEASE_ARRAY(buffer);

	}

	//Call Init() in all modules
	std::list<Module*>::iterator item;

	//LOG("Application Init");

	for (item = list_modules.begin(); item != list_modules.end() && ret; ++item)
	{
		ret = (*item)->Init();
		ret = (*item)->Start();

	}

	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
}

// Call PreUpdate, Update and PostUpdate on all modules
bool Application::Update()
{
	bool ret = true;
	PrepareUpdate();

	std::list<Module*>::iterator item = list_modules.begin();

	for (item = list_modules.begin(); item != list_modules.end() && ret; ++item)
	{
		ret = (*item)->PreUpdate(dt);

	}

	for (item = list_modules.begin(); item != list_modules.end() && ret; ++item)
	{
		ret = (*item)->Update(dt);
	}

	for (item = list_modules.begin(); item != list_modules.end() && ret; ++item)
	{
		ret = (*item)->PostUpdate();

	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	std::list<Module*>::reverse_iterator item;

	for (item = list_modules.rbegin(); item != list_modules.rend(); ++item)
	{
		ret = (*item)->CleanUp();
	}
	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

void Application::SaveConfig()
{
	LOG_COMMENT("Saving configuration");

	JsonParsing jsonFile;

	// Call Init() in all modules
	std::list<Module*>::iterator item;
	for (item = list_modules.begin(); item != list_modules.end(); ++item)
	{
		(*item)->SaveConfig(jsonFile.SetChild(jsonFile.GetRootValue(), (*item)->name));
	}
	char* buf;
	uint size = jsonFile.Save(&buf);
	RELEASE_ARRAY(buf);
	//jsonFile.SerializeFile(root, CONFIG_FILENAME);
	saveRequest = false;
}

void Application::LoadConfig()
{
	LOG_COMMENT("Loading configuration");

	char* buffer = nullptr;

	if (buffer != nullptr)
	{
		JsonParsing jsonFile((const char*)buffer);
		jsonFile.ValueToObject(jsonFile.GetRootValue());
		std::list<Module*>::iterator item;
		for (item = list_modules.begin(); item != list_modules.end(); ++item)
		{
			(*item)->LoadConfig(jsonFile.GetChild(jsonFile.GetRootValue(), (*item)->name));
		}
		RELEASE_ARRAY(buffer);
	}
	loadRequest = false;
}


Application* App = nullptr;
