#ifndef HEADER_VTKSNAPSHOTCOMMAND
#define HEADER_VTKSNAPSHOTCOMMAND

#include <QtGui>
#include "vtkRenderWindow.h"
#include "ui_DesignerTrackWidget.h"

#include <vtkstd/map>
#include "vtkPolyData.h"
#include "Lineage.h"

#include "vtkSphereSource.h"
#include "vtkCubeSource.h"

#include <string.h>

#include "vtkRendererCollection.h"

#include "TrackVisualization.h"

#include "vtkWindowToImageFilter.h"

#include <vtkFFMPEGWriter.h>

#include "vtkCommand.h"

#include <qlcdnumber.h>

class vtkSnapshotCommand : public vtkCommand
{
public:
  static vtkSnapshotCommand *New() { return new vtkSnapshotCommand; }
  virtual void Execute(vtkObject *caller, unsigned long, void *callData)
   {
   if(CallbackRecord)
       {
       CallbackvisualizationBox->GetRenderWindow()->Render();
       CallbackW2if->Modified();
       CallbackFFMPEGWriter->Write();
       CallbackImagesComptor ++;
       }
   }
   
  vtkSnapshotCommand():CallbackFFMPEGWriter(0),CallbackW2if(0),CallbackvisualizationBox(0), CallbackImagesComptor(0),CallbackFrameRate(0)/*, CallbackVideoLength(0)*/{}
  
  vtkFFMPEGWriter* CallbackFFMPEGWriter;
  vtkWindowToImageFilter* CallbackW2if;
  QVTKWidget* CallbackvisualizationBox;
  int CallbackImagesComptor;
  int CallbackFrameRate;
  QLCDNumber* CallbackVideoLength;
  bool CallbackRecord;

};


#endif
