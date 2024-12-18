#pragma once

class  JsonParsing;

class Module
{
private :
	bool enabled;

public:

	const char* name;

	Module(bool start_enabled = true) : enabled(start_enabled)
	{}
	virtual ~Module()
	{}
	virtual bool Init() 
	{
		return true; 
	}
	virtual bool Start()
	{
		return true;
	}
	virtual bool PreUpdate(float dt)
	{
		return true;
	}
	virtual bool Update(float dt)
	{
		return true;
	}
	virtual bool PostUpdate()
	{
		return true;
	}
	virtual bool CleanUp() 
	{ 
		return true; 
	}
	virtual bool LoadConfig(JsonParsing& node)
	{
		return true;
	}
	virtual bool SaveConfig(JsonParsing& node) const
	{
		return true;
	}
};