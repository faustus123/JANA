//
//    File: JQueueWithBarriers.h
// Created: Wed Oct 11 22:51:32 EDT 2017
// Creator: davidl (on Darwin harriet 15.6.0 i386)
//
// ------ Last repository commit info -----
// [ Date ]
// [ Author ]
// [ Source ]
// [ Revision ]
//
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Jefferson Science Associates LLC Copyright Notice:  
// Copyright 251 2014 Jefferson Science Associates LLC All Rights Reserved. Redistribution
// and use in source and binary forms, with or without modification, are permitted as a
// licensed user provided that the following conditions are met:  
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer. 
// 2. Redistributions in binary form must reproduce the above copyright notice, this
//    list of conditions and the following disclaimer in the documentation and/or other
//    materials provided with the distribution.  
// 3. The name of the author may not be used to endorse or promote products derived
//    from this software without specific prior written permission.  
// This material resulted from work developed under a United States Government Contract.
// The Government retains a paid-up, nonexclusive, irrevocable worldwide license in such
// copyrighted data to reproduce, distribute copies to the public, prepare derivative works,
// perform publicly and display publicly and to permit others to do so.   
// THIS SOFTWARE IS PROVIDED BY JEFFERSON SCIENCE ASSOCIATES LLC "AS IS" AND ANY EXPRESS
// OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL
// JEFFERSON SCIENCE ASSOCIATES, LLC OR THE U.S. GOVERNMENT BE LIABLE TO LICENSEE OR ANY
// THIRD PARTES FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
// OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
//
// Description:
//
//
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
#ifndef _JQueueWithBarriers_h_
#define _JQueueWithBarriers_h_

#include <cstdint>
#include <atomic>
#include <thread>
#include <vector>

#include "JQueue.h"
#include "JQueueWithLock.h"
#include "JParameterManager.h"

class JTaskBase;
class JEvent;

class JQueueWithBarriers : public JQueue
{
	public:
	
		//STRUCTORS
		JQueueWithBarriers(JParameterManager* aParams, 
				   const std::string& aName, 
				   std::size_t aQueueSize = 200, 
				   std::size_t aTaskBufferSize = 0);

		~JQueueWithBarriers(void);

		//COPIERS //needed because atomic not copyable
		JQueueWithBarriers(const JQueueWithBarriers& aQueue);
		JQueueWithBarriers& operator=(const JQueueWithBarriers& aQueue);

		//MOVERS //specify because deleted by default if copiers specified
		JQueueWithBarriers(JQueueWithBarriers&&) = default;
		JQueueWithBarriers& operator=(JQueueWithBarriers&&) = default;

		Flags_t AddTask(const std::shared_ptr<JTaskBase>& aTask);
		Flags_t AddTask(std::shared_ptr<JTaskBase>&& aTask);
		std::shared_ptr<JTaskBase> GetTask(void);
		bool AreEnoughTasksBuffered(void);

		uint32_t GetMaxTasks(void);
		uint32_t GetNumTasks(void);
		uint64_t GetNumTasksProcessed(void);
		std::size_t GetTaskBufferSize(void);
		std::size_t GetLatestBarrierEventUseCount(void) const{return mLatestBarrierEvent.use_count();}

		JQueue* CloneEmpty(void) const;
		void FinishedWithQueue(void){mEndThread = true; mThread->join(); mLatestBarrierEvent = nullptr;} //Call this when finished with the queue

		void EndThread(void){mEndThread = true;}
		void ThreadLoop(void);

	private:

		std::thread* mThread;
		std::atomic<bool> mEndThread{false};

		std::size_t mTaskBufferSize = 0;
		int mDebugLevel = 0;
		uint32_t mLogTarget = 0; //cout

		JQueueWithLock* mInputQueue = nullptr;
		JQueueWithLock* mOutputQueue = nullptr;

		std::shared_ptr<const JEvent> mLatestBarrierEvent = nullptr;
		std::weak_ptr<JTaskBase> mAnalyzeBarrierEventTask;
		std::chrono::nanoseconds mSleepTime = std::chrono::nanoseconds(1000);
		std::chrono::nanoseconds mSleepTimeIfBarrier = std::chrono::nanoseconds(100);
};

#endif // _JQueueWithBarriers_h_
