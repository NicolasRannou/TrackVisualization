#include "TrackWidget.h"

TrackWidget::TrackWidget(QWidget *parent) : QWidget(parent)
  {
  setupUi(this);
  m_Shape = 0;
  m_TrackShape = false;
  m_Begin = 0, m_End = 0, m_TotalTimeRange = 0, m_GlyphTime = 0;
  m_InternalTimer = new QTimer( this );
  m_AutoResetCamera = true;
  m_FileName = "video_name";
  m_FullFileName = "video_name";
  m_FrameRate = 30;
  m_FrameComptor = 0;
  m_W2if = vtkWindowToImageFilter::New();

#ifdef   USEFFMPEG
  m_FFMPEGWriter = vtkFFMPEGWriter::New();
  connect( m_InternalTimer, SIGNAL(timeout()), SLOT(timeout()) );
#endif

  }

TrackWidget::~TrackWidget()
  {
#ifdef   USEFFMPEG
  m_FFMPEGWriter->Delete();
#endif
  m_W2if->Delete();
  }

void TrackWidget::SetRenderer(vtkRenderer *renderer)
  {
  m_Renderer = renderer;
  visualizationBox-> GetRenderWindow()-> AddRenderer(this->m_Renderer);
  }

void TrackWidget::SetTotalTimeRange(int value)
  {
  m_TotalTimeRange = value;
  totalTimeRange->setValue(value);
  }

void TrackWidget::SetEndTime(int value)
  {
  m_End = value;
  end->setValue(value);
  }

void TrackWidget::SetGlyphTime(int value)
  {
  m_GlyphTime = value;
  glyphTime->setValue(value);
  }

void TrackWidget::SetRootNode(Lineage<TrackType> * root)
  {
  m_RootNode = root;
  }

void TrackWidget::on_trackShape_activated ( int index )
  {
  if(index == 0)
    {
    m_TrackShape = false;
    }
  else
    {
    m_TrackShape = true;
    }
  updateRenderingWindow();
  }

void TrackWidget::on_glyphShape_activated ( int index )
  {
  m_Shape = index;
  updateRenderingWindow();
  }
#ifdef   USEFFMPEG
void TrackWidget::on_startVideo_clicked()
  {
  m_W2if->SetInput(visualizationBox->GetRenderWindow());

  m_FileName = videoName->text().toStdString();

  m_FFMPEGWriter->SetFileName(m_FullFileName.c_str());
  m_FFMPEGWriter->SetQuality(m_VideoQuality);
  m_FFMPEGWriter->SetRate(m_FrameRate);
  m_FFMPEGWriter->SetInput(m_W2if->GetOutput());
  m_FFMPEGWriter->Start();

  bool enablePushButtons = true;
  endVideo->setEnabled(enablePushButtons);
  enablePushButtons = false;
  startVideo->setEnabled(enablePushButtons);
  videoName->setEnabled(enablePushButtons);
  frameRate->setEnabled(enablePushButtons);
  videoQuality->setEnabled(enablePushButtons);

  m_InternalTimer->start( 1000/m_FrameRate );
  }

void TrackWidget::on_endVideo_clicked()
  {
  m_FFMPEGWriter->End();

  bool enablePushButtons = false;
  endVideo->setEnabled(enablePushButtons);
  enablePushButtons = true;
  startVideo->setEnabled(enablePushButtons);
  videoName->setEnabled(enablePushButtons);
  frameRate->setEnabled(enablePushButtons);
  videoQuality->setEnabled(enablePushButtons);

  m_InternalTimer->stop();
  m_FrameComptor = 0;
  }

void TrackWidget::timeout()
  {
  visualizationBox->GetRenderWindow()->Render();
  m_W2if->Modified();
  m_FFMPEGWriter->Write();
  m_FrameComptor++;
  double doubleComptor;
  doubleComptor = (double)m_FrameComptor/(double)m_FrameRate;
  videoLength->setValue(doubleComptor);
  }
#endif

void TrackWidget::on_begin_valueChanged(int value)
  {
  if( value != m_Begin )
    {
    if( value < m_Begin )
      {
      m_Begin = value;
      glyphTimeSlider->setMinValue(value);
      updateRenderingWindow();
      }
    else
      {
      if( value <= m_End )
        {
        m_Begin = value;
        glyphTimeSlider->setMinValue(value);QFileDialog
        updateRenderingWindow();
        }
      else
        {
        begin->setValue(m_End);
        }
      }
    }
  }

void TrackWidget::on_end_valueChanged(int value)
  {
  if( value != m_End )
    {
    if( value > m_End )
      {
      m_End = value;
      glyphTimeSlider->setMaxValue(value);
      updateRenderingWindow();
      }
    else
      {
      if( value >= m_Begin )
        {
    	m_End = value;
    	updateRenderingWindow();
    	glyphTimeSlider->setMaxValue(value);
        }
      else
        {
    	end->setValue(m_Begin);
        }
      }
    }
  else
    glyphTimeSlider->setMaxValue(value);
  }

void TrackWidget::on_totalTimeRange_valueChanged(int value)
  {
  if( value != m_TotalTimeRange && value > 0 )
    {
    m_TotalTimeRange = value;
    updateRenderingWindow();
    }
  }

void TrackWidget::on_glyphTime_valueChanged(int value)
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
        glyphTime->setValue(m_End);
      else
        glyphTime->setValue(m_Begin);
      }
    }
  }

void TrackWidget::on_glyphTimeSlider_valueChanged(int value)
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

void TrackWidget::on_videoQuality_valueChanged(int value)
  {
    m_VideoQuality = value;
  }

void TrackWidget::on_frameRate_valueChanged(int value)
  {
  m_FrameRate = value;
  }

void TrackWidget::updateRenderingWindow()
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

  visualizationBox->update();
  }

void TrackWidget::on_autoResetCamera_toggled(bool on)
  {
  m_AutoResetCamera = on;
  }

void TrackWidget::ConfigureWidget()
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
  }

void TrackWidget::on_createFile_clicked( )
{
    QString filename = QFileDialog::getSaveFileName(
    this,
    tr( "Folder to Save Video" ),videoName->text(),
    0);
    m_FullFileName = filename.toStdString();
}
