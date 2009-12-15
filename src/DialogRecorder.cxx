#include "DialogRecorder.h"

#include <QFileDialog>

#include "vtkRenderWindow.h"


//---------------------------------------------------//
//                                                   //
//               CONSTRUCTOR/DESTRUCTOR              //
//                                                   //
//---------------------------------------------------//

/**
 * \brief Constructor
 */

DialogRecorder::
DialogRecorder(QWidget *iParent) : QDialog( iParent ), m_XMin( 0 ),
  m_XMax( 100 ), m_YMin(0), m_YMax( 100 ), m_ZMin( 0 ), m_ZMax( 100 ),
  m_TMin( 0 ), m_TMax( 100 ), m_RecordX( 0 ), m_RecordY( 0 ),
  m_RecordZ( 0 ), m_RecordT( 0 ), m_VideoName( "" ),
  m_FrameRate( 30 ), m_VideoQuality ( 1 )
{
	this->setupUi(this);

	m_VideoRecorder = vtkFFMPEGRenderWindowRecorder::New();
}

/**
 * \brief Destructor
 */
DialogRecorder::
~DialogRecorder( )
{

	m_VideoRecorder->Delete();

}

//---------------------------------------------------//
//                                                   //
//                    METHODS                        //
//                                                   //
//---------------------------------------------------//

/**
 * \brief Set the minimum value for the X spin box
 * \param[in] iValue Value of the X spin box
 */
void
DialogRecorder::
setXSpinMin(unsigned int iValue)
{
	this->xSpinMin->setMinimum(iValue);
	this->xSpinMax->setMinimum(iValue+1);
}

/**
 * \brief Set the maximum value for the X spin box
 * \param[in] iValue Value of the X spin box
 */
void
DialogRecorder::
setXSpinMax(unsigned int iValue)
{
	this->xSpinMin->setMinimum(iValue-1);
	this->xSpinMax->setMinimum(iValue);
}

/**
 * \brief Set the minimum value for the Y spin box
 * \param[in] iValue Value of the Y spin box
 */
void
DialogRecorder::
setYSpinMin(unsigned int iValue)
{
	this->ySpinMin->setMinimum(iValue);
	this->ySpinMax->setMinimum(iValue+1);
}

/**
 * \brief Set the maximum value for the Y spin box
 * \param[in] iValue Value of the Y spin box
 */
void
DialogRecorder::
setYSpinMax(unsigned int iValue)
{
	this->ySpinMin->setMinimum(iValue-1);
	this->ySpinMax->setMinimum(iValue);
}

/**
 * \brief Set the minimum value for the Y spin box
 * \param[in] iValue Value of the Y spin box
 */
void
DialogRecorder::
setZSpinMin(unsigned int iValue)
{
	this->zSpinMin->setMinimum(iValue);
	this->zSpinMax->setMinimum(iValue+1);
}

/**
 * \brief Set the maximum value for the Z spin box
 * \param[in] iValue Value of the Z spin box
 */
void
DialogRecorder::
setZSpinMax(unsigned int iValue)
{
	this->zSpinMin->setMinimum(iValue-1);
	this->zSpinMax->setMinimum(iValue);
}

/**
* \brief Set the minimum value for the Y spin box
* \param[in] iValue Value of the Y spin box
*/
void
DialogRecorder::
setTSpinMin(unsigned int iValue)
{
	this->tSpinMin->setMinimum(iValue);
	this->tSpinMax->setMinimum(iValue+1);
}

/**
* \brief Set the maximum value for the Z spin box
* \param[in] iValue Value of the Z spin box
*/
void
DialogRecorder::
setTSpinMax(unsigned int iValue)
{
	this->tSpinMin->setMinimum(iValue-1);
	this->tSpinMax->setMinimum(iValue);
}

/**
* \brief Set the rendering window to record video
* \param[in] iRenderingWindow Observed rendering window
*/
void
DialogRecorder::
SetRenderingWindow( vtkRenderWindow* iRenderingWindow )
{
	m_VideoRecorder->SetRenderingWindow(iRenderingWindow);
}

//---------------------------------------------------//
//                                                   //
//                    SIGNALS                        //
//                                                   //
//---------------------------------------------------//

/**
 * \brief Function called when checkbox state changes
 */
void
DialogRecorder::
on_xSliceCheckBox_stateChanged( int state )
{
	m_RecordX = state;
}

/**
 * \brief Function called when checkbox state changes
 */
void
DialogRecorder::
on_ySliceCheckBox_stateChanged( int state )
{
	m_RecordY = state;
}

/**
 * \brief Function called when checkbox state changes
 */
void
DialogRecorder::
on_zSliceCheckBox_stateChanged( int state )
{
	m_RecordZ = state;
}

/**
 * \brief Function called when checkbox state changes
 */
void
DialogRecorder::
on_tSliceCheckBox_stateChanged( int state )
{
	m_RecordT = state;
}

/**
 * \brief Function called when first slice changes
 */
void
DialogRecorder::
on_xSpinMin_valueChanged( int value )
{
	m_XMin = value;
}

/**
 * \brief Function called when first slice changes
 */
void
DialogRecorder::
on_ySpinMin_valueChanged( int value )
{
	m_YMin = value;
}
/**
 * \brief Function called when first slice changes
 */
void
DialogRecorder::
on_zSpinMin_valueChanged( int value )
{
	m_ZMin = value;
}

/**
 * \brief Function called when first slice changes
 */
void
DialogRecorder::
on_tSpinMin_valueChanged( int value )
{
	m_TMin = value;
}

/**
 * \brief Function called when last slice changes
 */
void
DialogRecorder::
on_xSpinMax_valueChanged( int value )
{
	m_XMax = value;
}

/**
 * \brief Function called when last slice changes
 */
void
DialogRecorder::
on_ySpinMax_valueChanged( int value )
{
	m_YMax = value;
}
/**
 * \brief Function called when last slice changes
 */
void
DialogRecorder::
on_zSpinMax_valueChanged( int value )
{
	m_ZMax = value;
}

/**
 * \brief Function called when last slice changes
 */
void
DialogRecorder::
on_tSpinMax_valueChanged( int value )
{
	m_TMax = value;
}

/**
 * \brief Get and print the location to store MegaCapture file
 */
void
DialogRecorder::
on_createFile_clicked()
{
	m_VideoName = QFileDialog::getSaveFileName( this,
      tr( "Folder to Save Video" ), "fileName", 0 );

  this->videoName->setText( m_VideoName );
}

/**
 * \brief Function called when FrameRate changes
 */
void
DialogRecorder::
on_frameRate_valueChanged( int value )
{
	m_FrameRate = value;
}

/**
 * \brief Function called when VideoQuality changes
 */
void
DialogRecorder::
on_videoQuality_valueChanged( int value )
{
	m_VideoQuality = value;
}


/**
 * \brief Function called when "Create video" clicked
 */

void
DialogRecorder::
on_startVideo_clicked()
{
  // Print parameters for testings
	std::cout<<"m_XMin: "<< m_XMin <<std::endl;
	std::cout<<"m_XMax: "<< m_XMax <<std::endl;
	std::cout<<"m_YMin: "<< m_YMin <<std::endl;
	std::cout<<"m_YMax: "<< m_YMax <<std::endl;
	std::cout<<"m_ZMin: "<< m_ZMin <<std::endl;
	std::cout<<"m_ZMax: "<< m_ZMax <<std::endl;
	std::cout<<"m_TMin: "<< m_TMin <<std::endl;
	std::cout<<"m_TMax: "<< m_TMax <<std::endl;
	std::cout<<"m_RecordX: "<< m_RecordX <<std::endl;
	std::cout<<"m_RecordY: "<< m_RecordY <<std::endl;
	std::cout<<"m_RecordZ: "<< m_RecordZ <<std::endl;
	std::cout<<"m_RecordT: "<< m_RecordT <<std::endl;
	std::cout<<"m_FrameRate: "<< m_FrameRate <<std::endl;
	std::cout<<"m_VideoQuality: "<< m_VideoQuality <<std::endl;

	//Create a QTDialogProgress

	//...

	// for X Slices
  if(m_RecordX>0)
  	{
    QString fileName = m_VideoName;

    fileName.insert( fileName.size(), QString("-X-"));
    fileName.insert( fileName.size(), QString::number( m_XMin, 10 ) );
    fileName.insert( fileName.size(), QString("-"));
    fileName.insert( fileName.size(), QString::number( m_XMax, 10 ) );
    fileName.insert( fileName.size(), QString(".avi"));

    m_VideoRecorder->SetFileName( fileName.toStdString() );
    std::cout<<"FileName X: "<< fileName.toStdString() << std::endl;

	  m_VideoRecorder->StartCapture();
    //...
	  for(int i = m_XMin; i < m_XMax+1; i++)
	  	{
	    //send signal to gofigure

	    //...

	    //capture screen
	    m_VideoRecorder->TakeSnapshot();
	  	}
    //...
	  m_VideoRecorder->EndCapture();
  	}

}
