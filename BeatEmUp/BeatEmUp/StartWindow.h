#ifndef START_WINDOW_H
#define START_WINDOW_H

#include "C4Widgets.h"

namespace C4
{
	class StartWindow : public Window, public Singleton<StartWindow>
	{
	private:

		//WidgetObserver<StartWindow>		startButtonObserver;

		PushButtonWidget	*hostGame;
		PushButtonWidget	*joinGame;
		PushButtonWidget	*quitGame;
 
		PushButtonWidget	*spawnChar;
 
		EditTextWidget		*ipAddress;
		EditTextWidget		*yourPort;

	public:

		StartWindow();
		~StartWindow();

		static void Open();

		void HandleWidgetEvent(Widget *widget, const WidgetEventData *eventData);

		void Preprocess(void) override;
	};

	// This is a pointer to the start window. We only keep this around so that
	// we can delete the window before exiting if it's still on the screen.

	extern StartWindow *TheStartWindow;
}

#endif

