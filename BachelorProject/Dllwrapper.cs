using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace BachelorProject
{
    public static class Dllwrapper
    {
        //[DllImport("CbctReconDlld.dll")]
        
       
            [DllImport("CbctReconDlld.dll")]
            public static extern int BuildRTKCommandFilter();// From DlgExternalCommand.h
        
        


        //public static extern Boolean msgBox(IntPtr h, string m, string c, int t);
        //public static extern IntPtr functionname([MarshalAs(UnmanagedType.FunctionPtr)] GuiUpdateDelegate gu, [MarshalAs(UnmanagedType.FunctionPtr)] LogDelegate ld);

    }
}
