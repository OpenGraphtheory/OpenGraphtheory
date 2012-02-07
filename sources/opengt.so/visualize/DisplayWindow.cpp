

// todo:
// ----------------------
// Arcs
// flood fill

// load image
// save image

// read cell contents

// simpler key callbacks

// more documentation

/*

	---------------------------------------------------------------------------------------------------------------------

	DisplayWindow Class - (C) 2008 Viktor "AlgorithMan" Engelmann
	This sourcecode is provided under the terms of the GNU General Public License v3, see GPL3.txt for details.
	THANKS TO:
		http://tronche.com/gui/x/xlib/
		http://www.pronix.de/pronix-28.html
	FloodFill Method taken from Wine
		http://source.winehq.org/source/graphics/x11drv/graphics.c?v=wine20031118#L1227

	-------------------------------------------------------------------------------------------------------------------

	This Class is intended to provide a simple interface to the X11 system. You can easily create windows and
	draw into them and it can call simple event handlers! You can also create multiple windows!

	This makes this class ideal for displaying calculation results or anything that doesnt need user interaction,
	because you can write your calculations like usual Shell Programs, pass the results to this class, keep on
	calculating and this class does all the rest for you.
	here is some example code:

	#include "DisplayWindow.h"
	int main(int argc, char** argv) {
		DisplayWindow Foo("Title", 640, 480);
		DisplayWindow Bar("Title2", 320, 240);

		Foo.Line(20,20,100,100);
		Foo.Flush(); // you need to flush the graphic buffers so your drawn stuff appears
		Bar.Circle(20,20,10);
		Bar.Flush();

		sleep(60);
		return(0);
	}

	you will notice that even while the program is sleeping (e.g. waiting in a scanf command), you can move the
	windows, cover them, uncover them etc. this is the case because updates are done by a seperate thread

	-------------------------------------------------------------------------------------------------------------------

	here are the signatures for the interface:

	DisplayWindow(string Caption, int Width, int Height);
		Constructor
	~DisplayWindow();
		Destructor
	void CloseWindow();
		Close the window - parts stay in ram until the DisplayWindow instance is destroyed.
	void Flush();
		copies the buffer to the screen - whatever you draw, it is not displayed, until this method is called
	int Width(); int Height();
		returns the width or height of the drawing area... they cant be changed after the construction
		so these methods are kinda pointless, but i included them anyways...
	bool Closed();
		checks if the window is already closed (e.g. because the user clicked the "X" button)




	okay, now the interesting methods:


	void PutPixel(int x, int y);
	void Line(int x1, int y1, int x2, int y2);



	LineStyle = LineSolid, LineOnOffDash, LineDoubleDash.
	CapStyle = CapNotLast, CapButt, CapRound, CapProjecting.
	JoinStyle = JoinMiter, JoinRound, or JoinBevel.
	FillStyle = FillSolid, FillTiled, FillStippled, or FillOpaqueStippled.


	---------------------------------------------------------------------------------------------------------------------

	The Constructor creates a window and a thread that processes the events and updates the "Win" window

	the paint methods (line, rectangle, circle, ...) draw on the PaintBuffer, which is copied to the "WinBackup" Buffer,
		whenever Flush() is called. When Update() is called or the window receives an "expose" event, the "WinBackup"
		Buffer is copied to the actual "Win" Window - this "Double Buffer" method prevents stuff from being displayed
		to early (because of an expose event, instead of a Flush() call - this would lead to messed up images)

	three semaphores (per window) prevent the threads from confusing each other (I hope...)
		the MainSemaphore is used almost everywhere to make almost all methods atomic
		the QuittingSemaphore is locked all the time while the thread is running. when CloseWindow sets "Close" to true,
			the Thread stops and releases the QuittingSemaphore - and the destructor waits for this semaphore, so the
			DisplayWindow instance isnt kicked out of the ram before it cleanly finished
		the UpdateSemaphore allows the window to update, while drawing on the PaintBuffer - it only gets locked while
			the PaintBuffer is copied to the WinBackup buffer or while the PaintBuffer is Copied to WinBackup

	you can close the window by clicking its "X" button or by using the CloseWindow method - your programs will still
		be running, only the output is suppressed - this also means that parts of the Data stay in RAM until the
		instance is destroyed (reaching an end of a block, deleting via a pointer...)

	---------------------------------------------------------------------------------------------------------------------

*/

#include "DisplayWindow.h"

namespace OpenGraphtheory
{
    namespace Visualization
    {
        MouseButton MouseButtonCode[] = {mbLeft, mbMiddle, mbRight};

      	Atom wm_delete_window=None;

        map<Display*, DisplayWindow*> WindowRegister;

        DisplayWindow::DisplayWindow(int Width, int Height, string Caption, string IconPath)
        {
            width = Width;
            height = Height;
            caption = Caption;
            MouseX = -1;
            MouseY = -1;
            MouseButtonStates = 0;
            OnMouseDown = NULL;
            OnMouseUp = NULL;
            OnMouseWheel = NULL;
            OnMouseEnter = NULL;
            OnMouseMove = NULL;
            OnMouseLeave = NULL;
            OnKeyDown = NULL;
            OnKeyUp = NULL;
            OnClose = NULL;
            OnError = NULL;

            // init semaphores
            Close = false;
            sem_init(&MainSemaphore, 0, 1);
            sem_init(&QuittingSemaphore, 0, 1);
            sem_init(&UpdateSemaphore, 0, 1);

            // create window
            XInitThreads();
            XServer = XOpenDisplay (NULL);
            if (XServer == NULL)
                throw "Connection to X-Server failed!";
            WindowRegister[XServer] = this;
            XSetErrorHandler(XServerErrorHandler);

            Screen = XDefaultScreen (XServer);
            cmap = XDefaultColormap (XServer, Screen);
            depth = XDefaultDepth (XServer, Screen);
            RootWindow = RootWindow (XServer, Screen);
            Win = XCreateSimpleWindow ( XServer, RootWindow, 10, 10, width, height, 0, BlackPixel(XServer,Screen),WhitePixel(XServer,Screen));
            static XSizeHints size_hints;
            size_hints.flags = PSize | PMinSize | PMaxSize;
            size_hints.min_width =  width;
            size_hints.max_width =  width;
            size_hints.min_height = height;
            size_hints.max_height = height;
            XSetStandardProperties ( XServer, Win, Caption.c_str(), IconPath.c_str(), None, 0, 0, &size_hints );
            XSelectInput ( XServer, Win, ExposureMask | StructureNotifyMask | ButtonPressMask | ButtonReleaseMask |
                                         KeyPressMask | KeyReleaseMask | EnterWindowMask | LeaveWindowMask |
                                         PointerMotionMask);
            XMapWindow ( XServer, Win );
            WinGraphicContext = XCreateGC(XServer, Win, 0, 0);

            // init buffers
            PaintBuffer = XCreatePixmap(XServer, Win, width, height, depth);
            PaintBufferGraphicContext = XCreateGC(XServer, PaintBuffer, 0, 0);
            WinBackup = XCreatePixmap(XServer, Win, width, height, depth);
            WinBackupGraphicContext = XCreateGC(XServer, WinBackup, 0, 0);
            foregroundcolor = BlackPixel (XServer, Screen);
            backgroundcolor = WhitePixel (XServer, Screen);
            Clear();
            Flush();

            // register close_window event (so the app isnt immediately terminated when the "X" button is clicked)
            if(wm_delete_window == None)
                wm_delete_window = XInternAtom(XServer, "WM_DELETE_WINDOW", True);
            Atom Atoms[] = {wm_delete_window};
            XSetWMProtocols(XServer, Win, Atoms, 1);

            // start thread
            if( pthread_create(&Thread,NULL,RunThread,this) )
                throw "error creating thread\n";
        }


    // -------------------------------------------------------------------------------------


        void* RunThread(void* DisplayWin)
        {
            DisplayWindow *pWin = (DisplayWindow*)(DisplayWin);

            sem_wait(&pWin->QuittingSemaphore);
            timespec SleepTime;
            SleepTime.tv_sec = 0;
            SleepTime.tv_nsec =  10000000; //  1/100 seconds
                            // 1000000000 = 1 second
            while(true)
            {
                pWin->ProcessEvents();
                sem_wait(&pWin->MainSemaphore);
                if(pWin->Close)
                    break;
                sem_post(&pWin->MainSemaphore);
                nanosleep(&SleepTime,NULL);
            }
            sem_post(&pWin->MainSemaphore);
            sem_post(&pWin->QuittingSemaphore);

            pWin->CloseWindow();
            return NULL;
        }


        void DisplayWindow::Flush()
        {
            ProcessEvents();

            sem_wait(&MainSemaphore);
            sem_wait(&UpdateSemaphore);
            if(!Close)
                XCopyArea(XServer, PaintBuffer, WinBackup, WinBackupGraphicContext, 0,0,width,height,0,0);
            sem_post(&UpdateSemaphore);
            sem_post(&MainSemaphore);
            Update();
        }


        void DisplayWindow::Update()
        {
            sem_wait(&UpdateSemaphore);
            if(!Close)
            {
                XCopyArea(XServer, WinBackup, Win, WinGraphicContext, 0,0,width,height,0,0);
                XFlush(XServer);
            }
            sem_post(&UpdateSemaphore);
        }


        void DisplayWindow::ProcessEvents()
        {
            sem_wait(&MainSemaphore);
            static XEvent xev;
            XFlush (XServer);
            for(int num_events = XPending(XServer); num_events > 0; --num_events)
            {
                XNextEvent (XServer, &xev);
                //if(xev.window == Win)
                    ProcessEvent (xev);
            }
            sem_post(&MainSemaphore);
        }

        void DisplayWindow::ProcessEvent (XEvent report)
        {
            switch (report.type)
            {

                // System Events
                case Expose:
                    if (report.xexpose.count <= 0)
                        Update();
                    break;

                case ClientMessage:
                    if((int)report.xclient.data.l[0] == wm_delete_window)
                    {
                        bool AbortClose = false;
                        if(OnClose != NULL)
                            OnClose(AbortClose);
                        if(!AbortClose)
                            CloseWindow();
                    }
                    break;

                // Mouse Events
                case ButtonPress:
                    if(report.xbutton.button <= 3 && report.xbutton.button >= 1)
                    {
                        if(OnMouseDown != NULL)
                            OnMouseDown(report.xbutton.x, report.xbutton.y, MouseButtonCode[report.xbutton.button-1],
                                                                                                MouseButtonStates);
                        MouseButtonStates |= MouseButtonCode[report.xbutton.button-1];
                    }
                    else
                    {
                        if(OnMouseWheel != NULL)
                            OnMouseWheel(report.xbutton.x, report.xbutton.y, (MouseWheel)(report.xbutton.button-3),
                                                                                                MouseButtonStates);
                    }
                    break;

                case ButtonRelease:
                    if(report.xbutton.button <= 3 && report.xbutton.button >= 1)
                    {
                        if(OnMouseUp != NULL)
                            OnMouseUp(report.xbutton.x, report.xbutton.y, MouseButtonCode[report.xbutton.button-1],
                                                                                               MouseButtonStates);
                        MouseButtonStates ^= MouseButtonCode[report.xbutton.button-1];
                    }
                    break;

                case EnterNotify:
                    MouseX = report.xbutton.x;
                    MouseY = report.xbutton.y;
                    if(OnMouseEnter != NULL)
                        OnMouseEnter(MouseX, MouseY, MouseButtonStates);
                    break;

                case LeaveNotify:
                    if(OnMouseLeave != NULL)
                        OnMouseLeave(report.xbutton.x, report.xbutton.y, MouseX, MouseY, MouseButtonStates);
                    MouseX = -1;
                    MouseY = -1;
                    break;

                case MotionNotify:
                    if(OnMouseMove != NULL && MouseX >= 0)
                        OnMouseMove(report.xbutton.x, report.xbutton.y, MouseX, MouseY, MouseButtonStates);
                    MouseX = report.xbutton.x;
                    MouseY = report.xbutton.y;
                    break;

                // Keyboard Events
                case KeyPress:
                    if(OnKeyDown != NULL)
                    {
                        KeySym key = XLookupKeysym(&report.xkey, 0);
                        OnKeyDown(XKeysymToKeycode(XServer, key), DeadKeyStates);
                    }
                    break;

                case KeyRelease:
                    if(OnKeyUp != NULL)
                    {
                        KeySym key = XLookupKeysym (&report.xkey, 0);
                        OnKeyUp(XKeysymToKeycode(XServer, key), DeadKeyStates);
                    }
                    break;
            }
        }


        // -----------------------------------------------------------------------------

        int XServerErrorHandler(Display* XServer, XErrorEvent* error)
        {
            char buffer[256];
            XGetErrorText(XServer, error->error_code, buffer, 256);

            map<Display*, DisplayWindow*>::iterator win = WindowRegister.find(XServer);
            if(win != WindowRegister.end())
            {
                DisplayWindow* Window = win->second;
                void (*Handler)(string ErrorText) = Window->OnError;

                if(Handler != NULL)
                {
                    Handler(string(buffer));
                    return 0;
                }
                else
                    cerr << "X11 Error: " << buffer << endl;
            }
            else
                cerr << "X11 Error: " << buffer << endl;

            return 1;
        }


        DisplayWindow::~DisplayWindow()
        {
            CloseWindow();
            WindowRegister.erase(XServer);

            if( pthread_join(Thread,NULL))
                throw "error joining thread\n";
            sem_destroy(&QuittingSemaphore);
            sem_destroy(&MainSemaphore);
            sem_destroy(&UpdateSemaphore);
            XCloseDisplay(XServer);
        }

        void DisplayWindow::CloseWindow()
        {
            sem_wait(&MainSemaphore);
            if(!Close)
            {
                Close = true;
                XFreeGC(XServer, WinGraphicContext);
                XFreeGC(XServer, WinBackupGraphicContext);
                XFreeGC(XServer, PaintBufferGraphicContext);
                XFreePixmap(XServer, WinBackup);
                XFreePixmap(XServer, PaintBuffer);
                XFreeColormap(XServer, cmap);
                XDestroyWindow(XServer, Win);
                XFlush(XServer);
            }
            sem_post(&MainSemaphore);
        }

        bool DisplayWindow::Closed()
        {
            sem_wait(&MainSemaphore);
            bool result = Close;
            sem_post(&MainSemaphore);
            return result;
        }

        void DisplayWindow::WaitUntilClosed()
        {
            timespec SleepTime;
            SleepTime.tv_sec = 0;
            SleepTime.tv_nsec =  500000000; //  1/20 seconds

            while(!Closed())
                nanosleep(&SleepTime,NULL);
        }

        void DisplayWindow::BringToFront()
        {
            XRaiseWindow(XServer, Win);
        }

        void DisplayWindow::SendToBack()
        {
            XLowerWindow(XServer, Win);
        }

        unsigned long DisplayWindow::GetPixel(int x, int y)
        {
            XImage *ximage = XGetImage(XServer, PaintBuffer, x, y, 1, 1, AllPlanes, ZPixmap);
            unsigned long result = XGetPixel(ximage, x, y);
            XColor color;
            color.pixel = result;
            color.flags = DoRed | DoGreen | DoBlue;
            XQueryColor(XServer, cmap, &color);
            cout << color.red/256 << " " << color.green/256 << " " << color.blue/256 << "\n";
            delete(ximage);
            return result;
        }


        // -----------------------------------------------------------------------------


        void DisplayWindow::SetColor(unsigned long color)
        {
            sem_wait(&MainSemaphore);
            if(!Close)
            {
                foregroundcolor = color;
                XSetForeground(XServer, PaintBufferGraphicContext, foregroundcolor);
            }
            sem_post(&MainSemaphore);
        }

        void DisplayWindow::SetColor(unsigned short R, unsigned short G, unsigned short B)
        {
            static XColor color;
            color.red = R*256;
            color.green = G*256;
            color.blue = B*256;
            if( XAllocColor(XServer, cmap, &color) != 0 )
                SetColor(color.pixel);
            else
                cerr << "error setting color " << R << " " << G << " " << B << "\n";
        }

        void DisplayWindow::SetBGColor(unsigned long color)
        {
            sem_wait(&MainSemaphore);
            if(!Close)
            {
                backgroundcolor = color;
                XSetBackground(XServer, PaintBufferGraphicContext, backgroundcolor);
            }
            sem_post(&MainSemaphore);
        }

        void DisplayWindow::Clear()
        {
            sem_wait(&MainSemaphore);
            if(!Close)
            {
                XSetForeground(XServer,PaintBufferGraphicContext, backgroundcolor);
                XFillRectangle (XServer, PaintBuffer, PaintBufferGraphicContext, 0, 0, width, height);
                XSetForeground(XServer, PaintBufferGraphicContext, foregroundcolor);
            }
            sem_post(&MainSemaphore);
        }



        void DisplayWindow::PutPixel(int x, int y)
        {
            sem_wait(&MainSemaphore);
            if(!Close)
                XDrawPoint(XServer, PaintBuffer, PaintBufferGraphicContext, x,y);
            sem_post(&MainSemaphore);
        }

        void DisplayWindow::Line(int x1, int y1, int x2, int y2, int LineWidth, int LineStyle, int CapStyle)
        {
            sem_wait(&MainSemaphore);
            if(!Close)
            {
                XSetLineAttributes(XServer, PaintBufferGraphicContext, LineWidth, LineStyle, CapStyle, JoinMiter);
                XDrawLine(XServer, PaintBuffer, PaintBufferGraphicContext, x1, y1, x2, y2);
            }
            sem_post(&MainSemaphore);
        }

        void DisplayWindow::Rectangle(int x1, int y1, int x2, int y2, int LineWidth, int LineStyle, int CapStyle)
        {
            sem_wait(&MainSemaphore);
            if(!Close)
            {
                XSetLineAttributes(XServer, PaintBufferGraphicContext, LineWidth, LineStyle, CapStyle, JoinMiter);
                XDrawRectangle(XServer, PaintBuffer, PaintBufferGraphicContext, x1,y1,x2,y2);
            }
            sem_post(&MainSemaphore);
        }

        void DisplayWindow::FillRectangle(int x1, int y1, int x2, int y2, int FillStyle)
        {
            sem_wait(&MainSemaphore);
            if(!Close)
            {
                XSetFillStyle(XServer, PaintBufferGraphicContext, FillStyle);
                XFillRectangle(XServer, PaintBuffer, PaintBufferGraphicContext, x1,y1,x2,y2);
            }
            sem_post(&MainSemaphore);
        }

        void DisplayWindow::Polygon(XPoint *Vertices, int VerticesCount, int LineWidth, int LineStyle, int CapStyle)
        {
            sem_wait(&MainSemaphore);
            if(!Close)
            {
                XSetLineAttributes(XServer, PaintBufferGraphicContext, LineWidth, LineStyle, CapStyle, JoinMiter);
                XDrawLines(XServer, PaintBuffer, PaintBufferGraphicContext, Vertices, VerticesCount, CoordModeOrigin);
                XDrawLine(XServer, PaintBuffer, PaintBufferGraphicContext,
                    Vertices[0].x, Vertices[0].y, Vertices[VerticesCount-1].x, Vertices[VerticesCount-1].y);
            }
            sem_post(&MainSemaphore);
        }

        void DisplayWindow::FillPolygon(XPoint *Vertices, int VerticesCount, int FillStyle)
        {
            sem_wait(&MainSemaphore);
            if(!Close)
            {
                XSetFillStyle(XServer, PaintBufferGraphicContext, FillStyle);
                XFillPolygon(XServer, PaintBuffer, PaintBufferGraphicContext, Vertices, VerticesCount, Complex, CoordModeOrigin);
            }
            sem_post(&MainSemaphore);
        }



        void DisplayWindow::Arc(int x, int y, int w, int h, float StartAngle, float EndAngle, int LineWidth, int LineStyle)
        {
            sem_wait(&MainSemaphore);
            if(!Close)
            {
                XSetLineAttributes(XServer, PaintBufferGraphicContext, LineWidth, LineStyle, CapNotLast, JoinMiter);
                XDrawArc (XServer, PaintBuffer, PaintBufferGraphicContext, x, y, w, h, (int)(StartAngle*64+0.5), (int)(EndAngle*64+0.5));
            }
            sem_post(&MainSemaphore);
        }

        void DisplayWindow::FillArc(int x, int y, int w, int h, float StartAngle, float EndAngle, int FillStyle)
        {
            sem_wait(&MainSemaphore);
            if(!Close)
            {
                XSetFillStyle(XServer, PaintBufferGraphicContext, FillStyle);
                XFillArc (XServer, PaintBuffer, PaintBufferGraphicContext, x, y, w, h, (int)(StartAngle*64+0.5), (int)(EndAngle*64+0.5));
            }
            sem_post(&MainSemaphore);
        }

        float sign(float x)
        {
            return x > 0 ? 1 : -1;
        }

        void DisplayWindow::Arc(int x1, int y1, int x2, int y2, float Offset, int LineWidth, int LineStyle)
        {
            if((y2 > y1 && Offset < 0) || (y2 < y1 && Offset > 0))
            {
                Arc(x2,y2,x1,y1,Offset,LineWidth,LineStyle);
                return;
            }


            sem_wait(&MainSemaphore);
            if(!Close)
            {

                Vector2D X1(x1, y1);
                Vector2D X2(x2, y2);
                Vector2D X3 = (X2-X1);
                X3 = ((X1+X2)/2.0) + X3.Perpendicular().Normalized() * ( Offset * X3.Length() / 2.0);
                Vector2D X4 = (X3+X2)/2.0 + (X3-X2).Perpendicular();
                Vector2D M = Vector2D::Intersection(X3,(X1+X2)/2.0,(X2+X3)/2.0,X4);

                int d = Vector2D::Distance(M,X2);
                int x = M.x - d;
                int y = M.y - d;
                int w = 2*d;
                int h = 2*d;

                float StartAngle, EndAngle;
                StartAngle = (M-X1).Theta();
                EndAngle = StartAngle - (M-X2).Theta();
                StartAngle = 180 - StartAngle; // translate to mirrored coordinate system

    /*
                if( Offset > 0 ) {
                    StartAngle = (M-X1).Theta();
                    EndAngle = StartAngle-(M-X2).Theta();
                    StartAngle = 180 - StartAngle; // translate to mirrored coordinate system
                } else {
                    StartAngle = (M-X2).Theta();
                    EndAngle = StartAngle-(M-X1).Theta();
                    StartAngle = 180 - StartAngle; // translate to mirrored coordinate system
                }
    */
    /*
                float x1angle = (M-X1).Theta();
                float x2angle = (M-X2).Theta();
                StartAngle = max(x1angle, x2angle);
                EndAngle = max(x1angle,x2angle)-StartAngle;
    */

                XSetLineAttributes(XServer, PaintBufferGraphicContext, LineWidth, LineStyle, CapNotLast, JoinMiter);
                XDrawArc (XServer, PaintBuffer, PaintBufferGraphicContext, x, y, w, h, (int)(StartAngle*64), (int)(EndAngle*64));
            }
            sem_post(&MainSemaphore);
        }

        void DisplayWindow::Ellipse(int CenterX, int CenterY, int width, int height, int LineWidth, int LineStyle)
        {
            sem_wait(&MainSemaphore);
            if(!Close)
            {
                XSetLineAttributes(XServer, PaintBufferGraphicContext, LineWidth, LineStyle, CapNotLast, JoinMiter);
                XDrawArc(XServer,PaintBuffer,PaintBufferGraphicContext,
                    CenterX-width/2,CenterY-height/2,CenterX+width/2,CenterY+height/2,0,360*64);
            }
            sem_post(&MainSemaphore);
        }

        void DisplayWindow::FillEllipse(int CenterX, int CenterY, int width, int height, int FillStyle)
        {
            sem_wait(&MainSemaphore);
            if(!Close)
            {
                XSetFillStyle(XServer, PaintBufferGraphicContext, FillStyle);
                XFillArc(XServer,PaintBuffer,PaintBufferGraphicContext,
                    CenterX-width/2,CenterY-height/2,CenterX+width/2,CenterY+height/2,0,360*64);
            }
            sem_post(&MainSemaphore);
        }

        void DisplayWindow::Circle(int CenterX, int CenterY, int Radius, int LineWidth, int LineStyle)
        {
            sem_wait(&MainSemaphore);
            if(!Close)
            {
                XSetLineAttributes(XServer, PaintBufferGraphicContext, LineWidth, LineStyle, CapNotLast, JoinMiter);
                XDrawArc (XServer, PaintBuffer, PaintBufferGraphicContext, CenterX-Radius, CenterY-Radius, 2*Radius, 2*Radius, 0, 360 * 64);
            }
            sem_post(&MainSemaphore);
        }

        void DisplayWindow::FillCircle(int CenterX, int CenterY, int Radius, int FillStyle)
        {
            sem_wait(&MainSemaphore);
            if(!Close)
            {
                XSetFillStyle(XServer, PaintBufferGraphicContext, FillStyle);
                XFillArc (XServer, PaintBuffer, PaintBufferGraphicContext, CenterX-Radius, CenterY-Radius, 2*Radius, 2*Radius, 0, 360 * 64);
            }
            sem_post(&MainSemaphore);
        }



        void DisplayWindow::SetFont(string FontName)
        {
            sem_wait(&MainSemaphore);
            if(!Close)
                XSetFont (XServer, PaintBufferGraphicContext, XLoadFont (XServer, FontName.c_str()));
            sem_post(&MainSemaphore);
        }

        void DisplayWindow::WriteText(int x, int y, string Text, bool SolidBackground)
        {
            sem_wait(&MainSemaphore);
            if(!Close)
            {
                if(SolidBackground)
                    XDrawString (XServer, PaintBuffer, PaintBufferGraphicContext, x, y, Text.c_str(), Text.length());
                else
                    XDrawImageString (XServer, PaintBuffer, PaintBufferGraphicContext, x, y, Text.c_str(), Text.length());
            }
            sem_post(&MainSemaphore);
        }

        void DisplayWindow::SaveBitmap(string FileName)
        {
            sem_wait(&UpdateSemaphore);
            if(!Close)
                XWriteBitmapFile(XServer, FileName.c_str(), WinBackup, width, height, -1, -1);
                //XCreateBitmapFromData(XServer, WinBackup, data, width, height)
            sem_post(&UpdateSemaphore);
        }

        void DisplayWindow::LoadPixmap(int x, int y, string FileName)
        {
            sem_wait(&MainSemaphore);
            if(!Close)
            {
                Pixmap temp;
                unsigned int h,w;
                int hx,hy;
                int status = XReadBitmapFile(XServer, RootWindow, FileName.c_str(), &w, &h, &temp, &hx, &hy);
                if(status == BitmapSuccess)
                    XCopyArea(XServer, temp, PaintBuffer, PaintBufferGraphicContext, 0,0,w,h,x,y);
            }
            sem_post(&MainSemaphore);
            Flush();
        }



    // ---------------------------------------------------------------------------------


        /**********************************************************************
         *          X11DRV_InternalFloodFill
         *
         * Internal helper function for flood fill.
         * (xorg,yorg) is the origin of the X image relative to the drawable.
         * (x,y) is relative to the origin of the X image.
         */
        /*
        static void X11DRV_InternalFloodFill(XImage *image, X11DRV_PDEVICE *physDev,
                                             int x, int y, int xOrg, int yOrg,
                                             Pixel pixel, WORD fillType ) {
            int left, right;

            #define TO_FLOOD(x,y)  ((fillType == FLOODFILLBORDER) ? \
                                    (XGetPixel(image,x,y) != pixel) : \
                                    (XGetPixel(image,x,y) == pixel))

            if (!TO_FLOOD(x,y)) return;

            // Find left and right boundaries

            left = right = x;
            while ((left > 0) && TO_FLOOD( left-1, y ))
                left--;
            while ((right < image->width) && TO_FLOOD( right, y ))
                right++;
            XFillRectangle( gdi_display, physDev->drawable, physDev->gc,
                            xOrg + left, yOrg + y, right-left, 1 );

            // Set the pixels of this line so we don't fill it again

            for (x = left; x < right; x++) {
                if (fillType == FLOODFILLBORDER)
                    XPutPixel( image, x, y, pixel );
                else
                    XPutPixel( image, x, y, ~pixel );
            }

            // Fill the line above

            if (--y >= 0) {
                x = left;
                while (x < right) {
                    while ((x < right) && !TO_FLOOD(x,y))
                        x++;
                    if (x >= right)
                        break;
                    while ((x < right) && TO_FLOOD(x,y))
                        x++;
                    X11DRV_InternalFloodFill(image, physDev, x-1, y,
                                             xOrg, yOrg, pixel, fillType );
                }
            }

            // Fill the line below

            if ((y += 2) < image->height) {
                x = left;
                while (x < right) {
                    while ((x < right) && !TO_FLOOD(x,y))
                        x++;
                    if (x >= right)
                        break;
                    while ((x < right) && TO_FLOOD(x,y))
                        x++;
                    X11DRV_InternalFloodFill(image, physDev, x-1, y,
                                             xOrg, yOrg, pixel, fillType );
                }
            }
        #undef TO_FLOOD
        }
        */

    }
}
