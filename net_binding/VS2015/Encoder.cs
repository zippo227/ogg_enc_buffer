using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace OggEncBuffer
{
    public class BufferEncoder
    {
        [DllImport("oggencbuffer_dynamic")]
        private static extern bool InitEncoder();

        [DllImport("oggencbuffer_dynamic")]
        private static extern bool CloseEncoder();

        [DllImport("oggencbuffer_dynamic")]
        private static extern int EncodeBuffer(System.IntPtr buffer, int length);

        public bool Init()
        {
            return InitEncoder();
        }

        public bool Free()
        {
            return CloseEncoder();
        }

        public int Encode(System.IntPtr buffer, int length)
        {
            return EncodeBuffer(buffer, length);
        }
    }
}
