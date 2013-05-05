/********************************************************************************
 Copyright (C) 2012 Eric Bataille <e.c.p.bataille@gmail.com>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.
********************************************************************************/


#include "d3dstuff.h"

void D3DStuff::prepareFrame()
{
    d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(config->GetInt("backR"), config->GetInt("backG"), config->GetInt("backB")), 1.0f, 0);
    d3ddev->BeginScene();
}

void D3DStuff::finalizeFrame()
{
    d3ddev->EndScene();
    d3ddev->Present(NULL, NULL, NULL, NULL);
}

void D3DStuff::initD3D(HWND hWnd, KBInfo * kbinfo, ConfigParser * config)
{
    d3d = Direct3DCreate9(D3D_SDK_VERSION);
    D3DPRESENT_PARAMETERS d3dpp;

    this->kbinfo = kbinfo;
    this->config = config;

    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = hWnd;

    d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                      D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3ddev);

    D3DXCreateFont(d3ddev, 18, 0, 1, 1, false, DEFAULT_CHARSET, 
        OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Arial", &font);
}

void D3DStuff::cleanD3D(void)
{
    if (v_buffer) v_buffer->Release();
    if (d3ddev) d3ddev->Release();
    if (d3d) d3d->Release();
}

void D3DStuff::drawLine(float x1, float y1, float x2, float y2, D3DCOLOR color)
{
    VERTEX vertex[2] = { { x1, y1, 1.0f, 1.0f, color}, { x2, y2, 1.0f, 1.0f, color} };
    d3ddev->CreateVertexBuffer(2*sizeof(VERTEX), 0, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_MANAGED, &v_buffer, NULL);

    VOID* pVoid;
    v_buffer->Lock(0, 0, (void**)&pVoid, 0);
    memcpy(pVoid, vertex, sizeof(vertex));
    v_buffer->Unlock();

    d3ddev->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
    d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(VERTEX));
    d3ddev->DrawPrimitive(D3DPT_LINESTRIP, 0, 1);
}


void D3DStuff::drawFillBox(float x1, float y1, float x2, float y2, D3DCOLOR color)
{
    VERTEX vertex[5] = { { x1, y1, 1.0f, 1.0f, color}, { x2, y1, 1.0f, 1.0f, color}, { x2, y2, 1.0f, 1.0f, color}, { x1, y2, 1.0f, 1.0f, color}, { x1, y1, 1.0f, 1.0f, color} };
    d3ddev->CreateVertexBuffer(5*sizeof(VERTEX), 0, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_MANAGED, &v_buffer, NULL);

    VOID* pVoid;
    v_buffer->Lock(0, 0, (void**)&pVoid, 0);
    memcpy(pVoid, vertex, sizeof(vertex));
    v_buffer->Unlock();

    d3ddev->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
    d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(VERTEX));
    d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 3);
}

void D3DStuff::drawBox(float x1, float y1, float x2, float y2, D3DCOLOR color)
{
    VERTEX vertex[5] = { { x1, y1, 1.0f, 1.0f, color}, { x1, y2, 1.0f, 1.0f, color}, { x2, y2, 1.0f, 1.0f, color},
                        { x2, y1, 1.0f, 1.0f, color}, { x1, y1, 1.0f, 1.0f, color} };
    d3ddev->CreateVertexBuffer(5*sizeof(VERTEX), 0, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_MANAGED, &v_buffer, NULL);

    VOID* pVoid;
    v_buffer->Lock(0, 0, (void**)&pVoid, 0);
    memcpy(pVoid, vertex, sizeof(vertex));
    v_buffer->Unlock();

    d3ddev->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
    d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(VERTEX));
    d3ddev->DrawPrimitive(D3DPT_LINESTRIP, 0, 4);
}

void D3DStuff::drawText(RECT &rect, D3DCOLOR color, LPSTR text)
{
    if (text == "") return;
    font->DrawText(NULL, text, strlen(text), &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER, color);
}