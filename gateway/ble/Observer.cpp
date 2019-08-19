/*
 * Observer.cpp
 *
 *  Created on: Aug 4, 2019
 *      Author: ion
 */


#include <Observer.h>


using namespace BLE;

Observer::Observer(uint16_t appId):appId(appId)
{
	this->gattcif = 0;
	this->gattsif = 0;
}

Observer::~Observer()
{

}
bool Observer::AttachObs(Observer *listobs[], uint8_t &current_len, uint8_t max_len,  Observer *obs)
{
	bool ret = false;
	if(current_len + 1 <= max_len)
	{
		listobs[current_len] = obs;
		current_len += 1u;
		ret = true;
	}
	return ret;
}
void Observer::DeleteObs(Observer *listobs[], uint8_t& current_len, Observer *obs)
{
	uint8_t i = 0;
	uint8_t foundIdx = 0xFF;

	for(i = 0; i < current_len; i++)
	{
		if(obs == listobs[i])
		{
			foundIdx = i;
		}
	}

	for(i = foundIdx; i < current_len - 1; i++)
	{
		listobs[i] = listobs[i + 1];
	}
	if(foundIdx != 0xFF)
	{
		current_len -= 1u;
	}

}
void Observer::DeleteObs(Observer *listobs[], uint8_t &current_len, uint8_t idx)
{
	uint8_t i = 0;
	if(idx < current_len)
	{
		for(i = idx; i < current_len - 1; i++)
		{
			listobs[i] = listobs[i + 1];
		}
		current_len -= 1u;
	}

}

