#ifndef HEADER_TRACKWIDGET
#define HEADER_TRACKWIDGET

#include <QtGui>
#include "vtkRenderWindow.h"
#include "ui_DesignerTrackWidget.h"

#include <vtkstd/map>
#include "vtkPolyData.h"
#include "Lineage.h"

#include "vtkSphereSource.h"
#include "vtkCubeSource.h"

#include "TrackVisualization.h"

#include "vtkWindowToImageFilter.h"

#ifdef   USEFFMPEG
 #include "vtkFFMPEGWriter.h"
#endif


class TrackWidget : public QWidget, private Ui::TrackWidget
{
    Q_OBJECT

    public:
        typedef vtkstd::map<double, vtkPolyData*> TrackType;

        TrackWidget(QWidget *parent = 0);
        ~TrackWidget();

        void SetRenderer (vtkRenderer *renderer);void SetTotalTimeRange(int value);
        void SetEndTime(int value);
        void SetGlyphTime(int value);
        void SetRootNode(Lineage<TrackType> * root);

        void ConfigureWidget();

    private:
        vtkRenderer *m_Renderer;
        bool m_TrackShape;
        int m_Begin, m_End, m_TotalTimeRange, m_GlyphTime;
        int m_Shape;
        const char* m_FullFileName;
        const char* m_FileName;
        QTimer *m_InternalTimer;
        int m_VideoQuality;
        int m_FrameRate;
        int m_FrameComptor;
        bool m_AutoResetCamera;
        Lineage<TrackType>* m_RootNode;
#ifdef   USEFFMPEG
        vtkFFMPEGWriter* m_FFMPEGWriter;
#endif
        vtkWindowToImageFilter* m_W2if;
        void updateRenderingWindow();

    private slots:
        void on_trackShape_activated ( int index );
        void on_glyphShape_activated ( int index );
        void on_createFile_clicked();
#ifdef   USEFFMPEG
        void on_startVideo_clicked();
        void on_endVideo_clicked();
        void timeout();
#endif
        void on_begin_valueChanged(int value);
        void on_end_valueChanged(int value);
        void on_totalTimeRange_valueChanged(int value);
        void on_videoQuality_valueChanged(int value);
        void on_frameRate_valueChanged(int value);
        void on_glyphTime_valueChanged(int value);
        void on_glyphTimeSlider_valueChanged(int value);
        void on_autoResetCamera_toggled(bool on);

};

#endif

