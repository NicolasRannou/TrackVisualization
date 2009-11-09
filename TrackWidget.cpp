#include "TrackWidget.h"

TrackWidget::TrackWidget(QWidget *parent) : QWidget(parent)
  {
    setupUi(this);
    Shape = 0;
    linesON = true;
    tubesON = false;
    Begin = 0, End = 0, TotalTimeRange = 0, GlyphTime = 0;
  }

void TrackWidget::SetNumberOfArgs(int numberOfArgs)
  {
  this->NumberOfArgs = numberOfArgs;
  nombre1->setValue(this->NumberOfArgs);
  }

void TrackWidget::SetRenderWindow(vtkRenderer *renderWindow)
  {
  this->RenderWindow = renderWindow;
  
  visualizationBox-> GetRenderWindow()-> AddRenderer(this->RenderWindow);
  }

void TrackWidget::on_boutonEgal_clicked()
  {
    int somme = nombre1->value() + nombre2->value();
    result->setNum(somme); 
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

  // Generate the polydatas
  }

void TrackWidget::on_begin_valueChanged(int value)
  {
  
  if(value >= End)
    {
    begin->setValue(value-1);
    }
  else
    {
    Begin = value;
    }
  }

void TrackWidget::on_end_valueChanged(int value)
  {

  if(value <= Begin)
    {
    end->setValue(value+1);
    }
  else
    {
    End = value;
    }
  }

void TrackWidget::on_totalTimeRange_valueChanged(int value)
  {
  TotalTimeRange = value;
  }

void TrackWidget::on_glyphTime_valueChanged(int value)
  {
  if(value > End)
    {
    glyphTime->setValue(value-1);
    }
  else if((value < Begin))
    {
    glyphTime->setValue(value+1);
    }
  else 
    {
    GlyphTime = value;
    }
  }
