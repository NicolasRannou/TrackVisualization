#ifndef HEADER_TRACKWIDGET
#define HEADER_TRACKWIDGET

#include <QtGui>
#include "vtkRenderWindow.h"
#include "ui_DesignerTrackWidget.h"

#include <vtkstd/map>
#include "vtkPolyData.h"
#include "Lineage.h"

#include "vtkSphereSource.h"

#include "TrackVisualization.h"


class TrackWidget : public QWidget, private Ui::TrackWidget
{
    Q_OBJECT


    public:
        typedef vtkstd::map<double, vtkPolyData*> TrackType;

        TrackWidget(QWidget *parent = 0);
        void SetRenderWindow (vtkRenderer *renderWindow);
        void SetTotalTimeRange(int value);
        void SetEndTime(int value);
        void SetGlyphTime(int value);
        void SetRootNode(Lineage<TrackType> * root);

    private:
        vtkRenderer *RenderWindow;
        bool tubesON, linesON;
        int Begin, End, TotalTimeRange, GlyphTime;
        int Shape;
        Lineage<TrackType>* RootNode;

    private slots:
        void on_lines_clicked();
        void on_tubes_clicked();
        void on_tubes_toggled(bool on);
        void on_lines_toggled(bool on);
        void on_glyphShape_activated ( int index );
        void on_apply_clicked();
        void on_begin_valueChanged(int value);
	    void on_end_valueChanged(int value);
	    void on_totalTimeRange_valueChanged(int value);
	    void on_glyphTime_valueChanged(int value);
};


#endif

