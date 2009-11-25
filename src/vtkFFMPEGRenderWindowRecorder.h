#ifndef __VTKFFMPEGRENDERWINDOWRECORDER_H
#define __VTKFFMPEGRENDERWINDOWRECORDER_H

#include <iostream>
#include <string>

#include "vtkObject.h"

class vtkRenderWindow;
class vtkWindowToImageFilter;
class vtkFFMPEGWriter;

class vtkFFMPEGRenderWindowRecorder : public vtkObject
{
   public:
     static vtkFFMPEGRenderWindowRecorder *New();

     vtkSetMacro( m_VideoQuality, int );
     vtkGetMacro( m_VideoQuality, int );

     vtkSetMacro( m_FrameRate, int );
     vtkGetMacro( m_FrameRate, int );

     void SetFileName( const std::string& );
     void SetRenderWindow( vtkRenderWindow* );
     void StartCapture();
     void EndCapture();
     void TakeSnapshot( );

   protected:
     vtkFFMPEGRenderWindowRecorder();
     ~vtkFFMPEGRenderWindowRecorder();

     vtkRenderWindow*        m_RenderWindow;
     vtkWindowToImageFilter* m_ImageFilter;
     vtkFFMPEGWriter*        m_ImageWriter;
     int                     m_FrameRate;
     int                     m_VideoQuality;
     std::string             m_FileName;
     bool                    m_ControlIfVideoStarted;
};
#endif
