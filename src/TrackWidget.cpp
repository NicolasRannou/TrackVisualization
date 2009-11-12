#include "TrackWidget.h"

TrackWidget::TrackWidget(QWidget *parent) : QWidget(parent)
  {
    setupUi(this);
    Shape = 0;
    linesON = true;
    tubesON = false;
    Begin = 0, End = 0, TotalTimeRange = 0, GlyphTime = 0;

    FileName = "video_name";
    VideoQuality = 1;
    FrameRate = 30;

    FFMPEGWriter = vtkFFMPEGWriter::New();
    W2if = vtkWindowToImageFilter::New();
    SnapshotObserver = vtkSnapshotCommand::New();

    SnapshotObserver->CallbackFFMPEGWriter = FFMPEGWriter;
    SnapshotObserver->CallbackW2if = W2if;
    SnapshotObserver->CallbackvisualizationBox = visualizationBox;
    SnapshotObserver->CallbackImagesComptor = 0;
    SnapshotObserver->CallbackFrameRate = 30;
    SnapshotObserver->CallbackRecord = false;

    this->visualizationBox->GetRenderWindow()->AddObserver(vtkCommand::StartEvent, SnapshotObserver);
    }

TrackWidget::~TrackWidget()
  {
  FFMPEGWriter->Delete();
  W2if->Delete();
  SnapshotObserver->Delete();
  }

void TrackWidget::SetRenderer(vtkRenderer *renderer)
  {
  Renderer = renderer;
  
  visualizationBox-> GetRenderWindow()-> AddRenderer(this->Renderer);

  //this->RenderWindow->AddRenderer(this->Renderer);
  //this->RenderWindowInteractor->SetRenderWindow(this->RenderWindow);
  }

void TrackWidget::SetRenderWindow (vtkRenderWindow *renderWindow)
  {
  RenderWindow = renderWindow;

  visualizationBox->SetRenderWindow(this->RenderWindow);
  //this->visualizationBox->GetRenderWindow()->Render();
  //this->visualizationBox->update();
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

void TrackWidget::on_lines_clicked()
  {

  }


void TrackWidget::on_tubes_clicked()
  {

  }

void TrackWidget::on_tubes_toggled(bool on)
  {
  tubesON = on;
  //updateRenderingWindow();

  }

void TrackWidget::on_lines_toggled(bool on)
  {
  linesON = on;
  }

void TrackWidget::on_glyphShape_activated ( int index )
  {
  Shape = index;
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

  SnapshotObserver->CallbackImagesComptor = 0;
  SnapshotObserver->CallbackFrameRate = FrameRate;

  bool enablePushButtons = true;
  endVideo->setEnabled(enablePushButtons);
  enablePushButtons = false;
  startVideo->setEnabled(enablePushButtons);
  SnapshotObserver->CallbackRecord = true;

  }

void TrackWidget::on_endVideo_clicked()
  {
  FFMPEGWriter->End();

  bool enablePushButtons = false;
  endVideo->setEnabled(enablePushButtons);
  enablePushButtons = true;
  startVideo->setEnabled(enablePushButtons);
  SnapshotObserver->CallbackRecord = false;
  }

void TrackWidget::on_begin_valueChanged(int value)
  {
    Begin = value;
    //updateRenderingWindow();
  }

void TrackWidget::on_end_valueChanged(int value)
  {
    End = value;
    //updateRenderingWindow();
  }

void TrackWidget::on_totalTimeRange_valueChanged(int value)
  {
  TotalTimeRange = value;
  glyphTimeSlider->setMaxValue(value);
  //updateRenderingWindow();
  }

void TrackWidget::on_glyphTime_valueChanged(int value)
  {
    GlyphTime = value;
    //updateRenderingWindow();
  }

void TrackWidget::on_glyphTimeSlider_valueChanged(int value)
  {
    GlyphTime = value;
    //updateRenderingWindow();
  }

void TrackWidget::on_videoQuality_valueChanged(int value)
  {
    VideoQuality = value;
    //updateRenderingWindow();
  }

void TrackWidget::on_frameRate_valueChanged(int value)
  {
    FrameRate = value;
    //updateRenderingWindow();
  }

void TrackWidget::updateRenderingWindow()
  {

  double * trackTimeRange = new double[2];
  trackTimeRange[0] = Begin;
  trackTimeRange[1] = End;

/*//doesn't work crossed initialisation
  switch (ShapeInt)
      {
      case 0:
  	vtkSphereSource *sphereShape = vtkSphereSource::New();
  	sphereShape->SetRadius(0.1);

  	PlotTracksTemplate<TrackType, vtkSphereSource >(Renderer, RootNode, sphereShape, linesON,
  	  TotalTimeRange, trackTimeRange, GlyphTime);
  	sphereShape->Delete();
  	break;

      case 1:
  	vtkCubeSource *cubeShape = vtkCubeSource::New();
  	cubeShape->SetXLength(0.2);
  	cubeShape->SetYLength(0.2);
  	cubeShape->SetZLength(0.2);

  	PlotTracksTemplate<TrackType, vtkCubeSource >(Renderer, RootNode, cubeShape, linesON,
  	  TotalTimeRange, trackTimeRange, GlyphTime);
  	cubeShape->Delete();
  	break;

      default:
	  break;
      }*/

    if(Shape == 0)
      {
      vtkSphereSource *sphereShape = vtkSphereSource::New();
      sphereShape->SetRadius(0.1);

      PlotTracksTemplate<TrackType, vtkSphereSource >(Renderer, RootNode, sphereShape, tubesON,
        TotalTimeRange, trackTimeRange, GlyphTime);
      sphereShape->Delete();
      }

    if(Shape == 1)
      {
      vtkCubeSource *cubeShape = vtkCubeSource::New();
      cubeShape->SetXLength(0.2);
      cubeShape->SetYLength(0.2);
      cubeShape->SetZLength(0.2);

      PlotTracksTemplate<TrackType, vtkCubeSource >(Renderer, RootNode, cubeShape, tubesON,
        TotalTimeRange, trackTimeRange, GlyphTime);
      cubeShape->Delete();
      }

  Renderer->ResetCamera();
  visualizationBox->update();
  }
