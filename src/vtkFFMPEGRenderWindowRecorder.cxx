#include "vtkFFMPEGRenderWindowRecorder.h"

#include "vtkRenderWindow.h"
#include "vtkWindowToImageFilter.h"
#include "vtkFFMPEGWriter.h"

#include "vtkGraphicsFactory.h"
#include "vtkDebugLeaks.h"

vtkFFMPEGRenderWindowRecorder::
vtkFFMPEGRenderWindowRecorder() :
  m_FrameRate (30), m_VideoQuality (1),
  m_ControlIfVideoStarted (false), m_FileName("goFigure2")
{
	m_RenderWindow = vtkRenderWindow::New();
	m_ImageFilter  = vtkWindowToImageFilter::New();
	m_ImageWriter  = vtkFFMPEGWriter::New();
}

vtkFFMPEGRenderWindowRecorder::
~vtkFFMPEGRenderWindowRecorder()
{
	m_RenderWindow->Delete();
	m_ImageFilter ->Delete();
	m_ImageWriter ->Delete();
}

vtkFFMPEGRenderWindowRecorder*
vtkFFMPEGRenderWindowRecorder::
New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret =
    vtkGraphicsFactory::CreateInstance("vtkFFMPEGRenderWindowRecorder");
  if ( ret )
    {
    return static_cast<vtkFFMPEGRenderWindowRecorder *>(ret);
    }
#ifdef VTK_DEBUG_LEAKS
  vtkDebugLeaks::ConstructClass("vtkFFMPEGRenderWindowRecorder");
#endif
  return new vtkFFMPEGRenderWindowRecorder;
}


void
vtkFFMPEGRenderWindowRecorder::
SetFileName( const std::string& iFileName)
{
	m_FileName = iFileName;
}

void
vtkFFMPEGRenderWindowRecorder::
SetRenderWindow( vtkRenderWindow* iRenderWindow)
{
	m_RenderWindow = iRenderWindow;

	m_ImageFilter->SetInput(m_RenderWindow);
	m_ImageWriter->SetInput(m_ImageFilter->GetOutput());
}

void
vtkFFMPEGRenderWindowRecorder::
StartCapture()
{

	cout<< "FileName: " << m_FileName << endl;

	m_ImageWriter->SetFileName(m_FileName.c_str());
	m_ImageWriter->SetQuality(m_VideoQuality);
	m_ImageWriter->SetRate(m_FrameRate);

	if (m_ImageWriter->GetInput() != NULL)
		{
    m_ImageWriter->Start();
    m_ControlIfVideoStarted = true;
		}
}

void
vtkFFMPEGRenderWindowRecorder::
EndCapture()
{
	if (m_ControlIfVideoStarted)
	  {
	  m_ImageWriter->End();
	  m_ControlIfVideoStarted = false;
	  }
}

void
vtkFFMPEGRenderWindowRecorder::
TakeSnapshot()
{
	if (m_ControlIfVideoStarted)
		{
	  m_ImageFilter->Modified();
	  m_ImageWriter->Write();
		}
}
