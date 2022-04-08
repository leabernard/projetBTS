#include "MonMagnifiqueThread.h"

ManageConvoy * GlobalThread::managerRef = NULL;

void GlobalThread::manageQuit(int sig)
{
	qDebug() << "Signal received : " << sig;
	if (managerRef != NULL)
	{
		managerRef->disconnectHost();
		managerRef->getTerminationLock();
		delete managerRef;
	}
}
