//For the visualization
#include "vtkSphereSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkProperty.h"

//For the testcoordinates geometry, properties, transformation
#include "vtkFFMPEGRenderWindowRecorder.h"

#include <stdio.h>

int main( int argc, char* argv[] )
{
  if( argc != 2 )
    {
    std::cerr <<"Usage: " <<std::endl;
    std::cerr <<"" <<std::endl;
    return EXIT_FAILURE;
    }

  // create sphere geometry
  vtkSphereSource *sphere = vtkSphereSource::New();
  sphere->SetRadius(1.0);
  sphere->SetThetaResolution(18);
  sphere->SetPhiResolution(18);

  // map to graphics library
  vtkPolyDataMapper *map = vtkPolyDataMapper::New();
  map->SetInput(sphere->GetOutput());

  // actor creation and properties definition
  vtkActor *aSphere = vtkActor::New();
  aSphere->SetMapper(map);
  aSphere->GetProperty()->SetColor(0,0,1);
  aSphere->GetProperty()->SetOpacity(0.5);
  //aSphere->GetProperty()->SetColor(0,0,1); // sphere color blue

  // a renderer and render window
  vtkRenderer *ren1 = vtkRenderer::New();
  vtkRenderWindow *renWin = vtkRenderWindow::New();
  renWin->AddRenderer(ren1);

  // an interactor
  vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
  iren->SetRenderWindow(renWin);
  iren->Initialize();

  // add the actor to the scene
  ren1->AddActor(aSphere);
  ren1->SetBackground(1,0,0); // Background color white

  // create the video
  vtkFFMPEGRenderWindowRecorder *testRecorder = vtkFFMPEGRenderWindowRecorder::New();
  testRecorder->SetRenderWindow(renWin);
  testRecorder->SetFileName( argv[1] );
  testRecorder->StartCapture();

  for (int i = 0; i < 100; i ++)
    {
    renWin->Render();
    testRecorder->TakeSnapshot();
    }

  testRecorder->EndCapture();

  // remove the video
  //remove( "RecordingTest" );

  return EXIT_SUCCESS;
}