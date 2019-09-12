#include <efi.h>
#include <efilib.h>


VOID
efi_panic(EFI_STATUS efi_status, INTN line)
{
    Print(L"panic at line:%d\n", line);
    Print(L"EFI_STATUS = %d\n", efi_status);
    while (1);
}

static inline VOID
MY_EFI_ASSERT(EFI_STATUS status, INTN line)
{
    if (EFI_ERROR(status)) {
        efi_panic(status, line);
    }
}


EFI_LOADED_IMAGE *
OpenLoadedImageProtocol(EFI_HANDLE Handle, EFI_HANDLE AgentHandle)
{
    EFI_LOADED_IMAGE *LoadedImage = NULL;
    
    MY_EFI_ASSERT(
        uefi_call_wrapper(
            BS->OpenProtocol,
            6,
            Handle, /* Handle */
            &LoadedImageProtocol, /* Protocol */
            (VOID **)&LoadedImage, /* Interface */
            AgentHandle, /* AgentHandle */
            NULL, /* ControllrHandle */
            EFI_OPEN_PROTOCOL_GET_PROTOCOL /* Attributes */
        )
    , __LINE__);

    return LoadedImage;
}

inline VOID
WaitDebuggerAArch64(VOID *ImageBase)
{
    // Wait to connect from debugger
    // Please read EntryPointAddress from x2
    // and set x0 = 1 to break loop
    Print(L"Wait to connect from debugger\n");
    Print(L"Please read EntryPointAddress from x2\n");
    Print(L"and set x0 = 1 to break loop\n");
    __asm__ volatile (
        "mov x0, xzr;"
        "mov x1, %[iamge_base];"
        "loop:;"
        "cbz x0, loop;"
        :
        : [iamge_base] "r" (ImageBase)
        : "x0", "x1"
    );
}


EFI_STATUS
efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    InitializeLib(ImageHandle, SystemTable);

    EFI_LOADED_IMAGE *LoadedImage = OpenLoadedImageProtocol(ImageHandle, ImageHandle);

    Print(L"current entry: 0x%016lx\n", LoadedImage->ImageBase);

    WaitDebuggerAArch64(LoadedImage->ImageBase);

    Print(L"Hello World!\n");

	return EFI_SUCCESS;
}
