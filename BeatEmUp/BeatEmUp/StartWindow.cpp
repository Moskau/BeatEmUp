#include "StartWindow.h"
#include "Game.h"

using namespace C4;

// The StartWindow constructor initializes the Window base class with the name of the panel
// resource to load. The startButtonObserver member is initialized with the functions that
// is called when the "Start" push button in the window posts an activate event.

StartWindow *C4::TheStartWindow = nullptr;

StartWindow::StartWindow() :
Window("panels/Start"),			//register the window file
Singleton<StartWindow>(TheStartWindow)//,
//startButtonObserver(this, &StartWindow::HandleStartButtonEvent)
{
}

StartWindow::~StartWindow()
{
}

void StartWindow::Preprocess(void)
{
	// We must call the Window base class Preprocess() function first to initialize the
	// internal structures that are used to search for widgets.

	Window::Preprocess();

	// Find the push button widget named "Start" and assign our observer to it.

	//Widget *button = FindWidget("Start");
	//button->SetObserver(&startButtonObserver);

	hostGame = static_cast<PushButtonWidget *>(this->FindWidget("hostGame"));
	joinGame = static_cast<PushButtonWidget *>(this->FindWidget("joinGame"));
	quitGame = static_cast<PushButtonWidget *>(this->FindWidget("quitGame"));

	spawnChar = static_cast<PushButtonWidget *>(this->FindWidget("spawnChar"));

	ipAddress = static_cast<EditTextWidget *>(this->FindWidget("ipAddress"));
	yourPort = static_cast<EditTextWidget *>(this->FindWidget("yourPort"));
}

void StartWindow::HandleWidgetEvent(Widget *widget, const WidgetEventData *eventData)
{
	
	if (eventData->eventType == kEventWidgetActivate)
	{
		if (widget == hostGame)
		{
			TheGame->HostGame();
			this->Close();
		}
		else if (widget == joinGame)
		{
			TheGame->SetPort(yourPort->GetText());
			TheGame->JoinGame(ipAddress->GetText());
			this->Close();
		}
		else if (widget == spawnChar)
		{
			TheMessageMgr->SendMessageAll(RequestMessage());
			this->Close();
		}
		else if (widget == quitGame)
		{
			TheGame->QuitGame();
		}
	}
}

void StartWindow::Open()
{
	if (TheStartWindow)
	{
		TheInterfaceMgr->SetActiveWindow(TheStartWindow);
	}
	else
	{
		TheInterfaceMgr->AddWidget(new StartWindow);
	}
}
