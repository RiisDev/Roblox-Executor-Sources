#include "HardDriveSerialNumber.h"

#pragma warning (disable : 4996)

int MasterHardDiskSerial::ReadPhysicalDriveInNTWithAdminRights(void)
{
	int iDone = FALSE;
	int iDrive = 0;

	for (iDrive = 0; iDrive < MAX_IDE_DRIVES; iDrive++)
	{
		HANDLE hPhysicalDriveIOCTL = 0;

		//  Try to get a handle to PhysicalDrive IOCTL, report failure
		//  and exit if can't.
		char cszDriveName[256];

		sprintf_s(cszDriveName, 256, "\\\\.\\PhysicalDrive%d", iDrive);

		//  Windows NT, Windows 2000, must have admin rights
		hPhysicalDriveIOCTL = CreateFileA(cszDriveName,
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
			OPEN_EXISTING, 0, NULL);
		if (hPhysicalDriveIOCTL == INVALID_HANDLE_VALUE)
		{
			SecureZeroMemory(m_cszErrorMessage, sizeof(m_cszErrorMessage));
			sprintf_s(m_cszErrorMessage, 256, "%d ReadPhysicalDriveInNTWithAdminRights ERROR ,CreateFileA(%s) returned INVALID_HANDLE_VALUE", __LINE__, cszDriveName);

		}
		else
		{
			GETVERSIONOUTPARAMS VersionParams;
			DWORD               dwBytesReturned = 0;

			// Get the version, etc of PhysicalDrive IOCTL
			memset((void*)&VersionParams, 0, sizeof(VersionParams));

			if (!DeviceIoControl(hPhysicalDriveIOCTL, DFP_GET_VERSION,
				NULL,
				0,
				&VersionParams,
				sizeof(VersionParams),
				&dwBytesReturned, NULL))
			{

				DWORD dwErr = GetLastError();
				SecureZeroMemory(m_cszErrorMessage, sizeof(m_cszErrorMessage));
				sprintf_s(m_cszErrorMessage, 256, "%d ReadPhysicalDriveInNTWithAdminRights ERROR DeviceIoControl() %d, DFP_GET_VERSION) returned 0, error is %d\n", __LINE__, (int)hPhysicalDriveIOCTL, (int)dwErr);
			}

			// If there is a IDE device at number "iI" issue commands
			// to the device
			if (VersionParams.bIDEDeviceMap <= 0)
			{
				SecureZeroMemory(m_cszErrorMessage, sizeof(m_cszErrorMessage));
				sprintf_s(m_cszErrorMessage, 256, "%d ReadPhysicalDriveInNTWithAdminRights ERROR No device found at iPosition %d (%d)", __LINE__, (int)iDrive, (int)VersionParams.bIDEDeviceMap);
			}
			else
			{
				BYTE             bIDCmd = 0;   // IDE or ATAPI IDENTIFY cmd
				SENDCMDINPARAMS  scip;
				//SENDCMDOUTPARAMS OutCmd;

				// Now, get the ID sector for all IDE devices in the system.
				// If the device is ATAPI use the IDE_ATAPI_IDENTIFY command,
				// otherwise use the IDE_ATA_IDENTIFY command
				bIDCmd = (VersionParams.bIDEDeviceMap >> iDrive & 0x10) ? \
					IDE_ATAPI_IDENTIFY : IDE_ATA_IDENTIFY;

				memset(&scip, 0, sizeof(scip));
				memset(byIdOutCmd, 0, sizeof(byIdOutCmd));

				if (DoIDENTIFY(hPhysicalDriveIOCTL,
					&scip,
					(PSENDCMDOUTPARAMS)&byIdOutCmd,
					(BYTE)bIDCmd,
					(BYTE)iDrive,
					&dwBytesReturned))
				{
					DWORD dwDiskData[256];
					int iIjk = 0;
					USHORT *punIdSector = (USHORT *)
						((PSENDCMDOUTPARAMS)byIdOutCmd)->bBuffer;

					for (iIjk = 0; iIjk < 256; iIjk++)
						dwDiskData[iIjk] = punIdSector[iIjk];

					PrintIdeInfo(iDrive, dwDiskData);

					iDone = TRUE;
				}
			}

			CloseHandle(hPhysicalDriveIOCTL);
		}
	}

	return iDone;
}

int MasterHardDiskSerial::ReadPhysicalDriveInNTUsingSmart(void)
{
	int iDone = FALSE;
	int iDrive = 0;

	for (iDrive = 0; iDrive < MAX_IDE_DRIVES; iDrive++)
	{
		HANDLE hPhysicalDriveIOCTL = 0;

		//  Try to get a handle to PhysicalDrive IOCTL, report failure
		//  and exit if can't.
		char cszDriveName[256];

		sprintf_s(cszDriveName, 256, "\\\\.\\PhysicalDrive%d", iDrive);

		//  Windows NT, Windows 2000, Windows Server 2003, Vista
		hPhysicalDriveIOCTL = CreateFileA(cszDriveName,
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, OPEN_EXISTING, 0, NULL);
		// if (hPhysicalDriveIOCTL == INVALID_HANDLE_VALUE)
		//    printf ("Unable to open physical iDrive %d, error code: 0x%lX\n",
		//            iDrive, GetLastError ());

		if (hPhysicalDriveIOCTL == INVALID_HANDLE_VALUE)
		{
			SecureZeroMemory(m_cszErrorMessage, sizeof(m_cszErrorMessage));
			sprintf_s(m_cszErrorMessage, 256, "%d ReadPhysicalDriveInNTUsingSmart ERROR, CreateFileA(%s) returned INVALID_HANDLE_VALUE Error Code %d", __LINE__, cszDriveName, GetLastError());
		}
		else
		{
			GETVERSIONINPARAMS GetVersionParams;
			DWORD dwBytesReturned = 0;

			// Get the version, etc of PhysicalDrive IOCTL
			memset((void*)& GetVersionParams, 0, sizeof(GetVersionParams));

			if (!DeviceIoControl(hPhysicalDriveIOCTL, SMART_GET_VERSION,
				NULL,
				0,
				&GetVersionParams, sizeof(GETVERSIONINPARAMS),
				&dwBytesReturned, NULL))
			{
				DWORD dwErr = GetLastError();
				SecureZeroMemory(m_cszErrorMessage, sizeof(m_cszErrorMessage));
				sprintf_s(m_cszErrorMessage, 256, "\n%d ReadPhysicalDriveInNTUsingSmart ERROR DeviceIoControl(%d, SMART_GET_VERSION) returned 0, error is %d", __LINE__, (int)hPhysicalDriveIOCTL, (int)dwErr);
			}
			else
			{
				// Print the SMART version
				// PrintVersion (& GetVersionParams);
				// Allocate the command cszBuffer
				ULONG CommandSize = sizeof(SENDCMDINPARAMS) + IDENTIFY_BUFFER_SIZE;
				PSENDCMDINPARAMS Command = (PSENDCMDINPARAMS)malloc(CommandSize);
				// Retrieve the IDENTIFY data
				// Prepare the command
#define ID_CMD          0xEC            // Returns ID sector for ATA
				Command->irDriveRegs.bCommandReg = ID_CMD;
				DWORD BytesReturned = 0;
				if (!DeviceIoControl(hPhysicalDriveIOCTL,
					SMART_RCV_DRIVE_DATA, Command, sizeof(SENDCMDINPARAMS),
					Command, CommandSize,
					&BytesReturned, NULL))
				{
					SecureZeroMemory(m_cszErrorMessage, sizeof(m_cszErrorMessage));
					sprintf_s(m_cszErrorMessage, 256, "SMART_RCV_DRIVE_DATA IOCTL");

					// Print the error
					//PrintError ("SMART_RCV_DRIVE_DATA IOCTL", GetLastError());
				}
				else
				{
					// Print the IDENTIFY data
					DWORD dwDiskData[256];
					USHORT *punIdSector = (USHORT *)
						(PIDENTIFY_DATA)((PSENDCMDOUTPARAMS)Command)->bBuffer;

					for (int iIjk = 0; iIjk < 256; iIjk++)
						dwDiskData[iIjk] = punIdSector[iIjk];

					PrintIdeInfo(iDrive, dwDiskData);
					iDone = TRUE;
				}
				// Done
				CloseHandle(hPhysicalDriveIOCTL);
				free(Command);
			}
		}
	}

	return iDone;
}

char * MasterHardDiskSerial::flipAndCodeBytes(int iPos, int iFlip, const char * pcszStr, char * pcszBuf)
{
	int iI;
	int iJ = 0;
	int iK = 0;

	pcszBuf[0] = '\0';
	if (iPos <= 0)
		return pcszBuf;

	if (!iJ)
	{
		char cP = 0;
		// First try to gather all characters representing hex digits only.
		iJ = 1;
		iK = 0;
		pcszBuf[iK] = 0;
		for (iI = iPos; iJ && !(pcszStr[iI] == '\0'); ++iI)
		{
			char cC = tolower(pcszStr[iI]);
			if (isspace(cC))
				cC = '0';
			++cP;
			pcszBuf[iK] <<= 4;

			if (cC >= '0' && cC <= '9')
				pcszBuf[iK] |= (char)(cC - '0');
			else if (cC >= 'a' && cC <= 'f')
				pcszBuf[iK] |= (char)(cC - 'a' + 10);
			else
			{
				iJ = 0;
				break;
			}

			if (cP == 2)
			{
				if ((pcszBuf[iK] != '\0') && !isprint(pcszBuf[iK]))
				{
					iJ = 0;
					break;
				}
				++iK;
				cP = 0;
				pcszBuf[iK] = 0;
			}

		}
	}

	if (!iJ)
	{
		// There are non-digit characters, gather them as is.
		iJ = 1;
		iK = 0;
		for (iI = iPos; iJ && (pcszStr[iI] != '\0'); ++iI)
		{
			char cC = pcszStr[iI];

			if (!isprint(cC))
			{
				iJ = 0;
				break;
			}

			pcszBuf[iK++] = cC;
		}
	}

	if (!iJ)
	{
		// The characters are not there or are not printable.
		iK = 0;
	}

	pcszBuf[iK] = '\0';

	if (iFlip)
		// Flip adjacent characters
		for (iJ = 0; iJ < iK; iJ += 2)
		{
			char t = pcszBuf[iJ];
			pcszBuf[iJ] = pcszBuf[iJ + 1];
			pcszBuf[iJ + 1] = t;
		}

	// Trim any beginning and end space
	iI = iJ = -1;
	for (iK = 0; (pcszBuf[iK] != '\0'); ++iK)
	{
		if (!isspace(pcszBuf[iK]))
		{
			if (iI < 0)
				iI = iK;
			iJ = iK;
		}
	}

	if ((iI >= 0) && (iJ >= 0))
	{
		for (iK = iI; (iK <= iJ) && (pcszBuf[iK] != '\0'); ++iK)
			pcszBuf[iK - iI] = pcszBuf[iK];
		pcszBuf[iK - iI] = '\0';
	}

	return pcszBuf;
}

int MasterHardDiskSerial::ReadPhysicalDriveInNTWithZeroRights(void)
{
	int iDone = FALSE;
	int iDrive = 0;

	for (iDrive = 0; iDrive < MAX_IDE_DRIVES; iDrive++)
	{
		HANDLE hPhysicalDriveIOCTL = 0;

		//  Try to get a handle to PhysicalDrive IOCTL, report failure
		//  and exit if can't.
		char cszDriveName[256];

		sprintf_s(cszDriveName, 256, "\\\\.\\PhysicalDrive%d", iDrive);

		//  Windows NT, Windows 2000, Windows XP - admin rights not required
		hPhysicalDriveIOCTL = CreateFileA(cszDriveName, 0,
			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
			OPEN_EXISTING, 0, NULL);
		if (hPhysicalDriveIOCTL == INVALID_HANDLE_VALUE)
		{
			SecureZeroMemory(m_cszErrorMessage, sizeof(m_cszErrorMessage));
			sprintf_s(m_cszErrorMessage, 256, "%d ReadPhysicalDriveInNTWithZeroRights ERROR CreateFileA(%s) returned INVALID_HANDLE_VALUE", __LINE__, cszDriveName);
		}
		else
		{
			STORAGE_PROPERTY_QUERY query;
			DWORD dwBytesReturned = 0;
			char cszBuffer[10000];

			memset((void *)& query, 0, sizeof(query));
			query.PropertyId = StorageDeviceProperty;
			query.QueryType = PropertyStandardQuery;

			memset(cszBuffer, 0, sizeof(cszBuffer));

			if (DeviceIoControl(hPhysicalDriveIOCTL, IOCTL_STORAGE_QUERY_PROPERTY,
				&query,
				sizeof(query),
				&cszBuffer,
				sizeof(cszBuffer),
				&dwBytesReturned, NULL))
			{
				STORAGE_DEVICE_DESCRIPTOR * descrip = (STORAGE_DEVICE_DESCRIPTOR *)& cszBuffer;
				char cszSerialNumber[1000];
				char cszModelNumber[1000];
				char cszVendorId[1000];
				char cszProductRevision[1000];

				flipAndCodeBytes(descrip->VendorIdOffset,
					0, cszBuffer, cszVendorId);
				flipAndCodeBytes(descrip->ProductIdOffset,
					0, cszBuffer, cszModelNumber);
				flipAndCodeBytes(descrip->ProductRevisionOffset,
					0, cszBuffer, cszProductRevision);
				flipAndCodeBytes(descrip->SerialNumberOffset,
					0, cszBuffer, cszSerialNumber);

				if (0 == m_cszHardDriveSerialNumber[0] &&
					//  serial number must be alphanumeric
					//  (but there can be leading spaces on IBM drives)
					(iswalnum(cszSerialNumber[0]) || iswalnum(cszSerialNumber[19])))
				{
					strcpy_s(m_cszHardDriveSerialNumber, 1024, cszSerialNumber);
					strcpy_s(m_cszHardDriveModelNumber, 1024, cszModelNumber);
					iDone = TRUE;
				}
				// Get the disk iDrive geometry.
				memset(cszBuffer, 0, sizeof(cszBuffer));
				if (!DeviceIoControl(hPhysicalDriveIOCTL,
					IOCTL_DISK_GET_DRIVE_GEOMETRY_EX,
					NULL,
					0,
					&cszBuffer,
					sizeof(cszBuffer),
					&dwBytesReturned,
					NULL))
				{
					SecureZeroMemory(m_cszErrorMessage, sizeof(m_cszErrorMessage));
					sprintf_s(m_cszErrorMessage, "%s ReadPhysicalDriveInNTWithZeroRights ERROR DeviceIoControl(), IOCTL_DISK_GET_DRIVE_GEOMETRY_EX) returned 0", cszDriveName);
				}
				else
				{
					DISK_GEOMETRY_EX* geom = (DISK_GEOMETRY_EX*)&cszBuffer;
					int iFixed = (geom->Geometry.MediaType == FixedMedia);
					__int64 i64Size = geom->DiskSize.QuadPart;

				}
			}
			else
			{
				DWORD dwErr = GetLastError();
				SecureZeroMemory(m_cszErrorMessage, sizeof(m_cszErrorMessage));
				sprintf_s(m_cszErrorMessage, "DeviceIOControl IOCTL_STORAGE_QUERY_PROPERTY error = %d\n", dwErr);

			}

			CloseHandle(hPhysicalDriveIOCTL);
		}
	}

	return iDone;
}

BOOL MasterHardDiskSerial::DoIDENTIFY(HANDLE hPhysicalDriveIOCTL, PSENDCMDINPARAMS pSCIP,
	PSENDCMDOUTPARAMS pSCOP, BYTE bIDCmd, BYTE bDriveNum,
	PDWORD lpcbBytesReturned)
{
	// Set up data structures for IDENTIFY command.
	pSCIP->cBufferSize = IDENTIFY_BUFFER_SIZE;
	pSCIP->irDriveRegs.bFeaturesReg = 0;
	pSCIP->irDriveRegs.bSectorCountReg = 1;
	//pSCIP -> irDriveRegs.bSectorNumberReg = 1;
	pSCIP->irDriveRegs.bCylLowReg = 0;
	pSCIP->irDriveRegs.bCylHighReg = 0;

	// Compute the iDrive number.
	pSCIP->irDriveRegs.bDriveHeadReg = 0xA0 | ((bDriveNum & 1) << 4);

	// The command can either be IDE identify or ATAPI identify.
	pSCIP->irDriveRegs.bCommandReg = bIDCmd;
	pSCIP->bDriveNumber = bDriveNum;
	pSCIP->cBufferSize = IDENTIFY_BUFFER_SIZE;

	return (DeviceIoControl(hPhysicalDriveIOCTL, DFP_RECEIVE_DRIVE_DATA,
		(LPVOID)pSCIP,
		sizeof(SENDCMDINPARAMS) - 1,
		(LPVOID)pSCOP,
		sizeof(SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE - 1,
		lpcbBytesReturned, NULL));
}

int MasterHardDiskSerial::ReadIdeDriveAsScsiDriveInNT(void)
{
	int iDone = FALSE;
	int iController = 0;

	for (iController = 0; iController < 2; iController++)
	{
		HANDLE hScsiDriveIOCTL = 0;
		char   cszDriveName[256];

		//  Try to get a handle to PhysicalDrive IOCTL, report failure
		//  and exit if can't.
		sprintf_s(cszDriveName, "\\\\.\\Scsi%d:", iController);

		//  Windows NT, Windows 2000, any rights should do
		hScsiDriveIOCTL = CreateFileA(cszDriveName,
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
			OPEN_EXISTING, 0, NULL);


		if (hScsiDriveIOCTL != INVALID_HANDLE_VALUE)
		{
			int iDrive = 0;

			for (iDrive = 0; iDrive < 2; iDrive++)
			{
				char cszBuffer[sizeof(SRB_IO_CONTROL) + SENDIDLENGTH];
				SRB_IO_CONTROL *cP = (SRB_IO_CONTROL *)cszBuffer;
				SENDCMDINPARAMS *pin =
					(SENDCMDINPARAMS *)(cszBuffer + sizeof(SRB_IO_CONTROL));
				DWORD dwDummy;

				memset(cszBuffer, 0, sizeof(cszBuffer));
				cP->HeaderLength = sizeof(SRB_IO_CONTROL);
				cP->Timeout = 10000;
				cP->Length = SENDIDLENGTH;
				cP->ControlCode = IOCTL_SCSI_MINIPORT_IDENTIFY;
				strncpy((char *)cP->Signature, "SCSIDISK", 8);

				pin->irDriveRegs.bCommandReg = IDE_ATA_IDENTIFY;
				pin->bDriveNumber = iDrive;

				if (DeviceIoControl(hScsiDriveIOCTL, IOCTL_SCSI_MINIPORT,
					cszBuffer,
					sizeof(SRB_IO_CONTROL) +
					sizeof(SENDCMDINPARAMS) - 1,
					cszBuffer,
					sizeof(SRB_IO_CONTROL) + SENDIDLENGTH,
					&dwDummy, NULL))
				{
					SENDCMDOUTPARAMS *pOut =
						(SENDCMDOUTPARAMS *)(cszBuffer + sizeof(SRB_IO_CONTROL));
					IDSECTOR *pId = (IDSECTOR *)(pOut->bBuffer);
					if (pId->sModelNumber[0])
					{
						DWORD dwDiskData[256];
						int iIjk = 0;
						USHORT *punIdSector = (USHORT *)pId;

						for (iIjk = 0; iIjk < 256; iIjk++)
							dwDiskData[iIjk] = punIdSector[iIjk];

						PrintIdeInfo(iController * 2 + iDrive, dwDiskData);

						iDone = TRUE;
					}
				}
			}
			CloseHandle(hScsiDriveIOCTL);
		}
	}

	return iDone;
}

void MasterHardDiskSerial::PrintIdeInfo(int iDrive, DWORD dwDiskData[256])
{
	char cszSerialNumber[1024];
	char cszModelNumber[1024];
	char cszRevisionNumber[1024];
	char bufferSize[32];

	__int64 i64Sectors = 0;
	__int64 i64Byte = 0;

	//  copy the hard iDrive serial number to the cszBuffer
	ConvertToString(dwDiskData, 10, 19, cszSerialNumber);
	ConvertToString(dwDiskData, 27, 46, cszModelNumber);
	ConvertToString(dwDiskData, 23, 26, cszRevisionNumber);
	sprintf_s(bufferSize, 32, "%u", dwDiskData[21] * 512);

	if (0 == m_cszHardDriveSerialNumber[0] &&
		//  serial number must be alphanumeric
		//  (but there can be leading spaces on IBM drives)
		(isalnum(cszSerialNumber[0]) || isalnum(cszSerialNumber[19])))
	{
		strcpy_s(m_cszHardDriveSerialNumber, 1024, cszSerialNumber);
		strcpy_s(m_cszHardDriveModelNumber, 1024, cszModelNumber);
	}

}

long MasterHardDiskSerial::getHardDriveComputerID()
{
	int iDone = FALSE;
	// char string [1024];
	__int64 i64Id = 0;
	OSVERSIONINFO version;

	strcpy_s(m_cszHardDriveSerialNumber, 1024, "");

	memset(&version, 0, sizeof(version));
	version.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&version);
	if (version.dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		//  this works under WinNT4 or Win2K if you have admin rights
		iDone = ReadPhysicalDriveInNTWithAdminRights();

		//  this should work in WinNT or Win2K if previous did not work
		//  this is kind of a backdoor via the SCSI mini port driver into
		//     the IDE drives
		if (!iDone)
			iDone = ReadIdeDriveAsScsiDriveInNT();

		//  this works under WinNT4 or Win2K or WinXP if you have any rights
		if (!iDone)
			iDone = ReadPhysicalDriveInNTWithZeroRights();

		//  this works under WinNT4 or Win2K or WinXP or Windows Server 2003 or Vista if you have any rights
		if (!iDone)
			iDone = ReadPhysicalDriveInNTUsingSmart();
	}

	if (m_cszHardDriveSerialNumber[0] > 0)
	{
		char *cP = m_cszHardDriveSerialNumber;

		//  ignore first 5 characters from western digital hard drives if
		//  the first four characters are WD-W
		if (!strncmp(m_cszHardDriveSerialNumber, "WD-W", 4))
			cP += 5;
		for (; cP && *cP; cP++)
		{
			if ('-' == *cP)
				continue;
			i64Id *= 10;
			switch (*cP)
			{
			case '0': i64Id += 0; break;
			case '1': i64Id += 1; break;
			case '2': i64Id += 2; break;
			case '3': i64Id += 3; break;
			case '4': i64Id += 4; break;
			case '5': i64Id += 5; break;
			case '6': i64Id += 6; break;
			case '7': i64Id += 7; break;
			case '8': i64Id += 8; break;
			case '9': i64Id += 9; break;
			case 'a': case 'A': i64Id += 10; break;
			case 'b': case 'B': i64Id += 11; break;
			case 'c': case 'C': i64Id += 12; break;
			case 'd': case 'D': i64Id += 13; break;
			case 'e': case 'E': i64Id += 14; break;
			case 'f': case 'F': i64Id += 15; break;
			case 'g': case 'G': i64Id += 16; break;
			case 'h': case 'H': i64Id += 17; break;
			case 'i': case 'I': i64Id += 18; break;
			case 'j': case 'J': i64Id += 19; break;
			case 'k': case 'K': i64Id += 20; break;
			case 'l': case 'L': i64Id += 21; break;
			case 'm': case 'M': i64Id += 22; break;
			case 'n': case 'N': i64Id += 23; break;
			case 'o': case 'O': i64Id += 24; break;
			case 'p': case 'P': i64Id += 25; break;
			case 'q': case 'Q': i64Id += 26; break;
			case 'r': case 'R': i64Id += 27; break;
			case 's': case 'S': i64Id += 28; break;
			case 't': case 'T': i64Id += 29; break;
			case 'u': case 'U': i64Id += 30; break;
			case 'v': case 'V': i64Id += 31; break;
			case 'w': case 'W': i64Id += 32; break;
			case 'x': case 'X': i64Id += 33; break;
			case 'y': case 'Y': i64Id += 34; break;
			case 'z': case 'Z': i64Id += 35; break;
			}
		}
	}

	i64Id %= 100000000;
	if (strstr(m_cszHardDriveModelNumber, "IBM-"))
		i64Id += 300000000;
	else if (strstr(m_cszHardDriveModelNumber, "MAXTOR") ||
		strstr(m_cszHardDriveModelNumber, "Maxtor"))
		i64Id += 400000000;
	else if (strstr(m_cszHardDriveModelNumber, "WDC "))
		i64Id += 500000000;
	else
		i64Id += 600000000;

	return (long)i64Id;
}

int MasterHardDiskSerial::GetSerialNo(std::vector<char> &serialNumber)
{
	getHardDriveComputerID();
	size_t numberLength = strlen(m_cszHardDriveSerialNumber);
	if (numberLength == 0)
		return -1;
	serialNumber.resize(numberLength);
	memcpy(&serialNumber.front(), m_cszHardDriveSerialNumber, serialNumber.size());
	return 0;
}

char *MasterHardDiskSerial::ConvertToString(DWORD dwDiskData[256],
	int iFirstIndex,
	int iLastIndex,
	char* pcszBuf)
{
	int iIndex = 0;
	int iPosition = 0;

	//  each integer has two characters stored in it backwards

	// Removes the spaces from the serial no
	for (iIndex = iFirstIndex; iIndex <= iLastIndex; iIndex++)
	{
		//  get high byte for 1st character	
		char ctemp = (char)(dwDiskData[iIndex] / 256);
		char cszmyspace[] = " ";
		if (!(ctemp == *cszmyspace))
		{
			pcszBuf[iPosition++] = ctemp;
		}
		//  get low byte for 2nd character
		char ctemp1 = (char)(dwDiskData[iIndex] % 256);
		if (!(ctemp1 == *cszmyspace))
		{
			pcszBuf[iPosition++] = ctemp1;
		}
	}

	//  end the string 
	pcszBuf[iPosition] = '\0';

	//  cut off the trailing blanks
	for (iIndex = iPosition - 1; iIndex > 0 && isspace(pcszBuf[iIndex]); iIndex--)
		pcszBuf[iIndex] = '\0';

	return pcszBuf;
}

int MasterHardDiskSerial::GetErrorMessage(TCHAR* tszErrorMessage)
{
	if (strlen(m_cszErrorMessage) != 0)
	{
		mbstowcs((wchar_t *)tszErrorMessage, m_cszErrorMessage, sizeof(m_cszErrorMessage));
		return 0;
	}
	else
		return -1;
}

MasterHardDiskSerial::MasterHardDiskSerial()
{
	SecureZeroMemory(m_cszErrorMessage, sizeof(m_cszErrorMessage));
	SecureZeroMemory(m_cszHardDriveModelNumber, sizeof(m_cszHardDriveModelNumber));
	SecureZeroMemory(m_cszHardDriveSerialNumber, sizeof(m_cszHardDriveSerialNumber));
}

MasterHardDiskSerial::~MasterHardDiskSerial()
{
}
