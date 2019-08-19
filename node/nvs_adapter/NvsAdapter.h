/*
 * nvs_adapter.h
 *
 *  Created on: Jul 17, 2019
 *      Author: ion
 */

#ifndef NVS_ADAPTER_NVSADAPTER_H_
#define NVS_ADAPTER_NVSADAPTER_H_


namespace NVS_ADAPTER
{
	class NVSAdapter
	{
		static bool isNvsInitiated;
	public:
		NVSAdapter();
		~NVSAdapter();

		void Init(void);

	};
}


#endif /* NVS_ADAPTER_NVSADAPTER_H_ */
