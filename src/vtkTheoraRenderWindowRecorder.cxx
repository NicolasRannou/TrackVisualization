#include "vtkTheoraRenderWindowRecorder.h"

#include "vtkRenderWindow.h"
#include "vtkWindowToImageFilter.h"
#include "vtkOggTheoraWriter.h"

#include "vtkObjectFactory.h"
#include "vtkDebugLeaks.h"


//-----------------------------------------------------------
// constructor
//
vtkTheoraRenderWindowRecorder::
vtkTheoraRenderWindowRecorder() :
  m_FrameRate (30), m_VideoQuality (1),
  m_FileName("goFigure2"), m_ControlIfVideoStarted (false)
{
	m_ImageFilter  = vtkWindowToImageFilter::New();
	m_ImageWriter  = vtkOggTheoraWriter::New();
}

//-----------------------------------------------------------
// destructor
//
vtkTheoraRenderWindowRecorder::
~vtkTheoraRenderWindowRecorder()
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
vtkTheoraRenderWindowRecorder*
vtkTheoraRenderWindowRecorder::
New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret =
    vtkObjectFactory::CreateInstance("vtkTheoraRenderWindowRecorder");
  if ( ret )
    {
    return static_cast<vtkTheoraRenderWindowRecorder *>(ret);
    }

  return new vtkTheoraRenderWindowRecorder;
}



//-----------------------------------------------------------
// set the name of the video (can be a path)
//
void
vtkTheoraRenderWindowRecorder::
SetFileName( const std::string& iFileName)
{
	m_FileName = iFileName;
}

//-----------------------------------------------------------
// set the render window to be observed
//
void
vtkTheoraRenderWindowRecorder::
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
vtkTheoraRenderWindowRecorder::
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
vtkTheoraRenderWindowRecorder::
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
vtkTheoraRenderWindowRecorder::
TakeSnapshot()
{
	if (m_ControlIfVideoStarted)
		{
	  m_ImageFilter->Modified();
	  m_ImageWriter->Write();
		}
}
