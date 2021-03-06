
//#include "JFunctions.h"
#include "JEvent.h"
#include "JTask.h"
#include "JApplication.h"
#include "JEventProcessor.h"

std::shared_ptr<JTaskBase> JMakeAnalyzeEventTask(std::shared_ptr<const JEvent>&& aEvent, JApplication* aApplication)
{
	//Get processors
	std::vector<JEventProcessor*> sProcessors;
	aApplication->GetJEventProcessors(sProcessors);

	//Define function that will be executed by the task (running processors on the event)
	auto sRunProcessors = [sProcessors](const std::shared_ptr<const JEvent>& aEvent) -> void
	{
		for(auto sProcessor : sProcessors){
		
			// Make sure Init function is called for this processor, but only once.
			std::call_once(sProcessor->init_flag, &JEventProcessor::Init, sProcessor);

			// Don't try catching exception here. Any exception should
			// be caught in JThreadManager::ExecuteTask which calls us.
			sProcessor->Process(aEvent);
		}
	};
	auto sPackagedTask = std::packaged_task<void(const std::shared_ptr<const JEvent>&)>(sRunProcessors);

	//Get the JTask, set it up, and return it
	auto sTask = aApplication->GetVoidTask(); //std::make_shared<JTask<void>>(aEvent, sPackagedTask);
	sTask->SetEvent(std::move(aEvent));
	sTask->SetTask(std::move(sPackagedTask));
	return std::static_pointer_cast<JTaskBase>(sTask);
}
