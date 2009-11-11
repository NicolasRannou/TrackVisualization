#include "TrackWidget.h"

TrackWidget::TrackWidget(QWidget *parent) : QWidget(parent)
  {
    setupUi(this);
    Shape = 0;
    linesON = true;
    tubesON = false;
    Begin = 0, End = 0, TotalTimeRange = 0, GlyphTime = 0;
  }

void TrackWidget::SetRenderer(vtkRenderer *renderer)
  {
  this->Renderer = renderer;
  
  this->visualizationBox-> GetRenderWindow()-> AddRenderer(this->Renderer);

  //this->RenderWindow->AddRenderer(this->Renderer);
  //this->RenderWindowInteractor->SetRenderWindow(this->RenderWindow);
  }

void TrackWidget::SetRenderWindow (vtkRenderWindow *renderWindow)
  {
  this->RenderWindow = renderWindow;

  this->visualizationBox->SetRenderWindow(this->RenderWindow);
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
  if(tubesON)
  { 
  //printf("tubesON  ");
  }
  else
  {
  //printf("tubesOFF  ");
  }

  }

void TrackWidget::on_lines_toggled(bool on)
  {
  linesON = on;
  }

void TrackWidget::on_glyphShape_activated ( int index )
  {

  Shape = index;
  // 0 is sphere, 1 is cube
  //printf("%d",index);
  }

void TrackWidget::on_apply_clicked()
  {
  // Check if the parameters are the good ones
  if(linesON)
    {
    printf("Lines ON \n");
    }
  else
    {
    printf("Tubes ON \n");
    }

    
    printf("Shape: %d \n", Shape);
    printf("Begin: %d \n", Begin);
    printf("End: %d \n", End);
    printf("TotalTimeRange: %d \n", TotalTimeRange);
    printf("GlyphTime: %d \n", GlyphTime);

    //typedef vtkSphereSource GlyphShape;
    vtkSphereSource *glyphShape = vtkSphereSource::New();
    glyphShape->SetRadius(0.1);

    double * trackTimeRange = new double[2];
      trackTimeRange[0] = Begin;
      trackTimeRange[1] = End;

    PlotTracksTemplate<TrackType, vtkSphereSource >(this->RenderWindow, this->RenderWindowInteractor, this->Renderer, RootNode, glyphShape, linesON,
    	TotalTimeRange, trackTimeRange, GlyphTime);

    this->Renderer->ResetCamera();
    this->visualizationBox->update();

    glyphShape->Delete();
  }

void TrackWidget::on_begin_valueChanged(int value)
  {
    Begin = value;
  }

void TrackWidget::on_end_valueChanged(int value)
  {
    End = value;
  }

void TrackWidget::on_totalTimeRange_valueChanged(int value)
  {
  TotalTimeRange = value;
  }

void TrackWidget::on_glyphTime_valueChanged(int value)
  {
    GlyphTime = value;
  }
