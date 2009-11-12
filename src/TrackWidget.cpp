#include "TrackWidget.h"

TrackWidget::TrackWidget(QWidget *parent) : QWidget(parent)
  {
    setupUi(this);
    Shape = 0;
    linesON = true;
    tubesON = false;
    Begin = 0, End = 0, TotalTimeRange = 0, GlyphTime = 0;
    MpegWriter = vtkMPEG2Writer::New();
    AVIWriter = vtkFFMPEGWriter::New();
    AVIWriter->SetQuality(2);
    AVIWriter->SetRate(30);
    W2if = vtkWindowToImageFilter::New();
   //
   // visualizationBox->GetRenderWindow()->AddObserver();
    SnapshotObserver = vtkSnapshotCommand::New();
    SnapshotObserver->CallbackMpegWriter = MpegWriter;
    SnapshotObserver->CallbackAVIWriter = AVIWriter;
    SnapshotObserver->CallbackW2if = W2if;
    SnapshotObserver->CallbackvisualizationBox = visualizationBox;


    //this->visualizationBox->GetInteractor()->AddObserver(QVTKWidget::DragMoveEvent, SnapshotObserver,1);
    this->visualizationBox->GetRenderWindow()->AddObserver(vtkCommand::StartEvent, SnapshotObserver);
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

/*void TrackWidget::on_glyphShape_activated ( const QString& shapeChar)
  {
    QByteArray convert = shapeChar.toLatin1();
    ShapeChar = convert.data();
    printf("test: %s", ShapeChar);
  }*/

void TrackWidget::on_apply_clicked()
  {
    updateRenderingWindow();
  }

void TrackWidget::on_startVideo_clicked()
  {
  W2if->SetInput(visualizationBox->GetRenderWindow());
  /*
  MpegWriter->SetFileName("test.mpg");
  MpegWriter->SetInput(W2if->GetOutput());
  MpegWriter->Start();*/
  AVIWriter->SetFileName("test.avi");
  AVIWriter->SetInput(W2if->GetOutput());
  AVIWriter->Start();
  }

void TrackWidget::on_endVideo_clicked()
  {
  AVIWriter->End();
  }

void TrackWidget::on_visualizationBox_mouseEvent(QMouseEvent *event)
  {
    printf("Mouse event \n");
  /*visualizationBox->GetRenderWindow()->Render();
  W2if->Modified();
  MpegWriter->Write();*/
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

/*
void TrackWidget::on_glyphTimeSlider_sliderMoved(int value)
  {
    //GlyphTime = value;
    updateRenderingWindow();
  }

void TrackWidget::on_timeRangeSlider_sliderMoved(int value)
  {
    //GlyphTime = value;
    updateRenderingWindow();
  }
*/

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
