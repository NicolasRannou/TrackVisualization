#include "TrackWidget.h"

#include "vtkSphereSource.h"
#include "vtkCubeSource.h"
#include "vtkWindowToImageFilter.h"
#include "vtkPolyData.h"
#include "vtkRenderer.h"

#include "TrackVisualization.h"

TrackWidget::
TrackWidget(QWidget *parent) :
  QWidget(parent), m_TrackShape( false ),   m_Begin( 0 ), m_End( 0 ),
  m_TotalTimeRange( 0 ), m_GlyphTime( 0 ), m_Shape( 0 ),
  m_AutoResetCamera( true )
{
  this->setupUi(this);
}

TrackWidget::
~TrackWidget()
{
}

void
TrackWidget::
SetRenderer(vtkRenderer *renderer)
{
  m_Renderer = renderer;
  this->visualizationBox->GetRenderWindow()->AddRenderer(m_Renderer);
}

void
TrackWidget::
SetTotalTimeRange(const int& value)
{
  m_TotalTimeRange = value;
  totalTimeRange->setValue(value);
}

void
TrackWidget::
SetEndTime(const int& value)
{
  m_End = value;
  end->setValue(value);
}

void
TrackWidget::
SetGlyphTime(const int& value)
{
  m_GlyphTime = value;
  glyphTime->setValue(value);
}

void
TrackWidget::
SetRootNode(Lineage<TrackType> * root)
{
  m_RootNode = root;
}

void
TrackWidget::
on_trackShape_activated ( int index )
{
  m_TrackShape = ( index != 0 );
  updateRenderingWindow();
}

void
TrackWidget::
on_glyphShape_activated ( int index )
{
  m_Shape = index;
  updateRenderingWindow();
}

void
TrackWidget::
on_begin_valueChanged(int value)
{
  if( value != m_Begin )
    {
    if( value < m_Begin )
      {
      m_Begin = value;
      this->glyphTimeSlider->setMinimum(value);
      updateRenderingWindow();
      }
    else
      {
      if( value <= m_End )
        {
        m_Begin = value;
        this->glyphTimeSlider->setMinimum(value);
        updateRenderingWindow();
        }
      else
        {
        this->begin->setValue(m_End);
        }
      }
    }
}

void
TrackWidget::
on_end_valueChanged(int value)
{
  if( value != m_End )
    {
    if( value > m_End )
      {
      m_End = value;
      this->glyphTimeSlider->setMaximum(value);
      updateRenderingWindow();
      }
    else
      {
      if( value >= m_Begin )
        {
        m_End = value;
        updateRenderingWindow();
        this->glyphTimeSlider->setMaximum(value);
        }
      else
        {
        this->end->setValue(m_Begin);
        }
      }
    }
  else
    {
    this->glyphTimeSlider->setMaximum(value);
    }
}

void
TrackWidget::
on_totalTimeRange_valueChanged(int value)
{
  if( (value != m_TotalTimeRange) && (value > 0) )
    {
    m_TotalTimeRange = value;
    updateRenderingWindow();
    }
}

void
TrackWidget::
on_glyphTime_valueChanged(int value)
{
  if( value != m_GlyphTime )
    {
    if( value <= m_End && value >= m_Begin)
      {
      m_GlyphTime = value;
      updateRenderingWindow();
      }
    else
      {
      if(value > m_End)
        {
        glyphTime->setValue(m_End);
        }
      else
        {
        glyphTime->setValue(m_Begin);
        }
      }
    }
}

void
TrackWidget::
on_glyphTimeSlider_valueChanged(int value)
{
  if( value != m_GlyphTime )
    {
    if( value <= m_End && value >= m_Begin)
      {
      m_GlyphTime = value;
      updateRenderingWindow();
      }
    else
      {
      glyphTime->setValue(m_End);
      }
    }
}

void
TrackWidget::
updateRenderingWindow()
{
  double * trackTimeRange = new double[2];
  trackTimeRange[0] = m_Begin;
  trackTimeRange[1] = m_End;

  if(m_Shape == 0)
    {
    vtkSphereSource *sphereShape = vtkSphereSource::New();
    sphereShape->SetRadius(0.1);

    PlotTracksTemplate<TrackType, vtkSphereSource >(m_Renderer, m_RootNode, sphereShape, m_TrackShape,
      m_TotalTimeRange, trackTimeRange, m_GlyphTime);
    sphereShape->Delete();
    }

  if(m_Shape == 1)
    {
    vtkCubeSource *cubeShape = vtkCubeSource::New();
    cubeShape->SetXLength(0.2);
    cubeShape->SetYLength(0.2);
    cubeShape->SetZLength(0.2);

    PlotTracksTemplate<TrackType, vtkCubeSource >(m_Renderer, m_RootNode, cubeShape, m_TrackShape,
      m_TotalTimeRange, trackTimeRange, m_GlyphTime);
    cubeShape->Delete();
    }

  if(m_AutoResetCamera)
    {
    m_Renderer->ResetCamera();
    }

  this->visualizationBox->update();
  delete trackTimeRange;
  }

void
TrackWidget::
on_autoResetCamera_toggled(bool on)
{
  m_AutoResetCamera = on;
}

void
TrackWidget::
ConfigureWidget()
{
  double * trackTimeRange = new double[2];
  trackTimeRange[0] = m_Begin;
  trackTimeRange[1] = m_End;

  vtkSphereSource *sphereShape = vtkSphereSource::New();
  sphereShape->SetRadius(0.1);

  PlotTracksTemplate<TrackType, vtkSphereSource >(m_Renderer, m_RootNode, sphereShape, m_TrackShape,
    m_TotalTimeRange, trackTimeRange, m_GlyphTime);
  sphereShape->Delete();

  m_Renderer->ResetCamera();
  visualizationBox->update();

  delete trackTimeRange;
}
