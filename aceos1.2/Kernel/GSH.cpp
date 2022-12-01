#include <GSH.h>
/*donot worry this function would not expanded as inline as it contians
aggreate initializer {0,0}
*/
BOOLEAN __GSH::Register(const BYTE DeviceType,const char * Description, void * Handler)
	{__StorageHandlerID * NewHandler=(__StorageHandlerID * )AllocHeap( sizeof(__StorageHandlerID));
	char Key[2]={0,0};
	Key[0]=DeviceType;

	NewHandler->DeviceType=DeviceType;
	StrNCpy(NewHandler->Description,Description,10);
	NewHandler->Description[10]=0;
	NewHandler->Handler=Handler;

	return Handlers.Add(NewHandler,Key);
	}
BYTE __GSH::GetRegisteredDeviceTypes(BYTE (*FeedBack)(const BYTE DeviceType,const char * Description))
	{BYTE i,HCount=Handlers.Count();
	__StorageHandlerID * Handler;
	for(i=0;i<HCount;i++)
		{Handler=Handlers.Item(i);
		FeedBack(Handler->DeviceType,Handler->Description);
		}
	return i;
	}

BYTE __GSH::GetNoOfDevices(const BYTE DeviceType)
	{char DeviceKey[2]={0,0};
	DeviceKey[0]=DeviceType;
	__StorageHandlerID * Handler=Handlers.Item(DeviceKey);
	if ( Handler==NULL )
		{LastErrorCode=GSH_HANDLER_NOT_INSTALLED;
		return 0;
		}
	return ((GenericStorageHandler *)Handler->Handler)->GetNoOfDevices();
	}
UINT16 __GSH::GetDeviceParameters(BYTE * DPFDevice, BYTE * Buffer)
	{char DeviceKey[2]={0,0};
	DeviceKey[0]=DPFDevice[0];
	__StorageHandlerID * Handler=Handlers.Item(DeviceKey);
	if ( Handler==NULL )
		{LastErrorCode=GSH_HANDLER_NOT_INSTALLED;
		return 0;
		}
	return ((GenericStorageHandler *)Handler->Handler)->GetDeviceParameters(DPFDevice, Buffer);
	}
UINT16 __GSH::Reset(BYTE * DPFDevice)
	{char DeviceKey[2]={0,0};
	DeviceKey[0]=DPFDevice[0];
	__StorageHandlerID * Handler=Handlers.Item(DeviceKey);
	if ( Handler==NULL )
		{LastErrorCode=GSH_HANDLER_NOT_INSTALLED;
		return 0;
		}
	return ((GenericStorageHandler *)Handler->Handler)->Reset(DPFDevice);
	}
UINT16 __GSH::Status(BYTE * DPFDevice)
	{char DeviceKey[2]={0,0};
	DeviceKey[0]=DPFDevice[0];
	__StorageHandlerID * Handler=Handlers.Item(DeviceKey);
	if ( Handler==NULL )
		{LastErrorCode=GSH_HANDLER_NOT_INSTALLED;
		return 0;
		}
	return ((GenericStorageHandler *)Handler->Handler)->Status(DPFDevice);
	}
UINT16 __GSH::Read(BYTE * DPFDevice, _GSH_IO_Parameter * InputParameter, BYTE * Buffer)
	{char DeviceKey[2]={0,0};
	DeviceKey[0]=DPFDevice[0];
	__StorageHandlerID * Handler=Handlers.Item(DeviceKey);
	if ( Handler==NULL )
		{LastErrorCode=GSH_HANDLER_NOT_INSTALLED;
		return 0;
		}
	return ((GenericStorageHandler *)Handler->Handler)->Read(DPFDevice, InputParameter, Buffer);
	}
UINT16 __GSH::Write(BYTE * DPFDevice, _GSH_IO_Parameter * InputParameter, BYTE * Buffer)
	{char DeviceKey[2]={0,0};
	DeviceKey[0]=DPFDevice[0];
	__StorageHandlerID * Handler=Handlers.Item(DeviceKey);
	if ( Handler==NULL )
		{LastErrorCode=GSH_HANDLER_NOT_INSTALLED;
		return 0;
		}
	return ((GenericStorageHandler *)Handler->Handler)->Write(DPFDevice, InputParameter, Buffer);
	}
UINT16 __GSH::Verify(BYTE * DPFDevice, _GSH_IO_Parameter * InputParameter)
	{char DeviceKey[2]={0,0};
	DeviceKey[0]=DPFDevice[0];
	__StorageHandlerID * Handler=Handlers.Item(DeviceKey);
	if ( Handler==NULL )
		{LastErrorCode=GSH_HANDLER_NOT_INSTALLED;
		return 0;
		}
	return ((GenericStorageHandler *)Handler->Handler)->Verify(DPFDevice, InputParameter);
	}
UINT16 __GSH::Format(BYTE * DPFDevice, _GSH_IO_Parameter * InputParameter, BYTE * Buffer)
	{char DeviceKey[2]={0,0};
	DeviceKey[0]=DPFDevice[0];
	__StorageHandlerID * Handler=Handlers.Item(DeviceKey);
	if ( Handler==NULL )
		{LastErrorCode=GSH_HANDLER_NOT_INSTALLED;
		return 0;
		}
	return ((GenericStorageHandler *)Handler->Handler)->Format(DPFDevice, InputParameter, Buffer);
	}


void * __GSH::GetHandler(const BYTE DeviceType)
	{char DeviceKey[2]={0,0};
	DeviceKey[0]=DeviceType;
	__StorageHandlerID * Handler=Handlers.Item(DeviceKey);
	if ( Handler==NULL )
		{LastErrorCode=GSH_HANDLER_NOT_INSTALLED;
		return 0;
		}
	return Handler->Handler;
	}
char * __GSH::GetErrorDescription(int ErrCode)
	{switch ( ErrCode)
		{case GSH_NO_ERROR:
			return "No Error";
		case GSH_HANDLER_NOT_INSTALLED:
			return "Handler Not Installed";
		default:
			return "Unknown Error";
		}
	}



