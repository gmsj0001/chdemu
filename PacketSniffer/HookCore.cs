using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.IO;

namespace PacketSniffer
{
    public class Packet
    {
        public short len;
        public short cipher;
        public byte[] data = new byte[0x2000];


        private MemoryStream ms;
        private BinaryReader r;
        private BinaryWriter w;

        public short pos
        {
            get { return (short)ms.Position; }
            set { ms.Position = value; }
        }

        private void AppendLength() { if (pos + 4 > len) len = (short)(pos + 4); }

        public byte ReadByte() { return r.ReadByte(); }
        public sbyte ReadSByte() { return r.ReadSByte(); }
        public void WriteByte(byte value) { w.Write(value); AppendLength(); }
        public void WriteSByte(sbyte value) { w.Write(value); AppendLength(); }

        public byte[] ReadBytes(int count) { return r.ReadBytes(count); }
        public void WriteBytes(byte[] b) { w.Write(b); AppendLength(); }
        public byte[] ReadAllBytes() { byte[] b = new byte[len - 4]; Array.Copy(data, b, len - 4); return b; }

        public short ReadShort() { return r.ReadInt16(); }
        public ushort ReadUShort() { return r.ReadUInt16(); }
        public void WriteShort(short value) { w.Write(value); AppendLength(); }
        public void WriteUShort(ushort value) { w.Write(value); AppendLength(); }

        public int ReadInt() { return r.ReadInt32(); }
        public uint ReadUInt() { return r.ReadUInt32(); }
        public void WriteInt(int value) { w.Write(value); AppendLength(); }
        public void WriteUInt(uint value) { w.Write(value); AppendLength(); }

        public long ReadInt64() { return r.ReadInt64(); }
        public ulong ReadUInt64() { return r.ReadUInt64(); }
        public void WriteInt64(long value) { w.Write(value); AppendLength(); }
        public void WriteUInt64(ulong value) { w.Write(value); AppendLength(); }

        public Packet()
        {
            ms = new MemoryStream(data);
            r = new BinaryReader(ms);
            w = new BinaryWriter(ms);
        }

    }

    public class Hook
    {
        private class PacketArray
        {
            private IntPtr m_pBasePtr;
            public Packet this[int index]
            {
                get
                {
                    Packet pac = new Packet();
                    pac.len = Marshal.ReadInt16(m_pBasePtr, 0x2004 * index);
                    pac.cipher = Marshal.ReadInt16(m_pBasePtr, 0x2004 * index + 2);
                    Marshal.Copy((IntPtr)(m_pBasePtr.ToInt32() + 0x2004 * index + 4), pac.data, 0, 0x2000);
                    return pac;
                }
                set
                {
                    Marshal.WriteInt16(m_pBasePtr, 0x2004 * index, value.len);
                    Marshal.WriteInt16(m_pBasePtr, 0x2004 * index + 2, value.cipher);
                    Marshal.Copy(value.data, 0, (IntPtr)(m_pBasePtr.ToInt32() + 0x2004 * index + 4), 0x2000);
                }
            }
            internal PacketArray(IntPtr BasePtr)
            {
                m_pBasePtr = BasePtr;
            }
        }

        private class MapData
        {
            private IntPtr m_pMapData;
            public const int BufferSize = 512;

            public MapData(IntPtr BasePtr)
            {
                m_pMapData = BasePtr;
                _Data = new PacketArray((IntPtr)(m_pMapData.ToInt32() + 8));
            }

            public int SetIndex
            {
                get { return Marshal.ReadInt32(m_pMapData, 0); }
            }

            public int GetIndex
            {
                get { return Marshal.ReadInt32(m_pMapData, 4); }
                set { Marshal.WriteInt32(m_pMapData, 4, value); }
            }

            private PacketArray _Data;
            public PacketArray Data
            {
                get { return _Data; }
            }
        }

        [DllImport("kernel32.dll", CharSet = CharSet.Ansi)]
        private static extern int CreateFileMapping(int hFile, int lpFileMappingAttributes, int flProtect, int dwMaximumSizeHigh, int dwMaximumSizeLow, string lpName);
        [DllImport("kernel32.dll")]
        private static extern int MapViewOfFile(int hFileMappingObject, int dwDesiredAccess, int dwFileOffsetHigh, int dwFileOffsetLow, int dwNumberOfBytesToMap);
        [DllImport("kernel32.dll", CharSet = CharSet.Ansi)]
        private static extern int OpenFileMapping(int dwDesiredAccess, bool bInheritHandle, string lpName);

        [DllImport("kernel32.dll")]
        private static extern IntPtr VirtualAllocEx(IntPtr hProcess, IntPtr lpAddress, uint dwSize, uint flAllocationType, uint flProtect);
        [DllImport("kernel32.dll")]
        private static extern int WriteProcessMemory(IntPtr hProcess, IntPtr lpBaseAddress, byte[] lpBuffer, uint dwSize, IntPtr lpNumberOfBytesWritten);
        [DllImport("kernel32.dll")]
        private static extern IntPtr CreateRemoteThread(IntPtr hProcess, IntPtr lpThreadAttributes, uint dwStackSize, IntPtr lpStartAddress, IntPtr lpParameter, uint dwCreateFlags, IntPtr lpThreadId);
        [DllImport("kernel32.dll")]
        private static extern int CloseHandle(IntPtr hObject);
        [DllImport("kernel32.dll")]
        private static extern int WaitForSingleObject(IntPtr hObject, uint dwMilliseconds);

        private static MapData m_MapData;

        public static bool InitHook()
        {
            var hMapFile = OpenFileMapping(2, false, "GhostCHD_PacketSniffer");
            if (hMapFile == 0)
            {

                foreach (var process in System.Diagnostics.Process.GetProcesses())
                {
                    if (process.ProcessName.ToLower().StartsWith("LaTaleClient".ToLower()))
                    {
                        IntPtr lpAddress = VirtualAllocEx(process.Handle, IntPtr.Zero, BitConverter.ToUInt32(BinRes.SnifferInject, 5), 0x1000, 0x40);
                        if (lpAddress == IntPtr.Zero)
                        {
                            System.Windows.Forms.MessageBox.Show("VirtualAllocEx Failed!");
                            Environment.Exit(-1);
                        }
                        WriteProcessMemory(process.Handle, lpAddress, BinRes.SnifferInject, (uint)BinRes.SnifferInject.Length, IntPtr.Zero);
                        IntPtr hThread = CreateRemoteThread(process.Handle, IntPtr.Zero, 0, lpAddress, IntPtr.Zero, 0, IntPtr.Zero);
                        if (hThread == IntPtr.Zero)
                        {
                            System.Windows.Forms.MessageBox.Show("CreateRemoteThread Failed!");
                            Environment.Exit(-1);
                        }
                        WaitForSingleObject(hThread, unchecked((uint)-1));
                        CloseHandle(hThread);
                        hMapFile = OpenFileMapping(2, false, "GhostCHD_PacketSniffer");
                        if (hMapFile == 0)
                        {
                            System.Windows.Forms.MessageBox.Show("OpenFileMapping Failed!");
                            Environment.Exit(-1);
                        }
                        break;
                    }
                }
            }
            if (hMapFile != 0)
            {
                var pMapData = MapViewOfFile(hMapFile, 2, 0, 0, 0);
                m_MapData = new MapData((IntPtr)pMapData);
                return true;
            }
            return false;
        }

        public static Packet GetPacket()
        {
            if (m_MapData == null)
                return null;
            if (m_MapData.GetIndex != m_MapData.SetIndex)
            {
                Packet p = m_MapData.Data[m_MapData.GetIndex];
                m_MapData.GetIndex = (m_MapData.GetIndex + 1) % MapData.BufferSize;
                return p;
            }
            return null;
        }
    }
}
