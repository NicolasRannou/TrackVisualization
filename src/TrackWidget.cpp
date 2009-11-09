#include "TrackWidget.h"

TrackWidget::TrackWidget(QWidget *parent) : QWidget(parent)
  {
    setupUi(this);
    Shape = 0;
    linesON = true;
    tubesON = false;
    Begin = 0, End = 0, TotalTimeRange = 0, GlyphTime = 0;
  }

void TrackWidget::SetRenderWindow(vtkRenderer *renderWindow)
  {
  this->RenderWindow = renderWindow;
  
  visualizationBox-> GetRenderWindow()-> AddRenderer(this->RenderWindow);
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

    typedef vtkSphereSource GlyphShape;
    vtkSphereSource *glyphShape = vtkSphereSource::New();
    glyphShape->SetRadius(0.1);

    double * trackTimeRange = new double[2];
      trackTimeRange[0] = Begin;
      trackTimeRange[1] = End;

    //PlotTracksTemplate<TrackType, GlyphShape >(RenderWindow, RootNode, GlyphShape, linesON,
    //		TotalTimeRange, trackTimeRange, GlyphTime);
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
