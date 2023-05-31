using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace Mapper_Gui.model
{
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    public struct X52_Joystick
    {
        [MarshalAs(UnmanagedType.U2)]
        public ushort x;
        [MarshalAs(UnmanagedType.U2)]
        public ushort y;
        [MarshalAs(UnmanagedType.U2)]
        public ushort rz;
        [MarshalAs(UnmanagedType.U1)]
        public byte z;
        [MarshalAs(UnmanagedType.U1)]
        public byte rx;
        [MarshalAs(UnmanagedType.U1)]
        public byte ry;
        [MarshalAs(UnmanagedType.U1)]
        public byte slider;
        [MarshalAs(UnmanagedType.U1)]
        public byte mx;
        [MarshalAs(UnmanagedType.U1)]
        public byte my;
        [MarshalAs(UnmanagedType.U1)]
        public byte size;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 12)]
        public byte[] button;
    };
}
