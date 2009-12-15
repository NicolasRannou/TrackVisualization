#ifndef __DIALOGRECORDER_H
#define __DIALOGRECORDER_H

#include <QtGui>
#include "ui_DesignerDialogRecorder.h"

#include "vtkFFMPEGRenderWindowRecorder.h"

#include <iostream>
#include <string>

class vtkRenderWindow;

class DialogRecorder : public QDialog, private Ui::DialogVideoRecorder
{
    Q_OBJECT

    public:
        explicit DialogRecorder( QWidget* parent = 0);
        ~DialogRecorder();

    private:
        void setXSpinMin(unsigned int iValue);
        void setXSpinMax(unsigned int iValue);
        void setYSpinMin(unsigned int iValue);
        void setYSpinMax(unsigned int iValue);
        void setZSpinMin(unsigned int iValue);
        void setZSpinMax(unsigned int iValue);
        void setTSpinMin(unsigned int iValue);
        void setTSpinMax(unsigned int iValue);

        void SetRenderingWindow( vtkRenderWindow* iRenderingWindow );

        unsigned int m_XMin;
        unsigned int m_XMax;
        unsigned int m_YMin;
        unsigned int m_YMax;
        unsigned int m_ZMin;
        unsigned int m_ZMax;
        unsigned int m_TMin;
        unsigned int m_TMax;

        unsigned int m_RecordX;
        unsigned int m_RecordY;
        unsigned int m_RecordZ;
        unsigned int m_RecordT;

        QString m_VideoName;

        unsigned int m_FrameRate;
        unsigned int m_VideoQuality;

        vtkFFMPEGRenderWindowRecorder *m_VideoRecorder;

    private slots:
        //Create video pushbutton
        void on_startVideo_clicked();

        //Video selection checkBoxes
        void on_xSliceCheckBox_stateChanged( int state );
        void on_ySliceCheckBox_stateChanged( int state );
        void on_zSliceCheckBox_stateChanged( int state );
        void on_tSliceCheckBox_stateChanged( int state );

        //First slice for recording
        void on_xSpinMin_valueChanged(int value);
        void on_ySpinMin_valueChanged(int value);
        void on_zSpinMin_valueChanged(int value);
        void on_tSpinMin_valueChanged(int value);

        //Last slice for recording
        void on_xSpinMax_valueChanged(int value);
        void on_ySpinMax_valueChanged(int value);
        void on_zSpinMax_valueChanged(int value);
        void on_tSpinMax_valueChanged(int value);

        //Video parameters
        void on_createFile_clicked();
        void on_frameRate_valueChanged(int value);
        void on_videoQuality_valueChanged(int value);
};

#endif

