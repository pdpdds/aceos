/*
  Name: 		FAT File System for ACE Os
  Author: 				Sam (samuelhard@yahoo.com)
  Date: 				09-Sep-02 22:42
*/
#include <ChType.h>
#include <FAT.h>
#include <HardDisk.h>
#include <Partition.h>
//-------------------FATInfo member functions-----------------------
FATInfo::FATInfo(BYTE * DPF,struct _Partition * DiskPart, BYTE * FirstSector)
	{Initialize(DPF, DiskPart, FirstSector);
	}
void FATInfo::Initialize(BYTE * DPF,struct _Partition * DiskPart, BYTE * FirstSector)
	{
	this->Part.Initialize(DiskPart);
	MemCpy( this->DPF , DPF , 5 );
	DPF[6]=0;
	MemCpy( (BYTE *)&this->FDI._BS , (BYTE *) &FirstSector[0] , sizeof(struct BootSectorStart) );
	MemCpy( (BYTE *)&this->FDI._BPB , (BYTE *) &FirstSector[11] , sizeof(struct BPB) );
	MemCpy( (BYTE *)&this->FDI._FAT12_16 ,(BYTE *) &FirstSector[36] , sizeof(struct FAT12_16) );
	MemCpy( (BYTE *)&this->FDI._FAT32    ,(BYTE *) &FirstSector[36] , sizeof(struct FAT32) );

	this->FDI.FATSize = this->FDI._BPB.FATSize16 ? this->FDI._BPB.FATSize16 : this->FDI._FAT32.FATSize32;
	this->FDI.TotRootDirSectors=( ((this->FDI._BPB.RootDirEntries*32) + (this->FDI._BPB.BytesPerSector-1)) / this->FDI._BPB.BytesPerSector );
	this->FDI.DataSectors=(this->FDI._BPB.TotalSectors16?this->FDI._BPB.TotalSectors16:this->FDI._BPB.TotalSectors32)-( this->FDI._BPB.ReservedSectors + ( this->FDI._BPB.NumberFATs* this->FDI.FATSize ) + this->FDI.TotRootDirSectors );

	this->FDI.CountOfClusters=this->FDI.DataSectors/this->FDI._BPB.SectorsPerCluster;
	if ( this->FDI.CountOfClusters < 4085 )
		this->FDI._FATType=12;
	else
		if ( this->FDI.CountOfClusters < 65525 )
			this->FDI._FATType=16;
		else
			this->FDI._FATType=32;

	}
inline BYTE * FATInfo::GetDPF()
	{return DPF;
	}
inline Partition * FATInfo::GetPartition()
	{return &Part;
	}
inline BYTE FATInfo::FATType()
	{return this->FDI._FATType;
	}
inline UINT32 FATInfo::GetFATSize()
	{return this->FDI.FATSize;
	}
inline BYTE FATInfo::GetClusterSize()
	{return this->FDI._BPB.SectorsPerCluster;
	}
inline UINT16 FATInfo::GetSectorSize()
	{return this->FDI._BPB.BytesPerSector;
	}
inline UINT32 FATInfo::GetTotalSectors()
	{return this->FDI._BPB.TotalSectors16?this->FDI._BPB.TotalSectors16:this->FDI._BPB.TotalSectors32;
	}
inline UINT32 FATInfo::GetTotalRootDirSectors()
	{return this->FDI.TotRootDirSectors;
	}
inline UINT16 FATInfo::RootDirEntries()
	{return this->FDI._BPB.RootDirEntries;
	}
inline UINT32 FATInfo::GetTotalDataSectors()
	{return this->FDI.DataSectors;
	}
inline UINT32 FATInfo::GetTotalClusters()
	{return this->FDI.CountOfClusters;
	}
inline UINT32 FATInfo::GetReservedSectors()
	{return this->FDI._BPB.ReservedSectors;
	}
inline UINT16 FATInfo::GetFATStartSector(BYTE FATNo)
	{if ( FATNo >= this->FDI._BPB.NumberFATs )
		return 0;
	return this->FDI._BPB.HiddenSectors+this->FDI._BPB.ReservedSectors+(GetFATSize()*FATNo);
	}
inline UINT32 FATInfo::DataAreaStartSector()
	{return this->FDI._BPB.ReservedSectors + (this->FDI._BPB.NumberFATs * this->FDI.FATSize) + this->FDI.TotRootDirSectors;
	}
/* this function returns the logical sector in the data area*/
inline UINT32 FATInfo::FirstSectorOfCluster(UINT32 Cluster)
	{return ((Cluster-2)*this->FDI._BPB.SectorsPerCluster) + this->DataAreaStartSector();
	}
inline UINT16 FATInfo::RootDirStartSector()
	{if ( this->FDI._FATType == 12 || this->FDI._FATType == 16 )
		return this->FDI._BPB.ReservedSectors+( this->FDI._BPB.NumberFATs * this->FDI._BPB.FATSize16 ) ;
	else
		return GetLogicalSector(this->FDI._FAT32.RootCluster,0);
	}
inline UINT16 FATInfo::RootDirStartCluster()
	{if ( this->FDI._FATType == 12 || this->FDI._FATType == 16 )
		return GetClusterCluster(this->FDI._BPB.ReservedSectors+( this->FDI._BPB.NumberFATs * this->FDI._BPB.FATSize16 )) ;
	else
		return this->FDI._FAT32.RootCluster;
	}
inline UINT32 FATInfo::GetClusterCluster(UINT32 LogicalSector)
	{UINT32 Result;
	
	Result=( LogicalSector-(this->FDI._BPB.HiddenSectors+this->FDI._BPB.ReservedSectors+(this->FDI._BPB.NumberFATs*this->FDI.FATSize)+GetTotalRootDirSectors()) ) / this->FDI._BPB.SectorsPerCluster ;
	Result=((int)Result<0?0:Result)+2;
	return Result;
	}

/* this function will return the FAT Sector which refers the given Cluster*/
inline UINT32 FATInfo::FATSectorNumber( UINT32 Cluster)
	{UINT32 FATOffset;
	if ( this->FDI._FATType == 12 )
		FATOffset=Cluster + ( Cluster / 2 );
	else
		if ( this->FDI._FATType == 16 )
			FATOffset=Cluster*2;
		else
			if ( this->FDI._FATType == 32 )
				FATOffset=Cluster*4;
	return this->FDI._BPB.ReservedSectors + ( FATOffset / this->FDI._BPB.BytesPerSector ) ;
	}
/* this function will returns the BYTE offset from FAT Sector which refers the given Cluster*/
inline UINT32 FATInfo::FATSectorOffset(UINT32 Cluster )
	{UINT32 FATOffset;
	if ( this->FDI._FATType == 12 )
		FATOffset=Cluster + ( Cluster / 2 );
	else
		if ( this->FDI._FATType == 16 )
			FATOffset=Cluster*2;
		else
			if ( this->FDI._FATType == 32 )
				FATOffset=Cluster*4;
	return FATOffset % this->FDI._BPB.BytesPerSector;
	}
// ******** you must read the sector and pass it me
// to read the sector use FATSectorNumber function
// This is not the Data Area Sector, it is the FAT Area
inline UINT32 FATInfo::GetFATEntry( UINT32 Cluster , BYTE * Sector )
	{UINT16 FATOffset=FATSectorOffset(Cluster);
	if ( this->FDI._FATType == 12 )
		{UINT16 Result=*( (WORD *) &Sector[FATOffset]);
		if ( Cluster & 0x0001 )  //Odd cluster
			return Result>>4;
		else
			return Result & 0x0FFF;;
		}
	else
		{
		if( this->FDI._FATType == 16  )
			return * ((WORD *) &Sector[FATOffset] );
		else
			if(  this->FDI._FATType == 32 )
				return 	(* ((DWORD *) &Sector[FATOffset] )) & 0xFFFFFFF; //only 28 bits is valid
		}	
	}

inline UINT32 FATInfo::GetLogicalSector(UINT32 Cluster,UINT16 Sector)
	{if ( Sector >=this->FDI._BPB.SectorsPerCluster )
		return 0;
	return this->FDI._BPB.HiddenSectors+GetReservedSectors()+(this->FDI._BPB.NumberFATs*this->FDI.FATSize)+ GetTotalRootDirSectors() + ((Cluster-2) * this->FDI._BPB.SectorsPerCluster) + Sector;
	}
inline UINT32 FATInfo::GetPhysicalSector(UINT32 LogicalSector )
	{return (LogicalSector%this->FDI._BPB.SectorsPerTrack)+1;
	}
inline UINT32 FATInfo::GetPhysicalTrack(UINT32 LogicalSector)
	{return LogicalSector/(this->FDI._BPB.SectorsPerTrack*this->FDI._BPB.NumberOfHeads);
	}
inline UINT32 FATInfo::GetPhysicalHead(UINT32 LogicalSector)
	{return (LogicalSector/this->FDI._BPB.SectorsPerTrack) % this->FDI._BPB.NumberOfHeads;
	}
inline UINT32 FATInfo::GetRelativeSector(UINT32 Head,UINT32 Track, UINT32 Sector)
	{return Sector-1+(Track * this->FDI._BPB.NumberOfHeads+ Head ) * this->FDI._BPB.SectorsPerTrack ;
	}


BYTE FATInfo::IsEndOfClusterChain( UINT32 FAT_Content )
	{switch( this->FDI._FATType )
		{case 12:
			return ((UINT16)FAT_Content)>=0x0FF8?True:False;
		case 16:
			return ((UINT16)FAT_Content)>=0xFFF8?True:False;
		case 32:
			return FAT_Content>=0x0FFFFFF8?True:False;
		default:
			return True;
		}
	}
BYTE FATInfo::IsBadCluster(UINT32 FAT_Content )
	{switch( this->FDI._FATType )
		{case 12:
			return ((UINT16)FAT_Content)==0x0FF7?True:False;
		case 16:
			return ((UINT16)FAT_Content)==0xFFF7?True:False;
		case 32:
			return FAT_Content==0x0FFFFFF7?True:False;
		default:
			return 0;
		}
	}
struct FATOpenFileInfo
	{
	BYTE SectorBuffer[1024];	//this is the internal FAT cache for the particular file
	
	FATInfo * FInfo;	

	UINT32 BufferedFATSector;   //this is related to the above field SectorBuffer - this indicates the buffered FAT's sector number
 	BYTE * Buffer;				//data cache buffer
	UINT16 BufferOffset;

	BYTE BufferIsValid;			//intially false after a read true.
	BYTE Mode;					//File open mode not implemented
	BYTE DriveLetter;			//drive letter
	struct DirEntry DEInfo;		//DirEntry of the opened file
	UINT32 TotalBytesPassed;	//Total bytes read or writen from the begining
	UINT32 CurrentCluster;		//Current cluster number in the linked list of clusters
	};
Collection <class FATOpenFileInfo *> __SysFATOpenFileInfo;
Collection <class FATInfo *> __SysFATInternal;
BYTE NextHDDDrive='C';		
BYTE NextFloppyDrive='A';

//---------------------------Other Functions-------------------------------------------
void CallBackPartition(BYTE * DPF, Partition * Part)
	{BYTE DriveLetter[2]={0,0};BYTE StorageKey[3];
	BYTE FirstSector[512];
	StorageKey[0]=DPF[0];
	StorageKey[1]=DPF[1];
	StorageKey[2]=0;
	if ( Part->PartType()==0x1 || Part->PartType()==0x4 || Part->PartType()==0x6 || \
		Part->PartType()==0xB || Part->PartType()==0xC || Part->PartType()==0xE || \
		Part->PartType()==0x11 || Part->PartType()==0x14 || Part->PartType()==0x1B || \
		Part->PartType()==0x1C || Part->PartType()==0x1E ) 

		{FATInfo * newFAT=(FATInfo *)AllocHeap( sizeof(FATInfo) );
		if ( newFAT == 0 )
			{Printf("\n\r FAT FS Internal Error :: Allocation Failed.");
			return;
			}
		BYTE ReadResult = __SysHDDHandler->ReadSectors( StorageKey, Part->PartitionBegining() , 1,(BYTE *) &FirstSector[0],True);
		if (  ReadResult != HDD_NO_ERROR )
			{Printf("\n\r FAT FS Internal Error :: HDD Read failed on %s.", DPF);
			return;
			}
		DriveLetter[0]=NextHDDDrive++;
		newFAT->Initialize(DPF,Part->Part,&FirstSector[0]);
		Printf("\n\r FAT%d is found on %s. Assigning drive letter %s: ...", newFAT->FATType(), DPF,DriveLetter);
		__SysFATInternal.Add(newFAT,(char *)DriveLetter);
		Printf(" done");
		}
	}
void InitFATFileSystem()
	{BYTE Key[]={'H','0',0};
	__SysFATInternal.Initialize();
	EnumeratePartitions(Key, CallBackPartition, 0 , '0' );
	__SysFATOpenFileInfo.Initialize();
	}
/* this function returns the FATInfo of the given Drive*/
FATInfo * GetFATInfo(BYTE DriveLetter)
	{BYTE Drive[2]={0,0};
	Drive[0]=DriveLetter;
	return __SysFATInternal.Item((char *)Drive);
	}
//----------this is internal function ,only FAT driver uses this function
char * SplitFirstDirectory(char * FirstDir, char * DirectoryPath)
	{char * DirSep=0;
	*FirstDir=0;
	for(int i=0;DirectoryPath[i];i++)
		if ( DirectoryPath[i] == '\\' )	
			{DirSep=&DirectoryPath[i];
			break;
			}
	if ( !DirSep )
		return 0;
	StrNCpy(FirstDir,DirectoryPath, DirSep-DirectoryPath);
	FirstDir[DirSep-DirectoryPath]=0;
	return DirSep;
	}
/*Internal function. This function is used as callback function
to find out whether the given file name is exist in the list
*/
BYTE FindFileInEnumeration(struct DirEntry  * DEInfo,void * vFileName,struct DirEntry  * Result)
	{char File[13],*FileName=(char *)vFileName;
	StrNCpy(File,(char *)DEInfo->FileName,11);
	File[12]=0;
	if ( !StrNICmp(File,FileName,StrLen(FileName)) )
		{MemCpy(Result,DEInfo,sizeof(struct DirEntry ));
		return 0;	//break enumeration
		}
	else
		return 1; 	//continue enumeration
	}
struct DirEntry  * GetDirectoryEntry(char * FilePath,struct DirEntry * DEInfo)
	{char Drive[4],Dir[256],FileName[10],Ext[5],File[14];
	char CurDir[13],* NextDir;
	int i;

	SplitPath(FilePath, Drive, Dir, FileName, Ext);
	File[0]=0;
	if ( FileName[0] ) 
		{StrCpy(File,FileName);
		for(i=StrLen(File);i<9;i++)
			File[i]=' ';
		File[8]=0;
		}
	if ( Ext[0] )
		{StrCat(File,Ext+1);
		for(i=StrLen(File);i<3;i++)
			File[i]=' ';
		File[12]=0;
		}
	NextDir=SplitFirstDirectory(CurDir,Dir+1);
	if ( CurDir[0] == 0 )	
		{if ( EnumerateFilesInRoot(Drive[0],FindFileInEnumeration,File+1, DEInfo) != 2 )
			return 0;
		else
			return DEInfo;
		}
	else
		{if ( EnumerateFilesInRoot(Drive[0],FindFileInEnumeration,CurDir, DEInfo) != 2 )
			return 0;
		do
			{NextDir=SplitFirstDirectory(CurDir,&Dir[NextDir-Dir+1]);
			if ( EnumerateFilesInFolder(Drive[0],DEInfo,FindFileInEnumeration,CurDir, DEInfo) != 2 )
				return 0;
			}while (NextDir[1]);
		if ( File[1] )
			if ( EnumerateFilesInFolder(Drive[0],DEInfo,FindFileInEnumeration,File, DEInfo) != 2 )
				return 0;
		return DEInfo;
		}
	}
BYTE EnumerateFilesInFolder(BYTE DriveLetter, DirEntry * FolderDE, BYTE (*CallBackFn)(struct DirEntry  *, void * ,struct DirEntry  *), void * CallBackPara , struct DirEntry  * Result)
	{BYTE Drive[2]={0,0};
	Drive[0]=DriveLetter;
	
	BYTE (*CallBackFun)(struct DirEntry  *,void *,struct DirEntry  * ) = (BYTE (*)(struct DirEntry  *, void * ,struct DirEntry  *)) ((UINT32)CallBackFn-0x19000);

	DirEntry * DEInfo;
	UINT16 * DirData,i,ByteReadSize;
	BYTE StorageKey[3];

	FATInfo *FInfo=__SysFATInternal.Item((char *)Drive);
	if ( FInfo == NULL )
		{Printf("\n\r FAT FS Internal Error :: No FAInfo for drive %c:.", Drive[0]);
		return 0;
		}
	StrNCpy((char *)StorageKey,(char *)FInfo->GetDPF(),2);
	StorageKey[2]=0;
	ByteReadSize=FInfo->GetSectorSize()*FInfo->GetClusterSize();
	DirData = (UINT16 *)AllocHeap(ByteReadSize);
	if ( DirData==NULL )
		{Printf("\n\r FAT FS Internal Error :: Failed to allocate %ld", ByteReadSize);
		return 0;
		}
	UINT32 DirCluster=FolderDE->FirstClusterHigh;
	DirCluster<<=16;
	DirCluster|=FolderDE->FirstClusterLow;
	do
		{UINT32 LBASector = FInfo->GetPartition()->PartitionBegining() + FInfo->FirstSectorOfCluster(DirCluster);
		BYTE ReadResult = __SysHDDHandler->ReadSectors( StorageKey, LBASector , FInfo->GetClusterSize() ,(BYTE *) DirData,True);
		if ( ReadResult != HDD_NO_ERROR )
			return 0;
		for(i=0; i<=ByteReadSize; i+=sizeof(struct DirEntry))	
			{DEInfo=(struct DirEntry *)((UINT32)DirData + i);
			if ( DEInfo->FileName[0] == 0 ) //this is the end
				return 0;
			if ( DEInfo->FileName[0]!=0xE5 )// valid entry
				{if ( DEInfo->FileName[0]==0x05 )
				      DEInfo->FileName[0]=0xE5;
				if ( !CallBackFun(DEInfo,CallBackPara, Result ) )
					return 2;
				}
			}
		UINT32 FATSector=FInfo->GetPartition()->PartitionBegining() + FInfo->FATSectorNumber(DirCluster);
		ReadResult = __SysHDDHandler->ReadSectors( StorageKey, FATSector , 1 ,(BYTE *) DirData,True);
		if ( ReadResult != HDD_NO_ERROR )
			return 0;
		DirCluster = FInfo->GetFATEntry(DirCluster, (BYTE *)DirData);
		}while( !FInfo->IsEndOfClusterChain(DirCluster) && !FInfo->IsBadCluster(DirCluster) );
	return 1;
	}
BYTE EnumerateFilesInRoot(BYTE DriveLetter, BYTE (*CallBackFn)(struct DirEntry  * ,void *,struct DirEntry  *), void * CallBackPara, struct DirEntry  * Result )
	{BYTE Drive[2]={0,0};
	Drive[0]=DriveLetter;

	BYTE (*CallBackFun)(struct DirEntry  *, void * , struct DirEntry  *) = (BYTE (*)(struct DirEntry  *, void *, struct DirEntry  * )) ((UINT32)CallBackFn-0x19000);

	DirEntry * DEInfo;
	UINT16 * RDSector,i,ByteReadSize;
	BYTE StorageKey[3];
	FATInfo * FInfo=__SysFATInternal.Item((char *)Drive);

	if ( FInfo == NULL )
		{Printf("\n\r FAT FS Internal Error :: No FAInfo for drive %c:.", Drive[0]);
		return 0;
		}

	if ( FInfo->FATType() == 32 ) //FAT32 contains no special root directory it is also like ordinary directory
		{DirEntry RD32Info;
		RD32Info.FirstClusterHigh=FInfo->RootDirStartCluster()>>16;
		RD32Info.FirstClusterLow=(FInfo->RootDirStartCluster()<<16)>>16;
		return EnumerateFilesInFolder(DriveLetter, &RD32Info, CallBackFn, CallBackPara, Result);
		}

	StrNCpy((char *)StorageKey,(char *)FInfo->GetDPF(),2);
	StorageKey[2]=0;
	ByteReadSize=FInfo->GetSectorSize();
	RDSector = (UINT16 *)AllocHeap(ByteReadSize);
	if ( RDSector==NULL )
		{Printf("\n\r FAT FS Internal Error :: Failed to allocate %ld", ByteReadSize);
		return 0;
		}
	UINT32 LBASec =FInfo->GetPartition()->PartitionBegining()+FInfo->RootDirStartSector();
	for (;LBASec < FInfo->RootDirEntries(); LBASec++ )
		{BYTE ReadResult = __SysHDDHandler->ReadSectors( StorageKey, LBASec , 1,(BYTE *) RDSector,True);
		if ( ReadResult != HDD_NO_ERROR )
			return 0;
		for(i=0; i<=ByteReadSize;i+=sizeof(struct DirEntry))	
			{DEInfo=(struct DirEntry *)((UINT32)RDSector + i);
			if ( DEInfo->FileName[0] == 0 ) //this is the end
				return 0;
			if ( DEInfo->FileName[0]!=0xE5 )// valid entry
				{if ( DEInfo->FileName[0]==0x05 )
				      DEInfo->FileName[0]=0xE5;
				if ( !CallBackFun(DEInfo, CallBackPara, Result ) )
					return 2;	
				}
			}
		}
	return 1;
	}
UINT16 FATFileOpen(char * FilePath,BYTE Mode)
	{struct DirEntry DEInfo;
	if ( GetDirectoryEntry(FilePath,&DEInfo) != 0 )
		{struct FATOpenFileInfo * FATFileInfo = (struct FATOpenFileInfo *)AllocHeap(sizeof(struct FATOpenFileInfo));
		if ( FATFileInfo == 0 )
			{Printf("\n\r FAT :: File Open Error (Unable to allocate)");
			return 0;
			}
		FATFileInfo->FInfo=GetFATInfo(FilePath[0]);
		
		FATFileInfo->Buffer=(BYTE *)AllocHeap(FATFileInfo->FInfo->GetClusterSize()*FATFileInfo->FInfo->GetSectorSize());
		if ( FATFileInfo->Buffer == 0 )
			{Printf("\n\r FAT :: File Open Error (Unable to allocate)");
			return 0;
			}
		FATFileInfo->Mode=Mode;
		MemCpy(&FATFileInfo->DEInfo,&DEInfo,sizeof(struct DirEntry));
		FATFileInfo->CurrentCluster=DEInfo.FirstClusterHigh;
		FATFileInfo->CurrentCluster<<=16;
		FATFileInfo->CurrentCluster|=DEInfo.FirstClusterLow;;
		FATFileInfo->BufferOffset=0;
		FATFileInfo->BufferIsValid=False;

		FATFileInfo->DriveLetter=FilePath[0];
		FATFileInfo->BufferedFATSector=0;
		FATFileInfo->TotalBytesPassed=0;
		if ( __SysFATOpenFileInfo.Add(FATFileInfo) )
			return __SysFATOpenFileInfo.Count();
		else
			return 0;
		}
	}
void FATFileClose(UINT16 HandleID)
	{Printf("\n\r FAT warning :: File Close Not Implemented");
	}
BYTE FATIsEndOfFile(UINT16 HandleID)
	{struct FATOpenFileInfo * FATFileInfo =  __SysFATOpenFileInfo.Item(HandleID-1);
	if ( FATFileInfo == 0 )
		{Printf("\n\r FAT :: File Read Error. File Not Opened");
		return 0;
		}
	BYTE Drive[2]={0,0};
	Drive[0]=FATFileInfo->DriveLetter;

	FATInfo *FInfo=__SysFATInternal.Item((char *)Drive);
	if ( FInfo == NULL )
		{Printf("\n\r FAT FS Internal Error :: No FAInfo for drive %c:.", Drive[0]);
		return 0;
		}
	if ( FInfo->IsEndOfClusterChain(FATFileInfo->CurrentCluster) || FATFileInfo->CurrentCluster == 0 )
		return !FATFileInfo->BufferIsValid;
	else
		return 0;
	}
BYTE FATReadFileCluster(struct FATOpenFileInfo * FATFileInfo,FATInfo *FInfo)
	{BYTE StorageKey[3];
	StrNCpy((char *)StorageKey,(char *)FInfo->GetDPF(),2);
	StorageKey[2]=0;

	if ( ! FATFileInfo->CurrentCluster || FInfo->IsBadCluster(FATFileInfo->CurrentCluster))
		{Printf("\n\r FAT FS Internal Error :: Requested to read free or bad cluster as file data");
		return 0;
		}	
	if  ( !FInfo->IsEndOfClusterChain(FATFileInfo->CurrentCluster)  )
		{
		UINT32 LBASector = FInfo->GetPartition()->PartitionBegining() + FInfo->FirstSectorOfCluster(FATFileInfo->CurrentCluster);
		BYTE ReadResult = __SysHDDHandler->ReadSectors( StorageKey, LBASector , FInfo->GetClusterSize() ,(BYTE *) FATFileInfo->Buffer,True);
		if ( ReadResult != HDD_NO_ERROR )
			{Printf("\n\r FAT FS Internal Error :: Cannot read %ld sector on device %s",LBASector, StorageKey);
			return 0;
			}
		UINT32 FATSector= FInfo->GetPartition()->PartitionBegining() + FInfo->FATSectorNumber(FATFileInfo->CurrentCluster);
		if ( FATFileInfo->BufferedFATSector != FATSector )
			{ReadResult = __SysHDDHandler->ReadSectors( StorageKey, FATSector , 2 ,(BYTE *) FATFileInfo->SectorBuffer,True);
			if ( ReadResult != HDD_NO_ERROR )
				return 0;
			FATFileInfo->BufferedFATSector=FATSector;
			}
		FATFileInfo->CurrentCluster = FInfo->GetFATEntry(FATFileInfo->CurrentCluster, FATFileInfo->SectorBuffer);
		FATFileInfo->BufferOffset=0;
		FATFileInfo->BufferIsValid=True;
		return 1;
		}	
	else
		return 0;	
	}
UINT16 FATReadFile(UINT16 HandleID, UINT32 SizeInBytes, BYTE * Buffer)
	{UINT32 TotReadBytes=0,BufferTotalBytes=0;
	struct FATOpenFileInfo * FATFileInfo =  __SysFATOpenFileInfo.Item(HandleID-1);
	if ( FATFileInfo == 0 )
		{Printf("\n\r FAT :: File Read Error. File Not Opened");
		return 0;
		}
	FATInfo * FInfo = FATFileInfo->FInfo;
	UINT16 BufferSize=FInfo->GetClusterSize()*FInfo->GetSectorSize();
	if ( FATFileInfo->BufferIsValid  )
		{
		BufferTotalBytes = ( BufferSize ) - FATFileInfo->BufferOffset;
		if ( BufferTotalBytes>=SizeInBytes )
			BufferTotalBytes=SizeInBytes;
		if ( FATFileInfo->TotalBytesPassed+BufferTotalBytes > FATFileInfo->DEInfo.FileSize )
			BufferTotalBytes=FATFileInfo->DEInfo.FileSize-FATFileInfo->TotalBytesPassed;
		MemCpy(Buffer, &FATFileInfo->Buffer[FATFileInfo->BufferOffset],BufferTotalBytes);
		TotReadBytes=BufferTotalBytes;
		FATFileInfo->TotalBytesPassed+=BufferTotalBytes;
		FATFileInfo->BufferOffset+=BufferTotalBytes;
		if ( FATFileInfo->BufferOffset >= BufferSize )
			FATFileInfo->BufferIsValid=False;
		}
	while ( TotReadBytes < SizeInBytes )
		{UINT32 BufferBytes=BufferSize;
		if ( !FATReadFileCluster(FATFileInfo,FInfo) )
			return TotReadBytes;
		if ( TotReadBytes+BufferBytes > SizeInBytes )
			BufferBytes=SizeInBytes-TotReadBytes;
		if ( FATFileInfo->TotalBytesPassed+BufferBytes > FATFileInfo->DEInfo.FileSize )
			{SizeInBytes=BufferBytes=FATFileInfo->DEInfo.FileSize-FATFileInfo->TotalBytesPassed;
			FATFileInfo->BufferIsValid=False;
			}
		MemCpy((BYTE *)&Buffer[TotReadBytes],(BYTE *)FATFileInfo->Buffer,BufferBytes);
		TotReadBytes+=BufferBytes;
		FATFileInfo->BufferOffset+=BufferBytes;
		FATFileInfo->TotalBytesPassed+=BufferBytes;
		};
	return TotReadBytes;
	}
static __inline__ char * MaxPtr(char *p1, char *p2)
	{if (p1 > p2)
		return p1;
	else
		return p2;
	}

int SplitPath(const char *path, char *drive, char *dir, char *name, char *ext)
	{
	int flags = 0, len;
	const char *pp, *pe;
	if (drive)
		*drive = '\0';
	if (dir)
		*dir = '\0';
	if (name)
		*name = '\0';
	if (ext)
		*ext = '\0';

	pp = path;

	if ((IsAlpha(*pp) || StrChr("[\\]^_`", *pp)) && (pp[1] == ':'))
		{if (drive)
			{StrNCpy(drive, pp, 2);
			drive[2] = '\0';
			}
		pp += 2;
		}
	pe = MaxPtr( (char *) StrRChr(pp, '\\'),(char *) StrRChr(pp, '/'));
	if (pe) 
		{pe++;
		len = pe - pp;
		if (dir)
			{StrNCpy(dir, pp, len);
			dir[len] = '\0';
			}
		pp = pe;
		}
	else
		pe = pp;

	/* Special case: "c:/path/." or "c:/path/.."
	These mean FILENAME, not EXTENSION.  */
	while (*pp == '.')
		++pp;
	if (pp > pe)
		{if (name)
			{len = pp - pe;
			StrNCpy(name, pe, len);
			name[len] = '\0';
		      /* advance name over '.'s so they don't get scragged later on when the
		       * rest of the name (if any) is copied (for files like .emacs). - WJC
		       */
			name+=len;
			}
		}
	pe = StrRChr(pp, '.');
	if (pe)
		{if (ext) 
			StrCpy(ext, pe);
		}
	else 
		pe = StrChr( pp, '\0');
	if (pp != pe)
		{len = pe - pp;
		if (name)
			{StrNCpy(name, pp, len);
			name[len] = '\0';
			}
		}

	return flags;
	}
void MergePath(char *path, const char *drive, const char *dir, const char *name, const char *ext)
	{*path = '\0';
	if (drive && *drive)
		{path[0] = drive[0];
		path[1] = ':';
		path[2] = 0;
		}
	if (dir && *dir)
		{
		char last_dir_char = dir[StrLen(dir) - 1];
		StrCat(path, dir);
		if (last_dir_char != '/' && last_dir_char != '\\')
			StrCat(path, StrChr(dir, '\\') ? "\\" : "/");
		}
	if (name)
		StrCat(path, name);
	if (ext && *ext)
		{
		if (*ext != '.')
			StrCat(path, ".");
		StrCat(path, ext);
		}
	}

