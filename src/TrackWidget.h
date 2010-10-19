#ifndef __TRACKWIDGET_H
#define __TRACKWIDGET_H

#include <QtGui>
#include "ui_DesignerTrackWidget.h"

#include <map>
#include "Lineage.h"

#include <iostream>
#include <string>

class vtkPolyData;
class vtkRenderer;

class TrackWidget : public QWidget, private Ui::TrackWidget
{
    Q_OBJECT

    public:
        typedef std::map<double, vtkPolyData*> TrackType;

        TrackWidget(QWidget *parent = 0);
        ~TrackWidget();

        void SetRenderer(vtkRenderer *renderer);
        void SetTotalTimeRange(const int& value);
        void SetEndTime(const int& value);
        void SetGlyphTime(const int& value);
        void SetRootNode(Lineage<TrackType> * root);

        void ConfigureWidget();

    private:
        vtkRenderer *m_Renderer;
        bool m_TrackShape;
        int m_Begin;
        int m_End;
        int m_TotalTimeRange;
        int m_GlyphTime;
        int m_Shape;
        bool m_AutoResetCamera;
        Lineage<TrackType>* m_RootNode;

        void updateRenderingWindow();


    private slots:
        void on_trackShape_activated ( int index );
        void on_glyphShape_activated ( int index );
        void on_begin_valueChanged(int value);
        void on_end_valueChanged(int value);
        void on_totalTimeRange_valueChanged(int value);
        void on_glyphTime_valueChanged(int value);
        void on_glyphTimeSlider_valueChanged(int value);
        void on_autoResetCamera_toggled(bool on);
};

#endif

