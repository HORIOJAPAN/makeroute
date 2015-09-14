using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;

using System.Diagnostics;
using System.Threading;

namespace HJ_MakeRoute
{
    public partial class MakeRouteForm : Form
    {
        int speed;
        int accuracy;

        SharedMemory shMem;



        public MakeRouteForm()
        {
            InitializeComponent();

            speed = 1;
            accuracy = 1;

            shMem = new SharedMemory("CoordinateOfMap");

        }

        private void speedRB_CheckedChanged(object sender, EventArgs e)
        {
            RadioButton RBtn = (RadioButton)sender;

            speed = Int32.Parse(RBtn.Tag.ToString());

        }

        private void accuracyRB_CheckedChanged(object sender, EventArgs e)
        {
            RadioButton RBtn = (RadioButton)sender;

            accuracy = Int32.Parse(RBtn.Tag.ToString());
        }

        private void saveBtn_Click(object sender, EventArgs e)
        {
            saveRouteDlog.FileName = newRouteDlog.FileName;
            saveRouteDlog.ShowDialog();
            using (StreamWriter w = new StreamWriter(saveRouteDlog.FileName))
            {
                w.Write(routeTxt.Text);
            }
        }

        private void confirmBtn_Click(object sender, EventArgs e)
        {
            String str = String.Format("{0} {1} {2} {3} \r\n", x_val.Text, y_val.Text, speed, accuracy);
            routeTxt.Text += str;

            shMem.setShMemData(1, 3);
        }

        private void insertBtn_Click(object sender, EventArgs e)
        {
            int pos = routeTxt.SelectionStart;
            String str = String.Format("{0} {1} {2} {3}", x_val.Text, y_val.Text, speed, accuracy);

            routeTxt.Text = routeTxt.Text.Insert(pos, str);

            shMem.setShMemData(1, 3);

        }
        private void MakeRouteForm_Activated(object sender, System.EventArgs e)
        {
            int checkChange = shMem.getShMemData();
            if ( checkChange != 0 )
            {
                x_val.Text = shMem.getShMemData(1).ToString();
                y_val.Text = shMem.getShMemData(2).ToString();

                shMem.setShMemData(0);
            }
        }

        private void newRouteDlog_FileOk(object sender, CancelEventArgs e)
        {
            Process myProcess = new Process();
            //myProcess.StartInfo.FileName = "C:\\Users\\user\\Documents\\Visual Studio 2013\\Projects\\HJ_MakeRoute\\Debug\\MakeRoutepp.exe";
            myProcess.StartInfo.FileName = "../../../Debug\\MakeRoutepp.exe";
            myProcess.StartInfo.Arguments = newRouteDlog.FileName;
            myProcess.StartInfo.WindowStyle = ProcessWindowStyle.Normal;
            myProcess.Start();
        }

        private void NewBtn_Click(object sender, EventArgs e)
        {
            newRouteDlog.ShowDialog();
        }

        private void saveRouteDlog_FileOk(object sender, CancelEventArgs e)
        {

        }

    }
}
