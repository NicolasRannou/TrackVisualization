#include "vtkFFMPEGRenderWindowRecorder.h"

#include "vtkRenderWindow.h"
#include "vtkWindowToImageFilter.h"
#include "vtkFFMPEGWriter.h"

#include "vtkObjectFactory.h"
#include "vtkDebugLeaks.h"


//-----------------------------------------------------------
// constructor
//
vtkFFMPEGRenderWindowRecorder::
vtkFFMPEGRenderWindowRecorder() :
  m_FrameRate (30), m_VideoQuality (1),
  m_FileName("goFigure2"), m_ControlIfVideoStarted (false)
{
	m_ImageFilter  = vtkWindowToImageFilter::New();
	m_ImageWriter  = vtkFFMPEGWriter::New();
}

//-----------------------------------------------------------
// destructor
//
vtkFFMPEGRenderWindowRecorder::
~vtkFFMPEGRenderWindowRecorder()
{
  if(m_ImageFilter != NULL)
  	{
	  m_ImageFilter ->Delete();
  	}
	if(m_ImageWriter != NULL)
		{
    m_ImageWriter ->Delete();
		}
}

//-----------------------------------------------------------
// create a new vtkObject from the constructor
//
vtkFFMPEGRenderWindowRecorder*
vtkFFMPEGRenderWindowRecorder::
New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret =
    vtkObjectFactory::CreateInstance("vtkFFMPEGRenderWindowRecorder");
  if ( ret )
    {
    return static_cast<vtkFFMPEGRenderWindowRecorder *>(ret);
    }

  return new vtkFFMPEGRenderWindowRecorder;
}



//-----------------------------------------------------------
// set the name of the video (can be a path)
//
void
vtkFFMPEGRenderWindowRecorder::
SetFileName( const std::string& iFileName)
{
	m_FileName = iFileName;
}

//-----------------------------------------------------------
// set the render window to be observed
//
void
vtkFFMPEGRenderWindowRecorder::
SetRenderingWindow( vtkRenderWindow* iRenderWindow)
{
	m_RenderWindow = iRenderWindow;

	m_ImageFilter->SetInput(m_RenderWindow);
	m_ImageWriter->SetInput(m_ImageFilter->GetOutput());
}

//-----------------------------------------------------------
// start the video acquisition
//
void
vtkFFMPEGRenderWindowRecorder::
StartCapture()
{
	m_ImageWriter->SetFileName(m_FileName.c_str());
	m_ImageWriter->SetQuality(m_VideoQuality);
	m_ImageWriter->SetRate(m_FrameRate);

	if (m_ImageWriter->GetInput() != NULL)
		{
    m_ImageWriter->Start();
    m_ControlIfVideoStarted = true;
		}
}

//-----------------------------------------------------------
// end the video acquisition
//
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

//-----------------------------------------------------------
// take a snapshot of the current render window
//
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
