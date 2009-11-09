#ifndef HEADER_TRACKWIDGET
#define HEADER_TRACKWIDGET

#include <QtGui>
#include "vtkRenderWindow.h"
#include "ui_DesignerTrackWidget.h"

class TrackWidget : public QWidget, private Ui::TrackWidget
{
    Q_OBJECT

    private:
        int NumberOfArgs;
        char *contentOfArgs[];
        vtkRenderer *RenderWindow;
        bool tubesON, linesON;
        int Begin, End, TotalTimeRange, GlyphTime;
        int Shape;

    public:
        TrackWidget(QWidget *parent = 0);
        void SetNumberOfArgs(int numberOfArgs);
        void SetRenderWindow (vtkRenderer *renderWindow);

    private slots:
        void on_boutonEgal_clicked();
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

