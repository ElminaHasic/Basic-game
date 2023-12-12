#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include<ctime>
#include <iostream>

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

#define TimerLeptir 1
#define TimerKugla 2
#define pritisnuto(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1: 0)

struct Object
{
    double x;
    double y;
    double w;
    double h;
    double dx;
    double dy;
    int counterX =0, counterY= 0;
    int counterXr =0, counterYr= 0;
    int counterXRI =0, counterYRI= 4;
    int counterXLI =0, counterYLI= 5;
    int counterXRS =0, counterYRS= 1;
    int counterXLS =0, counterYLS= 3;

};

Object bkg;
Object leptir;
Object loptica;
Object lik;
Object gameOver;

HBITMAP background, likB, likC, leptirLB, leptirLC, leptirDC, leptirDB, lopticaB, lopticaC;
HBITMAP gameOverbm;

static bool LeptirDesnoLeti = true, checkTimerLeptir = false, checkTimerKugla = false;
static bool KuglaIdeLijevo = false, crtajKuglu = false;
static bool liklijevo = false, likdesno = false, RightIdle = false, LeftIdle = false;
static bool FirstTime = true, crtaj = true, space = false, kuglaKretanje = true, provjera = false;

void draw(HWND hwnd);
void  Karakter(HWND hwnd);
void initialize();
void drawLeptir(HWND hwnd);
void Kugla(HWND hwnd,DWORD pocetak);
void Input();

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS |CS_HREDRAW | CS_VREDRAW;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
               0,                   /* Extended possibilites for variation */
               szClassName,         /* Classname */
               _T("Zadaca3"),       /* Title Text */
               WS_OVERLAPPEDWINDOW, /* default window */
               350,       /* Windows decides the position */
               150,       /* where the window ends up on the screen */
               1100,                 /* The programs width */
               600,                 /* and height in pixels */
               HWND_DESKTOP,        /* The window is a child-window to desktop */
               NULL,                /* No menu */
               hThisInstance,       /* Program Instance handler */
               NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    initialize();
    DWORD pocetak = GetTickCount();
    while (true)
    {
        DWORD vrijeme_pocetka = GetTickCount();
        if(PeekMessage(&messages, hwnd, 0,0, PM_REMOVE))
        {
            if(messages.message == WM_QUIT)
                break;
            TranslateMessage(&messages);
            DispatchMessage(&messages);
        }

        Input();
        draw(hwnd);
        Karakter(hwnd);
        drawLeptir(hwnd);

        if(kuglaKretanje)
            Kugla(hwnd, pocetak);


        while(GetTickCount() - vrijeme_pocetka < 20)
        {
            Sleep(5);
        }


    }


    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {

    case WM_TIMER:
    {
        switch(wParam)
        {
        case TimerLeptir:
        {
            checkTimerLeptir = true;
            LeptirDesnoLeti = !LeptirDesnoLeti;

        }
        break; // leptir timer

        case TimerKugla:
        {

            crtajKuglu = true;

            srand(time(0));
            static int smjerKugle = rand() % 100;
            checkTimerKugla = true;

            if(smjerKugle < 50)
            {
                KuglaIdeLijevo = true;
                loptica.x = 1999;
            }
            else
            {
                KuglaIdeLijevo = false;
                loptica.x = 0;
            }

            smjerKugle = rand() % 100;

        }
        break; //Timer Kugla

        }
    }
    break;


    case WM_KEYUP:
    {
        switch(wParam)
        {
        case VK_LEFT:
        {
            LeftIdle = true;
        }
        break;

        case VK_RIGHT:
        {
            RightIdle = true;
        }
        break;

        case VK_SPACE:
        {
            space = false;
            if(likdesno)
                RightIdle = true;
            if(liklijevo)
                LeftIdle = true;

            if(!((lik.x > 437 && lik.x < 665) && lik.y <=147))
            {
                lik.y = 260;
            }
            else
            {
                lik.y = 147;
            }

        }
        break;

        case 68:
        {
            RightIdle = true;
        }
        break;

        case 65:
        {
            LeftIdle = true;
        }
        break;

        }
    }
    break;

    case WM_LBUTTONDOWN:
    {
        crtaj = true;
        kuglaKretanje = true;

    }
    break;
    case WM_RBUTTONDOWN:
    {
        crtaj = true;
        kuglaKretanje = true;


    }
    break;



    case WM_MBUTTONDOWN:
    {

        crtaj = true;
        kuglaKretanje = true;

    }
    break;

    case WM_DESTROY:
        PostQuitMessage (0);
        break;
    default:
        return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

void initialize()
{
    background = (HBITMAP) LoadImage (NULL, "bkg.bmp", IMAGE_BITMAP, 0,0, LR_LOADFROMFILE);
    BITMAP infobkg, infolik, infoleptir, infolopta, infoGameOver;
    GetObject(background, sizeof(BITMAP), &infobkg);
    bkg.w = infobkg.bmWidth;
    bkg.h = infobkg.bmHeight;

    //Leptir
    leptirLB = (HBITMAP) LoadImage(NULL, "leptirLB.bmp", IMAGE_BITMAP, 200,200,LR_LOADFROMFILE);
    leptirLC = (HBITMAP) LoadImage(NULL, "leptirLC.bmp", IMAGE_BITMAP, 200,200,LR_LOADFROMFILE);
    leptirDB = (HBITMAP) LoadImage(NULL, "leptirDB.bmp", IMAGE_BITMAP, 200,200,LR_LOADFROMFILE);
    leptirDC= (HBITMAP) LoadImage(NULL, "leptirDC.bmp", IMAGE_BITMAP, 200,200,LR_LOADFROMFILE);

    GetObject(leptirDB, sizeof(BITMAP), &infoleptir);
    leptir.w = infoleptir.bmWidth;
    leptir.h = infoleptir.bmHeight;
    leptir.counterX = 0;
    leptir.counterY =0;
    leptir.dx = 3;
    leptir.dy = 0.5;
    leptir.x=0;
    leptir.y = 0;

    //Lik
    likB = (HBITMAP) LoadImage (NULL, "likWhite.bmp", IMAGE_BITMAP, 0,0, LR_LOADFROMFILE);
    likC = (HBITMAP) LoadImage (NULL, "likBlack.bmp", IMAGE_BITMAP, 0,0, LR_LOADFROMFILE);
    GetObject(likB, sizeof(BITMAP), &infolik);
    lik.w = infolik.bmWidth;
    lik.h = infolik.bmHeight;
    lik.dx = 3;
    lik.dy = 4;
    lik.y = 260;
    lik.x = 1100/2;

    // Kugla
    lopticaB = (HBITMAP) LoadImage(NULL, "lopticaB.bmp", IMAGE_BITMAP, 300,70, LR_LOADFROMFILE);
    lopticaC = (HBITMAP) LoadImage(NULL, "lopticaC.bmp", IMAGE_BITMAP, 300,70, LR_LOADFROMFILE);

    GetObject(lopticaB, sizeof(BITMAP), &infolopta);
    loptica.w = infolopta.bmWidth;
    loptica.h = infolopta.bmHeight;
    loptica.dx = 1.5 * lik.dx;
    loptica.counterX = 0;
    loptica.x =0;
    loptica.y = lik.y + lik.h/12 - 25 ;

    //Game over
    gameOverbm = (HBITMAP) LoadImage(NULL, "GameOver.bmp", IMAGE_BITMAP, 0,0, LR_LOADFROMFILE);
    GetObject(gameOverbm, sizeof(BITMAP), &infoGameOver);
    gameOver.w = infoGameOver.bmWidth;
    gameOver.h = infoGameOver.bmHeight;

}

void Input()
{

    if(pritisnuto(VK_RIGHT) || pritisnuto(68))
    {
        likdesno = true;
        liklijevo = false;
        RightIdle = false;
        LeftIdle = false;
        FirstTime = false;
        space = false;
        if(lik.x<1037.5)
            lik.x+=lik.dx;

        if(!((lik.x > 437 && lik.x < 665) && lik.y <=147))
        {
            lik.y = 260;
        }

    }
    if(pritisnuto(VK_LEFT) || pritisnuto(65))
    {
        likdesno = false;
        liklijevo = true;
        RightIdle = false;
        LeftIdle = false;
        FirstTime = false;
        space = false;
        if(lik.x > -20)
            lik.x-=lik.dx;

        if(!((lik.x > 437 && lik.x < 665) && lik.y <=147))
        {
            lik.y = 260;
        }
    }
    if(pritisnuto(VK_SPACE))
    {
        if (lik.y > -4)
            lik.y -=lik.dy;
        space = true;
        RightIdle = false;
        LeftIdle = false;
        if(FirstTime)
        {
            likdesno = true;
            FirstTime = false;
        }
        if(likdesno)
        {
            if(lik.x< 1037.5)
                lik.x+=1;
        }
        if(liklijevo)
        {
            if(lik.x > -20)
                lik.x -=1;
        }
    }

}


void draw(HWND hwnd)
{

    HDC hdc = GetDC(hwnd);
    RECT rect;
    GetClientRect(hwnd, &rect);
    HDC hdcBuffer = CreateCompatibleDC(hdc);
    HDC hdcMem = CreateCompatibleDC(hdc);

    HBITMAP bmpBuffer = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
    HBITMAP oldBuffer = (HBITMAP) SelectObject(hdcBuffer, bmpBuffer);

    HBITMAP bmpMem = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
    HBITMAP oldMem = (HBITMAP) SelectObject(hdcMem, bmpMem);

    SelectObject(hdcBuffer, background);
    BitBlt(hdcMem, 0,0, bkg.w, bkg.h, hdcBuffer, 0,0, SRCCOPY);


    if(loptica.x == 0 && !KuglaIdeLijevo)
    {
        if(lik.y <= 147)
            loptica.y = 154 ;
        else
            loptica.y = 267;
    }
    if(KuglaIdeLijevo && loptica.x == 1040.5)
    {
        if(lik.y <= 147)
            loptica.y = 154 ;
        else
            loptica.y = 267;
    }



    //Leptir
    if(LeptirDesnoLeti)
    {
        SelectObject(hdcBuffer, leptirDB);
        BitBlt(hdcMem, leptir.x,leptir.y, (leptir.w /5),  (leptir.h /2), hdcBuffer, leptir.counterX *(leptir.w /5), leptir.counterY* (leptir.h /2), SRCAND );
        SelectObject(hdcBuffer, leptirDC);
        BitBlt(hdcMem, leptir.x,leptir.y, (leptir.w /5),  (leptir.h /2), hdcBuffer, leptir.counterX *(leptir.w /5), leptir.counterY* (leptir.h /2), SRCPAINT );
    }
    else
    {
        SelectObject(hdcBuffer, leptirLB);
        BitBlt(hdcMem, leptir.x,leptir.y, (leptir.w /5),  (leptir.h /2), hdcBuffer, leptir.counterX *(leptir.w /5), leptir.counterY* (leptir.h /2), SRCAND );
        SelectObject(hdcBuffer, leptirLC);
        BitBlt(hdcMem, leptir.x,leptir.y, (leptir.w /5),  (leptir.h /2), hdcBuffer, leptir.counterX *(leptir.w /5), leptir.counterY* (leptir.h /2), SRCPAINT );
    }


    if(((!KuglaIdeLijevo && (loptica.x <= lik.x + lik.w /8 - 30 && loptica.x >= lik.x) ) &&  ( ( lik.y -20 >= (loptica.y -loptica.h +10))    &&  ( (lik.y - lik.h /6) <= (loptica.y -loptica.h +10)))      ) || ((KuglaIdeLijevo && (loptica.x <= lik.x + lik.w /8 - 30 && loptica.x >= lik.x))  && ( ( lik.y - 20>= (loptica.y -loptica.h +10))    &&  ( (lik.y - lik.h /6) <= (loptica.y -loptica.h +10)))  ) )
    {

        crtaj = false;
        lik.x = 1100/2;
        lik.y = 260;
        int x = rand()% 100;
        if(x < 50)
        {
            KuglaIdeLijevo = true;
            loptica.x = 1999;
        }
        else
        {
            KuglaIdeLijevo = false;
            loptica.x = 0;
        }
        if(checkTimerKugla && provjera)
            KillTimer(hwnd, TimerKugla);

        kuglaKretanje = false;
        provjera = false;

        HDC memDC2 = CreateCompatibleDC(hdc);
        HBITMAP oldGameOver = (HBITMAP) SelectObject(memDC2, gameOverbm);
        StretchBlt(hdc, 0,0, rect.right, rect.bottom, memDC2, 0,0, gameOver.w, gameOver.h, SRCCOPY);

        SelectObject(memDC2, oldGameOver);
        DeleteDC(memDC2);
        DeleteObject(oldGameOver);


    }
    else
    {
        if(crtaj)
        {
            provjera = true;

            //kugla

            if(crtajKuglu)
            {
                SelectObject(hdcBuffer, lopticaB);
                BitBlt(hdcMem,loptica.x, loptica.y, (loptica.w /8),  loptica.h, hdcBuffer, loptica.counterX*(loptica.w /8)+3, 0, SRCAND );
                SelectObject(hdcBuffer, lopticaC);
                BitBlt(hdcMem, loptica.x, loptica.y, (loptica.w /8),  loptica.h, hdcBuffer, loptica.counterX*(loptica.w /8)+3,0, SRCPAINT );
            }

            //Lik
            SelectObject(hdcBuffer, likB);

            if(RightIdle)
            {

                BitBlt(hdcMem, lik.x, lik.y, lik.w / 8, lik.h /6, hdcBuffer, lik.counterXRI* (lik.w /8), lik.counterYRI * (lik.h /6), SRCAND );
                SelectObject(hdcBuffer, likC);
                BitBlt(hdcMem, lik.x, lik.y, lik.w / 8, lik.h /6, hdcBuffer, lik.counterXRI* (lik.w /8), lik.counterYRI * (lik.h /6), SRCPAINT );


            }
            else if(LeftIdle)
            {

                BitBlt(hdcMem, lik.x, lik.y, lik.w / 8, lik.h /6, hdcBuffer, lik.counterXLI* (lik.w /8), lik.counterYLI * (lik.h /6), SRCAND );
                SelectObject(hdcBuffer, likC);
                BitBlt(hdcMem, lik.x, lik.y, lik.w / 8, lik.h /6, hdcBuffer, lik.counterXLI* (lik.w /8), lik.counterYLI * (lik.h /6), SRCPAINT );

            }
            else
            {
                if(likdesno)
                {

                    if(space)
                    {

                        BitBlt(hdcMem, lik.x, lik.y, lik.w / 8, lik.h /6, hdcBuffer, lik.counterXRS* (lik.w /8), lik.counterYRS * (lik.h /6), SRCAND );
                        SelectObject(hdcBuffer, likC);
                        BitBlt(hdcMem, lik.x, lik.y, lik.w / 8, lik.h /6, hdcBuffer, lik.counterXRS* (lik.w /8), lik.counterYRS * (lik.h /6), SRCPAINT );

                    }
                    else
                    {

                        BitBlt(hdcMem, lik.x, lik.y, lik.w / 8, lik.h /6, hdcBuffer, lik.counterXr* (lik.w /8), lik.counterYr * (lik.h /6), SRCAND );
                        SelectObject(hdcBuffer, likC);
                        BitBlt(hdcMem, lik.x, lik.y, lik.w / 8, lik.h /6, hdcBuffer, lik.counterXr* (lik.w /8), lik.counterYr * (lik.h /6), SRCPAINT );
                    }
                }
                if(liklijevo)
                {
                    if(space)
                    {

                        BitBlt(hdcMem, lik.x, lik.y, lik.w / 8, lik.h /6, hdcBuffer, lik.counterXLS* (lik.w /8), lik.counterYLS * (lik.h /6), SRCAND );
                        SelectObject(hdcBuffer, likC);
                        BitBlt(hdcMem, lik.x, lik.y, lik.w / 8, lik.h /6, hdcBuffer, lik.counterXLS* (lik.w /8), lik.counterYLS * (lik.h /6), SRCPAINT );

                    }
                    else
                    {

                        BitBlt(hdcMem, lik.x, lik.y, lik.w / 8, lik.h /6, hdcBuffer, lik.counterX* (lik.w /8), lik.counterY * (lik.h /6), SRCAND );
                        SelectObject(hdcBuffer, likC);
                        BitBlt(hdcMem, lik.x, lik.y, lik.w / 8, lik.h /6, hdcBuffer, lik.counterX* (lik.w /8), lik.counterY * (lik.h /6), SRCPAINT );
                    }
                }
            }

            SelectObject(hdcBuffer, likB);
            if(FirstTime)
            {
                BitBlt(hdcMem, lik.x, lik.y, lik.w / 8, lik.h /6, hdcBuffer, lik.counterXRI* (lik.w /8), lik.counterYRI * (lik.h /6), SRCAND );
                SelectObject(hdcBuffer, likC);
                BitBlt(hdcMem, lik.x, lik.y, lik.w / 8, lik.h /6, hdcBuffer, lik.counterXRI* (lik.w /8), lik.counterYRI * (lik.h /6), SRCPAINT );

            }

            StretchBlt(hdc, 0,0, rect.right, rect.bottom, hdcMem, 0,0, bkg.w, bkg.h, SRCCOPY);
        }
    }


    SelectObject(hdcBuffer, oldBuffer);
    DeleteDC(hdcBuffer);

    SelectObject(hdcMem, oldMem);
    DeleteDC(hdcMem);

    DeleteObject(bmpBuffer);
    DeleteObject(bmpMem);
}


void  Karakter(HWND hwnd)
{

    if(RightIdle)
    {
        lik.counterXRI+=1;
        if(lik.counterXRI == 2)
            lik.counterXRI = 0;

    }
    else if(LeftIdle)
    {
        lik.counterXLI+=1;
        if(lik.counterXLI == 2)
            lik.counterXLI = 0;

    }
    else
    {
        if(likdesno)
        {
            if(space)
            {
                lik.counterXRS+=1;
                if(lik.counterXRS == 3)
                    lik.counterXRS = 0;
            }
            else
            {
                lik.counterXr+=1;
                if(lik.counterXr == 8)
                    lik.counterXr = 0;
            }
        }
        if(liklijevo)
        {
            if(space)
            {
                lik.counterXLS+=1;
                if(lik.counterXLS == 3)
                    lik.counterXLS = 0;
            }
            else
            {
                lik.counterY = 2;
                lik.counterX +=1;
                if(lik.counterX ==8)
                    lik.counterX = 0;
            }
        }

    }
}

void drawLeptir(HWND hwnd)
{

    ++leptir.counterX;
    if(leptir.counterX == 5)
    {
        leptir.counterX = 0;
        leptir.counterY+=1;
        if(leptir.counterY == 2)
            leptir.counterY = 0;
    }

    if(checkTimerLeptir)
    {
        KillTimer(hwnd, TimerLeptir);
        checkTimerLeptir = false;
    }
//desno
    if(LeptirDesnoLeti)
    {
        if(leptir.x ==1101)
        {

            leptir.x = 1097;
            leptir.y = 0;
            SetTimer(hwnd, TimerLeptir, 7000, NULL);

        }
        else
        {
            if(leptir.x < 1100)
            {
                leptir.x +=leptir.dx;
                if(leptir.x <= 1100/2 - leptir.w / 5  &&  leptir.y <= 600 /4 - leptir.h / 2 )
                    leptir.y += leptir.dy;
                else
                    leptir.y -= leptir.dy;
            }
        }
    }
    else  //leptir leti lijevo
    {
        if(leptir.x == -40)
        {
            leptir.x = 0;
            leptir.y = 0;
            SetTimer(hwnd, TimerLeptir, 7000, NULL);

        }
        else
        {
            if(leptir.x > -40)
            {
                leptir.x -=leptir.dx;
                if(leptir.x >= 1100/2 - leptir.w / 5  &&  leptir.y <= 600 /4 - leptir.h / 2 )
                    leptir.y += leptir.dy;
                else
                    leptir.y -= leptir.dy;
            }
        }
    }
}


void Kugla(HWND hwnd, DWORD pocetak)
{
    if(GetTickCount() - pocetak >=5000)
    {
        crtajKuglu = true;
        loptica.counterX +=1;
        if(loptica.counterX == 8)
            loptica.counterX = 0;

        if(KuglaIdeLijevo)
        {
            loptica.x-=loptica.dx;
        }
        else
        {
            loptica.x+=loptica.dx;
        }

        if(loptica.x == 1102.5 || loptica.x == -8)
        {
            SetTimer(hwnd, TimerKugla, 5000, NULL);
        }
        if(checkTimerKugla)
        {
            KillTimer(hwnd, TimerKugla);
            checkTimerKugla = false;
        }
    }
}


