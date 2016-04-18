namespace BraitenbergProcessing
{
    partial class Generator
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
            this.exitButton = new System.Windows.Forms.Button();
            this.loadButton = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.strategyComboBox = new System.Windows.Forms.ComboBox();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.generateButton = new System.Windows.Forms.Button();
            this.giMinUD = new System.Windows.Forms.NumericUpDown();
            this.giMaxUD = new System.Windows.Forms.NumericUpDown();
            this.giIncrementUD = new System.Windows.Forms.NumericUpDown();
            this.gfIncrementUD = new System.Windows.Forms.NumericUpDown();
            this.gfMaxUD = new System.Windows.Forms.NumericUpDown();
            this.gfMinUD = new System.Windows.Forms.NumericUpDown();
            this.label3 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.giMinUD)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.giMaxUD)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.giIncrementUD)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.gfIncrementUD)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.gfMaxUD)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.gfMinUD)).BeginInit();
            this.SuspendLayout();
            // 
            // exitButton
            // 
            this.exitButton.Location = new System.Drawing.Point(635, 244);
            this.exitButton.Name = "exitButton";
            this.exitButton.Size = new System.Drawing.Size(75, 23);
            this.exitButton.TabIndex = 1;
            this.exitButton.Text = "Close";
            this.exitButton.UseVisualStyleBackColor = true;
            this.exitButton.Click += new System.EventHandler(this.button1_Click);
            // 
            // loadButton
            // 
            this.loadButton.Location = new System.Drawing.Point(618, 12);
            this.loadButton.Name = "loadButton";
            this.loadButton.Size = new System.Drawing.Size(92, 23);
            this.loadButton.TabIndex = 3;
            this.loadButton.Text = "Load template...";
            this.loadButton.UseVisualStyleBackColor = true;
            this.loadButton.Click += new System.EventHandler(this.loadButton_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 17);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(80, 13);
            this.label1.TabIndex = 5;
            this.label1.Text = "Select strategy:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(63, 44);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(18, 13);
            this.label2.TabIndex = 8;
            this.label2.Text = "gi:";
            // 
            // strategyComboBox
            // 
            this.strategyComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.strategyComboBox.FormattingEnabled = true;
            this.strategyComboBox.Items.AddRange(new object[] {
            "csl",
            "csl2",
            "2a",
            "2b",
            "3a",
            "3b",
            "singlecsl"});
            this.strategyComboBox.Location = new System.Drawing.Point(98, 14);
            this.strategyComboBox.Name = "strategyComboBox";
            this.strategyComboBox.Size = new System.Drawing.Size(121, 21);
            this.strategyComboBox.TabIndex = 4;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(136, 44);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(16, 13);
            this.label4.TabIndex = 10;
            this.label4.Text = "to";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(207, 44);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(81, 13);
            this.label5.TabIndex = 12;
            this.label5.Text = "in increments of";
            // 
            // generateButton
            // 
            this.generateButton.Location = new System.Drawing.Point(98, 106);
            this.generateButton.Name = "generateButton";
            this.generateButton.Size = new System.Drawing.Size(92, 23);
            this.generateButton.TabIndex = 19;
            this.generateButton.Text = "Generate!";
            this.generateButton.UseVisualStyleBackColor = true;
            this.generateButton.Click += new System.EventHandler(this.generateButton_Click);
            // 
            // giMinUD
            // 
            this.giMinUD.DecimalPlaces = 1;
            this.giMinUD.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.giMinUD.Location = new System.Drawing.Point(87, 42);
            this.giMinUD.Maximum = new decimal(new int[] {
            2,
            0,
            0,
            0});
            this.giMinUD.Name = "giMinUD";
            this.giMinUD.Size = new System.Drawing.Size(43, 20);
            this.giMinUD.TabIndex = 20;
            // 
            // giMaxUD
            // 
            this.giMaxUD.DecimalPlaces = 1;
            this.giMaxUD.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.giMaxUD.Location = new System.Drawing.Point(158, 42);
            this.giMaxUD.Maximum = new decimal(new int[] {
            2,
            0,
            0,
            0});
            this.giMaxUD.Name = "giMaxUD";
            this.giMaxUD.Size = new System.Drawing.Size(43, 20);
            this.giMaxUD.TabIndex = 21;
            this.giMaxUD.Value = new decimal(new int[] {
            10,
            0,
            0,
            65536});
            // 
            // giIncrementUD
            // 
            this.giIncrementUD.DecimalPlaces = 1;
            this.giIncrementUD.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.giIncrementUD.Location = new System.Drawing.Point(294, 42);
            this.giIncrementUD.Maximum = new decimal(new int[] {
            2,
            0,
            0,
            0});
            this.giIncrementUD.Name = "giIncrementUD";
            this.giIncrementUD.Size = new System.Drawing.Size(43, 20);
            this.giIncrementUD.TabIndex = 22;
            this.giIncrementUD.Value = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            // 
            // gfIncrementUD
            // 
            this.gfIncrementUD.DecimalPlaces = 1;
            this.gfIncrementUD.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.gfIncrementUD.Location = new System.Drawing.Point(294, 68);
            this.gfIncrementUD.Maximum = new decimal(new int[] {
            2,
            0,
            0,
            0});
            this.gfIncrementUD.Name = "gfIncrementUD";
            this.gfIncrementUD.Size = new System.Drawing.Size(43, 20);
            this.gfIncrementUD.TabIndex = 28;
            this.gfIncrementUD.Value = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            // 
            // gfMaxUD
            // 
            this.gfMaxUD.DecimalPlaces = 1;
            this.gfMaxUD.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.gfMaxUD.Location = new System.Drawing.Point(158, 68);
            this.gfMaxUD.Maximum = new decimal(new int[] {
            2,
            0,
            0,
            0});
            this.gfMaxUD.Name = "gfMaxUD";
            this.gfMaxUD.Size = new System.Drawing.Size(43, 20);
            this.gfMaxUD.TabIndex = 27;
            this.gfMaxUD.Value = new decimal(new int[] {
            10,
            0,
            0,
            65536});
            // 
            // gfMinUD
            // 
            this.gfMinUD.DecimalPlaces = 1;
            this.gfMinUD.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.gfMinUD.Location = new System.Drawing.Point(87, 68);
            this.gfMinUD.Maximum = new decimal(new int[] {
            2,
            0,
            0,
            0});
            this.gfMinUD.Name = "gfMinUD";
            this.gfMinUD.Size = new System.Drawing.Size(43, 20);
            this.gfMinUD.TabIndex = 26;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(207, 70);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(81, 13);
            this.label3.TabIndex = 25;
            this.label3.Text = "in increments of";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(136, 70);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(16, 13);
            this.label6.TabIndex = 24;
            this.label6.Text = "to";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(62, 70);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(19, 13);
            this.label7.TabIndex = 23;
            this.label7.Text = "gf:";
            // 
            // Generator
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(722, 279);
            this.Controls.Add(this.gfIncrementUD);
            this.Controls.Add(this.gfMaxUD);
            this.Controls.Add(this.gfMinUD);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.giIncrementUD);
            this.Controls.Add(this.giMaxUD);
            this.Controls.Add(this.giMinUD);
            this.Controls.Add(this.generateButton);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.strategyComboBox);
            this.Controls.Add(this.loadButton);
            this.Controls.Add(this.exitButton);
            this.Name = "Generator";
            this.Text = "Generator";
            ((System.ComponentModel.ISupportInitialize)(this.giMinUD)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.giMaxUD)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.giIncrementUD)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.gfIncrementUD)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.gfMaxUD)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.gfMinUD)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button exitButton;
        private System.Windows.Forms.Button loadButton;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ComboBox strategyComboBox;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Button generateButton;
        private System.Windows.Forms.NumericUpDown giMinUD;
        private System.Windows.Forms.NumericUpDown giMaxUD;
        private System.Windows.Forms.NumericUpDown giIncrementUD;
        private System.Windows.Forms.NumericUpDown gfIncrementUD;
        private System.Windows.Forms.NumericUpDown gfMaxUD;
        private System.Windows.Forms.NumericUpDown gfMinUD;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
    }
}