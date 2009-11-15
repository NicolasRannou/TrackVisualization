#include "TrackWidget.h"

TrackWidget::TrackWidget(QWidget *parent) : QWidget(parent)
  {
    setupUi(this);
    Shape = 0;
    TrackShape = false;
    Begin = 0, End = 0, TotalTimeRange = 0, GlyphTime = 0;

    internalTimer = new QTimer( this );
    connect( internalTimer, SIGNAL(timeout()), SLOT(timeout()) );
    videoLength->setValue(0);

    AutoResetCamera = true;

    FileName = "video_name";
    VideoQuality = 1;
    FrameRate = 30;
    FrameComptor = 0;

    FFMPEGWriter = vtkFFMPEGWriter::New();
    W2if = vtkWindowToImageFilter::New();
    }

TrackWidget::~TrackWidget()
  {
  FFMPEGWriter->Delete();
  W2if->Delete();
  }

void TrackWidget::SetRenderer(vtkRenderer *renderer)
  {
  Renderer = renderer;
  
  visualizationBox-> GetRenderWindow()-> AddRenderer(this->Renderer);
  }

void TrackWidget::SetRenderWindow (vtkRenderWindow *renderWindow)
  {
  RenderWindow = renderWindow;

  visualizationBox->SetRenderWindow(this->RenderWindow);
  }

void TrackWidget::SetRenderWindowInteractor (vtkRenderWindowInteractor *renderWindowInteractor)
  {
  this->RenderWindowInteractor = renderWindowInteractor;

  this->visualizationBox-> GetRenderWindow()->SetInteractor(this->RenderWindowInteractor);
  }

void TrackWidget::SetTotalTimeRange(int value)
  {
  TotalTimeRange = value;
  totalTimeRange->setValue(value);
  }

void TrackWidget::SetEndTime(int value)
  {
  End = value;
  end->setValue(value);
  }

void TrackWidget::SetGlyphTime(int value)
  {
  GlyphTime = value;
  glyphTime->setValue(value);
  }

void TrackWidget::SetRootNode(Lineage<TrackType> * root)
  {
  RootNode = root;
  }

void TrackWidget::on_trackShape_activated ( int index )
  {
  if(index == 0)
    {
	TrackShape = false;
    }
  else
    {
	TrackShape = true;
    }
  updateRenderingWindow();
  }

void TrackWidget::on_glyphShape_activated ( int index )
  {
  Shape = index;
  updateRenderingWindow();
  }

void TrackWidget::on_apply_clicked()
  {
    updateRenderingWindow();
  }

void TrackWidget::on_startVideo_clicked()
  {
  W2if->SetInput(visualizationBox->GetRenderWindow());

  QByteArray convertToConstChar = videoName->text().toLatin1();
  FileName = convertToConstChar.data();

  FFMPEGWriter->SetFileName(FileName);
  FFMPEGWriter->SetQuality(VideoQuality);
  FFMPEGWriter->SetRate(FrameRate);
  FFMPEGWriter->SetInput(W2if->GetOutput());
  FFMPEGWriter->Start();


  bool enablePushButtons = true;
  endVideo->setEnabled(enablePushButtons);
  enablePushButtons = false;
  startVideo->setEnabled(enablePushButtons);
  internalTimer->start( 1000/FrameRate );
  }

void TrackWidget::on_endVideo_clicked()
  {
  FFMPEGWriter->End();

  bool enablePushButtons = false;
  endVideo->setEnabled(enablePushButtons);
  enablePushButtons = true;
  startVideo->setEnabled(enablePushButtons);

  internalTimer->stop();
  FrameComptor = 0;
  }

void TrackWidget::on_begin_valueChanged(int value)
  {
  if( value != Begin )
	{
	if( value < Begin )
	  {
      Begin = value;
      glyphTimeSlider->setMinValue(value);
      updateRenderingWindow();
	  }
	else
	  {
	  if( value <= End )
	    {
		  Begin = value;
		  glyphTimeSlider->setMinValue(value);
	   	updateRenderingWindow();
	    }
	  else
	    {
	  	begin->setValue(End);
	    }
	  }
	}
  }

void TrackWidget::on_end_valueChanged(int value)
  {
  if( value != End )
    {
    if( value > End )
      {
      End = value;
      glyphTimeSlider->setMaxValue(value);
	  updateRenderingWindow();
      }
    else
      {
      if( value >= Begin )
        {
    	End = value;
    	updateRenderingWindow();
    	glyphTimeSlider->setMaxValue(value);
        }
      else
        {
    	end->setValue(Begin);
        }
      }
	}
  else
	  glyphTimeSlider->setMaxValue(value);
  }

void TrackWidget::on_totalTimeRange_valueChanged(int value)
  {

  if( value != TotalTimeRange && value > 0 )
  	{
	TotalTimeRange = value;
  	updateRenderingWindow();
  	}
  }

void TrackWidget::on_glyphTime_valueChanged(int value)
  {
	if( value != GlyphTime )
		{

		  if( value <= End && value >= Begin)
		    {
			GlyphTime = value;
		   	updateRenderingWindow();
		    }
		  else
		    {
			if(value > End)
		  	glyphTime->setValue(End);
			else
			glyphTime->setValue(Begin);
		    }
		}
  }

void TrackWidget::on_glyphTimeSlider_valueChanged(int value)
  {
	if( value != GlyphTime )
			{

			  if( value <= End && value >= Begin)
			    {
				GlyphTime = value;
			   	updateRenderingWindow();
			    }
			  else
			    {
			  	glyphTime->setValue(End);
			    }
			}
  }

void TrackWidget::on_videoQuality_valueChanged(int value)
  {
    VideoQuality = value;
  }

void TrackWidget::on_frameRate_valueChanged(int value)
  {
    FrameRate = value;
  }

void TrackWidget::updateRenderingWindow()
  {

  double * trackTimeRange = new double[2];
  trackTimeRange[0] = Begin;
  trackTimeRange[1] = End;

    if(Shape == 0)
      {
      vtkSphereSource *sphereShape = vtkSphereSource::New();
      sphereShape->SetRadius(0.1);

      PlotTracksTemplate<TrackType, vtkSphereSource >(Renderer, RootNode, sphereShape, TrackShape,
        TotalTimeRange, trackTimeRange, GlyphTime);
      sphereShape->Delete();
      }

    if(Shape == 1)
      {
      vtkCubeSource *cubeShape = vtkCubeSource::New();
      cubeShape->SetXLength(0.2);
      cubeShape->SetYLength(0.2);
      cubeShape->SetZLength(0.2);

      PlotTracksTemplate<TrackType, vtkCubeSource >(Renderer, RootNode, cubeShape, TrackShape,
        TotalTimeRange, trackTimeRange, GlyphTime);
      cubeShape->Delete();
      }
  if(AutoResetCamera)
    {
    Renderer->ResetCamera();
    }
  visualizationBox->update();
  }

void TrackWidget::on_autoResetCamera_toggled(bool on)
  {
  AutoResetCamera = on;
  }

void TrackWidget::ConfigureWidget()
  {
  double * trackTimeRange = new double[2];
  trackTimeRange[0] = Begin;
  trackTimeRange[1] = End;

  vtkSphereSource *sphereShape = vtkSphereSource::New();
  sphereShape->SetRadius(0.1);

  PlotTracksTemplate<TrackType, vtkSphereSource >(Renderer, RootNode, sphereShape, TrackShape,
    TotalTimeRange, trackTimeRange, GlyphTime);
  sphereShape->Delete();

  Renderer->ResetCamera();
  visualizationBox->update();
  }

void TrackWidget::timeout()
{
	visualizationBox->GetRenderWindow()->Render();
	W2if->Modified();
	FFMPEGWriter->Write();

    FrameComptor++;
    double doubleComptor;
    doubleComptor = (double)FrameComptor/(double)FrameRate;
    videoLength->setValue(doubleComptor);
}
