#include "mmanager.h"


std::list<IMMObject *> IMMObject::liveObjects;
std::list<IMMObject *> IMMObject::deadObjects;

IMMObject::IMMObject()
{
	liveObjects.push_back(this);
	refCount=0;
}

IMMObject::~IMMObject()
{

}

void IMMObject::CollectGarbage()
{
	for(std::list<IMMObject*>::iterator it=deadObjects.begin();it!=deadObjects.end();)
	{
		IMMObject *o=(*it);
		delete o;
		it++;
	}
	deadObjects.clear();
}

void IMMObject::AddRef()
{
	refCount++;
}

void IMMObject::Release()
{
	refCount--; 
	if(refCount<=0)
	{
		//remove self from live list
		liveObjects.remove(this);
		//add self to dead list
		deadObjects.push_back(this);
	}
}

void IMMObject::CollectRemainingObjects(bool bEmitWarnings)
{
	CollectGarbage();
	for(std::list<IMMObject*>::iterator it=liveObjects.begin();it!=liveObjects.end();it++)
	{
		IMMObject *o=(*it);
		if(bEmitWarnings)
		{
			//copy the object to a temporary buffer so that our '10 bytes' message doesn't
			//cause an access violation
			/*
			char szBuf[11]; ZeroMemory(szBuf,11);
			memcpy(szBuf,o,min(o->size(),10));
			CLog::Get().Write(LOG_APP,IDS_UNRELEASED_OBJECT,o,o->size(),szBuf);
			*/
			std::cerr << "Object wasn't properly deleted in CollectGarbage";
		}
		delete o;
	}
	liveObjects.clear();
}