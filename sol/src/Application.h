#pragma once

namespace sol
{

class Application
{
public:
	Application();
	virtual ~Application();

	void run();
};

Application *GetApplication();

} // namespace sol