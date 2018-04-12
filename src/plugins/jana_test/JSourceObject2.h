// $Id$
//
//    File: JSourceObject2.h
// Created: Mon Oct 23 22:39:14 EDT 2017
// Creator: davidl (on Darwin harriet 15.6.0 i386)
//

#ifndef _JSourceObject2_
#define _JSourceObject2_

#include <vector>
#include <utility>
#include <string>
#include <array>

#include <JANA/JObject.h>
#include "JLog.h"

class JSourceObject2 : public JObject
{
	public:
		JOBJECT_PUBLIC(JSourceObject2);
		
		//STRUCTORS
		JSourceObject2(double aHitE, int aHitID) : mHitE(aHitE), mHitID(aHitID) { }

		//GETTERS
		int GetHitID(void) const{return mHitID;}
		double GetHitE(void) const{return mHitE;}
		std::vector<double> GetRandoms(void) const{return mRandoms;}

		//SETTERS
		void SetHitID(int aHitID){mHitID = aHitID;}
		void SetHitE(double aHitE){mHitE = aHitE;}
		void AddRandom(double aRandom){mRandoms.push_back(aRandom);}
		void MoveRandoms(std::vector<double>&& aRandoms){mRandoms = std::move(aRandoms);}

	private:

		std::vector<double> mRandoms;
		std::array<double, 20> mGarbage = {}; //Simulate a "large" object

		double mHitE;
		int mHitID;
};

//STREAM OPERATOR
inline JLog& operator<<(JLog& aLog, const JSourceObject2& aObject)
{
	aLog.SetHeaders({"Hit2 ID", "Hit2 E", "Hit2 #Randoms"});
	aLog << aObject.GetHitID() << aObject.GetHitE() << aObject.GetRandoms().size();
	return aLog;
}

#endif // _JSourceObject2_
