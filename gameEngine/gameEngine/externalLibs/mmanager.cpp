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
	refCount++;}

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
			Logger::Instance()->err("Object wasn't properly deleted in CollectGarbage");
		}
		delete o;
	}
	liveObjects.clear();
}

string IMMObject::toString() {
	string info = "IMMobject";
	return info;
}