//----------------------------------------------------------------------------
//  Copyright (C) 2004-2016 by EMGU Corporation. All rights reserved.       
//----------------------------------------------------------------------------

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Emgu.CV;
using Emgu.Util;
using Emgu.CV.CvEnum;
using Emgu.CV.Structure;
using System.Threading;

using DirectShowLib;
//using AForge;
using AForge.Video.DirectShow;

using Emgu.CV.UI;

namespace CameraCapture
{
   public partial class CameraCapture : Form
   {
      private VideoCapture _Camcapture;
      private bool _captureInProgress;
      private Mat _frame;
      private Mat _grayFrame;
      private Mat _smallGrayFrame;
      private Mat _smoothedGrayFrame;
      private Mat _cannyFrame;
        int cannyThreshold_1;
        int cannyThreshold_2;
        private Mat _imageOutput;
        int contrast = 0;
        int CameraDevice = 0;
        private FilterInfoCollection videoDevices;
        private VideoCaptureDevice VideoSource = null;
        private bool DeviceExist = false;

        public CameraCapture()
      {
         InitializeComponent();
         getCamList();
         _frame = new Mat();
         _grayFrame = new Mat();
         _smallGrayFrame = new Mat();
         _smoothedGrayFrame = new Mat();
         _cannyFrame = new Mat();
         _imageOutput = new Mat();
       }

        private void getCamList()
        {
            try
            {
                videoDevices = new FilterInfoCollection(AForge.Video.DirectShow.FilterCategory.VideoInputDevice);
                comboBox1.Items.Clear();
                if (videoDevices.Count == 0)
                    throw new ApplicationException();

                DeviceExist = true;
                foreach (AForge.Video.DirectShow.FilterInfo device in videoDevices)
                {
                    comboBox1.Items.Add(device.Name);
                }
                comboBox1.SelectedIndex = 0; //make dafault to first cam
            }
            catch (ApplicationException)
            {
                DeviceExist = false;
                comboBox1.Items.Add("No capture device on your system");
            }
        }

   private void ProcessFrame(object sender, EventArgs arg)
      {
         if (_Camcapture != null && _Camcapture.Ptr != IntPtr.Zero)
         {
            _Camcapture.Retrieve(_frame, 0);
                
                //lineImage = new Mat();
           

            Image<Bgr, Byte> lineImage = new Image<Bgr, byte>(_frame.Width,_frame.Height);
            Image<Bgr, Byte> OutputImage = new Image<Bgr, byte>(_frame.Width, _frame.Height);
                CvInvoke.CvtColor(_frame, _grayFrame, ColorConversion.Bgr2Gray);

                //CvInvoke.PyrDown(_grayFrame, _smallGrayFrame);
                CvInvoke.GaussianBlur(_grayFrame, _imageOutput, new Size(5, 5), 1.5);

                //CvInvoke.PyrUp(_smallGrayFrame, _smoothedGrayFrame);
                // Image<Bgr, Byte>  = new Image<Bgr, byte>(_frame);

                // Image<Bgr, Byte> houghImage = img.CopyBlank();
                //CvInvoke.Canny(_smoothedGrayFrame, _cannyFrame, 100, 60);
               
                CvInvoke.Canny(_imageOutput, _cannyFrame, cannyThreshold_1, cannyThreshold_2);
               
                using (Emgu.CV.Util.VectorOfVectorOfPoint contours = new Emgu.CV.Util.VectorOfVectorOfPoint())
                {
                    CvInvoke.FindContours(_cannyFrame, contours, null, RetrType.List, ChainApproxMethod.ChainApproxSimple);
                    int count = contours.Size;
                    //Console.WriteLine("count is " + count);
                    //int numlines = 0;
                    for (int j = 0; j < count; j++)
                        using (Emgu.CV.Util.VectorOfPoint contour = contours[j])
                        using (Emgu.CV.Util.VectorOfPoint approxContour = new Emgu.CV.Util.VectorOfPoint())
                        {
                            CvInvoke.ApproxPolyDP(contour, approxContour, CvInvoke.ArcLength(contour, true) * 0.05, true);
                            //if (CvInvoke.ContourArea(approxContour, false) > 250)
                            {
                                //Console.WriteLine("contour " + j + "is" + contour.Size);
                                //Console.WriteLine()

                                //if (approxContour.Size == 2)
                                {
                                    // Console.WriteLine("found line");
                                    Point[] points = approxContour.ToArray();
                                    Point[] pts = contour.ToArray();
                                    LineSegment2D[] edges = PointCollection.PolyLine(pts, true);
                                    foreach (Point point in points)
                                    {
                                        //if (contour.Size < 50)
                                        {
                                            //Console.WriteLine("the line is");
                                            //Console.WriteLine("x = " + point.X);
                                            //Console.WriteLine("y = " + point.Y);
                                            //Console.WriteLine(line.Length);                        
                                            //CvInvoke.Line(lineImage, new Point(point.X, point.Y), new Point(point.X, point.Y), new MCvScalar(255, 0, 0), 4);
                                        }
                                    }
                                    //if (approxContour.Size == 2)
                                    {
                                        foreach (LineSegment2D line in edges)
                                        {
                                            //if (line.Length > 10)// && line.P1.Y > 1500 && line.P1.Y < 2500)
                                            {
                                                lineImage.Draw(line, new Bgr(Color.Red), 2);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    //Console.WriteLine("contour found " + numlines + " lines");
                }
            
                //CvInvoke.Canny(lineImage, _cannyFrame, cannyThreshold_1, cannyThreshold_2);
                LineSegment2D[] lines = CvInvoke.HoughLinesP(_cannyFrame, 1, Math.PI / 45, 20, 30, 5);
                foreach (LineSegment2D line in lines)
                //if (line.Length > 50 && line.P1.Y > 1500 && line.P1.Y < 2500)
                {
                    OutputImage.Draw(line, new Bgr(Color.Green), 2);
                }
                captureImageBox.Image = _frame;
            smoothedGrayscaleImageBox.Image = _cannyFrame;
            //smoothedGrayscaleImageBox.Image = _smoothedGrayFrame;
            cannyImageBox.Image = lineImage;
         }
      }

        private void captureButtonClick(object sender, EventArgs e)
      {
            if (_Camcapture != null)
            {
                if (_captureInProgress)
                {  //stop the capture
                    captureButton.Text = "Start Capture";
                    _Camcapture.Pause();
                    _captureInProgress = false;
                }
                else
                {
                    if (comboBox1.SelectedIndex != CameraDevice)
                    {
                        SetupVidCapture(comboBox1.SelectedIndex); //Setup capture with the new device
                    }
                    //start the capture
                    contrast = contrastBar.Value;
                    _Camcapture.SetCaptureProperty(CapProp.Contrast, contrast);
                    cannyThreshold_1 = CannyThreshold1.Value;
                    cannyThreshold_2 = CannyThreshold2.Value;
                    _Camcapture.SetCaptureProperty(CapProp.Sharpness, 1000);
                    captureButton.Text = "Stop";
                    _Camcapture.Start();
                    _Camcapture.ImageGrabbed += ProcessFrame;
                    
                    _captureInProgress = true;

                }
            }
            else
            {
                SetupVidCapture(comboBox1.SelectedIndex);
            }
      }
        private void Take_Picture_Click(object sender, EventArgs e)
        {
            if (_Camcapture != null)
            { 
                    if (comboBox1.SelectedIndex != CameraDevice)
                    {
                        SetupVidCapture(comboBox1.SelectedIndex); //Setup capture with the new device
                    }
                    //start the capture
                    contrast = contrastBar.Value;
                    _Camcapture.SetCaptureProperty(CapProp.Contrast, contrast);
                    cannyThreshold_1 = CannyThreshold1.Value;
                    cannyThreshold_2 = CannyThreshold2.Value;
                    _Camcapture.SetCaptureProperty(CapProp.Sharpness, 1000);
                    Take_Picture.Text = "Taking picture";
                    _Camcapture.Start();
                    _Camcapture.ImageGrabbed += ProcessFrame;
                    Thread.Sleep(250);
                    _Camcapture.Stop();
                    Take_Picture.Text = "Take Picture";
            }
            else
            {
                SetupVidCapture(comboBox1.SelectedIndex);
            }
        }
        private void ReleaseData()
      {
         if (_Camcapture != null)
            _Camcapture.Dispose();
      }

        private void SetupVidCapture(int Camera_Identifier)
        {
            //update the selected device
            CameraDevice = Camera_Identifier;

            //Dispose of Capture if it was created before
            if (_Camcapture != null) _Camcapture.Dispose();
            try
            {
                //Set up capture device
                _Camcapture = new VideoCapture(CameraDevice);
            }
            catch (NullReferenceException excpt)
            {
                MessageBox.Show(excpt.Message);
            }
        }
   
        private void contrastBar_MouseDown(object sender, MouseEventArgs e)
        {
            contrast = contrastBar.Value;
            _Camcapture.SetCaptureProperty(CapProp.Contrast, contrast);
        }

        private void contrastBar_ValueChanged(object sender, EventArgs e)
        {
            contrast = contrastBar.Value;
            _Camcapture.SetCaptureProperty(CapProp.Contrast, contrast);
            contrastBox.Text = "" + contrastBar.Value;
        }

        private void CannyTreshold_1_ValueChanged(object sender, EventArgs e)
        {
            cannyThreshold_1 = CannyThreshold1.Value;
            cannyThreshold1Box.Text = "" + CannyThreshold1.Value;
        }

        private void CannyTreshold_2_ValueChanged(object sender, EventArgs e)
        {
            cannyThreshold_2 = CannyThreshold2.Value;
            cannyThreshold2Box.Text = "" + CannyThreshold2.Value;
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            //CameraDevice = comboBox1.SelectedIndex;
            //_capture.Pause();
            if(_captureInProgress == true)
                captureButton.Text = "Start Capture";
            SetupVidCapture(comboBox1.SelectedIndex);
 
            //_capture.Start();
        }

     
    }
}
