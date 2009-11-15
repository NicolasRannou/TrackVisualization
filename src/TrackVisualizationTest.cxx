#include "vtkPolyData.h"
#include "vtkPolyDataReader.h"

#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkProperty.h"
#include "vtkCamera.h"

#include "vtkPoints.h"

#include "vtkSphereSource.h"

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <iostream>

#include <string.h>
#include <vtkstd/vector>
#include <vtkstd/map>
#include <string>

//QT
#include <QApplication>
#include <QtGui>
#include "TrackWidget.h"

//video
#include <vtkGenericMovieWriter.h>
#include <vtkMPEG2Writer.h>
#include <vtkWindowToImageFilter.h>

using namespace std;



//**************************************************************************//
//                         GLOBAL VARIABLES DECLARATION                     //
//**************************************************************************//




//**************************************************************************//
//                           FONCTIONS DECLARATION                          //
//**************************************************************************//

// Returns a pointer which contains the name of all the files
// in the target folder
char **GetAllFiles (const char *path);

// Returns the number of files contained in the target foldervtkRenderer *testtt;
int NumberOfFiles (const char *path);

// Returns the polyData contained in the target folder
vtkPolyData * loadPolyData(int i,char * path,char * fileName);

// Get the extension of the target file
const char* getFileExtension(const string& file);


//**************************************************************************//
//                               MAIN                                       //
//**************************************************************************//

int main( int argc, char *argv[] )
  {

  if (argc < 2) {
        fprintf (stderr, "Usage: %s /path_To_VTK_Files \n", argv[0]);
        return 1;
    }

  // Define variables
  static char **files = NULL;

  // Create the rendering window
  vtkRenderer *ren1 = vtkRenderer::New();
  vtkRenderWindow *renWin = vtkRenderWindow::New();
  vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();

  bool tubesON = false;
  if(argc >= 3)
    {
    if(strcmp(argv[2],"-tubesON") == 0)
      {
      tubesON = true;
     }
  }

  // Get the files which are in the folder "argv[1]"
  files = GetAllFiles (argv[1]);

  // Load all the polyDatas.vtk files and store it in a vtkstd::vector
  vtkstd::vector<vtkPolyData*>        polyDataList;
  int numberOfFiles = NumberOfFiles(argv[1]);

  for(int i=0; i<numberOfFiles; i++)
    {
    if(strcmp(getFileExtension(files[i]),".vtk") == 0)
      {
      polyDataList.push_back(loadPolyData(i,argv[1],files[i]));
      }
    }

  // PARAMETERS
  
  // totalTimeRange : to be defined for a good color repartition
  // trackTimeRange : to display the tracks which contains points in the chosen time range
  // glyphTime      : to display the glyph at the chosen time
  double totalTimeRange = 120;
  double * trackTimeRange = new double[2];
  trackTimeRange[0] = 0;
  trackTimeRange[1] = 60;
  double glyphTime = 30;

  // Tree creation with a map <polydata,time>
  typedef vtkstd::map<double, vtkPolyData*> TrackType;
  TrackType t0, t1, t2, t3, t4, t5, t6, t7, t8;

  vtkstd::vector<vtkPolyData*>::iterator
      polyDataIterator = polyDataList.begin();

  t0.insert( vtkstd::make_pair( 0, *polyDataIterator ) );
  polyDataIterator++;
  t1.insert( vtkstd::make_pair( 10, *polyDataIterator ) );
  polyDataIterator++;
  t2.insert( vtkstd::make_pair( 10, *polyDataIterator ) );
  polyDataIterator++;
  t3.insert( vtkstd::make_pair( 30, *polyDataIterator ) );
  polyDataIterator++;
  t4.insert( vtkstd::make_pair( 30, *polyDataIterator ) );
  polyDataIterator++;
  t5.insert( vtkstd::make_pair( 40, *polyDataIterator ) );
  polyDataIterator++;
  t6.insert( vtkstd::make_pair( 60, *polyDataIterator ) );
  polyDataIterator++;
  t7.insert( vtkstd::make_pair( 100, *polyDataIterator ) );
  polyDataIterator++;
  t8.insert( vtkstd::make_pair( 60, *polyDataIterator ) );

  Lineage<TrackType>* RootNode = new Lineage<TrackType>(0, &t0);
  Lineage<TrackType>* Node1 = new Lineage<TrackType>(RootNode, &t1);
  Lineage<TrackType>* Node2 = new Lineage<TrackType>(RootNode, &t2);
  RootNode->SetDaughter1( Node1 );
  RootNode->SetDaughter2( Node2 );
  Lineage<TrackType>* Node3 = new Lineage<TrackType>(Node1, &t3);
  Lineage<TrackType>* Node4 = new Lineage<TrackType>(Node1, &t4);
  Node1->SetDaughter1( Node3 );
  Node1->SetDaughter2( Node4 );
  Lineage<TrackType>* Node5 = new Lineage<TrackType>(Node2, &t5);
  Node2->SetDaughter1( Node5 );
  Lineage<TrackType>* Node6 = new Lineage<TrackType>(Node5, &t6);
  Node2->SetDaughter2( Node6 );

  Lineage<TrackType>* Node7 = new Lineage<TrackType>(Node3, &t7);
  Lineage<TrackType>* Node8 = new Lineage<TrackType>(Node5, &t8);
  Node5->SetDaughter2( Node8 );
  Node3->SetDaughter1( Node7 );
  
  // Create the rendering window
  renWin->AddRenderer(ren1);
  iren->SetRenderWindow(renWin);


  QApplication app(argc, argv);

  TrackWidget window(NULL);
/*
  vtkWindowToImageFilter* w2if = vtkWindowToImageFilter::New();
  w2if->SetInput(renWin);

  vtkMPEG2Writer* mpegWriter = vtkMPEG2Writer::New();
  mpegWriter->SetFileName("part2-1.mpg");
  mpegWriter->SetInputConnection(w2if->GetOutputPort());
  mpegWriter->Start();
  mpegWriter->End();*/


  //window.SetRenderWindow(renWin);
  //window.SetRenderWindowInteractor(iren);
  window.SetRenderer(ren1);
  window.SetTotalTimeRange(totalTimeRange);
  window.SetEndTime(totalTimeRange);
  window.SetGlyphTime(totalTimeRange/2);
  window.SetRootNode(RootNode);
  window.ConfigureWidget();

 //add tree root

  window.show();

  app.exec();

  // Delete everything
  delete files;
  delete trackTimeRange;

  for(vtkstd::vector<vtkPolyData*>::iterator
      polyDataIterator = polyDataList.begin();
      polyDataIterator != polyDataList.end();
      ++polyDataIterator)
    {
    (*polyDataIterator)->Delete();
    }

  ren1->Delete();
  renWin->Delete();
  iren->Delete();



  return 0;
  }



//**************************************************************************//
//                        FONCTIONS DEFINITION                              //
//**************************************************************************//


//**************************************************************************//
char ** GetAllFiles (const char *path)
  {

  DIR *dir = opendir (path);
  struct dirent *dp;          /* returned from readdir() */
  size_t filecount = 0;       /* number of entries in directory */
  size_t i = 0;
  char **files;

  if (dir == NULL) {
    return NULL;            /* opendir() failed */
  }
  /* Pass 1: Count number of files and subdirectories */
  while ((dp = readdir (dir)) != NULL) {
    filecount++;
  }
  /* Allocate space for the result */
  files = (char **) malloc (filecount * sizeof (*files));
  if (files == NULL) {
    return NULL;            /* malloc failed */
  }
  /* Pass 2: Collect all filenames */
  rewinddir (dir);
  while ((dp = readdir (dir)) != NULL) {
    files[i] = strdup (dp->d_name);
    if (files[i] == NULL) {
      /* memory allocation failure; free what's been allocated
       * so far and return NULL.
       */
      while (i > 0) {
        free (files[--i]);
      }
      free (files);
      return NULL;
    }
    //printf ("%d: %s\n", i, dp->d_name);
    i++;
  }

  closedir (dir);
  return files;
  }

//**************************************************************************//
int NumberOfFiles (const char *path)
  {

  DIR *dir = opendir (path);
  struct dirent *dp;          /* returned from readdir() */
  int numberOfFiles = 0;

  if (dir == NULL) {
    return NULL;            /* opendir() failed */
  }
  /* Pass 1: Count number of files and subdirectories */
  while ((dp = readdir (dir)) != NULL) {
    numberOfFiles++;
  }

  closedir (dir);
  return numberOfFiles;
  }

//**************************************************************************//
vtkPolyData * loadPolyData(int i,char * path,char * fileName)
  {

  char str[100];
  strcpy (str,path);
  strcat (str,fileName);

  vtkPolyDataReader *reader = vtkPolyDataReader::New();
  reader->SetFileName(str);
  reader->Update();

  vtkPolyData* polyDataReader = vtkPolyData::New();
  polyDataReader->DeepCopy(reader->GetOutput());

  reader->Delete();

  return polyDataReader;
  }

//**************************************************************************//
const char* getFileExtension(const string& file)
  {

  string str = file;
  string ext = "";
  const char* p;

  for(int i=0; i<str.length(); i++)
    {
    if(str[i] == '.')
      {
      for(int j = i; j<str.length(); j++)
        {
        ext += str[j];
        }
      return p = ext.c_str();
      }
    }

  return p = ext.c_str();
  }
