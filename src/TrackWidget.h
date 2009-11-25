#ifndef __TRACKWIDGET_H
#define __TRACKWIDGET_H

#include <QtGui>
#include "ui_DesignerTrackWidget.h"

#include <map>
#include "Lineage.h"

#include <iostream>
#include <string>

#ifdef   USEFFMPEG
class vtkFFMPEGWriter;
#endif

class vtkFFMPEGRenderWindowRecorder;

class vtkPolyData;
class vtkWindowToImageFilter;
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
        std::string m_FullFileName;
        std::string m_FileName;
        QTimer *m_InternalTimer;
        int m_VideoQuality;
        int m_FrameRate;
        int m_FrameCounter;
        bool m_AutoResetCamera;
        Lineage<TrackType>* m_RootNode;
#ifdef   USEFFMPEG
        vtkFFMPEGWriter* m_FFMPEGWriter;
        vtkFFMPEGRenderWindowRecorder* m_FFMPEGRecorder;
#endif
        vtkWindowToImageFilter* m_W2if;

        void updateRenderingWindow();


    private slots:
        void on_trackShape_activated ( int index );
        void on_glyphShape_activated ( int index );
        void on_createFile_clicked();
        void on_begin_valueChanged(int value);
        void on_end_valueChanged(int value);
        void on_totalTimeRange_valueChanged(int value);
        void on_videoQuality_valueChanged(int value);
        void on_frameRate_valueChanged(int value);
        void on_glyphTime_valueChanged(int value);
        void on_glyphTimeSlider_valueChanged(int value);
        void on_autoResetCamera_toggled(bool on);
#ifdef   USEFFMPEG
        void on_startVideo_clicked();
        void on_endVideo_clicked();
        void timeout();
#endif

};

#endif

