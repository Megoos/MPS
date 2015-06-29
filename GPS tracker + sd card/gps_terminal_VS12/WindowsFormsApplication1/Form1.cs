using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;
using System.IO;
using Microsoft.SqlServer.Server;


namespace WindowsFormsApplication1
{
    public partial class Form1 : Form
    {
        private string _coordinat1;
        private string _coordinat2;
        private int _t11;
        private int _t21;
        public Form1()
        {
            InitializeComponent();
            button2.Enabled = false;
            
        }

        
        private void button2_Click(object sender, EventArgs e)
        {
            _t11 = _t11 + 37;
             _t21 = _t21 + 123;
            
            _coordinat1 = "55.{0}";
            _coordinat2 = "12.{0}";
            _coordinat1 = string.Format(_coordinat1, _t11);
            serialPort1.WriteLine(_coordinat1);
            
            _coordinat2 = string.Format(_coordinat2, _t21);
            serialPort1.WriteLine(_coordinat2);
            
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            string[] myPort;

            myPort = System.IO.Ports.SerialPort.GetPortNames();
            comboBox1.Items.AddRange(myPort);
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (serialPort1.IsOpen == true)
            serialPort1.Close();
        }


        private void button3_Click(object sender, EventArgs e)
        {
                serialPort1.BaudRate = 9600;
                serialPort1.PortName = comboBox1.Text.ToString();

                try
                {
                    if (serialPort1.IsOpen == false)
                    {
                        serialPort1.Open();
                        comboBox1.Enabled = false;
                        button3.Enabled = false;
                        button2.Enabled = true;
                        _t11 = 345678;
                        _t21 = 121212;
                    }
                }
                catch
                {
                    MessageBox.Show("Порт занят");
                }
            
           
            }
      
        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            serialPort1.PortName = comboBox1.Text.ToString();
        }
    }
}
