
#include <iostream>
#include <map>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <semaphore.h>
#include <pthread.h>

#include "vector2d.h"
#include "color.h"

namespace OpenGraphtheory
{
    namespace Visualization
    {

        enum MouseButton {mbLeft=1, mbMiddle=2, mbRight=4};
        enum MouseWheel {mwUp=1, mwDown=2};
        enum DeadKey {kbShift=1, kbAlt=2, kbCtrl=4 };

        void* RunThread(void* WindowHandler);
        int XServerErrorHandler(Display* XServer, XErrorEvent* error);


        class DisplayWindow
        {
            friend void* RunThread(void* WindowHandler);

            protected:

                int width; // general window attributes
                int height;
                int depth;
                int MouseX;
                int MouseY;
                unsigned short MouseButtonStates;
                unsigned short DeadKeyStates;
                std::string caption;
                unsigned long foregroundcolor;
                unsigned long backgroundcolor;
                Colormap cmap;

                pthread_t Thread; // communication with main thread
                sem_t MainSemaphore;
                sem_t QuittingSemaphore;
                sem_t UpdateSemaphore;
                bool Close;

                Display *XServer; // communication with X-Server
                int Screen;
                Window RootWindow;
                Window Win;
                Pixmap PaintBuffer;
                Pixmap WinBackup;
                GC WinGraphicContext;
                GC WinBackupGraphicContext;
                GC PaintBufferGraphicContext;

                void ProcessEvent(XEvent report);
                void ProcessEvents();
                void Update();

                void SetColor(unsigned long color);
                void SetBGColor(unsigned long color);

            public:

                DisplayWindow(int Width, int Height, std::string Caption="DisplayWindow", std::string IconPath="NONE");
                ~DisplayWindow();
                void CloseWindow();
                void Flush();
                int Width() {return width;}
                int Height() {return height;}
                void BringToFront();
                void SendToBack();
                unsigned long GetPixel(int x, int y);
                bool Closed();
                void WaitUntilClosed();

                void SetColor(unsigned short R, unsigned short G, unsigned short B);
                void SetBGColor(unsigned short R, unsigned short G, unsigned short B);
                void Clear();

                void PutPixel(int x, int y);
                void Line(int x1, int y1, int x2, int y2, int LineWidth=1, int LineStyle=LineSolid, int CapStyle=CapNotLast);
                void Rectangle(int x1, int y1, int x2, int y2, int LineWidth=1, int LineStyle=LineSolid, int CapStyle=CapNotLast);
                void FillRectangle(int x1, int y1, int x2, int y2, int FillStyle=FillSolid);
                void Polygon(XPoint *Vertices, int VerticesLength, int LineWidth=1, int LineStyle=LineSolid, int CapStyle=CapNotLast);
                void FillPolygon(XPoint *Vertices, int VerticesCount, int FillStyle=FillSolid);

                void Arc(int x, int y, int w, int h, float StartAngle, float EndAngle, int LineWidth=1, int LineStyle=LineSolid);
                void FillArc(int x, int y, int w, int h, float StartAngle, float EndAngle, int FillStyle=FillSolid);
                void Arc(int x1, int y1, int x2, int y2, float offset, int LineWidth=1, int LineStyle=LineSolid);
                void Ellipse(int CenterX, int CenterY, int width, int height, int LineWidth=1, int LineStyle=LineSolid);
                void FillEllipse(int CenterX, int CenterY, int width, int height, int FillStyle=FillSolid);
                void Circle(int CenterX, int CenterY, int Radius, int LineWidth=1, int LineStyle=LineSolid);
                void FillCircle(int CenterX, int CenterY, int Radius, int FillStyle=FillSolid);

                void SetFont(std::string FontName);
                void WriteText(int x, int y, std::string Text, bool SolidBackground=true);

                void SaveBitmap(std::string FileName);
                void LoadPixmap(int x, int y, std::string FileName);

                // event handlers
                void (*OnMouseDown) (int X, int Y, MouseButton Button,   unsigned short ButtonStates);
                void (*OnMouseUp)   (int X, int Y, MouseButton Button,   unsigned short ButtonStates);
                void (*OnMouseWheel)(int X, int Y, MouseWheel Direction, unsigned short ButtonStates);
                void (*OnMouseEnter)(int X, int Y,                       unsigned short ButtonStates);
                void (*OnMouseMove) (int X, int Y, int fromX, int fromY, unsigned short ButtonStates);
                void (*OnMouseLeave)(int X, int Y, int fromX, int fromY, unsigned short ButtonStates);

                void (*OnKeyDown)(int KeyCode, unsigned short DeadKeyStates);
                void (*OnKeyUp)  (int KeyCode, unsigned short DeadKeyStates);
                void (*OnClose)(bool& AbortClosing);

                void (*OnError)(std::string ErrorText);

        };


    }
}
