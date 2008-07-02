//#define OV_DEBUG
#pragma unmanaged
#include <windows.h>
#include <uxtheme.h>
#include "DeskBand.h"
#include "OVIMEUI.h"
#include "DotNETHeader.h"


#define RECTWIDTH(x)   ((x).right - (x).left)
#define RECTHEIGHT(x)  ((x).bottom - (x).top)

extern ULONG        g_cDllRef;
extern HINSTANCE    g_hInst;

extern CLSID CLSID_OVDeskBand;

static const WCHAR szOVDeskBand[] = L"OVDeskBand"; 

CDeskBand::CDeskBand()
{
    m_cRef       = 1;
    m_pSite      = NULL;
    m_fHasFocus  = FALSE;
    m_fIsDirty   = FALSE;
    m_dwBandID   = 0;
    m_hwnd       = NULL;
    m_hwndParent = NULL;
}

CDeskBand::~CDeskBand()
{
    if (m_pSite)
    {
        m_pSite->Release();
    }
}

//
// IUnknown
//
STDMETHODIMP CDeskBand::QueryInterface(REFIID riid, void **ppv)
{
    HRESULT hr = S_OK;

    if (IsEqualIID(IID_IUnknown, riid)       ||
        IsEqualIID(IID_IOleWindow, riid)     ||
        IsEqualIID(IID_IDockingWindow, riid) ||
        IsEqualIID(IID_IDeskBand, riid)      ||
        IsEqualIID(IID_IDeskBand2, riid))
    {
        *ppv = static_cast<IOleWindow *>(this);
    }
    else if (IsEqualIID(IID_IPersist, riid) ||
             IsEqualIID(IID_IPersistStream, riid))
    {
        *ppv = static_cast<IPersist *>(this);
    }
    else if (IsEqualIID(IID_IObjectWithSite, riid))
    {
        *ppv = static_cast<IObjectWithSite *>(this);
    }
    else if (IsEqualIID(IID_IInputObject, riid))
    {
        *ppv = static_cast<IInputObject *>(this);
    }
    else
    {
        hr = E_NOINTERFACE;
        *ppv = NULL;
    }

    if (*ppv)
    {
        AddRef();
    }

    return hr;
}

STDMETHODIMP_(ULONG) CDeskBand::AddRef()
{
    return InterlockedIncrement(&m_cRef);
}

STDMETHODIMP_(ULONG) CDeskBand::Release()
{
    ULONG cRef = InterlockedDecrement(&m_cRef);
    if (0 == cRef)
    {
        delete this;
    }

    return cRef;
}

//
// IOleWindow 
//
STDMETHODIMP CDeskBand::GetWindow(HWND *phwnd)
{
    *phwnd = m_hwnd;
    return S_OK;
}

STDMETHODIMP CDeskBand::ContextSensitiveHelp(BOOL)
{
    return E_NOTIMPL;
}

// 
// IDockingWindow
//
STDMETHODIMP CDeskBand::ShowDW(BOOL fShow)
{
	murmur("OVDeskBand:ShowDW invoked, fShow:%d, hwnd:%x",fShow, m_hwnd); 
	
	RemoveOrphanBand(m_dwBandID);		
    if (m_hwnd)
    {
		ShowWindow(m_hwnd, fShow ? SW_SHOWNOACTIVATE : SW_HIDE);  

    }
	
	/*
	if(!fShow)
	{	

		murmur("\t Setting zero border space.");
		if(m_pSite)
		{
		IDockingWindowSite* dws;
		if(SUCCEEDED( m_pSite->QueryInterface(IID_IDockingWindowSite, reinterpret_cast<void **>(&dws)))) 
		{
			murmur("\t Got IDockingwindowsite interface."); 
			BORDERWIDTHS bws;
			bws.top =0;
			bws.bottom =0;
			bws.right =0;
			bws.left =0;
			if(SUCCEEDED(dws->SetBorderSpaceDW(static_cast<IOleWindow *>(this), &bws)))
				murmur("\t Setting zero border space succeeded."); 

			dws->Release();
		}
		else
			murmur("\t Failed to get IDockingwindowsite interface.");
		}
		else
			murmur("\t null m_pSite");
	}
		
	*/
    return S_OK;
}

STDMETHODIMP CDeskBand::CloseDW(DWORD)
{
	murmur("OVDeskBand:ClosedDW invoked");
	
	RemoveOrphanBand(0);

    if (m_hwnd)
    {	
			
        ShowWindow(m_hwnd, SW_HIDE);
		DestroyWindow(m_hwnd);
        m_hwnd = NULL;
		
    
    }

    return S_OK;
}

void CDeskBand::RemoveOrphanBand(DWORD myBandID)
{
	if(m_pSite)
		{
			murmur("\tTry to remove redundant OVDeskbands; my bandid:%d", myBandID); 
			
			IBandSite *bs;
			
			if (SUCCEEDED(m_pSite->QueryInterface(IID_IBandSite, reinterpret_cast<void **>(&bs))))
			{
				
				int i=0;
				DWORD dwbid;
				wchar_t wstr[256];
				char str[256];
				while(SUCCEEDED(bs->EnumBands(i, &dwbid)))
				{
					IDeskBand *db;
					IPersist *ps;
					CLSID cid;
					CLSID CLSID_Orphan = {0, 0, 0, {0, 0, 0, 0, 0, 0, 0, 0}};
					bs->QueryBand(dwbid,reinterpret_cast<IDeskBand **>(&db), NULL, wstr, 256);
					
					wcstombs(str, wstr, 256);
					murmur("\tQuery Band, bandid %d, name %s", dwbid, str);
					if(SUCCEEDED(bs->GetBandObject(dwbid,IID_IPersist,reinterpret_cast<void **>(&ps))))
					{
						ps->GetClassID(&cid);
						murmur("\tBand CLSID:%x-%x-%x-{%x-%x-%x-%x-%x-%x-%x-%x}",   
						cid.Data1, cid.Data2, cid.Data3, 
						cid.Data4[0],cid.Data4[1],cid.Data4[2],cid.Data4[3],
						cid.Data4[4],cid.Data4[5],cid.Data4[6],cid.Data4[7]);
						
						if(cid == CLSID_OVDeskBand && (myBandID>0 && dwbid != myBandID))
						{
						   murmur("\tRedundunt OVDeskband found");
						   bs->RemoveBand(dwbid);	
						}
						else if (cid == CLSID_Orphan)
						{
							murmur("\tRemove orphan band with null CLSID"); 
							bs->RemoveBand(dwbid);
						}
						
					}
					else
					{
						murmur("\tRemove orphan band without CLSID");
						bs->RemoveBand(dwbid); // remove orphan band w/o clsid 
					}
					
				
					
					i++;
				}

				murmur("OVDeskBand:ShowDW: Enumband ended"); 
				bs->Release(); 
			}
			else
				murmur("OVDeskBand:ShowDW: Get site interface failed"); 
		}
	else
		murmur("OVDeskBand:ShowDW: Null m_pSite");
}

STDMETHODIMP CDeskBand::ResizeBorderDW(const RECT * prcBorder, IUnknown * punkToolbarSite, BOOL)
{
	murmur("OVDeskBand:ResizeBOrderDW:border.left:%d, border.top:%d",prcBorder->left,prcBorder->top);
    //return E_NOTIMPL;
	HRESULT hr;
	IDockingWindowSite *dws;
	if (punkToolbarSite)
    {
        hr = punkToolbarSite->QueryInterface(IID_IDockingWindowSite, reinterpret_cast<void **>(&dws)); 
			

		if (SUCCEEDED(hr))
		{
			BORDERWIDTHS pbw;
			pbw.top =0;
			pbw.bottom =0;
			pbw.right =0;
			pbw.left =0;


			hr = dws->RequestBorderSpaceDW(static_cast<IOleWindow *>(this), &pbw);
			if (SUCCEEDED(hr))
				hr = dws->SetBorderSpaceDW(static_cast<IOleWindow *>(this), &pbw);

		}	
	
		dws->Release();
	}
	
	return hr;
}

//
// IDeskBand
//
STDMETHODIMP CDeskBand::GetBandInfo(DWORD dwBandID, DWORD, DESKBANDINFO *pdbi)
{
    HRESULT hr = E_INVALIDARG;

    if (pdbi)
    {
        m_dwBandID = dwBandID;

        if (pdbi->dwMask & DBIM_MINSIZE)
        {
            pdbi->ptMinSize.x = 85;
            pdbi->ptMinSize.y = 30;
        }

        if (pdbi->dwMask & DBIM_MAXSIZE)
        {
			pdbi->ptMaxSize.y = -1;
            
        }

        if (pdbi->dwMask & DBIM_INTEGRAL)
        {
            pdbi->ptIntegral.y = 1;
        }
    
        if (pdbi->dwMask & DBIM_ACTUAL)
        {
            pdbi->ptActual.x = 85;
            pdbi->ptActual.y = 30;
        }

        if (pdbi->dwMask & DBIM_TITLE)
        {
            // Don't show title by removing this flag.
            pdbi->dwMask &= ~DBIM_TITLE;
        }

        if (pdbi->dwMask & DBIM_MODEFLAGS)
        {
            pdbi->dwModeFlags = DBIMF_NORMAL | DBIMF_FIXED;// |DBIMF_VARIABLEHEIGHT;
        }

        if (pdbi->dwMask & DBIM_BKCOLOR)
        {
            // Use the default background color by removing this flag.
            pdbi->dwMask &= ~DBIM_BKCOLOR;
        }

        hr = S_OK;
    }
    
    return hr;
}

//
// IDeskBand2
//
STDMETHODIMP CDeskBand::CanRenderComposited(BOOL *pfCanRenderComposited)
{
    *pfCanRenderComposited = TRUE;

    return S_OK;
}

STDMETHODIMP CDeskBand::SetCompositionState(BOOL fCompositionEnabled)
{
    m_fCompositionEnabled = fCompositionEnabled;

    InvalidateRect(m_hwnd, NULL, TRUE);
    UpdateWindow(m_hwnd);

    return S_OK;
}

STDMETHODIMP CDeskBand::GetCompositionState(BOOL *pfCompositionEnabled)
{
    *pfCompositionEnabled = m_fCompositionEnabled;

    return S_OK;
}

//
// IPersist
//
STDMETHODIMP CDeskBand::GetClassID(CLSID *pclsid)
{
    *pclsid = CLSID_OVDeskBand;
    return S_OK;
}

//
// IPersistStream
//
STDMETHODIMP CDeskBand::IsDirty()
{
    return m_fIsDirty ? S_OK : S_FALSE;
}

STDMETHODIMP CDeskBand::Load(IStream *pStm)
{
    return S_OK;
}

STDMETHODIMP CDeskBand::Save(IStream *pStm, BOOL fClearDirty)
{
    if (fClearDirty)
    {
        m_fIsDirty = FALSE;
    }

    return S_OK;
}

STDMETHODIMP CDeskBand::GetSizeMax(ULARGE_INTEGER *pcbSize)
{
    return E_NOTIMPL;
}

//
// IObjectWithSite
//
STDMETHODIMP CDeskBand::SetSite(IUnknown *pUnkSite)
{
    HRESULT hr = S_OK;

    m_hwndParent = NULL;

    if (m_pSite)
    {
        m_pSite->Release();
    }


    if (pUnkSite)
    {
		//m_pUnkSite = pUnkSite;
        IOleWindow *pow;
        hr = pUnkSite->QueryInterface(IID_IOleWindow, reinterpret_cast<void **>(&pow));
        if (SUCCEEDED(hr))
        {
            hr = pow->GetWindow(&m_hwndParent);
            if (SUCCEEDED(hr))
            {
                WNDCLASSW wc = { 0 };
                wc.style         = CS_HREDRAW | CS_VREDRAW;
                wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
                wc.hInstance     = g_hInst;
                wc.lpfnWndProc   = WndProc;
                wc.lpszClassName = szOVDeskBand;
                wc.hbrBackground = CreateSolidBrush(0xff0000ff); //RGB(255, 255, 0));

                RegisterClassW(&wc);

                CreateWindowExW(0, 
                                szOVDeskBand, 
                                NULL, 
                                WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 
                                0, 
                                0, 
                                0, 
                                0, 
                                m_hwndParent, 
                                NULL, 
                                g_hInst, 
                                this);

                if (!m_hwnd)
                {
                    hr = E_FAIL;
                }
            }

            pow->Release();
        }

        hr = pUnkSite->QueryInterface(IID_IInputObjectSite, reinterpret_cast<void **>(&m_pSite));
		
    }

    return hr;
}

STDMETHODIMP CDeskBand::GetSite(REFIID riid, void **ppv)
{
    HRESULT hr = E_FAIL;

    if (m_pSite)
    {
        hr =  m_pSite->QueryInterface(riid, ppv);
    }
    else
    {
        *ppv = NULL;
    }

    return hr;
}

//
// IInputObject
//
STDMETHODIMP CDeskBand::UIActivateIO(BOOL fActivate, MSG *)
{
    if (fActivate)
    {
        SetFocus(m_hwnd);
    }

    return S_OK;
}

STDMETHODIMP CDeskBand::HasFocusIO()
{
    return m_fHasFocus ? S_OK : S_FALSE;
}

STDMETHODIMP CDeskBand::TranslateAcceleratorIO(MSG *)
{
    return S_FALSE;
};

void CDeskBand::OnFocus(const BOOL fFocus)
{
    m_fHasFocus = fFocus;
	murmur("Deskband OnFacus:%d",fFocus);

    if (m_pSite)
    {
        m_pSite->OnFocusChangeIS(static_cast<IOleWindow*>(this), m_fHasFocus);
    }
}

void CDeskBand::OnPaint(const HDC hdcIn)
{
    HDC hdc = hdcIn;
    PAINTSTRUCT ps;
    static WCHAR szContent[] = L"OVDeskBand";
    static WCHAR szContentGlass[] = L"OVDeskBand";
	
	POINT pt;
	
    if (!hdc)
    {
        hdc = BeginPaint(m_hwnd, &ps);
    }


    if (hdc)
    {
        RECT rc;
		HBRUSH hbrush, hbrushOld;
		
        GetClientRect(m_hwnd, &rc);
		pt.x = rc.left;
		pt.y  = rc.top;
		ClientToScreen(m_hwnd, &pt);  
	
		murmur("OVDeskband on paint @(%d,%d)", pt.x, pt.y);
		
		hbrush = CreateSolidBrush(0xff0000ff);  //Transparent brush
		hbrushOld = (HBRUSH) SelectObject(hdc, hbrush);

		Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
		
		SelectObject(hdc, hbrushOld);
		DeleteObject(hbrush);
		
		/*		
        SIZE size;


        if (m_fCompositionEnabled)
        {
			
			
            HTHEME hTheme = OpenThemeData(NULL, L"BUTTON");
            if (hTheme)
            {
                HDC hdcPaint = NULL;
                HPAINTBUFFER hBufferedPaint = BeginBufferedPaint(hdc, &rc, BPBF_TOPDOWNDIB, NULL, &hdcPaint);

                DrawThemeParentBackground(m_hwnd, hdcPaint, &rc); 

                GetTextExtentPointW(hdc, szContentGlass, ARRAYSIZE(szContentGlass), &size);
                RECT rcText;
                rcText.left   = (RECTWIDTH(rc) - size.cx) / 2;
                rcText.top    = (RECTHEIGHT(rc) - size.cy) / 2;
                rcText.right  = rcText.left + size.cx;
                rcText.bottom = rcText.top + size.cy;

                DTTOPTS dttOpts = {sizeof(dttOpts)};
                dttOpts.dwFlags = DTT_COMPOSITED | DTT_TEXTCOLOR | DTT_GLOWSIZE;
                dttOpts.crText = RGB(255, 255, 0);
                dttOpts.iGlowSize = 10;
                //DrawThemeTextEx(hTheme, hdcPaint, 0, 0, szContentGlass, -1, 0, &rcText, &dttOpts);

                EndBufferedPaint(hBufferedPaint, TRUE);

                CloseThemeData(hTheme);
            }
			
        }
        else
        {
		
            SetBkColor(hdc, 0xff0000ff); //RGB(255, 255, 0)); 
            GetTextExtentPointW(hdc, szContent, ARRAYSIZE(szContent), &size);
			
            TextOutW(hdc, 
                     (RECTWIDTH(rc) - size.cx) / 2, 
                     (RECTHEIGHT(rc) - size.cy) / 2, 
                     szContent, 
                     ARRAYSIZE(szContent));
			
        } 
		*/
		
    }
	
    if (!hdcIn)
    {
        EndPaint(m_hwnd, &ps);
		
    }
	

	
	UIDockStatusWindow(pt.x, pt.y+1);
	
	
}

LRESULT CALLBACK CDeskBand::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT lResult = 0;
    
    #pragma warning(push)
    #pragma warning(disable:4312)
    CDeskBand *pDeskBand = reinterpret_cast<CDeskBand *>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    #pragma warning(push)


    switch (uMsg)
    {
    case WM_CREATE:
		{
		murmur("OVDeskband:WM_CREATE");
        pDeskBand = reinterpret_cast<CDeskBand *>(reinterpret_cast<CREATESTRUCT *>(lParam)->lpCreateParams);
        pDeskBand->m_hwnd = hwnd;
        
        #pragma warning(push)
        #pragma warning(disable:4244)
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pDeskBand));
        #pragma warning(push)
        
        break;
		}
    case WM_SETFOCUS:
		{
		murmur("OVDeskband:WM_SETFOCUS");
        pDeskBand->OnFocus(TRUE);
        break;
		}

    case WM_KILLFOCUS:
		{
		murmur("OVDeskband:WM_KILLFOCUS");
        pDeskBand->OnFocus(FALSE);
        break;
		}

    case WM_PAINT:
		{
		murmur("OVDeskband:WM_PAINT");
        pDeskBand->OnPaint(NULL);
        break;
		}

    case WM_PRINTCLIENT:
		{
		murmur("OVDeskband:WM_PRINTCLIENT");
        pDeskBand->OnPaint(reinterpret_cast<HDC>(wParam));
        break;
		}

    case WM_ERASEBKGND:
		{
		murmur("OVDeskband:WM_ERASEBKGND"); 
        if (pDeskBand->m_fCompositionEnabled)
        {
            lResult = 1;
        }
        break;
		}
	case WM_WINDOWPOSCHANGING:
		{
		murmur("OVDeskband:WM_WINDOWPOSCHANGING"); 
		// pDeskBand->OnPaint(reinterpret_cast<HDC>(wParam));
        break;
		}
	case WM_WINDOWPOSCHANGED:
		{
		murmur("OVDeskband:WM_WINDOWPOSCHANGED"); 

/*		
		
		if (pDeskBand->m_pSite)
		{
			HRESULT hr;
			IDockingWindowSite *dws;
			hr = pDeskBand->m_pSite->QueryInterface(IID_IDockingWindowSite, reinterpret_cast<void **>(&dws)); 

			if (SUCCEEDED(hr))
			{
				murmur("Got idockingwindowsite interface");
				BORDERWIDTHS pbw;
				pbw.top =0;
				pbw.bottom =0;
				pbw.right =0;
				pbw.left = 0;

				IDockingWindow *dw;

				if(SUCCEEDED(pDeskBand->QueryInterface(IID_IDockingWindow, reinterpret_cast<void **>(&dw))))
				{
					hr = dws->RequestBorderSpaceDW(dw, &pbw);
					if (SUCCEEDED(hr))
						hr = dws->SetBorderSpaceDW(dw, &pbw);
					dw->Release();
				}
				dws->Release();	
			}	
	
			
			pDeskBand->OnPaint(reinterpret_cast<HDC>(wParam));
		}
		*/
			//pDeskBand->OnPaint(reinterpret_cast<HDC>(wParam));
	        break;
		}
	case WM_CHILDACTIVATE:
		{
		murmur("OVDeskband:WM_CHILDACTIVATE"); 
		break;
		}
	case WM_SIZE:
		{
		murmur("OVDeskband:WM_SIZE"); 
		break;
		}
	case WM_SHOWWINDOW:
		{
		murmur("OVDeskband:WM_SHOWINDOW"); 
		break;
		}
	case WM_DESTROY:
		{
		murmur("OVDeskband:WM_DESTROY"); 
		break;
		}
	case WM_NCCREATE:
		{
		murmur("OVDeskband:WM_NCCREATE"); 
		break;

		}
	case WM_NCCALCSIZE:
		{
		murmur("OVDeskband:WM_NCCALCSIZE"); 
		break;
		}
	case WM_STYLECHANGING:
		{
		murmur("OVDeskband:WM_STYLECHANGING"); 
		break;
		}
	case WM_STYLECHANGED:
		{
		murmur("OVDeskband:WM_STYLECHANGED"); 
		break;
		}
	case WM_MOVE:
		{
		murmur("OVDeskband:WM_MOVE"); 
		break;
		}
	case WM_NCDESTROY:
		{
		murmur("OVDeskband:WM_NCDESTROY"); 
		break;
		}
	case WM_NCPAINT:
		{
		murmur("OVDeskband:WM_NCNCPAINT"); 
		break;
		}
	case WM_NCHITTEST:
		{
		murmur("OVDeskband:WM_NCHITTEST"); 
		break;
		}
	case WM_SETCURSOR:
		{
		murmur("OVDeskband:WM_SETCURSOR"); 
		break;
		}
	case WM_MOUSEMOVE:
		{
		murmur("OVDeskband:WM_MOUSEMOVE"); 
		break;
		}
	default:
		{
		murmur("OVDeskband:WndProc:Unknown Message: %x", uMsg);
		break;
		}
    }

    if (uMsg != WM_ERASEBKGND)
    {
        lResult = DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return lResult;
}
