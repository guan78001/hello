namespace FontTessellation
{
    partial class TesselationDemo
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
            this.lbFont = new System.Windows.Forms.ListBox();
            this.label1 = new System.Windows.Forms.Label();
            this.tbGlyph = new System.Windows.Forms.TrackBar();
            this.label2 = new System.Windows.Forms.Label();
            this.panel1 = new System.Windows.Forms.Panel();
            this.label3 = new System.Windows.Forms.Label();
            this.mtbGlyph = new System.Windows.Forms.MaskedTextBox();
            this.btnGO = new System.Windows.Forms.Button();
            this.demo = new FontTessellation.Tessellator();
            ((System.ComponentModel.ISupportInitialize)(this.tbGlyph)).BeginInit();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // lbFont
            // 
            this.lbFont.FormattingEnabled = true;
            this.lbFont.Location = new System.Drawing.Point(12, 28);
            this.lbFont.Name = "lbFont";
            this.lbFont.Size = new System.Drawing.Size(175, 225);
            this.lbFont.TabIndex = 1;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 12);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(70, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = "Select a font:";
            // 
            // tbGlyph
            // 
            this.tbGlyph.LargeChange = 32;
            this.tbGlyph.Location = new System.Drawing.Point(9, 274);
            this.tbGlyph.Maximum = 255;
            this.tbGlyph.Minimum = 33;
            this.tbGlyph.Name = "tbGlyph";
            this.tbGlyph.Size = new System.Drawing.Size(175, 42);
            this.tbGlyph.TabIndex = 10;
            this.tbGlyph.Value = 65;
            this.tbGlyph.Scroll += new System.EventHandler(this.tbGlyph_Scroll);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(9, 258);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(154, 13);
            this.label2.TabIndex = 11;
            this.label2.Text = "Use the slider to select a glyph:";
            // 
            // panel1
            // 
            this.panel1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.panel1.Controls.Add(this.demo);
            this.panel1.Location = new System.Drawing.Point(193, 12);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(604, 424);
            this.panel1.TabIndex = 12;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 320);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(167, 13);
            this.label3.TabIndex = 13;
            this.label3.Text = "or type a character and press GO:";
            // 
            // mtbGlyph
            // 
            this.mtbGlyph.Font = new System.Drawing.Font("Microsoft Sans Serif", 16F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.mtbGlyph.Location = new System.Drawing.Point(15, 339);
            this.mtbGlyph.Mask = "?";
            this.mtbGlyph.Name = "mtbGlyph";
            this.mtbGlyph.Size = new System.Drawing.Size(38, 32);
            this.mtbGlyph.TabIndex = 14;
            // 
            // btnGO
            // 
            this.btnGO.Location = new System.Drawing.Point(59, 346);
            this.btnGO.Name = "btnGO";
            this.btnGO.Size = new System.Drawing.Size(75, 23);
            this.btnGO.TabIndex = 15;
            this.btnGO.Text = "Go!";
            this.btnGO.UseVisualStyleBackColor = true;
            this.btnGO.Click += new System.EventHandler(this.btnGO_Click);
            // 
            // demo
            // 
            this.demo.BackColor = System.Drawing.Color.White;
            this.demo.Dock = System.Windows.Forms.DockStyle.Fill;
            this.demo.Location = new System.Drawing.Point(0, 0);
            this.demo.Name = "demo";
            this.demo.Size = new System.Drawing.Size(600, 420);
            this.demo.TabIndex = 0;
            // 
            // TesselationDemo
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(809, 448);
            this.Controls.Add(this.btnGO);
            this.Controls.Add(this.mtbGlyph);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.tbGlyph);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.lbFont);
            this.Name = "TessellationDemo";
            this.Text = "Laurent Dupuis\'s Font tessellation Demo";
            ((System.ComponentModel.ISupportInitialize)(this.tbGlyph)).EndInit();
            this.panel1.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private Tessellator demo;
        private System.Windows.Forms.ListBox lbFont;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TrackBar tbGlyph;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.MaskedTextBox mtbGlyph;
        private System.Windows.Forms.Button btnGO;
    }
}

