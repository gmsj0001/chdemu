namespace PacketSniffer
{
    partial class FrmSniffer
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
            this.hexBox = new Be.Windows.Forms.HexBox();
            this.lstPacket = new System.Windows.Forms.ListBox();
            this.btnStart = new System.Windows.Forms.Button();
            this.btnStop = new System.Windows.Forms.Button();
            this.btnSave = new System.Windows.Forms.Button();
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.btnClear = new System.Windows.Forms.Button();
            this.timInject = new System.Windows.Forms.Timer(this.components);
            this.timPacket = new System.Windows.Forms.Timer(this.components);
            this.SuspendLayout();
            // 
            // hexBox
            // 
            this.hexBox.Font = new System.Drawing.Font("新宋体", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.hexBox.LineInfoForeColor = System.Drawing.Color.Empty;
            this.hexBox.LineInfoVisible = true;
            this.hexBox.Location = new System.Drawing.Point(184, 11);
            this.hexBox.Name = "hexBox";
            this.hexBox.ShadowSelectionColor = System.Drawing.Color.FromArgb(((int)(((byte)(100)))), ((int)(((byte)(60)))), ((int)(((byte)(188)))), ((int)(((byte)(255)))));
            this.hexBox.Size = new System.Drawing.Size(485, 244);
            this.hexBox.StringViewVisible = true;
            this.hexBox.TabIndex = 0;
            this.hexBox.UseFixedBytesPerLine = true;
            this.hexBox.VScrollBarVisible = true;
            // 
            // lstPacket
            // 
            this.lstPacket.FormattingEnabled = true;
            this.lstPacket.ItemHeight = 12;
            this.lstPacket.Location = new System.Drawing.Point(12, 11);
            this.lstPacket.Name = "lstPacket";
            this.lstPacket.Size = new System.Drawing.Size(166, 292);
            this.lstPacket.TabIndex = 1;
            this.lstPacket.Click += new System.EventHandler(this.lstPacket_Click);
            // 
            // btnStart
            // 
            this.btnStart.Location = new System.Drawing.Point(190, 271);
            this.btnStart.Name = "btnStart";
            this.btnStart.Size = new System.Drawing.Size(75, 21);
            this.btnStart.TabIndex = 2;
            this.btnStart.Text = "Start";
            this.btnStart.UseVisualStyleBackColor = true;
            this.btnStart.Click += new System.EventHandler(this.btnStart_Click);
            // 
            // btnStop
            // 
            this.btnStop.Enabled = false;
            this.btnStop.Location = new System.Drawing.Point(271, 271);
            this.btnStop.Name = "btnStop";
            this.btnStop.Size = new System.Drawing.Size(75, 21);
            this.btnStop.TabIndex = 3;
            this.btnStop.Text = "Stop";
            this.btnStop.UseVisualStyleBackColor = true;
            this.btnStop.Click += new System.EventHandler(this.btnStop_Click);
            // 
            // btnSave
            // 
            this.btnSave.Location = new System.Drawing.Point(590, 271);
            this.btnSave.Name = "btnSave";
            this.btnSave.Size = new System.Drawing.Size(75, 23);
            this.btnSave.TabIndex = 5;
            this.btnSave.Text = "Save";
            this.btnSave.UseVisualStyleBackColor = true;
            this.btnSave.Click += new System.EventHandler(this.btnSave_Click);
            // 
            // btnClear
            // 
            this.btnClear.Location = new System.Drawing.Point(509, 271);
            this.btnClear.Name = "btnClear";
            this.btnClear.Size = new System.Drawing.Size(75, 23);
            this.btnClear.TabIndex = 6;
            this.btnClear.Text = "Clear";
            this.btnClear.UseVisualStyleBackColor = true;
            this.btnClear.Click += new System.EventHandler(this.btnClear_Click);
            // 
            // timInject
            // 
            this.timInject.Enabled = true;
            this.timInject.Interval = 1000;
            this.timInject.Tick += new System.EventHandler(this.timInject_Tick);
            // 
            // timPacket
            // 
            this.timPacket.Enabled = true;
            this.timPacket.Tick += new System.EventHandler(this.timPacket_Tick);
            // 
            // FrmSniffer
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(677, 312);
            this.Controls.Add(this.btnClear);
            this.Controls.Add(this.btnSave);
            this.Controls.Add(this.btnStop);
            this.Controls.Add(this.btnStart);
            this.Controls.Add(this.lstPacket);
            this.Controls.Add(this.hexBox);
            this.Name = "FrmSniffer";
            this.Text = "PacketSniffer";
            this.ResumeLayout(false);

        }

        #endregion

        private Be.Windows.Forms.HexBox hexBox;
        private System.Windows.Forms.ListBox lstPacket;
        private System.Windows.Forms.Button btnStart;
        private System.Windows.Forms.Button btnStop;
        private System.Windows.Forms.Button btnSave;
        private System.Windows.Forms.SaveFileDialog saveFileDialog1;
        private System.Windows.Forms.Button btnClear;
        private System.Windows.Forms.Timer timInject;
        private System.Windows.Forms.Timer timPacket;
    }
}