#ifndef _TRACKVISUALIZATION_H
#define _TRACKVISUALIZATION_H

#include "vtkPoints.h"

#include "vtkSphereSource.h"
#include "vtkCubeSource.h"

#include "vtkLineSource.h"
#include "vtkCellArray.h"
#include "vtkTubeFilter.h"
#include "vtkCleanPolyData.h"

#include "vtkMath.h"

#include "vtkGlyph3D.h"

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <iostream>

#include <string.h>
#include <vtkstd/vector>
#include <vtkstd/map>
#include <iostream>
#include <string>

#include "vtkDataObject.h"
#include "vtkPointSet.h"

#include "vtkFloatArray.h"
#include "vtkPointData.h"
#include "vtkLookupTable.h"

#include "vtkSpline.h"
#include "vtkCardinalSpline.h"


//**************************************************************************//
//                            FONCTIONS DECLARATION                         //
//**************************************************************************//

// Create an actor for each spline
void SplineActor(vtkActor *splineActor, vtkPoints *points,
    vtkFloatArray *colors, vtkLookupTable *LUT, bool tubesON);

// Create the glyph actor
template< class GlyphShape>
void GlyphActor(vtkActor *glyphActor, vtkPoints *points, vtkFloatArray *colors,
    vtkLookupTable *LUT, GlyphShape *glyphShape, double *range);

// Get the points in each spline
template< class TrackType>
void TrackSplines(Lineage<TrackType> * polyDataList,
    vtkstd::vector<vtkPoints*> * pointsList, double *trackTimeRange);

// Get the color of each point in each spline
template< class TrackType>
void TrackSplinesColors(Lineage<TrackType> * polyDataList,
    vtkstd::vector<vtkFloatArray*> * pointsColorList, double totalTimeRange,
    double *trackTimeRange);

// Draw the nodes and the splines from the tree
template<class TrackType,
  class GlyphShape >
void PlotTracksTemplate( vtkRenderer* ren1,
    Lineage<TrackType>* RootNode,GlyphShape * glyphShape, bool tubesON,
    double totalTimeRange, double *trackTimeRange, double glyphTime);

// Get the point and the color of each node
template< class TrackType>
void pointsAndScalarsGlyph(Lineage<TrackType> * polyDataList,
    vtkPoints * pointsGlyph, vtkFloatArray *scalarsGlyph,double * counter,
    double totalTimeRange, double *trackTimeRange, double glyphTime);


//**************************************************************************//
//                            FONCTIONS DEFINITION                          //
//**************************************************************************//


template< class TrackType,
  class GlyphShape >
void PlotTracksTemplate( vtkRenderer* ren1,
    Lineage<TrackType>* RootNode,GlyphShape * glyphShape, bool tubesON,
    double totalTimeRange, double *trackTimeRange, double glyphTime)
  {

  // Create the rendering window
  vtkRenderWindow *renWin = vtkRenderWindow::New();
  renWin->AddRenderer(ren1);
  vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
  iren->SetRenderWindow(renWin);

  // Store barycenters position and colors in *vtkPoints
  // and *vtkFloatArray
  double * counter = new double;
  counter[0] = 0.0;
  vtkPoints* pointsGlyph = vtkPoints::New();

  vtkFloatArray *scalarsGlyph = vtkFloatArray::New();

  pointsAndScalarsGlyph<TrackType > (RootNode,
      pointsGlyph, scalarsGlyph, counter, totalTimeRange, trackTimeRange,
      glyphTime);

  // Create the lookup table
  double *range = new double[3];
  range[0] = 0;
  range[1] = 1;

  vtkLookupTable *LUT = vtkLookupTable::New();
  LUT->SetTableRange(range);
  LUT->SetNumberOfTableValues(256);
  LUT->SetHueRange(0,0.7);
  LUT->SetSaturationRange(1,1);
  LUT->SetValueRange(1,1);
  LUT->Build();

  // Add the glyph actor to the renderer
  vtkActor *glyphActor = vtkActor::New();
  GlyphActor<GlyphShape>(glyphActor,pointsGlyph, scalarsGlyph, LUT, glyphShape,
      range);

  ren1->AddActor(glyphActor);

  // Store Track Splines points (*vtkPoints) and colors (*vtkFloatArray)
  // in vtkstd::vector
  vtkstd::vector<vtkPoints*> * trackPoints = new vtkstd::vector<vtkPoints*>;
  vtkstd::vector<vtkFloatArray*> * trackColors =
    new vtkstd::vector<vtkFloatArray*>;

  TrackSplines<TrackType>(RootNode, trackPoints,
      trackTimeRange);

  TrackSplinesColors<TrackType >(RootNode,
      trackColors,totalTimeRange, trackTimeRange);

  vtkstd::vector<vtkFloatArray*>::iterator pointsColorIterator =
    trackColors[0].begin();

  // Create the splines polydata and plot it
  for(vtkstd::vector<vtkPoints*>::iterator
      pointsIterator = trackPoints[0].begin();
      pointsIterator != trackPoints[0].end();
      ++pointsIterator)
    {
    vtkActor * splineActor = vtkActor::New();

    SplineActor(splineActor, *pointsIterator, *pointsColorIterator, LUT,
        tubesON);

    ren1->AddActor(splineActor);

    splineActor->Delete();

    // Increment color iterator
    ++pointsColorIterator;
    }

  // Delete everything

    pointsGlyph->Delete();
    scalarsGlyph->Delete();
    glyphShape->Delete();
    glyphActor->Delete();

   for(vtkstd::vector<vtkPoints*>::iterator
      polyDataIterator = trackPoints[0].begin();
      polyDataIterator != trackPoints[0].end();
      ++polyDataIterator)
    {
    (*polyDataIterator)->Delete();
    }

    for(vtkstd::vector<vtkFloatArray*>::iterator
      polyDataIterator = trackColors[0].begin();
      polyDataIterator != trackColors[0].end();
      ++polyDataIterator)
    {
    (*polyDataIterator)->Delete();
    }

  LUT->Delete();
  delete range;

  // Render the volume

  //ren1->ResetCamera();
  //renWin->Render();

  //iren->Start();

  renWin->Delete();
  iren->Delete();
}

//**************************************************************************//
template< class TrackType>
void TrackSplines(Lineage<TrackType> * polyDataList,
    vtkstd::vector<vtkPoints*> * pointsList, double *trackTimeRange ) 
  {

  if (!polyDataList->IsLeaf())
    {
    typename TrackType::iterator containerIterator;
    containerIterator = polyDataList->GetTrack()[0].begin();

    if( (containerIterator->first >= trackTimeRange[0])
        && (containerIterator->first <= trackTimeRange[1]))
      {
      if((polyDataList->GetDaughter1() != 0)
          && (polyDataList->GetDaughter1()->GetTrack()[0].begin()->first
            >= trackTimeRange[0])
          && (polyDataList->GetDaughter1()->GetTrack()[0].begin()->first
            <= trackTimeRange[1]))
        {
        vtkPoints* splinesPoints = vtkPoints::New();
        double *barycenter = new double[3];
        double x,y,z;

        if((polyDataList->GetDaughter2() != 0)
            && (polyDataList->GetDaughter2()->GetTrack()[0].begin()->first
              >= trackTimeRange[0])
            && (polyDataList->GetDaughter2()->GetTrack()[0].begin()->first
              <= trackTimeRange[1]))
          {
          barycenter = GetBarycenter(containerIterator->second);
          x = barycenter[0];
          y = barycenter[1];
          z = barycenter[2];
          splinesPoints->InsertPoint (1, x, y, z);

          containerIterator
            = polyDataList->GetDaughter1()->GetTrack()[0].begin();

          barycenter = GetBarycenter(containerIterator->second);
          x = barycenter[0];
          y = barycenter[1];
          z = barycenter[2];
          splinesPoints->InsertPoint (0, x, y, z);

          containerIterator
            = polyDataList->GetDaughter2()->GetTrack()[0].begin();

          barycenter = GetBarycenter(containerIterator->second);
          x = barycenter[0];
          y = barycenter[1];
          z = barycenter[2];
          splinesPoints->InsertPoint (2, x, y, z);
          /*
          if((polyDataList->GetDaughter1()->GetTrack()[0].begin()->GetDaughter1() != 0)&&(polyDataList->GetDaughter1()->GetTrack()[0].begin()->GetDaughter2() == 0))
            {
            cout<<"ONLY CHILD 1"<<endl;
            }
            
          if((polyDataList->GetDaughter1()->GetTrack()[0].begin()->GetDaughter1() == 0)&&(polyDataList->GetDaughter1()->GetTrack()[0].begin()->GetDaughter2() != 0))
            {
            cout<<"ONLY CHILD 1"<<endl;
            }
            
          if((polyDataList->GetDaughter2()->GetTrack()[0].begin()->GetDaughter1() != 0)&&(polyDataList->GetDaughter2()->GetTrack()[0].begin()->GetDaughter2() == 0))
            {
            cout<<"ONLY CHILD 2"<<endl;
            }*/

          pointsList->push_back(splinesPoints);
          }
        else
          {
          barycenter = GetBarycenter(containerIterator->second);
          x = barycenter[0];
          y = barycenter[1];
          z = barycenter[2];
          splinesPoints->InsertPoint (1, x, y, z);

          containerIterator
            = polyDataList->GetDaughter1()->GetTrack()[0].begin();

          barycenter = GetBarycenter(containerIterator->second);
          x = barycenter[0];
          y = barycenter[1];
          z = barycenter[2];
          splinesPoints->InsertPoint (0, x, y, z);

          /*if(!polyDataList->IsRoot())
            {
            containerIterator
            = polyDataList->GetMother()->GetTrack()[0].begin();
            barycenter = GetBarycenter(containerIterator->second);
            x = barycenter[0];
            y = barycenter[1];
            z = barycenter[2];
            splinesPoints->InsertPoint (2, x, y, z);
            //splinesPoints->InsertPoint (3, x, y, z);
            }*/
            
            pointsList->push_back(splinesPoints);
          
          }
        }
      else
        {
        if((polyDataList->GetDaughter2() != 0)
            && (polyDataList->GetDaughter2()->GetTrack()[0].begin()->first
              >= trackTimeRange[0])
            && (polyDataList->GetDaughter2()->GetTrack()[0].begin()->first
              <= trackTimeRange[1]))
          {
          vtkPoints* splinesPoints = vtkPoints::New();
          double *barycenter = new double[3];
          double x,y,z;

          barycenter = GetBarycenter(containerIterator->second);
          x = barycenter[0];
          y = barycenter[1];
          z = barycenter[2];
          splinesPoints->InsertPoint (1, x, y, z);

          containerIterator
            = polyDataList->GetDaughter2()->GetTrack()[0].begin();
          barycenter = GetBarycenter(containerIterator->second);
          x = barycenter[0];
          y = barycenter[1];
          z = barycenter[2];
          splinesPoints->InsertPoint (0, x, y, z);
          
          /*if(!polyDataList->IsRoot())
            {
            containerIterator
            = polyDataList->GetMother()->GetTrack()[0].begin();
            barycenter = GetBarycenter(containerIterator->second);
            x = barycenter[0];
            y = barycenter[1];
            z = barycenter[2];
            splinesPoints->InsertPoint (2, x, y, z);
            //splinesPoints->InsertPoint (3, x, y, z);
            }*/
            
            pointsList->push_back(splinesPoints);
          
          }
        }
      }

    if(polyDataList->GetDaughter1() != 0)
      {
        TrackSplines<TrackType>(polyDataList->GetDaughter1(),
           pointsList, trackTimeRange);
      }

    if(polyDataList->GetDaughter2() != 0)
      {
        TrackSplines<TrackType>(polyDataList->GetDaughter2(),
           pointsList, trackTimeRange);
      }
    }
  }

//**************************************************************************//
template< class TrackType>
void TrackSplinesColors(Lineage<TrackType> * polyDataList,
    vtkstd::vector<vtkFloatArray*> * pointsColorList,
    double totalTimeRange, double *trackTimeRange)
  {

  if (!polyDataList->IsLeaf())
    {
    typename TrackType::iterator containerIterator;
    containerIterator = polyDataList->GetTrack()[0].begin();

    if( (containerIterator->first >= trackTimeRange[0])
        && (containerIterator->first <= trackTimeRange[1]))
      {
      if((polyDataList->GetDaughter1() != 0)
          && (polyDataList->GetDaughter1()->GetTrack()[0].begin()->first
            >= trackTimeRange[0])
          && (polyDataList->GetDaughter1()->GetTrack()[0].begin()->first
            <= trackTimeRange[1]))
        {
        if((polyDataList->GetDaughter2() != 0)
            && (polyDataList->GetDaughter2()->GetTrack()[0].begin()->first
              >= trackTimeRange[0])
            && (polyDataList->GetDaughter2()->GetTrack()[0].begin()->first
              <= trackTimeRange[1]))
          {
          vtkFloatArray* splinesColors = vtkFloatArray::New();

          splinesColors->SetNumberOfValues(3);
          splinesColors->InsertValue (1,
              containerIterator->first/totalTimeRange);
          containerIterator
            = polyDataList->GetDaughter1()->GetTrack()[0].begin();
          splinesColors->InsertValue (0,
              containerIterator->first/totalTimeRange);
          containerIterator
            = polyDataList->GetDaughter2()->GetTrack()[0].begin();
          splinesColors->InsertValue (2,
              containerIterator->first/totalTimeRange);

          pointsColorList[0].push_back(splinesColors);
          }
        else
          {
          vtkFloatArray* splinesColors = vtkFloatArray::New();

          splinesColors->SetNumberOfValues(2);
          splinesColors->InsertValue (1,
              containerIterator->first/totalTimeRange);
          containerIterator
            = polyDataList->GetDaughter1()->GetTrack()[0].begin();
          splinesColors->InsertValue (0,
              containerIterator->first/totalTimeRange);

          
          /*if(!polyDataList->IsRoot())
            {
            containerIterator
            = polyDataList->GetMother()->GetTrack()[0].begin();
            
            splinesColors->InsertValue (2,
              containerIterator->first/totalTimeRange);
            //splinesColors->InsertValue (3,
            //  containerIterator->first/totalTimeRange);
            }*/
            
            pointsColorList[0].push_back(splinesColors);            
          }
        }
      else
        {
        if((polyDataList->GetDaughter2() != 0)
            && (polyDataList->GetDaughter2()->GetTrack()[0].begin()->first
              >= trackTimeRange[0])
            && (polyDataList->GetDaughter2()->GetTrack()[0].begin()->first
              <= trackTimeRange[1]))
          {
          vtkFloatArray* splinesColors = vtkFloatArray::New();

          splinesColors->SetNumberOfValues(2);
          splinesColors->InsertValue (1,
              containerIterator->first/totalTimeRange);
          containerIterator
            = polyDataList->GetDaughter2()->GetTrack()[0].begin();
          splinesColors->InsertValue (0,
              containerIterator->first/totalTimeRange);

          /*if(!polyDataList->IsRoot())
            {
            containerIterator
            = polyDataList->GetMother()->GetTrack()[0].begin();
            
            splinesColors->InsertValue (2,
              containerIterator->first/totalTimeRange);
            //splinesColors->InsertValue (3,
            //  containerIterator->first/totalTimeRange);
            }*/

            pointsColorList[0].push_back(splinesColors);  
            
          }
        }
      }

    if(polyDataList->GetDaughter1() != 0)
      {
        TrackSplinesColors<TrackType>(
            polyDataList->GetDaughter1(), pointsColorList,totalTimeRange,
            trackTimeRange);
      }

    if(polyDataList->GetDaughter2() != 0)
      {
        TrackSplinesColors<TrackType>(
            polyDataList->GetDaughter2(), pointsColorList, totalTimeRange,
            trackTimeRange);
      }
    }
  }

//**************************************************************************//
template< class TrackType>
void pointsAndScalarsGlyph(Lineage<TrackType> * polyDataList,
    vtkPoints * pointsGlyph, vtkFloatArray *scalarsGlyph, double * counter,
    double totalTimeRange, double *trackTimeRange, double glyphTime)
  {

  double *barycenter = new double[3];
  double x, y, z;
  //typedef typename TrackType::iterator TrackTypeIterator;
  typename TrackType::iterator containerIterator;
  containerIterator = polyDataList->GetTrack()[0].begin();

  //if( (containerIterator->first >= trackTimeRange[0])
  //&& (containerIterator->first <= trackTimeRange[1]))
  if( containerIterator->first == glyphTime)
    {
    barycenter = GetBarycenter(containerIterator->second);
    x = barycenter[0];
    y = barycenter[1];
    z = barycenter[2];
    pointsGlyph->InsertPoint (counter[0], x, y, z);
    scalarsGlyph->InsertValue(counter[0],
        containerIterator->first/totalTimeRange);
    counter[0]++;
    }

  if(!polyDataList->IsLeaf())
    {
    if(polyDataList->GetDaughter1() != 0)
      {
      pointsAndScalarsGlyph<TrackType>(
          polyDataList->GetDaughter1(), pointsGlyph, scalarsGlyph, counter,
          totalTimeRange, trackTimeRange, glyphTime);
      }
    if(polyDataList->GetDaughter2() != 0)
      {
      pointsAndScalarsGlyph<TrackType>(
         polyDataList->GetDaughter2(), pointsGlyph, scalarsGlyph, counter,
         totalTimeRange, trackTimeRange, glyphTime);
      }
    }
  }

//**************************************************************************//
double * GetBarycenter(vtkPolyData* polyData)
  {

  double *centerOfGravity = new double[3];

  vtkPoints *dataPoints = polyData->GetPoints();
  int totalTimeRange = dataPoints->GetNumberOfPoints();

  centerOfGravity[0] = 0; centerOfGravity[1] = 0; centerOfGravity[2] = 0;

  for (int i = 0; i < totalTimeRange; i++)
    {
    centerOfGravity[0] += dataPoints->GetPoint(i)[0];
    centerOfGravity[1] += dataPoints->GetPoint(i)[1];
    centerOfGravity[2] += dataPoints->GetPoint(i)[2];
    }

  centerOfGravity[0] = centerOfGravity[0]/totalTimeRange;
  centerOfGravity[1] = centerOfGravity[1]/totalTimeRange;
  centerOfGravity[2] = centerOfGravity[2]/totalTimeRange;

  return centerOfGravity;
  }

//**************************************************************************//
template< class GlyphShape>
void GlyphActor(vtkActor *glyphActor, vtkPoints *points, vtkFloatArray *colors,
    vtkLookupTable *LUT, GlyphShape *glyphShape, double *range)
  {

  // Create a barycentersGlyph to plot all the spheres
  vtkPolyData *barycenterPolydata = vtkPolyData::New();
  barycenterPolydata->SetPoints(points);
  barycenterPolydata->GetPointData()->SetScalars(colors);

  vtkGlyph3D *barycentersGlyph = vtkGlyph3D::New();
  barycentersGlyph->SetInput(barycenterPolydata);
  barycentersGlyph->SetSource(glyphShape->GetOutput());
  barycentersGlyph->SetVectorModeToUseNormal();
  barycentersGlyph->SetScaleModeToScaleByVector();
  barycentersGlyph->SetColorModeToColorByScalar();

  // Create the barycentersGlyph mapper
  vtkPolyDataMapper *sphereMapper = vtkPolyDataMapper::New();
  sphereMapper->SetInput(barycentersGlyph->GetOutput());
  sphereMapper->SetScalarRange(range);
  sphereMapper->SetLookupTable(LUT);

  // Create the barycentersGlyph actor and render it
  glyphActor->SetMapper(sphereMapper);

  barycenterPolydata->Delete();
  barycentersGlyph->Delete();
  sphereMapper->Delete();
  }

//**************************************************************************//
void SplineActor(vtkActor *splineActor, vtkPoints *points,
    vtkFloatArray *colors, vtkLookupTable *LUT, bool tubesON)
  {

    int numberOfInputPoints = points->GetNumberOfPoints();
    double point[3];

    vtkCardinalSpline * aSplineX = vtkCardinalSpline::New();
    vtkCardinalSpline * aSplineY = vtkCardinalSpline::New();
    vtkCardinalSpline * aSplineZ = vtkCardinalSpline::New();

    for(int i = 0; i<numberOfInputPoints; i++)
      {
      points->GetPoint(i, point);

      aSplineX[0].AddPoint(i,point[0]);
      aSplineY[0].AddPoint(i,point[1]);
      aSplineZ[0].AddPoint(i,point[2]);
      }

    // Generate the polyline for the spline.
    vtkPoints * splinesPoints = vtkPoints::New();
    vtkPolyData * splineData = vtkPolyData::New();

    // Number of points on the spline
    int numberOfOutputPoints = 50;

    // Interpolate x, y and z by using the three spline filters and
    // create new points
    double t;
    vtkFloatArray * trackSplineColor = vtkFloatArray::New();
    trackSplineColor->SetNumberOfValues(numberOfOutputPoints);

    for (int i=0; i<numberOfOutputPoints; i++)
      {
      t =
     static_cast<double>(numberOfInputPoints-1)/
     static_cast<double>(numberOfOutputPoints-1)*static_cast<double>(i);

      splinesPoints->InsertPoint(i, aSplineX[0].Evaluate(t),
          aSplineY[0].Evaluate(t), aSplineZ[0].Evaluate(t));

      for(int j=0; j<numberOfInputPoints; j++)
        {
        if(j<t && t<j+1)
          {
          trackSplineColor->InsertValue(i,colors->GetValue(j)+
              (colors->GetValue(j+1)-
               colors->GetValue(j))*(t-j));
          }
        }
      }

    // Create the polyline
    vtkCellArray * trackLines = vtkCellArray::New();
    trackLines->InsertNextCell(numberOfOutputPoints);
    for (int i=0; i<numberOfOutputPoints; i++)
      {
      trackLines->InsertCellPoint(i);
      }

    splineData->SetPoints(splinesPoints);
    splineData->SetLines(trackLines);
    splineData->GetPointData()->SetScalars(trackSplineColor);

    vtkPolyDataMapper * splineMapper = vtkPolyDataMapper::New();

    // Check if user wants to create tubes or not
    if(tubesON)
      {
      vtkTubeFilter * splineTubes = vtkTubeFilter::New();
      splineTubes->SetNumberOfSides(8);
      splineTubes->SetInput(splineData);
      splineTubes->SetRadius(.05);
      splineMapper->SetInputConnection(splineTubes->GetOutputPort());
      splineTubes->Delete();
      }
    else
      {
      splineMapper->SetInput(splineData);
      }

    splineMapper->SetLookupTable(LUT);

    splineActor->SetMapper(splineMapper);
    //splineActor->GetProperty()->SetDiffuseColor(1.0, 0.0, 0.0);
    //splineActor->GetProperty()->SetSpecular(.3);
    //splineActor->GetProperty()->SetSpecularPower(30);

    // Delete everything

    splineData->Delete();
    splinesPoints->Delete();
    trackSplineColor->Delete();
    trackLines->Delete();
    splineMapper->Delete();

    aSplineX->Delete();
    aSplineY->Delete();
    aSplineZ->Delete();
  }


#endif
