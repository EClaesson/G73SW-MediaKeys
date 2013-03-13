using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace Installer
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Do you want to install G73SW-MediaKeys? This will overwrite all standard functionality of the media related Fn-shortcuts. (y/n)");
            string installChoise = Console.ReadLine().Trim();

            if (installChoise == "y" || installChoise == "Y")
            {
                Console.WriteLine("Killing DMedia.exe process...");
                try { Process.GetProcessesByName("DMedia.exe")[0].Kill(); } catch (Exception e) { }
                try { Process.GetProcessesByName("DMedia")[0].Kill(); } catch (Exception e) { }

                Console.WriteLine("Searching for old DMedia.exe...");
                string[] driveList = Environment.GetLogicalDrives();
                string installPath = "";

                foreach (string drive in driveList)
                {
                    if (File.Exists(drive + "Program Files (x86)\\ASUS\\ATK Package\\ATK Media\\DMedia.exe"))
                    {
                        installPath = drive + "Program Files (x86)\\ASUS\\ATK Package\\ATK Media\\DMedia.exe";
                    }
                }

                if (String.IsNullOrEmpty(installPath))
                {
                    Console.WriteLine("Unable to find old DMedia.exe! Make sure drivers/software for Fn-shortcuts are installed.");
                }
                else
                {
                    Console.WriteLine("Found DMedia.exe at " + installPath);
                    Console.WriteLine("Copying DMedia.exe...");
                    
                    try
                    {
                        File.Delete(installPath);
                        File.Copy("DMedia.exe", installPath);
                        Console.WriteLine("Install successful!");

                        Console.WriteLine("Starting DMedia.exe...");

                        try
                        {
                            Process.Start(installPath);
                        }
                        catch (Exception e)
                        {
                            Console.WriteLine("Unable to start DMedia.exe " + e.Message);
                        }
                    }
                    catch (Exception e)
                    {
                        Console.WriteLine("Unable to write to DMedia.exe. " + e.Message);
                    }
                }
            }

            Console.WriteLine("Press return to close");
            Console.ReadLine();
        }
    }
}
