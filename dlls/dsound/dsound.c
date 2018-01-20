/*
 * Copyright 2017 André Hentschel
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

/* NOTE: The guest side uses mingw's headers. The host side uses Wine's headers. */

#include <windows.h>
#include <stdio.h>
#include <dsound.h>
#include <dsconf.h>

#include "windows-user-services.h"
#include "dll_list.h"
#include "qemu_dsound.h"

#ifdef QEMU_DLL_GUEST
#include <debug.h>
#else
#include <wine/debug.h>
#endif

WINE_DEFAULT_DEBUG_CHANNEL(qemu_dsound);

struct qemu_DirectSoundCreate
{
    struct qemu_syscall super;
    uint64_t lpcGUID;
    uint64_t ppDS;
    uint64_t pUnkOuter;
};

#ifdef QEMU_DLL_GUEST

HRESULT IDirectSoundImpl_Create(IUnknown *outer_unk, REFIID riid, void **ppv, BOOL has_ds8)
{
    WINE_FIXME("Stub!\n");
    return E_FAIL;
}

HRESULT DSOUND_Create(REFIID riid, void **ppv)
{
    return IDirectSoundImpl_Create(NULL, riid, ppv, FALSE);
}

HRESULT DSOUND_Create8(REFIID riid, void **ppv)
{
    return IDirectSoundImpl_Create(NULL, riid, ppv, TRUE);
}

WINBASEAPI HRESULT WINAPI DirectSoundCreate(LPCGUID lpcGUID, LPDIRECTSOUND *ppDS, IUnknown *pUnkOuter)
{
    struct qemu_DirectSoundCreate call;
    call.super.id = QEMU_SYSCALL_ID(CALL_DIRECTSOUNDCREATE);
    call.lpcGUID = (ULONG_PTR)lpcGUID;
    call.ppDS = (ULONG_PTR)ppDS;
    call.pUnkOuter = (ULONG_PTR)pUnkOuter;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_DirectSoundCreate(struct qemu_syscall *call)
{
    struct qemu_DirectSoundCreate *c = (struct qemu_DirectSoundCreate *)call;
    WINE_FIXME("Unverified!\n");
    c->super.iret = DirectSoundCreate(QEMU_G2H(c->lpcGUID), QEMU_G2H(c->ppDS), QEMU_G2H(c->pUnkOuter));
}

#endif

struct qemu_DirectSoundCreate8
{
    struct qemu_syscall super;
    uint64_t lpcGUID;
    uint64_t ppDS;
    uint64_t pUnkOuter;
};

#ifdef QEMU_DLL_GUEST

WINBASEAPI HRESULT WINAPI DirectSoundCreate8(LPCGUID lpcGUID, LPDIRECTSOUND8 *ppDS, IUnknown *pUnkOuter)
{
    struct qemu_DirectSoundCreate8 call;
    call.super.id = QEMU_SYSCALL_ID(CALL_DIRECTSOUNDCREATE8);
    call.lpcGUID = (ULONG_PTR)lpcGUID;
    call.ppDS = (ULONG_PTR)ppDS;
    call.pUnkOuter = (ULONG_PTR)pUnkOuter;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_DirectSoundCreate8(struct qemu_syscall *call)
{
    struct qemu_DirectSoundCreate8 *c = (struct qemu_DirectSoundCreate8 *)call;
    WINE_FIXME("Unverified!\n");
    c->super.iret = DirectSoundCreate8(QEMU_G2H(c->lpcGUID), QEMU_G2H(c->ppDS), QEMU_G2H(c->pUnkOuter));
}

#endif

