using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace OggEncBuffer
{
    class TestWrapper
    {
        [DllImport("liboggencbuffer_dynamic")]
        private static extern bool InitEncoder();

        [DllImport("liboggencbuffer_dynamic")]
        private static extern bool CloseEncoder();

        [DllImport("liboggencbuffer_dynamic")]
        private static extern int EncodeBuffer(System.IntPtr buffer, int length);
    }
}
