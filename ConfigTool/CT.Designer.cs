using System;
namespace CT
{
    partial class CTool
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(CTool));
            this.CMB_ComPort = new System.Windows.Forms.ComboBox();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.toolStripProgressBar1 = new System.Windows.Forms.ToolStripProgressBar();
            this.toolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveToFileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.loadFromFileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.loadDefaultsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.optionsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.resetEepromToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.updateFirmwareToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.sendTLogToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuComPort = new System.Windows.Forms.ToolStripMenuItem();
            this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.gettingStartedToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.PANEL_tabs = new System.Windows.Forms.TabControl();
            this.tabPageConfig = new System.Windows.Forms.TabPage();
            this.btnSave = new System.Windows.Forms.Button();
            this.groupBox8 = new System.Windows.Forms.GroupBox();
            this.btnClearSMS = new System.Windows.Forms.Button();
            this.btnGetSms = new System.Windows.Forms.Button();
            this.btnSMS = new System.Windows.Forms.Button();
            this.btnBalance = new System.Windows.Forms.Button();
            this.btnConsole = new System.Windows.Forms.Button();
            this.txtURL = new System.Windows.Forms.TextBox();
            this.label20 = new System.Windows.Forms.Label();
            this.txtPhone4 = new System.Windows.Forms.TextBox();
            this.label24 = new System.Windows.Forms.Label();
            this.txtPhone3 = new System.Windows.Forms.TextBox();
            this.label23 = new System.Windows.Forms.Label();
            this.txtPhone2 = new System.Windows.Forms.TextBox();
            this.label22 = new System.Windows.Forms.Label();
            this.txtPhone1 = new System.Windows.Forms.TextBox();
            this.label21 = new System.Windows.Forms.Label();
            this.label18 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.cbxAutoUpdate = new System.Windows.Forms.CheckBox();
            this.cbxShowUpdateDialog = new System.Windows.Forms.CheckBox();
            this.hint = new System.Windows.Forms.ToolTip(this.components);
            this.btnConnect = new System.Windows.Forms.Button();
            this.btnDisconnect = new System.Windows.Forms.Button();
            this.lblRel = new System.Windows.Forms.Label();
            this.statusStrip1.SuspendLayout();
            this.menuStrip1.SuspendLayout();
            this.PANEL_tabs.SuspendLayout();
            this.tabPageConfig.SuspendLayout();
            this.groupBox8.SuspendLayout();
            this.SuspendLayout();
            // 
            // CMB_ComPort
            // 
            this.CMB_ComPort.FormattingEnabled = true;
            this.CMB_ComPort.Location = new System.Drawing.Point(68, 42);
            this.CMB_ComPort.Name = "CMB_ComPort";
            this.CMB_ComPort.Size = new System.Drawing.Size(98, 21);
            this.CMB_ComPort.TabIndex = 4;
            this.CMB_ComPort.Click += new System.EventHandler(this.comboBox1_Click);
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripProgressBar1,
            this.toolStripStatusLabel1});
            this.statusStrip1.Location = new System.Drawing.Point(0, 426);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(547, 22);
            this.statusStrip1.TabIndex = 8;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // toolStripProgressBar1
            // 
            this.toolStripProgressBar1.Name = "toolStripProgressBar1";
            this.toolStripProgressBar1.Size = new System.Drawing.Size(500, 16);
            // 
            // toolStripStatusLabel1
            // 
            this.toolStripStatusLabel1.Name = "toolStripStatusLabel1";
            this.toolStripStatusLabel1.Size = new System.Drawing.Size(0, 17);
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.optionsToolStripMenuItem,
            this.helpToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(547, 24);
            this.menuStrip1.TabIndex = 10;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.saveToFileToolStripMenuItem,
            this.loadFromFileToolStripMenuItem,
            this.toolStripSeparator2,
            this.loadDefaultsToolStripMenuItem,
            this.toolStripSeparator1,
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(35, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // saveToFileToolStripMenuItem
            // 
            this.saveToFileToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("saveToFileToolStripMenuItem.Image")));
            this.saveToFileToolStripMenuItem.Name = "saveToFileToolStripMenuItem";
            this.saveToFileToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.S)));
            this.saveToFileToolStripMenuItem.Size = new System.Drawing.Size(213, 22);
            this.saveToFileToolStripMenuItem.Text = "Save Config file...";
            this.saveToFileToolStripMenuItem.Click += new System.EventHandler(this.saveToFileToolStripMenuItem_Click);
            // 
            // loadFromFileToolStripMenuItem
            // 
            this.loadFromFileToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("loadFromFileToolStripMenuItem.Image")));
            this.loadFromFileToolStripMenuItem.Name = "loadFromFileToolStripMenuItem";
            this.loadFromFileToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.O)));
            this.loadFromFileToolStripMenuItem.Size = new System.Drawing.Size(213, 22);
            this.loadFromFileToolStripMenuItem.Text = "Load Config File...";
            this.loadFromFileToolStripMenuItem.Click += new System.EventHandler(this.loadFromFileToolStripMenuItem_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(210, 6);
            // 
            // loadDefaultsToolStripMenuItem
            // 
            this.loadDefaultsToolStripMenuItem.Name = "loadDefaultsToolStripMenuItem";
            this.loadDefaultsToolStripMenuItem.Size = new System.Drawing.Size(213, 22);
            this.loadDefaultsToolStripMenuItem.Text = "Load Defaults";
            this.loadDefaultsToolStripMenuItem.Click += new System.EventHandler(this.loadDefaultsToolStripMenuItem_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(210, 6);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Alt | System.Windows.Forms.Keys.F4)));
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(213, 22);
            this.exitToolStripMenuItem.Text = "Exit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // optionsToolStripMenuItem
            // 
            this.optionsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.resetEepromToolStripMenuItem,
            this.updateFirmwareToolStripMenuItem,
            this.sendTLogToolStripMenuItem,
            this.mnuComPort});
            this.optionsToolStripMenuItem.Name = "optionsToolStripMenuItem";
            this.optionsToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.G)));
            this.optionsToolStripMenuItem.ShowShortcutKeys = false;
            this.optionsToolStripMenuItem.Size = new System.Drawing.Size(56, 20);
            this.optionsToolStripMenuItem.Text = "Options";
            // 
            // resetEepromToolStripMenuItem
            // 
            this.resetEepromToolStripMenuItem.Name = "resetEepromToolStripMenuItem";
            this.resetEepromToolStripMenuItem.Size = new System.Drawing.Size(187, 22);
            this.resetEepromToolStripMenuItem.Text = "Reset EEPROM";
            this.resetEepromToolStripMenuItem.ToolTipText = "Initialize EEPROM with default values";
            this.resetEepromToolStripMenuItem.Click += new System.EventHandler(this.BUT_ResetOSD_EEPROM_click);
            // 
            // updateFirmwareToolStripMenuItem
            // 
            this.updateFirmwareToolStripMenuItem.Name = "updateFirmwareToolStripMenuItem";
            this.updateFirmwareToolStripMenuItem.Size = new System.Drawing.Size(187, 22);
            this.updateFirmwareToolStripMenuItem.Text = "Update Firmware...";
            this.updateFirmwareToolStripMenuItem.ToolTipText = "Re-Flash the OSD with a new firmware image";
            this.updateFirmwareToolStripMenuItem.Click += new System.EventHandler(this.updateFirmwareToolStripMenuItem_Click);
            // 
            // sendTLogToolStripMenuItem
            // 
            this.sendTLogToolStripMenuItem.Name = "sendTLogToolStripMenuItem";
            this.sendTLogToolStripMenuItem.Size = new System.Drawing.Size(187, 22);
            this.sendTLogToolStripMenuItem.Text = "Load TLog.";
            this.sendTLogToolStripMenuItem.ToolTipText = "Load a Mavlink transmission log to play into the OSD to test the layout";
            this.sendTLogToolStripMenuItem.Click += new System.EventHandler(this.sendTLogToolStripMenuItem_Click);
            // 
            // mnuComPort
            // 
            this.mnuComPort.Name = "mnuComPort";
            this.mnuComPort.Size = new System.Drawing.Size(187, 22);
            this.mnuComPort.Text = "Connect to COM port";
            this.mnuComPort.Click += new System.EventHandler(this.mnuComPort_Click);
            // 
            // helpToolStripMenuItem
            // 
            this.helpToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.gettingStartedToolStripMenuItem,
            this.aboutToolStripMenuItem});
            this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            this.helpToolStripMenuItem.Size = new System.Drawing.Size(40, 20);
            this.helpToolStripMenuItem.Text = "Help";
            // 
            // gettingStartedToolStripMenuItem
            // 
            this.gettingStartedToolStripMenuItem.Name = "gettingStartedToolStripMenuItem";
            this.gettingStartedToolStripMenuItem.Size = new System.Drawing.Size(158, 22);
            this.gettingStartedToolStripMenuItem.Text = "Getting started";
            this.gettingStartedToolStripMenuItem.Click += new System.EventHandler(this.gettingStartedToolStripMenuItem_Click);
            // 
            // aboutToolStripMenuItem
            // 
            this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
            this.aboutToolStripMenuItem.Size = new System.Drawing.Size(158, 22);
            this.aboutToolStripMenuItem.Text = "About ";
            this.aboutToolStripMenuItem.Click += new System.EventHandler(this.aboutToolStripMenuItem_Click);
            // 
            // PANEL_tabs
            // 
            this.PANEL_tabs.Controls.Add(this.tabPageConfig);
            this.PANEL_tabs.Location = new System.Drawing.Point(3, 83);
            this.PANEL_tabs.Margin = new System.Windows.Forms.Padding(2);
            this.PANEL_tabs.Name = "PANEL_tabs";
            this.PANEL_tabs.SelectedIndex = 0;
            this.PANEL_tabs.Size = new System.Drawing.Size(362, 333);
            this.PANEL_tabs.TabIndex = 0;
            // 
            // tabPageConfig
            // 
            this.tabPageConfig.Controls.Add(this.btnSave);
            this.tabPageConfig.Controls.Add(this.groupBox8);
            this.tabPageConfig.Location = new System.Drawing.Point(4, 22);
            this.tabPageConfig.Margin = new System.Windows.Forms.Padding(2);
            this.tabPageConfig.Name = "tabPageConfig";
            this.tabPageConfig.Padding = new System.Windows.Forms.Padding(2);
            this.tabPageConfig.Size = new System.Drawing.Size(354, 307);
            this.tabPageConfig.TabIndex = 1;
            this.tabPageConfig.Text = "Config";
            this.tabPageConfig.UseVisualStyleBackColor = true;
            // 
            // btnSave
            // 
            this.btnSave.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnSave.Location = new System.Drawing.Point(221, 277);
            this.btnSave.Name = "btnSave";
            this.btnSave.Size = new System.Drawing.Size(128, 25);
            this.btnSave.TabIndex = 17;
            this.btnSave.Text = "Save to EEPROM";
            this.hint.SetToolTip(this.btnSave, "Read current settings form the modem\'s memory");
            this.btnSave.UseVisualStyleBackColor = true;
            this.btnSave.Click += new System.EventHandler(this.btnSave_Click);
            // 
            // groupBox8
            // 
            this.groupBox8.Controls.Add(this.btnClearSMS);
            this.groupBox8.Controls.Add(this.btnGetSms);
            this.groupBox8.Controls.Add(this.btnSMS);
            this.groupBox8.Controls.Add(this.btnBalance);
            this.groupBox8.Controls.Add(this.btnConsole);
            this.groupBox8.Controls.Add(this.txtURL);
            this.groupBox8.Controls.Add(this.label20);
            this.groupBox8.Controls.Add(this.txtPhone4);
            this.groupBox8.Controls.Add(this.label24);
            this.groupBox8.Controls.Add(this.txtPhone3);
            this.groupBox8.Controls.Add(this.label23);
            this.groupBox8.Controls.Add(this.txtPhone2);
            this.groupBox8.Controls.Add(this.label22);
            this.groupBox8.Controls.Add(this.txtPhone1);
            this.groupBox8.Controls.Add(this.label21);
            this.groupBox8.Controls.Add(this.label18);
            this.groupBox8.Location = new System.Drawing.Point(5, 5);
            this.groupBox8.Name = "groupBox8";
            this.groupBox8.Size = new System.Drawing.Size(344, 268);
            this.groupBox8.TabIndex = 9;
            this.groupBox8.TabStop = false;
            this.groupBox8.Text = "SMS";
            // 
            // btnClearSMS
            // 
            this.btnClearSMS.Location = new System.Drawing.Point(248, 59);
            this.btnClearSMS.Name = "btnClearSMS";
            this.btnClearSMS.Size = new System.Drawing.Size(79, 21);
            this.btnClearSMS.TabIndex = 26;
            this.btnClearSMS.Text = "Clear SMS";
            this.btnClearSMS.UseVisualStyleBackColor = true;
            this.btnClearSMS.Click += new System.EventHandler(this.btnClearSMS_Click);
            // 
            // btnGetSms
            // 
            this.btnGetSms.Location = new System.Drawing.Point(248, 33);
            this.btnGetSms.Name = "btnGetSms";
            this.btnGetSms.Size = new System.Drawing.Size(79, 21);
            this.btnGetSms.TabIndex = 25;
            this.btnGetSms.Text = "Get SMS";
            this.btnGetSms.UseVisualStyleBackColor = true;
            this.btnGetSms.Click += new System.EventHandler(this.btnGetSms_Click);
            // 
            // btnSMS
            // 
            this.btnSMS.Location = new System.Drawing.Point(163, 59);
            this.btnSMS.Name = "btnSMS";
            this.btnSMS.Size = new System.Drawing.Size(79, 21);
            this.btnSMS.TabIndex = 17;
            this.btnSMS.Text = "Send SMS";
            this.btnSMS.UseVisualStyleBackColor = true;
            this.btnSMS.Click += new System.EventHandler(this.btnSMS_Click);
            // 
            // btnBalance
            // 
            this.btnBalance.Location = new System.Drawing.Point(163, 33);
            this.btnBalance.Name = "btnBalance";
            this.btnBalance.Size = new System.Drawing.Size(79, 21);
            this.btnBalance.TabIndex = 16;
            this.btnBalance.Text = "Get balance";
            this.btnBalance.UseVisualStyleBackColor = true;
            this.btnBalance.Click += new System.EventHandler(this.btnBalance_Click);
            // 
            // btnConsole
            // 
            this.btnConsole.Location = new System.Drawing.Point(163, 86);
            this.btnConsole.Name = "btnConsole";
            this.btnConsole.Size = new System.Drawing.Size(79, 21);
            this.btnConsole.TabIndex = 15;
            this.btnConsole.Text = "GSM console";
            this.hint.SetToolTip(this.btnConsole, "Connect directly to GSM and open console. Empty string to quit.");
            this.btnConsole.UseVisualStyleBackColor = true;
            this.btnConsole.Click += new System.EventHandler(this.btnConsole_Click);
            // 
            // txtURL
            // 
            this.txtURL.Location = new System.Drawing.Point(6, 158);
            this.txtURL.Name = "txtURL";
            this.txtURL.Size = new System.Drawing.Size(146, 20);
            this.txtURL.TabIndex = 14;
            this.txtURL.Text = "ykoctpa.ru/map?q=";
            this.hint.SetToolTip(this.txtURL, "URL of mapping service to send with coordinates");
            this.txtURL.Leave += new System.EventHandler(this.txtURL_Leave);
            // 
            // label20
            // 
            this.label20.AutoSize = true;
            this.label20.Location = new System.Drawing.Point(28, 142);
            this.label20.Name = "label20";
            this.label20.Size = new System.Drawing.Size(101, 13);
            this.label20.TabIndex = 13;
            this.label20.Text = "URL of map service";
            // 
            // txtPhone4
            // 
            this.txtPhone4.Location = new System.Drawing.Point(32, 114);
            this.txtPhone4.Name = "txtPhone4";
            this.txtPhone4.Size = new System.Drawing.Size(111, 20);
            this.txtPhone4.TabIndex = 12;
            this.hint.SetToolTip(this.txtPhone4, "Phone numbers to send SMS to");
            this.txtPhone4.Leave += new System.EventHandler(this.txtPhone4_Leave);
            // 
            // label24
            // 
            this.label24.AutoSize = true;
            this.label24.Location = new System.Drawing.Point(13, 118);
            this.label24.Name = "label24";
            this.label24.Size = new System.Drawing.Size(13, 13);
            this.label24.TabIndex = 11;
            this.label24.Text = "4";
            // 
            // txtPhone3
            // 
            this.txtPhone3.Location = new System.Drawing.Point(32, 88);
            this.txtPhone3.Name = "txtPhone3";
            this.txtPhone3.Size = new System.Drawing.Size(111, 20);
            this.txtPhone3.TabIndex = 10;
            this.hint.SetToolTip(this.txtPhone3, "Phone numbers to send SMS to");
            this.txtPhone3.Leave += new System.EventHandler(this.txtPhone3_Leave);
            // 
            // label23
            // 
            this.label23.AutoSize = true;
            this.label23.Location = new System.Drawing.Point(13, 92);
            this.label23.Name = "label23";
            this.label23.Size = new System.Drawing.Size(13, 13);
            this.label23.TabIndex = 9;
            this.label23.Text = "3";
            // 
            // txtPhone2
            // 
            this.txtPhone2.Location = new System.Drawing.Point(32, 62);
            this.txtPhone2.Name = "txtPhone2";
            this.txtPhone2.Size = new System.Drawing.Size(111, 20);
            this.txtPhone2.TabIndex = 8;
            this.hint.SetToolTip(this.txtPhone2, "Phone numbers to send SMS to");
            this.txtPhone2.Leave += new System.EventHandler(this.txtPhone2_Leave);
            // 
            // label22
            // 
            this.label22.AutoSize = true;
            this.label22.Location = new System.Drawing.Point(13, 66);
            this.label22.Name = "label22";
            this.label22.Size = new System.Drawing.Size(13, 13);
            this.label22.TabIndex = 7;
            this.label22.Text = "2";
            // 
            // txtPhone1
            // 
            this.txtPhone1.Location = new System.Drawing.Point(32, 34);
            this.txtPhone1.Name = "txtPhone1";
            this.txtPhone1.Size = new System.Drawing.Size(111, 20);
            this.txtPhone1.TabIndex = 6;
            this.hint.SetToolTip(this.txtPhone1, "Phone numbers to send SMS to");
            this.txtPhone1.Leave += new System.EventHandler(this.txtPhone1_Leave);
            // 
            // label21
            // 
            this.label21.AutoSize = true;
            this.label21.Location = new System.Drawing.Point(13, 38);
            this.label21.Name = "label21";
            this.label21.Size = new System.Drawing.Size(13, 13);
            this.label21.TabIndex = 5;
            this.label21.Text = "1";
            // 
            // label18
            // 
            this.label18.AutoSize = true;
            this.label18.Location = new System.Drawing.Point(13, 18);
            this.label18.Name = "label18";
            this.label18.Size = new System.Drawing.Size(119, 13);
            this.label18.TabIndex = 0;
            this.label18.Text = "Phones to send SMS to";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(6, 45);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(58, 13);
            this.label5.TabIndex = 16;
            this.label5.Text = "Serial Port:";
            // 
            // cbxAutoUpdate
            // 
            this.cbxAutoUpdate.Location = new System.Drawing.Point(0, 0);
            this.cbxAutoUpdate.Name = "cbxAutoUpdate";
            this.cbxAutoUpdate.Size = new System.Drawing.Size(104, 24);
            this.cbxAutoUpdate.TabIndex = 0;
            // 
            // cbxShowUpdateDialog
            // 
            this.cbxShowUpdateDialog.Location = new System.Drawing.Point(0, 0);
            this.cbxShowUpdateDialog.Name = "cbxShowUpdateDialog";
            this.cbxShowUpdateDialog.Size = new System.Drawing.Size(104, 24);
            this.cbxShowUpdateDialog.TabIndex = 0;
            // 
            // hint
            // 
            this.hint.ShowAlways = true;
            // 
            // btnConnect
            // 
            this.btnConnect.Location = new System.Drawing.Point(175, 25);
            this.btnConnect.Name = "btnConnect";
            this.btnConnect.Size = new System.Drawing.Size(100, 23);
            this.btnConnect.TabIndex = 17;
            this.btnConnect.Text = "Connect";
            this.hint.SetToolTip(this.btnConnect, "Connect to the beacon via specified COM-port.");
            this.btnConnect.UseVisualStyleBackColor = true;
            this.btnConnect.Click += new System.EventHandler(this.btnConnet_Click);
            // 
            // btnDisconnect
            // 
            this.btnDisconnect.Location = new System.Drawing.Point(175, 55);
            this.btnDisconnect.Name = "btnDisconnect";
            this.btnDisconnect.Size = new System.Drawing.Size(100, 23);
            this.btnDisconnect.TabIndex = 18;
            this.btnDisconnect.Text = "Disconnect";
            this.hint.SetToolTip(this.btnDisconnect, "Disconnect from beacon");
            this.btnDisconnect.UseVisualStyleBackColor = true;
            this.btnDisconnect.Click += new System.EventHandler(this.btnDisconnect_Click);
            // 
            // lblRel
            // 
            this.lblRel.AutoSize = true;
            this.lblRel.Location = new System.Drawing.Point(455, 45);
            this.lblRel.Name = "lblRel";
            this.lblRel.Size = new System.Drawing.Size(0, 13);
            this.lblRel.TabIndex = 19;
            // 
            // CTool
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(547, 448);
            this.Controls.Add(this.lblRel);
            this.Controls.Add(this.btnDisconnect);
            this.Controls.Add(this.btnConnect);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.PANEL_tabs);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.menuStrip1);
            this.Controls.Add(this.CMB_ComPort);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "CTool";
            this.Text = "GSM modem Config Tool";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.OSD_FormClosed);
            this.Load += new System.EventHandler(this.OSD_Load);
            this.Resize += new System.EventHandler(this.OSD_Resize);
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.PANEL_tabs.ResumeLayout(false);
            this.tabPageConfig.ResumeLayout(false);
            this.groupBox8.ResumeLayout(false);
            this.groupBox8.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        public  System.Windows.Forms.ComboBox CMB_ComPort;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripProgressBar toolStripProgressBar1;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveToFileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem loadFromFileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem loadDefaultsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem optionsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem resetEepromToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem updateFirmwareToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem sendTLogToolStripMenuItem;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel1;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.TabControl PANEL_tabs;

        private System.Windows.Forms.TabPage tabPageConfig;
        private System.Windows.Forms.GroupBox groupBox8;
        private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem gettingStartedToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem mnuComPort;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.CheckBox cbxAutoUpdate;
        private System.Windows.Forms.CheckBox cbxShowUpdateDialog;
        private System.Windows.Forms.TextBox txtURL;
        private System.Windows.Forms.Label label20;
        private System.Windows.Forms.TextBox txtPhone4;
        private System.Windows.Forms.Label label24;
        private System.Windows.Forms.TextBox txtPhone3;
        private System.Windows.Forms.Label label23;
        private System.Windows.Forms.TextBox txtPhone2;
        private System.Windows.Forms.Label label22;
        private System.Windows.Forms.TextBox txtPhone1;
        private System.Windows.Forms.Label label21;
        private System.Windows.Forms.Label label18;
        private System.Windows.Forms.ToolTip hint;
        private System.Windows.Forms.Button btnBalance;
        private System.Windows.Forms.Button btnConsole;
        private System.Windows.Forms.Button btnConnect;
        private System.Windows.Forms.Button btnDisconnect;
        private System.Windows.Forms.Button btnSave;
        private System.Windows.Forms.Button btnSMS;
        private System.Windows.Forms.Label lblRel;
        private System.Windows.Forms.Button btnClearSMS;
        private System.Windows.Forms.Button btnGetSms;
    }
	
}

