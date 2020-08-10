namespace CameraCapture
{
    partial class CameraCapture
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            ReleaseData();
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.captureButton = new System.Windows.Forms.Button();
            this.splitContainer2 = new System.Windows.Forms.SplitContainer();
            this.splitContainer3 = new System.Windows.Forms.SplitContainer();
            this.captureImageBox = new Emgu.CV.UI.ImageBox();
            this.panel1 = new System.Windows.Forms.Panel();
            this.label1 = new System.Windows.Forms.Label();
            this.smoothedGrayscaleImageBox = new Emgu.CV.UI.ImageBox();
            this.panel3 = new System.Windows.Forms.Panel();
            this.label3 = new System.Windows.Forms.Label();
            this.splitContainer4 = new System.Windows.Forms.SplitContainer();
            this.label7 = new System.Windows.Forms.Label();
            this.comboBox1 = new System.Windows.Forms.ComboBox();
            this.cannyThreshold2Box = new System.Windows.Forms.TextBox();
            this.cannyThreshold1Box = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.CannyThreshold2 = new System.Windows.Forms.TrackBar();
            this.label5 = new System.Windows.Forms.Label();
            this.CannyThreshold1 = new System.Windows.Forms.TrackBar();
            this.contrastBox = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.contrastBar = new System.Windows.Forms.TrackBar();
            this.cannyImageBox = new Emgu.CV.UI.ImageBox();
            this.panel4 = new System.Windows.Forms.Panel();
            this.label4 = new System.Windows.Forms.Label();
            this.Take_Picture = new System.Windows.Forms.Button();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.splitContainer2.Panel1.SuspendLayout();
            this.splitContainer2.Panel2.SuspendLayout();
            this.splitContainer2.SuspendLayout();
            this.splitContainer3.Panel1.SuspendLayout();
            this.splitContainer3.Panel2.SuspendLayout();
            this.splitContainer3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.captureImageBox)).BeginInit();
            this.panel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.smoothedGrayscaleImageBox)).BeginInit();
            this.panel3.SuspendLayout();
            this.splitContainer4.Panel1.SuspendLayout();
            this.splitContainer4.Panel2.SuspendLayout();
            this.splitContainer4.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.CannyThreshold2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.CannyThreshold1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.contrastBar)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.cannyImageBox)).BeginInit();
            this.panel4.SuspendLayout();
            this.SuspendLayout();
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 0);
            this.splitContainer1.Name = "splitContainer1";
            this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.Take_Picture);
            this.splitContainer1.Panel1.Controls.Add(this.captureButton);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.splitContainer2);
            this.splitContainer1.Size = new System.Drawing.Size(842, 832);
            this.splitContainer1.SplitterDistance = 43;
            this.splitContainer1.TabIndex = 0;
            // 
            // captureButton
            // 
            this.captureButton.Location = new System.Drawing.Point(15, 12);
            this.captureButton.Name = "captureButton";
            this.captureButton.Size = new System.Drawing.Size(102, 23);
            this.captureButton.TabIndex = 0;
            this.captureButton.Text = "Start Capture";
            this.captureButton.UseVisualStyleBackColor = true;
            this.captureButton.Click += new System.EventHandler(this.captureButtonClick);
            // 
            // splitContainer2
            // 
            this.splitContainer2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer2.Location = new System.Drawing.Point(0, 0);
            this.splitContainer2.Name = "splitContainer2";
            // 
            // splitContainer2.Panel1
            // 
            this.splitContainer2.Panel1.Controls.Add(this.splitContainer3);
            // 
            // splitContainer2.Panel2
            // 
            this.splitContainer2.Panel2.Controls.Add(this.splitContainer4);
            this.splitContainer2.Size = new System.Drawing.Size(842, 785);
            this.splitContainer2.SplitterDistance = 410;
            this.splitContainer2.TabIndex = 0;
            // 
            // splitContainer3
            // 
            this.splitContainer3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer3.Location = new System.Drawing.Point(0, 0);
            this.splitContainer3.Name = "splitContainer3";
            this.splitContainer3.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer3.Panel1
            // 
            this.splitContainer3.Panel1.Controls.Add(this.captureImageBox);
            this.splitContainer3.Panel1.Controls.Add(this.panel1);
            // 
            // splitContainer3.Panel2
            // 
            this.splitContainer3.Panel2.Controls.Add(this.smoothedGrayscaleImageBox);
            this.splitContainer3.Panel2.Controls.Add(this.panel3);
            this.splitContainer3.Size = new System.Drawing.Size(410, 785);
            this.splitContainer3.SplitterDistance = 378;
            this.splitContainer3.TabIndex = 0;
            // 
            // captureImageBox
            // 
            this.captureImageBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.captureImageBox.Location = new System.Drawing.Point(0, 45);
            this.captureImageBox.Name = "captureImageBox";
            this.captureImageBox.Size = new System.Drawing.Size(410, 333);
            this.captureImageBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.CenterImage;
            this.captureImageBox.TabIndex = 1;
            this.captureImageBox.TabStop = false;
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.label1);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(410, 45);
            this.panel1.TabIndex = 0;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 10);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(85, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Captured Image:";
            // 
            // smoothedGrayscaleImageBox
            // 
            this.smoothedGrayscaleImageBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.smoothedGrayscaleImageBox.Location = new System.Drawing.Point(0, 36);
            this.smoothedGrayscaleImageBox.Name = "smoothedGrayscaleImageBox";
            this.smoothedGrayscaleImageBox.Size = new System.Drawing.Size(410, 367);
            this.smoothedGrayscaleImageBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.CenterImage;
            this.smoothedGrayscaleImageBox.TabIndex = 1;
            this.smoothedGrayscaleImageBox.TabStop = false;
            // 
            // panel3
            // 
            this.panel3.Controls.Add(this.label3);
            this.panel3.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel3.Location = new System.Drawing.Point(0, 0);
            this.panel3.Name = "panel3";
            this.panel3.Size = new System.Drawing.Size(410, 36);
            this.panel3.TabIndex = 0;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 12);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(42, 13);
            this.label3.TabIndex = 0;
            this.label3.Text = "Output:";
            // 
            // splitContainer4
            // 
            this.splitContainer4.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer4.Location = new System.Drawing.Point(0, 0);
            this.splitContainer4.Name = "splitContainer4";
            this.splitContainer4.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer4.Panel1
            // 
            this.splitContainer4.Panel1.Controls.Add(this.label7);
            this.splitContainer4.Panel1.Controls.Add(this.comboBox1);
            this.splitContainer4.Panel1.Controls.Add(this.cannyThreshold2Box);
            this.splitContainer4.Panel1.Controls.Add(this.cannyThreshold1Box);
            this.splitContainer4.Panel1.Controls.Add(this.label6);
            this.splitContainer4.Panel1.Controls.Add(this.CannyThreshold2);
            this.splitContainer4.Panel1.Controls.Add(this.label5);
            this.splitContainer4.Panel1.Controls.Add(this.CannyThreshold1);
            this.splitContainer4.Panel1.Controls.Add(this.contrastBox);
            this.splitContainer4.Panel1.Controls.Add(this.label2);
            this.splitContainer4.Panel1.Controls.Add(this.contrastBar);
            // 
            // splitContainer4.Panel2
            // 
            this.splitContainer4.Panel2.Controls.Add(this.cannyImageBox);
            this.splitContainer4.Panel2.Controls.Add(this.panel4);
            this.splitContainer4.Size = new System.Drawing.Size(428, 785);
            this.splitContainer4.SplitterDistance = 378;
            this.splitContainer4.TabIndex = 0;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(11, 122);
            this.label7.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(76, 13);
            this.label7.TabIndex = 12;
            this.label7.Text = "Camera Select";
            // 
            // comboBox1
            // 
            this.comboBox1.FormattingEnabled = true;
            this.comboBox1.Items.AddRange(new object[] {
            "0",
            "1"});
            this.comboBox1.Location = new System.Drawing.Point(106, 119);
            this.comboBox1.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.comboBox1.Name = "comboBox1";
            this.comboBox1.Size = new System.Drawing.Size(188, 21);
            this.comboBox1.TabIndex = 11;
            this.comboBox1.SelectedIndexChanged += new System.EventHandler(this.comboBox1_SelectedIndexChanged);
            // 
            // cannyThreshold2Box
            // 
            this.cannyThreshold2Box.Location = new System.Drawing.Point(340, 78);
            this.cannyThreshold2Box.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.cannyThreshold2Box.Name = "cannyThreshold2Box";
            this.cannyThreshold2Box.Size = new System.Drawing.Size(76, 20);
            this.cannyThreshold2Box.TabIndex = 10;
            // 
            // cannyThreshold1Box
            // 
            this.cannyThreshold1Box.Location = new System.Drawing.Point(340, 47);
            this.cannyThreshold1Box.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.cannyThreshold1Box.Name = "cannyThreshold1Box";
            this.cannyThreshold1Box.Size = new System.Drawing.Size(76, 20);
            this.cannyThreshold1Box.TabIndex = 9;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(11, 83);
            this.label6.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(96, 13);
            this.label6.TabIndex = 8;
            this.label6.Text = "Canny Threshold 2";
            // 
            // CannyThreshold2
            // 
            this.CannyThreshold2.Location = new System.Drawing.Point(106, 78);
            this.CannyThreshold2.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.CannyThreshold2.Maximum = 300;
            this.CannyThreshold2.Name = "CannyThreshold2";
            this.CannyThreshold2.Size = new System.Drawing.Size(229, 45);
            this.CannyThreshold2.TabIndex = 7;
            this.CannyThreshold2.Value = 100;
            this.CannyThreshold2.ValueChanged += new System.EventHandler(this.CannyTreshold_2_ValueChanged);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(11, 52);
            this.label5.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(96, 13);
            this.label5.TabIndex = 6;
            this.label5.Text = "Canny Threshold 1";
            // 
            // CannyThreshold1
            // 
            this.CannyThreshold1.Location = new System.Drawing.Point(106, 45);
            this.CannyThreshold1.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.CannyThreshold1.Maximum = 300;
            this.CannyThreshold1.Name = "CannyThreshold1";
            this.CannyThreshold1.Size = new System.Drawing.Size(229, 45);
            this.CannyThreshold1.TabIndex = 5;
            this.CannyThreshold1.Value = 180;
            this.CannyThreshold1.ValueChanged += new System.EventHandler(this.CannyTreshold_1_ValueChanged);
            // 
            // contrastBox
            // 
            this.contrastBox.Location = new System.Drawing.Point(340, 14);
            this.contrastBox.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.contrastBox.Name = "contrastBox";
            this.contrastBox.Size = new System.Drawing.Size(76, 20);
            this.contrastBox.TabIndex = 3;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(32, 19);
            this.label2.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(46, 13);
            this.label2.TabIndex = 4;
            this.label2.Text = "Contrast";
            // 
            // contrastBar
            // 
            this.contrastBar.Location = new System.Drawing.Point(106, 10);
            this.contrastBar.Maximum = 100;
            this.contrastBar.Name = "contrastBar";
            this.contrastBar.Size = new System.Drawing.Size(229, 45);
            this.contrastBar.TabIndex = 3;
            this.contrastBar.Value = 40;
            this.contrastBar.ValueChanged += new System.EventHandler(this.contrastBar_ValueChanged);
            this.contrastBar.MouseDown += new System.Windows.Forms.MouseEventHandler(this.contrastBar_MouseDown);
            // 
            // cannyImageBox
            // 
            this.cannyImageBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.cannyImageBox.Location = new System.Drawing.Point(0, 35);
            this.cannyImageBox.Name = "cannyImageBox";
            this.cannyImageBox.Size = new System.Drawing.Size(428, 368);
            this.cannyImageBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.CenterImage;
            this.cannyImageBox.TabIndex = 1;
            this.cannyImageBox.TabStop = false;
            // 
            // panel4
            // 
            this.panel4.Controls.Add(this.label4);
            this.panel4.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel4.Location = new System.Drawing.Point(0, 0);
            this.panel4.Name = "panel4";
            this.panel4.Size = new System.Drawing.Size(428, 35);
            this.panel4.TabIndex = 0;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(17, 10);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(73, 13);
            this.label4.TabIndex = 0;
            this.label4.Text = "Canny Edges:";
            // 
            // Take_Picture
            // 
            this.Take_Picture.Location = new System.Drawing.Point(132, 12);
            this.Take_Picture.Name = "Take_Picture";
            this.Take_Picture.Size = new System.Drawing.Size(95, 23);
            this.Take_Picture.TabIndex = 1;
            this.Take_Picture.Text = "Take Picture";
            this.Take_Picture.UseVisualStyleBackColor = true;
            this.Take_Picture.Click += new System.EventHandler(this.Take_Picture_Click);
            // 
            // CameraCapture
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(842, 832);
            this.Controls.Add(this.splitContainer1);
            this.Name = "CameraCapture";
            this.Text = "Camera Capture";
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.ResumeLayout(false);
            this.splitContainer2.Panel1.ResumeLayout(false);
            this.splitContainer2.Panel2.ResumeLayout(false);
            this.splitContainer2.ResumeLayout(false);
            this.splitContainer3.Panel1.ResumeLayout(false);
            this.splitContainer3.Panel2.ResumeLayout(false);
            this.splitContainer3.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.captureImageBox)).EndInit();
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.smoothedGrayscaleImageBox)).EndInit();
            this.panel3.ResumeLayout(false);
            this.panel3.PerformLayout();
            this.splitContainer4.Panel1.ResumeLayout(false);
            this.splitContainer4.Panel1.PerformLayout();
            this.splitContainer4.Panel2.ResumeLayout(false);
            this.splitContainer4.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.CannyThreshold2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.CannyThreshold1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.contrastBar)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.cannyImageBox)).EndInit();
            this.panel4.ResumeLayout(false);
            this.panel4.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.Button captureButton;
        private System.Windows.Forms.SplitContainer splitContainer2;
        private System.Windows.Forms.SplitContainer splitContainer3;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.SplitContainer splitContainer4;
        private System.Windows.Forms.Panel panel3;
        private System.Windows.Forms.Panel panel4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private Emgu.CV.UI.ImageBox captureImageBox;
        private Emgu.CV.UI.ImageBox smoothedGrayscaleImageBox;
        private Emgu.CV.UI.ImageBox cannyImageBox;
        private System.Windows.Forms.TrackBar contrastBar;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox contrastBox;
        private System.Windows.Forms.TrackBar CannyThreshold2;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TrackBar CannyThreshold1;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox cannyThreshold2Box;
        private System.Windows.Forms.TextBox cannyThreshold1Box;
        private System.Windows.Forms.ComboBox comboBox1;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Button Take_Picture;
    }
}

