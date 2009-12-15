#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <iostream>

#include <vtkstd/vector>
#include <vtkstd/map>
#include <string>

//QT
#include <QApplication>
#include <QtGui>
#include "DialogRecorder.h"

using namespace std;



//**************************************************************************//
//                               MAIN                                       //
//**************************************************************************//

int main( int argc, char *argv[] )
  {

  QApplication app(argc, argv);

  DialogRecorder window(NULL);

  window.show();

  app.exec();

  return 0;
  }
