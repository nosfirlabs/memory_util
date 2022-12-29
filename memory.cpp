#include <Windows.h>
#include <winioctl.h>

#define IOCTL_READ_MEMORY CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_WRITE_MEMORY CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define PROCESS_NAME_LEN 32

typedef struct _MEMORY_OPERATION_DATA {
    ULONG ProcessId;
    WCHAR ProcessName[PROCESS_NAME_LEN];
    ULONG64 Address;
    SIZE_T Size;
    PVOID Buffer;
} MEMORY_OPERATION_DATA, *PMEMORY_OPERATION_DATA;

HANDLE hDevice;

BOOL OpenDevice()
{
    hDevice = CreateFile(L"\\\\.\\MemoryAccess",
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);

    if (hDevice == INVALID_HANDLE_VALUE)
        return FALSE;

    return TRUE;
}

BOOL CloseDevice()
{
    if (hDevice != INVALID_HANDLE_VALUE)
        CloseHandle(hDevice);

    hDevice = INVALID_HANDLE_VALUE;

    return TRUE;
}

BOOL ReadMemory(ULONG ProcessId, ULONG64 Address, SIZE_T Size, PVOID Buffer)
{
    BOOL bSuccess = FALSE;
    DWORD dwBytesReturned = 0;
    MEMORY_OPERATION_DATA data;
    data.ProcessId = ProcessId;
    data.Address = Address;
    data.Size = Size;
    data.Buffer = Buffer;

    if (!DeviceIoControl(hDevice, IOCTL_READ_MEMORY, &data, sizeof(data), &data, sizeof(data), &dwBytesReturned, NULL))
        return FALSE;

    return TRUE;
}

BOOL WriteMemory(ULONG ProcessId, ULONG64 Address, SIZE_T Size, PVOID Buffer)
{
    BOOL bSuccess = FALSE;
    DWORD dwBytesReturned = 0;
    MEMORY_OPERATION_DATA data;
    data.ProcessId = ProcessId;
    data.Address = Address;
    data.Size = Size;
    data.Buffer = Buffer;

    if (!DeviceIoControl(hDevice, IOCTL_WRITE_MEMORY, &data, sizeof(data), &data, sizeof(data), &dwBytesReturned, NULL))
        return FALSE;
        

int main()
{
    if (!OpenDevice())
    {
        printf("Failed to open device.\n");
        return 1;
    }

    // Find the process ID of the target process
    ULONG ProcessId = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32 pe;
        pe.dwSize = sizeof(pe);
        if (Process32First(hSnapshot, &pe))
        {
            do
            {
                if (_wcsicmp(pe.szExeFile, L"TargetProcess.exe") == 0)
                {
                    ProcessId = pe.th32ProcessID;
                    break;
                }
            } while (Process32Next(hSnapshot, &pe));
        }

        CloseHandle(hSnapshot);
    }

    if (ProcessId == 0)
    {
        printf("Failed to find process.\n");
        CloseDevice();
        return 1;
    }

    // Read memory from the target process
    ULONG Value = 0;
    if (!ReadMemory(ProcessId, 0x12345678, sizeof(Value), &Value))
    {
        printf("Failed to read memory.\n");
        CloseDevice();
        return 1;
    }

    printf("Value: %d\n", Value);

    // Write a new value to memory in the target process
    Value = 123;
    if (!WriteMemory(ProcessId, 0x12345678, sizeof(Value), &Value))
    {
        printf("Failed to write memory.\n");
        CloseDevice();
        return 1;
    }

    CloseDevice();
    return 0;
}
