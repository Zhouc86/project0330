// EgoDLL.cpp : Defines the exported functions for the DLL.
//

#include "framework.h"
#include "./include/EgoInterface.h"
#include "EgoController.h"

 

IEgoControl* GetEgoController(IEgoNotify* n)
{
	CEgoController* c=new CEgoController(n);
	return c;
}