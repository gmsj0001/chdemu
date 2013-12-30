using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace PacketSniffer
{
    public partial class FrmSniffer : Form
    {
        public FrmSniffer()
        {
            InitializeComponent();
        }

        private class PacListItem
        {
            public DateTime time;
            public Packet pac;
            public bool isSend;
            public int opCode;
            public override string ToString()
            {
                return time.ToString("HH:mm:ss") + (isSend ? " Send " : " Recv ") + opCode.ToString();
            }
        }

        int bufferPos = 0;
        bool startSniffer;

        private byte[] PacketToBytes(Packet pac)
        {
            byte[] b = new byte[pac.len - 4];
            Array.Copy(pac.data, 0, b, 0, pac.len - 4);
            return b;
        }

        private Packet BytesToPacket(byte[] b)
        {
            var p = new Packet();
            p.len = (short)(b.Length + 4);
            Array.Copy(b, 0, p.data, 0, b.Length);
            return p;
        }

        public void OnRecvPacket(Packet pac)
        {
            if (!startSniffer) return;
            var item = new PacListItem();
            
            item.time = DateTime.Now;
            item.isSend = (pac.cipher == 100);
            item.pac = pac;
            pac.pos = 0;
            item.opCode = pac.ReadInt();
            if (item.opCode == 0) return;
            lstPacket.Items.Add(item);
            lstPacket.SelectedIndex = lstPacket.Items.Count - 1;
        }

        private void lstPacket_Click(object sender, EventArgs e)
        {
            if (lstPacket.SelectedIndex == -1) return;
            hexBox.ByteProvider = new Be.Windows.Forms.DynamicByteProvider(PacketToBytes((lstPacket.SelectedItem as PacListItem).pac));
        }

        private void btnStart_Click(object sender, EventArgs e)
        {
            startSniffer = true;
            btnStart.Enabled = false;
            btnStop.Enabled = true;
        }

        private void btnStop_Click(object sender, EventArgs e)
        {
            startSniffer = false;
            btnStop.Enabled = false;
            btnStart.Enabled = true;
        }

        private void btnSave_Click(object sender, EventArgs e)
        {
            byte[] b = (hexBox.ByteProvider as Be.Windows.Forms.DynamicByteProvider).Bytes.GetBytes();
            if (saveFileDialog1.ShowDialog() == DialogResult.OK)
            {
                var fs = new FileStream(saveFileDialog1.FileName, FileMode.Create, FileAccess.Write);
                fs.Write(b, 0, b.Length);
                fs.Close();
            }
                
        }

        private void btnClear_Click(object sender, EventArgs e)
        {
            lstPacket.Items.Clear();
        }

        private void timInject_Tick(object sender, EventArgs e)
        {
            if (Hook.InitHook())
            {
                timInject.Enabled = false;
                this.Text += " - Connected";
            }
        }

        private void timPacket_Tick(object sender, EventArgs e)
        {
            Packet p;
            while ((p = Hook.GetPacket()) != null)
            {
                OnRecvPacket(p);
            }
        }

    }
}
