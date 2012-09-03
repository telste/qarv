/*
    qarv, a Qt interface to aravis.
    Copyright (C) 2012  Jure Varlec <jure.varlec@ad-vega.si>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef ARCAM_H
#define ARCAM_H

#include <QList>
#include <QString>
#include <QRect>
#include <QPair>
#include <QMetaType>
#include <QImage>
#include <QHostAddress>

//! Initialize glib and aravis. Call this once in the main program.
void arcamInit();

//! \name Forward declarations to avoid exposing arv.h
/**@{*/
struct _ArvCamera;
typedef _ArvCamera ArvCamera;
struct _ArvDevice;
typedef _ArvDevice ArvDevice;
struct _ArvBuffer;
typedef _ArvBuffer ArvBuffer;
struct _ArvStream;
typedef _ArvStream ArvStream;
/**@}*/

//! Objects of this class are used to identify cameras.
/*!
 * This class exposes three public strings containing the internal
 * Aravis id of the camera and the name of the camera vendor and model.
 * These strings are owned by the instance, not by Aravis.
 */
class ArCamId {
public:
  ArCamId();
  ArCamId(const char* id, const char* vendor, const char* model);
  ArCamId(const ArCamId& camid);
  ~ArCamId();
  const char *id, *vendor, *model;
};

Q_DECLARE_METATYPE(ArCamId)


//! ArCam provides an interface to an Aravis camera.
/*!
 * This class is mostly a thin wrapper around the arv_camera interface.
 * Only the parts that differ significantly from that interface are documented.
 * The arcamInit() function must be called once in the main program before
 * this class is used.
 */
class ArCam : public QObject {
  Q_OBJECT

public:
  //! A camera with the given ID is opened.
  ArCam(ArCamId id, QObject* parent = NULL);
  ~ArCam();

  static QList<ArCamId> listCameras(); //!< Returns a list of all cameras found.

  ArCamId getId(); //!< Returns the ID of the camera.

  //! \name Manipulate region of interest
  //@{
  QRect getROI();
  void setROI(QRect roi);
  QRect getROIMaxSize();
  //@}

  //! \name Manipulate pixel binning
  /**@{*/
  QSize getBinning();
  void setBinning(QSize bin);
  /**@}*/

  //! \name Choose pixel format
  /**@{*/
  QList<QString> getPixelFormats();
  QList<QString> getPixelFormatNames();
  QString getPixelFormat();
  void setPixelFormat(QString format);
  /**@}*/

  //! \name Manipulate frames-per-second.
  /**@{*/
  double getFPS();
  void setFPS(double fps);
  /**@}*/

  //! \name Manipulate exposure time (in microseconds)
  /**@{*/
  double getExposure();
  void setExposure(double exposure);
  QPair<double, double> getExposureLimits();
  bool hasAutoExposure();
  void setAutoExposure(bool enable);
  /**@}*/

  //! \name Manipulate sensor gain
  /**@{*/
  double getGain();
  void setGain(double gain);
  QPair<double, double> getGainLimits();
  bool hasAutoGain();
  void setAutoGain(bool enable);
  /**@}*/

  //! \name Start or stop acquisition
  /**@{*/
  void startAcquisition();
  void stopAcquisition();
  /**@}*/

  //! \name Get a captured frame
  /**@{*/
  QSize getFrameSize();
  QByteArray getFrame(bool dropInvalid = false);
  /**@}*/

  //! \name Manipulate network parameters of an ethernet camera
  /**@{*/
  int getMTU();
  void setMTU(int mtu);
  QHostAddress getIP();
  QHostAddress getHostIP();
  int getEstimatedBW();
  /**@}*/

signals:
  //! Emitted when a new frame is ready.
  void frameReady();

private:
  void swapBuffers();
  static QList<ArCamId> cameraList;
  ArvCamera* camera;
  ArvDevice* device;
  ArvStream* stream;
  ArvBuffer* currentFrame;
  bool acquiring;

  friend void streamCallback(ArvStream* stream, ArCam* cam);
};

#endif // ARCAM_H
